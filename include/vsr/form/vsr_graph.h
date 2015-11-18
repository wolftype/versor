

#ifndef VSR_GRAPH_H_INCLUDED
#define VSR_GRAPH_H_INCLUDED

#include "vsr_set.h"
//#include "vsr_smart.h"

namespace vsr{


  //empty struct default data (so reference can be returned
  struct nada {

  };
 
 /*! Templated half edge structure (pointers to any type, with any type at any edge)
  *  Navigates references to surface topology of data only (DOES NOT STORE DATA)
  *
  *  thoughts: could this half-edge approach be applied to any simplicial complex? i.e. by treating edges
  *  as generic k-1 simplices...

      @todo generalize half-edge data structure to any dimension
      @todo add templates to faces as well
  */ 
 template<class T, class E=nada>
 struct HEGraph {

   struct HalfEdge;                     
   struct Face;
   struct Node;
   

   /*!
    *  A Node stores address of value of type T and pointer to an emanating edge.
    *  It includes functions for finding all emanating edges, faces, connected neighbors,
    *  and a boolean visitation flag that can be set to avoid endless looping algorithms  
    */
   struct Node {

     Node() : ptr(NULL), edge(NULL), bVisited(false){}

     T * ptr;                           ///< Pointer to type T
     HalfEdge * edge;                   ///< An emanating half-edge      
     bool bVisited;                     ///< Flag for keeping track of visitation

     /// Get reference to data;
     T& data() { return *ptr; }    
     /// Get const reference to data
     T data() const { return *ptr; }

     /// Set data value
     Node& data( T& v ) { ptr = &v; return *this; }

     /// Find all emanating edges
     vector<HalfEdge*> valence() const;
     
     /// Find edge loop (next of all emanating edges)
     vector<HalfEdge*> edgeLoop() const;

     /// Test for node closure (no null Edges in loop)
     bool closed() const;

    // bool bClosed; //maybe save state for cheaper comp

     /// Find any null edge of a node 
     HalfEdge& null();

     /// Find both null edges around a node
     vector<HalfEdge*> nulls();

     /// Find all faces
     vector<Face*> faces();

     /// Find all Neighboring nodes
     vector<Node*> neighbors();

     /// set visitation flag value to t
     void visited(bool t) { bVisited=t; }
     /// get visitation flag value
     bool visited(){ return bVisited; }
     /// set visitation flag value to zero
     void reset() { bVisited=false; }
        
   };



      
   /*!
    * HALF EDGE Data structure 
    */
   struct HalfEdge{ 

     //int id;
     
     HalfEdge() : node(NULL), face(NULL), opp(NULL), next(NULL), bVisited(false)
     {}
         
     E * ptr;                      ///< Pointer to data assigned to each edge (e.g. constraint)
     /// Get reference to data;
     E& data() { return *ptr; }    
     /// Get const reference to data
     E data() const { return *ptr; }
         
     bool bVisited;                ///< Whether visited? 

          
     Node    * node;                ///< Incident vertex
     Face    * face;                ///< Face membership
    
     HalfEdge  * opp;               ///< Opposite half-edge
     HalfEdge  * next;              ///< Next half-edge counterclockwise

     HalfEdge& prev() { return *(next -> next); }  ///< Reference to Previous Edge (clockwise) 

     T& a() { return node -> data(); }             ///< Reference to T data at incident node
     T& b() { return prev().node -> data(); }      ///< Reference to T data at other end

     bool isBorder() { return opp == NULL; }       ///< Whether edge is on the edge of the manifold

     void set( HalfEdge& e){
        node = &(*e.node);
        face = &(*e.face);
        opp = &(*e.opp);
        next = &(*e.next);
     }

