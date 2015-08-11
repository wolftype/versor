// Versor
// Author: pablo colapinto
//
// this file was written with the help of Pierre Dechant
//
// given a simple root, find all the others through reflections
//
// used e.g. in vsr_groups.h to create reflection groups
//
//


#ifndef ROOT_SYSTEM_INCLUDED
#define ROOT_SYSTEM_INCLUDED
                      
#include <vector>
#include "detail/vsr_generic_op.h"

using namespace std;

namespace vsr{

//template<class V>
//struct SymmetryGroup {
//
//  using Pin1 = V;
//  using Pin2 = decltype(V()*V());
//  using Pin3 = decltype(V()*V()*V());
//
//  vector<Pin1> p1; 
//  vector<Pin2> p2; 
//  vector<Pin3> p3; 
//
//  ///Utility function to compare two vectors (looks at dot product, or norm of diff...)  
//  /// if ref=false then 180 degree reflections are considered equal         
//  static bool Compare(const V& a, const V& b, bool ref = true){   
//    return ( ref ? (a<=b)[0] > .99999 : fabs ((a<=b)[0]) > .99999 );// amt; 
//  } 
//
//  //seed generators with this until nothing new comes out
//  //probes will store transformation operator and result
//  struct VecProbe {
//
//    V vec = V(1.2234578,2.838967,.123689).unit();
//
//    Pin1 pin1; Pin2 pin2; Pin3 pin3;
//
//    enum { PIN1, PIN2, PIN3 };
//    int xf; ///< type of transformaiton used to make this 
//
//    VecProbe(){}
//
//    VecProbe( const V& input, const Pin1& versor) : xf(PIN1), pin1(versor), vec( input.reflect(versor)) {}
//    VecProbe( const V& input, const Pin2& rotor) : xf(PIN2), pin2(rotor), vec( input.spin(rotor)) {}
//    VecProbe( const V& input, const Pin3& rotvec) : xf(PIN3), pin3(rotvec), vec( input.reflect(rotvec)) {}
//
//  };
//
//  SymmetryGroup& add(const Pin1& _p1) { p1.push_back(_p1); }
//  SymmetryGroup& add(const Pin2& _p2) { p2.push_back(_p2); }
//  SymmetryGroup& add(const Pin3& _p3) { p3.push_back(_p3); }

 /// goal is to use generators to collect all the unique transformations of the group, 
 /// so we can apply them easily later. multiply all pins by all pins until none left, same for spins,
 /// and same for pins with spins ( and spins with pins?)
//  void System() {
// 
//     vector<VecProbe> results;
//     for (auto& j : p1 ){
//         results.push_back( VecProbe(j,j) );
//     }    
//     //pins of pins...
//     bool keepGoing = true;
//     while (keepGoing){ 
//         bool done = true;        
//         int cs = results.size();        // for all probes, add new probes until none are new
//                                         // recording transformation sequences along the way
//         for (int i=0; i<cs; ++i){
//           int ns = results.size();  
//          
//             for (int j=0;j<ns; ++j){
// 
//               auto nr = VecProbe(results[i].vec, results[j].versor ); 
// 
//               bool exists = 0; 
//               for ( int k=0; k<cs; ++k){ 
//                 exists = ( Compare(nr.vec.unit(),results[k].vec.unit(), false) );
//                 if (exists) {  
//                   break;
//                 }
//               }
// 
//               if (!exists)  { 
//                 results.push_back( nr );
//                 done = false;  //if even one is new, try them all again
//               }   
//             }
//         } 
//         // if none are new or results is too big, end loop
//         if (done || (results.size() > 500) ) { keepGoing = false; } // if not, then stop
// 
//     }
// 
//   }


//};


/*!-----------------------------------------------------------------------------
 * CALCULATES POLYTOPES FROM SIMPLE ROOTS IN ND (beware infinite groups . . .)
 *
 * Systems can be reflection groups, which include inverses, or not
 *-----------------------------------------------------------------------------*/
struct Root{


  //indices into a reflection operation group and its results
  struct ReflectIdx{
    int opIdx, resIdx;
  };

  ///Utility function to compare two unit vectors (looks at dot product, or norm of diff...)           
  template<class V>
  static bool Compare(const V& a, const V& b, bool ref = true){   
//    return ( ( a-b ).wt() ) < .000001; 
    return ( ref ? (a<=b)[0] > .99999 : fabs ((a<=b)[0]) > .99999 );// amt; 
  }            
    
