/*
 * =====================================================================================
 *
 *       Filename:  vsr_hull.h
 *
 *    Description:  convex (and other assorted) hulls assembled into a half edge graph
 *
 *    feed in ND Euclidean data, get out a graph connecting it. . .
 *
 *    This works in Euclidean space, homogenizing it along the way to calculate half spaces, 
 *
 *    so it expects Euclidean input vectors (i.e. a vector of Euclidean vectors) 
 *
 *        Version:  1.0
 *        Created:  03/11/2014 18:10:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#ifndef  vsr_hull_INC
#define  vsr_hull_INC

#include "vsr_graph.h"
#include "vsr_generic_op.h"

namespace vsr{


/*!-----------------------------------------------------------------------------
 *  convex (and other assorted) hulls assembled into a half edge graph 
 *-----------------------------------------------------------------------------*/
template<TT DIM>
struct ConvexHull {
 
    typedef NEVec<DIM> Type;            ///< Vector type
    typedef NEVec<DIM+1> HomType;       ///< Homogenized Vector type
    typedef decltype( HomType() ^ HomType() ) EdgeType;
    typedef HEGraph< Type > HEG;
    typedef typename HEG::Face Face;
    typedef typename HEG::HalfEdge Edge;

    /// The half edge graph
    HEG graph;

    // A BUNCH o' BOOLS to keep track of visitation
    vector<bool> bVisited;
    
    /*-----------------------------------------------------------------------------
     *  Utilities.  These sort of assume 3D for now ...
     *-----------------------------------------------------------------------------*/
    /// Make dual euclidean plane from a face
    //template<TT DIM>
    auto facetPlane( Face& f) RETURNS (
     (Euc::hom( f.a() ) ^ Euc::hom (f.b()) ^ Euc::hom( f.c() ) ).dual()
    )

    /// Make direct line from an edge
    //template<TT DIM>
    auto edgeLine ( Edge& e) RETURNS(
      Euc::hom( e.a() ) ^ Euc::hom( e.b() )
    )

    /// Make plane from two edges
    //template<TT DIM>
    auto edgePlane( Edge& ea,  Edge& eb ) RETURNS (
     (Euc::hom( ea.a() ) ^ Euc::hom (ea.b()) ^ Euc::hom( eb.a() ) ).dual()
    )

    ///are all members of group in same half space relative to plane?
    template<class DualPlaneType>
    bool isHalfSpace( DualPlaneType& dlp, vector<Type>& group ){
      for (auto& i: group){
        auto hi = Euc::hom(i);
        auto halfspace = (hi <= dlp)[0];
       // cout << halfspace << endl; 
        if (halfspace < -0.0001 ) return false;
      }
      return true;
    }

    //Are a bunch of booleans true?
    bool allTrue( vector<bool> b ){
      for (int i = 0;i < b.size(); ++i){
        if (!b[i]) {
           return false;
        };
      }
      return true;
    }


  /*-----------------------------------------------------------------------------
   *  MAKE CONVEX HULL GRAPH USING EVERY POINT IN GROUP
   *-----------------------------------------------------------------------------*/
  HEG& calc ( vector<Type>& group ) {

    graph.clear();
    bVisited = vector<bool>( group.size(), false);

    initialFace(group);
    convexPass(group);
    closeHoles();

    return graph;

  }

  HEG& initialFace(vector<Type>& group){
     
      graph.clear();
      bVisited = vector<bool>( group.size(), false);    

      auto *ta = &group[0];
      Type *tb, *tc;
      int idxA, idxB, idxC;
      idxA = 0;

      double min = 1000000;
      int it = 0;
      for (auto& i : group){
          if( ta != &i){
            auto dist = (*ta - i).wt();
            if (dist < min) {
              min = dist; 
              tb = &i;
              idxB = it;
             // cout << "new min " << dist << endl; 
            }
          }
          it++;
      }

      auto edge = Euc::hom(*ta) ^ Euc::hom(*tb);

      it = 0;
      for (auto& i : group){
          if( (ta!=&i) && (tb!=&i) ){
            auto plane = (edge ^ Euc::hom(i)).dual();
           // cout << "iter " << it << endl; 
            bool bShell = isHalfSpace(plane, group);
            if (bShell) {
              tc = &i;
              idxC = it; 
             // cout << "halfspace found" << endl; 
              break;
            }
          }
          it++;
      }


     // cout << idxA << " " << idxB << " " << idxC << endl; 

      //group[idxA].print();
     //  group[idxB].print();
     // group[idxC].print();

      graph.add( group[idxA] );  bVisited[idxA] = true;
      graph.add( group[idxB] );  bVisited[idxB] = true;
      graph.add( group[idxC] );  bVisited[idxC] = true;

      /* return graph; */

      return graph;

  }
    