    /// Find next null edge 
    /// (assumes this edge is null)
    /// Could be on same face . . . 
     HalfEdge& nextNull(bool clockwise){
        
        if (clockwise){
          HalfEdge * e = next -> opp;
          HalfEdge * t = next;
          while (e !=NULL){
              t = e -> next;
              e = e -> next -> opp;   
          }
          return *t;
        } else {
          HalfEdge * e = next -> next -> opp;
          HalfEdge * t = next -> next;
          while (e !=NULL){
              t = e -> next -> next;
              e = e -> next -> next -> opp;   
          }
          return *t;

        }
     }

     /// is clockwise edge relative to argument (Checks for shared Node with another HalfEdge)
     bool ccwFrom( HalfEdge& eb ){
       if ( node == eb.prev().node ) return true;
       return false;
     }
     /// is clockwise edge relative to argument (Checks for shared Node with another HalfEdge)
     bool cwFrom( HalfEdge& eb){
       if ( prev().node == eb.node  ) return true;
       return false;
     }

    /// Test for partnership
     bool isOpp( HalfEdge& eb ){
       if ( ( node == eb.prev().node ) && ( prev().node == eb.node ) ) return true;
       return false;
     }

    /// Seal together two halfedges
     void seal( HalfEdge& eb) {
       opp = &eb; eb.opp = this;
     }

     /// Check for simple Loop (triangle)
     bool triangle(){
        auto& eb = nextNull(false);
        if ( &nextNull(true) == &(eb.nextNull(false)) ) return true;
        else return false;
     }
    
     /// Edge Looop around graph until we return to this (or until null edge)
    /* vector<HalfEdge*> loop(){ */
    /*   vector<HalfEdge*> redge; */
    /*   HalfEdge * te; */
    /*   do{ */
    /*     if ( (next->opp)!=NULL) { */
    /*       te = next->opp->next; */
    /*     } else{ */
    /*       te = next; */
    /*     } */
    /*     redge.push_back(te); */
    /*   return redge; */
    /* } */



   };

   
   struct Face {

     Face() : edge(NULL) {}

     HalfEdge * edge;       //Any edge

     HalfEdge& ea() { return *edge; }
     HalfEdge& eb() { return *(edge->next); }
     HalfEdge& ec() { return *(edge->next->next); }

     /* HalfEdge ea() const { return *edge; } */
     /* HalfEdge eb() const { return *(edge->next); } */
     /* HalfEdge ec() const { return *(edge->next->next); } */

     Node& na() { return * ( ea().node ); }
     Node& nb() { return * ( eb().node ); }
     Node& nc() { return * ( ec().node ); }

     /* Node na() const { return * ( ec().node ); } */
     /* Node nb() const { return * ( ea().node ); } */
     /* Node nc() const { return * ( eb().node ); } */


     T& a() { return na().data() ; }
     T& b() { return nb().data() ; }
     T& c() { return nc().data() ; }

     /* T a() const { return na().data() ; } */
     /* T b() const { return nb().data() ; } */
     /* T c() const { return nc().data() ; } */

     vector<Face*> edgeNeighbors(){
       vector<Face*> vp;

        if ( ea().opp != NULL ) vp.push_back( ea().opp -> face );
        if ( eb().opp != NULL ) vp.push_back( eb().opp -> face );
        if ( ec().opp != NULL ) vp.push_back( ec().opp -> face );

        return vp;
     }



   };




   //METHODS
   //once three nodes exist, seed them into a facet (starts with b)
    void seedNodes(){

        Node *na, *nb, *nc; 
        HalfEdge *ea, *eb, *ec;
        Face *f;
        
        na = &(*(mNode[0]));  nb = &(*( mNode[1] ));  nc = &(*(mNode[2]));

         
        ea = new HalfEdge; 
        eb = new HalfEdge; 
        ec = new HalfEdge;
        
        f = new Face(); 

        //Assign a edge incident to b, b edge incident to c, etc      
        ea -> node = na;//nb;  
        eb -> node = nb;//nc; 
        ec -> node = nc;//na; 
        
        f -> edge = ea; //Pick any old edge

        na -> edge = eb; //emanating
        nb -> edge = ec; 
        nc -> edge = ea;

        ea -> next = eb; ea -> face = f;  // assign next edge and face
        eb -> next = ec; eb -> face = f;
        ec -> next = ea; ec -> face = f;

        //Store
        mHalfEdge.push_back(ea);
        mHalfEdge.push_back(eb);
        mHalfEdge.push_back(ec);

        mFace.push_back(f);

    }