    //Build a Root System from Simple Root Generators (ANY Dimension!)
   template<class V, class ... R>
   static vector<V> System( const V& x, const R&... v ){
      
      vector< V > initial;                      //<-- Initial Simple Roots
    
      int n = sizeof...(R) + 1;

      V root[] = {x, v... };
    
      for ( int i = 0; i < n; ++i ){
         initial.push_back( root[i].unit() );    
      } 

      return System( initial, false ); //flag for repeating opposite sides
   }
    
   // Build a Root System from A vector of Simple Roots
   template<class V>
   static vector<V> System( const vector<V>& root, bool ref=true, int nMaxIter = 500){

    //Copy simple roots into results first
    vector< V > results = root;

   int n = root.size();                        //<-- Number of Simple Roots

    bool keepGoing = true;
    int iter = 0; 
    while (keepGoing){   
        iter++;
        bool done = true;        
        int cs = results.size(); 

        for (int i=0; i<cs; ++i){  
          int ns = results.size();
        
          for (int j=0; j<ns; ++j ){

              V nr = results[j].reflect( results[i] ); 

              bool exists = 0; 
              for ( int k=0; k<ns; ++k){ 
                exists = ( Compare(nr.unit(),results[k].unit(),ref) );
                if (exists) {  
                  break;
                }
              }

              if (!exists)  { 
                results.push_back( nr );
                done = false;  //if even one is new, try them all again
              }   
            }
        } 
        
        if (done || (results.size() > nMaxIter) ) { keepGoing = false; } // if not, then stop
 
   }
   return results;
  }
  

   /*-----------------------------------------------------------------------------
    *  Calculate number of extra reflections to calculate for a given (unit) seed vector (beyond System results)

       @param vec a random seed vec (must be unit)
       @param op a vector of vec versors
    *-----------------------------------------------------------------------------*/
//  template<class V>
//  static vector<ReflectIdx> Reflections(const V& vec, const vector<V>& op){ //<-- vec can be random or specifc axis MUST BE UNIT
//
//    vector<ReflectIdx> rIdx;
//    
//    //FIRST PASS
//    vector<Vec> vvec;
//     
//    for(auto& i : op){
//      auto tvec = vec.reflect(i);
//      vvec.push_back(tvec);
//    }
//
//    //Keep doing it until none new, record results in rIdx
//    bool keepgoing=true;
//    while(keepgoing){
//      
//      keepgoing=false;
//      int tn = vvec.size();
//      for(int i =0; i<op.size(); ++i){
//        for (int j=0;j<tn;++j){
//      
//          auto tvec = vvec[j].reflect( op[i] );
//   
//          bool exists = false;
//          for (auto& k : vvec){
//            exists = Root::Compare(tvec,k);
//            if (exists) {
//              break;
//            }
//           }
//   
//          if (!exists){
//            vvec.push_back(tvec);
//          //  cout <<  i << " " << j << endl;
//            rIdx.push_back( {i,j} );
//            keepgoing=true;
//          }
//        }
//      }
//    }
//
//    return rIdx;
//  }

};

            
} //vsr::
#endif





    //FIRST PASS: reflect all roots around each other, and save if new 
    /* for (int i = 0; i < n; ++i){ */
    /*     for (int j = 0; j < n; ++j){ */

    /*     V nr = root[i].re(root[j]);             //<-- Reflection */
       
    /*     bool exists = 0; */
    /*     for ( int k = 0; k < results.size(); ++k){ */
    /*       exists = ( Compare(nr,results[k]) );  //<-- Use the Compare function */
    /*       if (exists) break; */
    /*     } */   
        
    /*     if (!exists) results.push_back( nr ); */ 
         
    /*   } */ 
    /* } */   
    
    //Okay, now repeat reflections until no new ones are created
    /* bool keepGoing = true; */ 
      
    /*   while (keepGoing){ */   
    /*     //For EACH simple root */
    /*     for (int i = 0; i < n; ++i){ */
    
    /*         //How big is results so far? (this will change every loop) */
    /*         int cs = results.size(); */
            
    /*         for (int j = 0; j < cs; ++j ){ */
       
    /*             bool done = true; //expect to be finished unless . . . */
    /*             V nr = results[j].re( root[i] ); */
                                  
    /*             bool exists = 0; */ 
    /*             for ( int k = 0; k < cs; ++k){ */ 
    /*               exists = ( Compare(nr,results[k]) ); */
    /*               if (exists) { */  
    /*                   break; //<-- break out of this mini search loop */
    /*               } */
    /*             } */
        
    /*             if (!exists)  { */ 
    /*               results.push_back( nr ); */
    /*               done = false; //... we reset here and do it again */
    /*             } */   
      
    /*             if (done) { keepGoing = false; } */

    /*           } */
    /*         } */  
    /*     } */  
