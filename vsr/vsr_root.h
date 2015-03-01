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
#include "vsr_products.h"   
#include "vsr_generic_op.h"

using namespace std;

namespace vsr{


/*!-----------------------------------------------------------------------------
 * CALCULATES POLYTOPES FROM SIMPLE ROOTS IN ND (beware infinite groups . . .)
 *
 * Systems can be reflection groups, which include inverses, or not
 *-----------------------------------------------------------------------------*/
struct Root{

  ///Utility function to compare two vectors (looks at dot product, or norm of diff...)           
  template<class V>
  static bool Compare(const V& a, const V& b, bool ref = true){   
//    return ( ( a-b ).wt() ) < .000001; 
    return ( ref ? (a<=b)[0] > .9999999 : fabs ((a<=b)[0]) > .9999999 );// amt; 
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

      return System( initial );
   }
    
   // Build a Root System from A vector of Simple Roots
   template<class V>
   static vector<V> System( const vector<V>& root, int nMaxIter = 5000){

    //Copy simple roots into results first
    vector< V > results = root;

    int n = root.size();                        //<-- Number of Simple Roots

    bool keepGoing = true;
    int iter = 0; 
    while (keepGoing){   
        iter++;
        bool done = true;
        
        int cs = results.size(); 

        for (int i = 0; i < cs; ++i){  
        
          int ns = results.size();
        
          for (int j = 0; j < ns; ++j ){

              V nr = results[j].reflect( results[i] ); 

              bool exists = 0; 
              for ( int k = 0; k < ns; ++k){ 
                exists = ( Compare(nr.unit(),results[k].unit()) );
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
        
        if (done || (iter > nMaxIter) ) { keepGoing = false; } // if not, then stop
 
   }


   return results;
}  
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