    void facet( HalfEdge * ea, HalfEdge * eb, HalfEdge * ec, Face * f){
      
      ea -> next = eb;  eb -> next = ec; ec -> next = ea;
      ea -> face = f; eb -> face = f; ec -> face = f;

      f -> edge = ea;

    }

    //preliminary
    Node& addNode( T& v ){
      Node * n = new Node;
      n -> ptr = &v;
      mNode.push_back( n );
      return *n;
    }

    //add faces and edges to nodes that already exist
    HEGraph& fillFace( int a, int b, int c ){
      
      Node *na, *nb, *nc; 
      HalfEdge *ea, *eb, *ec;
      Face *f;

      na = mNode[a];
      nb = mNode[b];
      nc = mNode[c];
      
      ea = new HalfEdge; 
      eb = new HalfEdge; 
      ec = new HalfEdge;
      
      f = new Face(); 

      na -> edge = eb; //emanating
      nb -> edge = ec; 
      nc -> edge = ea;
     
       //Assign a edge incident to b, b edge incident to c, etc      
      ea -> node = na;//nb;  
      eb -> node = nb;//nc; 
      ec -> node = nc;//na; 

      mHalfEdge.push_back(ea);
      mHalfEdge.push_back(eb);
      mHalfEdge.push_back(ec);

      mFace.push_back(f);


      facet(ea,eb,ec,f);

      return *this;
       
    }


    HEGraph& add( T& v ) {  
      int num = mNode.size();      
      if (num < 3 )  addNode( v );
      if (num == 2 ) seedNodes(); //if num was 2 coming in, it is now 3 so seed
      if (num >= 3 ) addAt ( v, lastEdge() );
      return *this;
   }



    //add a new Node (and Face) To Some Edge (creates three new half edges, a new node and a new face)
    HEGraph& addAt( T& v, HalfEdge& e ){

      Node * n = new Node;

      HalfEdge * ea = new HalfEdge; 
      HalfEdge * eb = new HalfEdge; 
      HalfEdge * ec = new HalfEdge;

      Face * f = new Face;

      //pick an edge
      f -> edge = ea;

      //basic facet
      ea -> next = eb;  eb -> next = ec; ec -> next = ea;
      ea -> face = f; eb -> face = f; ec -> face = f;

      //incidence: ea is outgoing edge from new node, ec is incident edge to it, eb shares with argument
      n -> ptr = &v; 
      n -> edge = ea;
      ec -> node = n;

      e.opp = eb; eb -> opp = &e;

      ea -> node = e.node; 
      eb -> node = e.next -> next -> node; 

      //Store     
      mNode.push_back(n);
      
      mHalfEdge.push_back(ea);
      mHalfEdge.push_back(eb);
      mHalfEdge.push_back(ec);

      mFace.push_back(f);
      
      return *this;
    }

    HEGraph& addAt( T& v, int x ){
      return addAt(v, edge(x));
    }

    //Given Four Points, make two triangles
    void seed( T& a, T& b, T& c, T& d){
       seed(a,b,c); add(d, lastEdge() );
    }

    //Insert new point on Edge make new face "downstream"
    void insert( T& pa, HalfEdge& e){

      //Three new halfedges, one new Node and one new Face
      HalfEdge * ea, *eb, *ec;
      Node * n = new Node;
      Face * f = new Face;

      ea = new HalfEdge; eb = new HalfEdge; ec = new HalfEdge;  

      //Assign, new face
      ea -> set( e ); //equal . . .
      ea -> face = f; f -> edge = ea; // . . . except for face
  
      ec->opp = eb; eb->opp  = ec; 
      ec->next = ea; 
      ec->node = n;
      ec->face = f;
    
      eb->node = &(*(e.next -> node));
      eb->next = &(*(e.next -> next));
      eb->face = &(*(e.face));
      
      n -> ptr = &pa; n -> edge = ea;
      
      e.next -> face = f;
      e.next -> next = ec;

      e.node = n; e.next = eb;       
      e.face -> edge = &e;

      //Store
      mNode.push_back(n);

      mHalfEdge.push_back( eb );
      mHalfEdge.push_back( ec );
      mHalfEdge.push_back( ea );

      mFace.push_back( f );

   }