   /*-----------------------------------------------------------------------------
    *  FIRST pass TESTS every point to make SHALLOWEST change in CONVEX curvature    
    *-----------------------------------------------------------------------------*/
   HEG& altConvexPass(vector<Type>& group){
      //While all group members have not been accounted for . . .  
       bool bFoundNew = true;
       while ( !allTrue( bVisited )  && bFoundNew ) {
       
          //Add Them to the graph
          //
          //Maybe no new vertices will be established
          bFoundNew = false;
          bool bUseEdgeB;

          auto face = graph.lastFace();           // Last Added Face
          auto dlp = facetPlane( face ).unit();   // Plane of face (Normalized)

          auto& edgeA = graph.lastEdge();         // Last Added Edge  
          auto& edgeB = *(graph.lastEdge().next); // Next to Last Added Edge

          auto ea = Euc::hom(edgeA.a());
          auto eb = Euc::hom(edgeA.b());
          auto ec = Euc::hom(edgeB.a());

          auto edgeLineA = ea ^ eb;                // Last Edge Line
          auto edgeLineB = ec ^ ea;                // Alternative Edge

          // Keep track of idx of point allowing shallowest convex curve
          VT min = 0; VT max = 10000; int idx;
     
         auto tdlp = dlp;
         for (int i = 0; i < group.size(); ++i){
                       
            if ( !bVisited[i] ) {

              auto n = Euc::hom( group[i] );      // Homogenize
              
              // Candidate Plane
              auto ndlp = (n ^ edgeLineA).dual().unit();
              // Dot product must be POSITIVE to make convex hull
              VT convex = (ndlp <= dlp )[0];
              
              if (convex > min ){

                    min = convex;
                    idx = i;
                    bUseEdgeB = false;
                    bFoundNew = true;
                    tdlp = ndlp;
                 
              } 

              // Try the other edge
              ndlp = (n ^ edgeLineB).dual().unit();
              convex = (ndlp <= dlp )[0];

              if (convex > min ){

                 min = convex;
                 idx = i;
                 bUseEdgeB = true;
                 bFoundNew = true;
                 tdlp=ndlp;
              }

            } 
        }

        //if legit
        if ( isHalfSpace(tdlp, group) ) {

          //ADD in Point
          if ( !bVisited[idx] && bFoundNew ){
            //cout << "adding " << idx << endl; 
            if (!bUseEdgeB) graph.add( group[idx] );
            else graph.addAt( group[idx], edgeB);
            bVisited[idx] = true; //did it!
          } 
        } else {
          //otherwise, bfoundnew is false
          bFoundNew = false;
        }
      }

      /////

      //Try to hull any stragglers
      for(int it=0; it < bVisited.size(); ++it){
        if (!bVisited[it]){
          //try some edge
          for (auto& e : graph.edge() ){
            auto dlp = ( edgeLine(*e) ^ Euc::hom( group[it] ) ).dual(); // Dual plane
            if ( isHalfSpace(dlp, group) ) {
                //success, add point to edge
                graph.addAt( group[it], *e );
                bVisited[it] = true;
                break;
            }
          }
        }
      }
      return graph;
    }

    
   /*-----------------------------------------------------------------------------
    *  FIRST pass TESTS every point to make SHALLOWEST change in CONVEX curvature    
    *-----------------------------------------------------------------------------*/
   HEG& convexPass(vector<Type>& group){
      //While all group members have not been accounted for . . .  
       bool bFoundNew = true;
       while ( !allTrue( bVisited )  && bFoundNew ) {
       
          //Add Them to the graph
          //
          //Maybe no new vertices will be established
          bFoundNew = false;
          bool bUseEdgeB;

          auto face = graph.lastFace();           // Last Added Face
          auto dlp = facetPlane( face ).unit();   // Plane of face (Normalized)

          auto& edgeA = graph.lastEdge();         // Last Added Edge  
          auto& edgeB = *(graph.lastEdge().next); // Next to Last Added Edge

          auto ea = Euc::hom(edgeA.a());
          auto eb = Euc::hom(edgeA.b());
          auto ec = Euc::hom(edgeB.a());

          auto edgeLineA = ea ^ eb;                // Last Edge Line
          auto edgeLineB = ec ^ ea;                // Alternative Edge

          // Keep track of idx of point allowing shallowest convex curve
          VT min = 0; VT max = 10000; int idx;
     
         for (int i = 0; i < group.size(); ++i){
                       
            if ( !bVisited[i] ) {

              auto n = Euc::hom( group[i] );      // Homogenize
              
              // Candidate Plane
              auto ndlp = (n ^ edgeLineA).dual().unit();
              // Dot product must be POSITIVE to make convex hull
              VT convex = (ndlp <= dlp )[0];
              
              if (convex > 0 ){

                 // Distance to last added plane:
                 VT dist = fabs( (n <= dlp)[0] );

                 // Higher dot product == shallower change in curvature
                 // Lower distance to original plane means ?
                 if ( convex > min && dist < max ) {
                 
                    min = convex;
                    max = dist;
                    idx = i;
                    bUseEdgeB = false;
                    bFoundNew = true;
                 
                  }

              } else {
                 
                  // Try the other edge
                  ndlp = (n ^ edgeLineB).dual().unit();
                  convex = (ndlp <= dlp )[0];

                  if (convex > 0 ){

                   VT dist = fabs( (n <= dlp)[0] );

                   if ( convex > min && dist < max ) {

                     min = convex;
                     max = dist;
                     idx = i;
                     bUseEdgeB = true;
                     bFoundNew = true;
                  }
                }
              }
            } 
          }

          //ADD in Point
          if ( !bVisited[idx] && bFoundNew ){
            //cout << "adding " << idx << endl; 
            if (!bUseEdgeB) graph.add( group[idx] );
            else graph.addAt( group[idx], edgeB);
            bVisited[idx] = true; //did it!
          } 
      }

      //Try to hull any stragglers
      for(int it=0; it < bVisited.size(); ++it){
        if (!bVisited[it]){
          //try some edge
          for (auto& e : graph.edge() ){
            auto dlp = ( edgeLine(*e) ^ Euc::hom( group[it] ) ).dual(); // Dual plane
            if ( isHalfSpace(dlp, group) ) {
                //success, add point to edge
                graph.addAt( group[it], *e );
                bVisited[it] = true;
                break;
            }
          }
        }
      }
      return graph;
    }


  
  /*-----------------------------------------------------------------------------
   *  CLOSE HOLES IN TOPOLOGY
   *-----------------------------------------------------------------------------*/
  HEG& closeHoles(vector<Type> group, int N=0){
        int iter = 0; 
        while ( !graph.nullEdges().empty() && iter<N ) {
            iter++;

            auto nullEdges = graph.nullEdges();
             
             //cout << nullEdges.size() << " null edges " << endl; 
            // First nullEdge 
            auto& ea = nullEdges[0];
            // Edge and Face geometric data
            auto ela = edgeLine(*ea);
            auto dlp = facetPlane( *( ea->face ) ).unit();

            // Find another edge with which to make new facet
            Edge * eb;

            VT min = 0; 
            bool bFound = false;
            for (auto& i : nullEdges ){ 
              
              //1. Ignore edges that point to same node
              if (ea->node != i->node){

                //2. Try simply sealing it up first (maybe edges share nodes)
                if ( ea->isOpp(*i) ) {
                //    cout << "simple seal" << endl;
                    ea->seal(*i); 
                    bFound = true;
                    break; //break if found
                }
                
                //3. Otherwise keep track of shallowest, most convex match 
                auto n = Euc::hom( i->a() );    
                auto ndlpA = ( n ^ ela ).dual().unit();
             //   auto ndlpB = ( ela ^ n ).dual().unit();
                VT convex = (ndlpA <= dlp)[0];
               // cout << "test convex: " << convex << " " << (dlp<=ndlpA)[0] << endl;
                if (convex > min ){
                   // add half space check (get rid of this...)
                   if(isHalfSpace(ndlpA,group)){ 
                    min = convex;
                    eb = i;
                  //  cout << "convex hs: " << min << endl;
                   }
                } 
              }   
            }

            // Called only if simple seal not found
            if (!bFound){
               bool bTestBoth = false;
              //Do ea and eb share a node already?
              if ( ea->ccwFrom( *eb ) ){  
                 //Are we closing a triangle?
                 if ( ea -> triangle() ) { graph.close( *ea ); }
                 //Otherwise close ccw
                 else { graph.close( *ea, *eb ); }//cout << "ccw" << endl; }
              } else if (ea->cwFrom( *eb )){
                 //Are we closing a triangle?
                 if ( ea -> triangle() ) { graph.close( *ea ); }
                 //Otherwise close cc
                  else {graph.close( *eb, *ea ); }//cout << "cc" << endl; }
              } else {
                 //Or just add a point
                 graph.close( *ea, *(eb -> node) ); 
                // cout << "pt" << endl; 
                 bTestBoth = true;
              }

             // cout << "made face #: " << graph.face().size() -1 << endl; 

              //Now try simple seal of new edges
              for (auto& i : nullEdges ){ 
                 if ( graph.edge( -1 ).isOpp(*i) ) { 
                    graph.edge( -1 ).seal(*i); 
                   // cout << "suture 1" << endl;
                    break; 
                 }
              }
              //Test on both new edges if closed to a point
              if (bTestBoth){
                for (auto& i : nullEdges ){ 
                 if ( graph.edge( -3 ).isOpp(*i) ) { 
                    graph.edge( -3 ).seal(*i); 
                    //cout << "suture 2" << endl;
                    break; 
                 }
                }
              }
            }

        }
        return graph;
    }

// ConvexHull
};  

} // vsr::

#endif   /* ----- #ifndef vsr_hull_INC  ----- */