  ///eb   /ea counter clockwise
   void close( HalfEdge& ha, HalfEdge& hb){

      //add an edge and a face
      Face * f = new Face;
      
      HalfEdge *ea, *eb, *ec;
      ea = new HalfEdge; eb = new HalfEdge; ec = new HalfEdge;
      
      //make facet
      facet( ea,eb,ec,f);
      
      ea -> node = hb.prev().node;
      ec -> node = hb.node;
      eb -> node = ha.prev().node;

      ha.opp = eb;  eb -> opp = &ha;
      hb.opp = ea;  ea -> opp = &hb;

      mHalfEdge.push_back( ea );
      mHalfEdge.push_back( eb );
      mHalfEdge.push_back( ec );

      mFace.push_back( f );
   }

   // close edge and node
   void close( HalfEdge& e, Node& n){

      //a new face
      Face * f = new Face;
      
      //3 new halfEdges
      HalfEdge *ea, *eb, *ec;
      ea = new HalfEdge; eb = new HalfEdge; ec = new HalfEdge;
      
      //make facet
      facet( ea,eb,ec,f);

      ea -> node = e.node;
      eb -> node = e.prev().node;
      ec -> node = &n;

      e.opp = eb; eb -> opp = &e;

      mHalfEdge.push_back( ea );
      mHalfEdge.push_back( eb );
      mHalfEdge.push_back( ec );

      mFace.push_back( f );


   }

   //Close Simple Triangle Hole
   void close( HalfEdge& e ){
      //add edges and a face
      Face * f = new Face;
      
      HalfEdge *ea, *eb, *ec;
      ea = new HalfEdge; eb = new HalfEdge; ec = new HalfEdge;
      
      //make facet
      facet( ea,eb,ec,f);

      auto& tb = e.nextNull(false);
      auto& tc = e.nextNull(true);

      ea -> node = tb.node;
      eb -> node = tc.node;
      ec -> node = e.node;

      ea -> opp = &e; e.opp = ea;
      eb -> opp = &tb; tb.opp = eb;
      ec -> opp = &tc; tc.opp = ec;

      mHalfEdge.push_back( ea );
      mHalfEdge.push_back( eb );
      mHalfEdge.push_back( ec );

      mFace.push_back( f );

   }

    //removes a face @todo code half-edge face removal
    void removeFacet( int idx ) {

    }

    /// removes an edge @todo code half-edge edge removal
    void removeEdge( int idx ) {

    }

    /// given an edge loop, determine whether it is closed
    bool isClosed( vector<HalfEdge*> loop){
        return loop.back()->node == loop[0]->prev().node;
    }

    /// given an edge loop around a node, find next outer edge loop
    vector<HalfEdge*> edgeLoop0( vector<HalfEdge*> loop ){//const Node& n){
      
      vector<HalfEdge*> result;
      
      if (!loop.empty()){
        //pick one node and get emanating edges
        auto v = loop[0]->node->valence();
        //find first one that is does not point to any node on current loop
        HalfEdge * first = NULL;
        for (auto& i : v){
          bool bExists=false;
          for (auto& j : loop){
            if (j->node==i->node){
              bExists=true;
              break;
            }
          }
          if (!bExists){
            first=i;
            break;
          }
        }

       //assuming we found a new emanating edge, add all first
        //cw edges that don't point back 
        //until we get back to first or hit border
        HalfEdge * tmp = first;
        while (tmp!=NULL && (tmp->node != first->node)  ){
          
          bool bExists=false;
          tmp = tmp->next;
          for (auto& j : loop){
            if (j->node==tmp->node){
              bExists=true;
              break;
            }
           }
           if (bExists) tmp = tmp->opp; //repeat
           else {
            result.push_back(tmp); 
           }        
        }
      }

      return result;
    }

  /// emanating edges from an input loop of emanating edges
  vector<HalfEdge*> emanatingEdges( vector<HalfEdge*> loop ){
      
      HalfEdge * first = NULL;

      vector<HalfEdge*> result;
     
      for(auto& i : loop){
        if (i->opp!=NULL){
          result.push_back(i->opp->next);
        }
      }
      return result;
  }

    vector<HalfEdge*> edgeLoop( vector<HalfEdge*> loop ){

            int maxNum = 100;
    
            vector<HalfEdge*> result;
            HalfEdge * tmp = NULL;
    
            if (!loop.empty()){
    
              //does loop close?
              bool bClosed = loop.back()->node == loop[0]->prev().node;
    
              //Get Emanating Edges
              auto edges = emanatingEdges(loop);
    
              //if not closed, connect edge to first emanating edge
              //(if first emanating edge isn't already on border)
              if (!bClosed){
                if (!edges.empty()){
                  tmp=edges[0];
                  if (!tmp->isBorder()){
                    int iter =0;
                    while (!tmp->isBorder() && iter<maxNum){
                      tmp=tmp->opp->next;
                      iter++;
                    }
                    result.push_back(tmp);
                    tmp = tmp->next;
                    result.push_back(tmp);
    
                    iter =0;
                    while(tmp->node != edges[0]->node && iter<maxNum){
                      tmp = tmp->next->opp->next;
                      result.push_back(tmp);
                      iter++;
                    }
                  } else if (edges.size()>2) {
                    result.push_back(tmp);          
                  }
    
                }
              }
    
              //Connect emanating edges (avoiding original loop's nodes)
              for (int i=0; i<edges.size();++i){
                tmp = edges[i];
                int nxt = i<edges.size()-1 ? i+1 : 0;
                int iter =0;
                while ( tmp!=NULL && tmp->node != edges[nxt]->node && iter < maxNum){
                  iter++;
                  tmp = tmp->next;
    
                  bool bExists=false;
                  for (auto& j : loop){
                    if ( (j->node==tmp->node) || (j->next->next->node==tmp->node) ){
                      bExists=true;
                      break;
                    }
                  }
    
                  if (bExists) {
                    if (tmp->opp==NULL){
                       result.push_back(tmp);
                    }
                    tmp = tmp->opp;
                   } else {
                    result.push_back(tmp); 
                   }        
               }
              }
    
            }
    
            return result;
    }
    

    //all nodes connected to a node
    /* vector<Node*> nodeLoop( Node& a){ */
    /*     vector<Node*> tmp; */
    /*     for (auto i : edgeLoop(a) ) tmp.push_back( i -> node ); */
    /* } */

    /// Get null edges of graph
    vector<HalfEdge*> nullEdges(){
      vector<HalfEdge*> tmp;
      for (auto& i : mHalfEdge){
        if ( i -> isBorder() ) tmp.push_back(i);
      }
      return tmp;
    }

    /// Any Null Edges?
    bool hasBorder(){
       for (auto& i : mHalfEdge){
          if ( i -> isBorder() ) return true;
       }
       return false;
    }

    /// Find first null edge
    HalfEdge& firstNull(){
       for (auto& i : mHalfEdge){
           if ( i -> isBorder() ) return *i;
       }
    }

    /// null edge path (boundary) of graph
    vector<HalfEdge*> nullEdgeLoop(){

      vector<HalfEdge*> tmp;
      int it = -1;
      
      for (int it = 0; it < mHalfEdge.size(); ++it){
          
          if ( mHalfEdge[it] -> opp != NULL ) continue;
          
          else {
             
             HalfEdge * he = mHalfEdge[it];
             
             do{          
                tmp.push_back( he );
                he = &(he -> nextNull() ); 
             } while ( he != mHalfEdge[it] ); 
             
             return tmp;  
         }
      }
    }


    //methods
    void reset() {
      for (auto& i : mHalfEdge){
        i -> bVisited = false;
      }
      for (auto& i : mNode){
        i -> visited(false);
      }
    }

    void clear() {
      mHalfEdge.clear();
      mFace.clear();
      mNode.clear();
    }

    //Most recently added elements of graph
    HalfEdge& lastEdge()    { return *mHalfEdge.back(); }
    Face& lastFace()    { return *mFace.back(); }
    Node& lastNode()    { return *(mHalfEdge.back() -> node); }

    Face& face( int idx ) { return *mFace[idx]; }
//    HalfEdge& edge( int idx ) { return *mHalfEdge[ (mHalfEdge.size() - 1) + idx ]; } //testing this for negative indiices
    HalfEdge& edge( int idx ) { return  idx < 0 ? *mHalfEdge[ mHalfEdge.size() + idx ] : *mHalfEdge[idx]; }
  //  HalfEdge edge( int idx ) const { return  idx < 0 ? *mHalfEdge[ mHalfEdge.size() + idx ] : *mHalfEdge[idx]; }
    Node& node( int idx ) { return *mNode[idx]; }


    vector<HalfEdge*>& edge() { return mHalfEdge; }
    vector<Face*>& face() { return mFace; }
    vector<Node*>& node() { return mNode; }

    vector<HalfEdge*> edge() const { return mHalfEdge; }
    vector<Face*> face() const { return mFace; }
    vector<Node*> node() const { return mNode; }

   //GENERIC ux, uy GRAPHING UTIL
    template<class S>
    HEGraph& UV(int w, int h, S& p, bool bCloseU=false,bool bCloseV=false);

    private:

    vector<HalfEdge*>    mHalfEdge;
    vector<Face*>        mFace;
    vector<Node*>        mNode;

   //some data container (unnecessary?)
   //vector<T> * data;

 };   


     //NODE METHODS
    template<class T,class E>
    inline typename HEGraph<T,E>::HalfEdge& HEGraph<T,E>::Node::null(){     
      HEGraph::HalfEdge * e = edge -> opp;
      while ( e != NULL && e != edge -> next -> next ){
        e = e -> next -> opp;
      }
      return *e;
    }

    template<class T,class E>
    inline bool HEGraph<T,E>::Node::closed() const{
      HEGraph::HalfEdge * e = edge -> opp;
      if (e == NULL) return false;
      while ( e != edge -> next -> next ){
          e = e -> next -> opp; 
          if (e == NULL) return false;
      }
      return true;
    }

    template<class T,class E>
    inline vector< typename HEGraph<T,E>::HalfEdge* > HEGraph<T,E>::Node::nulls(){
      vector<HEGraph::HalfEdge*> tmp;
      HEGraph::HalfEdge * e = edge -> opp;
      while ( e != NULL && e != edge -> next -> next ){
        e = e -> next -> opp;
      }
      if (e == NULL ){
        tmp.push_back( e );
        e = edge -> next -> next -> opp;
        while ( e != NULL ){
          e = e -> next -> next -> opp;
        }
        tmp.push_back(e);
      }
      return tmp;
    }

    
    //edge loop around a node collects (emanating) edges
    template<class T,class E>
    inline vector<typename HEGraph<T,E>::HalfEdge*> HEGraph<T,E>::Node::valence() const {
        vector<HEGraph::HalfEdge*> tmp;
        HEGraph::HalfEdge * e = edge;
       
        if (closed()){
       
          do {
            tmp.push_back(e);
            e = e -> next -> next -> opp;
          } while( e != edge );
       
          return tmp; 
       
        } else {
          //if it doesn't loop around to the beginning, 
          //iterate to end in clockwise direction, and then add in counter clockwise
          auto te = edge->opp;
          while (te != NULL ){
            e = te->next;
            te = te->next->opp;
          }
          //now e is cw most edge, add in all . . . there will always be one node left.. 
          while(e != NULL){
            tmp.push_back(e);
            e = e -> next -> next -> opp;
          }
        }
        return tmp;
    }

    template<class T, class E>
    inline vector<typename HEGraph<T, E>::HalfEdge*> HEGraph<T,E>::Node::edgeLoop() const {
      vector<HEGraph::HalfEdge*> tmp;
      auto v = valence();
      for(auto& i : v){
        tmp.push_back(i->next);
      }
      return tmp;
    }



    //faceloop around a node collects faces
    template<class T, class E>
    inline vector<typename HEGraph<T,E>::Face*> HEGraph<T,E>::Node::faces(){
      vector<HEGraph::Face*> tmp;
      for (auto& i : valence() ){
        tmp.push_back( i -> face );
      }
    }
   //for each  in valence, add in node at other end (plus last node if open)
    template<class T, class E>
    inline vector<typename HEGraph<T,E>::Node*> HEGraph<T,E>::Node::neighbors(){
      vector<typename HEGraph::Node*> tmp;
      auto res = valence();
      for(auto& i : res){
        tmp.push_back( i->node );
      }
      if( !closed() ) tmp.push_back( res.back()->next->node ); 
      return tmp;
    }

    /* //for each  in valence, add next to get edge loop */
    /* template<class T> */
    /* inline vector<typename HEGraph<T>::Node*> HEGraph<T>::Node::edgeLoop(){ */
    /*   vector<typename HEGraph<T>::Node*> tmp; */
    /*   auto res = valence(); */
    /*   for(auto& i : res){ */
    /*     tmp.push_back( i->node ); */
    /*   } */
    /*   if( !closed() ) tmp.push_back( res.back()->next->node ); */ 
    /*   return tmp; */
    /* } */



    /*-----------------------------------------------------------------------------
     *  c <--d
     *  | \  ^
     *  |  \ |
     *  a--->b
     *-----------------------------------------------------------------------------*/
    /// class S needs only be indexable by operator[]
    template<class T,class E> template<class S>
    inline HEGraph<T,E>& HEGraph<T,E>::UV(int w, int h, S& p, bool bCloseU, bool bCloseV){
      HEGraph& graph = *this;
      //left column
      for (int j = 0; j < h; ++j){
          int idx = j;
          int idxB = j+h;
          //add first two points
          if (j<2) graph.add( p[idx] );//adds a, c
          else graph.addAt( p[idx], graph.edge(-3) );
          //add next row over
          if (j==0) graph.add( p[idxB] );//adds b
          //else if (j<2) graph.addAt( p[idxB], graph.edge(-3) ); 
          else  graph.addAt( p[idxB], graph.edge(-1) );       //add to last edge
      }

      for (int i = 2; i < w; ++i){
        int idx = ((i-2) * (h-1) + 1)*6 -1;
        int pidx = i * h;
        int pidxB = pidx + 1;
        graph.addAt( p[pidx], graph.edge( idx ) );
        graph.addAt( p[pidxB], graph.edge(-3) );
        for (int j = 2; j < h; ++j){
           int pidxC = i * h + j; 
            graph.close( graph.edge(-3), graph.edge( idx + (j-1) * 6 ) );
            graph.add( p[pidxC] );  
        }
      }

      if (bCloseU){
        for (int j=0;j<h-1;++j){
          //int idxR = ((w-2)*(h-1)+j+1)*6;
          int idxR = ((w-2)*(h-1)+j)*6+5;
          int idxL = j*6;
          graph.close( graph.edge(idxR), *graph.edge(idxL).node);
          graph.close( graph.edge(-3), *graph.edge(idxL+6).node);
        }
      }
      if (bCloseV){
        if(!bCloseU){
          graph.close( graph.edge(6*(h-1)-3), graph.node(0) );
          graph.close( graph.lastEdge(), *graph.node(0).edge );
          for (int i=1;i<w-1;++i){
            int idxB = (i*(h-1))*6+2;
            int idxT = ((i+1)*(h-1)*6)-3;
            graph.close( graph.edge(idxT), *graph.edge(idxB).prev().node);
            graph.close( graph.lastEdge(), graph.edge(idxB) );
          }
        }else{}
      }



      return graph;
    }

} //vsr::


     /* struct Corner { */
     /*   Corner next, prev;  // next / prev corners in face */
     /*   HalfEdge hedge; */
     /*   Edge edge;      // incoming edge */
     /*   Node node;      // incident vertex */
     /* }; */

    //given three points, make new facet connecting them (deprecate)
    /* void seed( T& pa,  T& pb,  T& pc){ */

    /*    //Create */
    /*    Node *na, *nb, *nc; */
    /*    HalfEdge *ea, *eb, *ec; */
    /*    Face *f; */
        
    /*     na = new Node; nb = new Node; nc = new Node; */
         
    /*     ea = new HalfEdge; */ 
    /*     eb = new HalfEdge; */ 
    /*     ec = new HalfEdge; */
        
    /*     f = new Face(); */ 

    /*     //Assign a edge incident to b, b edge incident to c, etc */      
    /*     ea -> node = nb; */  
    /*     eb -> node = nc; */ 
    /*     ec -> node = na; */ 
        
    /*     na -> ptr  = &pa; */ 
    /*     nb -> ptr  = &pb; */ 
    /*     nc -> ptr  = &pc; */ 

    /*     f -> edge = ea; //Pick any old edge */

    /*     na -> edge = ea; nb -> edge = eb; nc -> edge = ec; */

    /*      ea -> next = eb; ea -> face = f;  // assign next edge and face */
    /*      eb -> next = ec; eb -> face = f; */
    /*      ec -> next = ea; ec -> face = f; */

    /*     //Store */
    /*     mNode.push_back(na); */
    /*     mNode.push_back(nb); */
    /*     mNode.push_back(nc); */

    /*     mHalfEdge.push_back(ea); */
    /*     mHalfEdge.push_back(eb); */
    /*     mHalfEdge.push_back(ec); */

    /*     mFace.push_back(f); */

    /* } */
   //last edge and point (or node??)
    /* void close( T& p ){ */
    /*   //should make a new face */
    /*   Face * f = new Face; */
      
    /*   HalfEdge *ea = new HalfEdge; */
    /*   HalfEdge *eb = new HalfEdge; */
    /*   HalfEdge *ec = new HalfEdge; */

    /*   ea -> face = f; eb -> face = f; ec -> face = f; */
    /*   f -> edge = ea; */
    /*   ea -> next = eb; eb -> next = ec; ec -> next = ea; */

    /*   HalfEdge& edge = lastEdge(); */
    /*   edge.opp = eb; */
      
    /*   mFace.push_back(f); */
    /*   mHalfEdge.push_back(ea); */
    /*   mHalfEdge.push_back(eb); */
    /*   mHalfEdge.push_back(ec); */

    /* } */

    /* //add a route to the halfedge graph */
    /* void connect(Node& a, Node& b){ */

    /*   //should make a new face */
    /*   Face * f = new Face; */
      
    /*   HalfEdge *ea = new HalfEdge; */
    /*   HalfEdge *eb = new HalfEdge; */
    /*   HalfEdge *ec = new HalfEdge; */

    /*   //find the common node */
    /*   Node * tmp = NULL; */
    /*   for (auto i : nodeLoop(a) ){ */
    /*     for (auto j : nodeLoop(b) ){ */
    /*       if ( i == j ) tmp = i; */
    /*       break; */
    /*     } */
    /*   } */


    /*   mFace.push_back(f); */
    /*   mHalfEdge.push_back(ea); */
    /*   mHalfEdge.push_back(eb); */
    /*   mHalfEdge.push_back(ec); */




#endif
