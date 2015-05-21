/*
 * =====================================================================================
 *
 *       Filename:  xHyperbolic.cpp
 *
 *    Description:  hyperbolic 3d
 *
 *        Version:  1.0
 *        Created:  07/24/2014 16:53:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_app.h"  
#include "form/vsr_group.h" 


using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {    
   
  Pnt mouse;

  float time;
  float amt;
  float radius;

  bool bReset;

  void setup(){
      bindGLV();
      
      gui(amt,"amt",-100,100);
      gui(radius,"radius",0,100);
      gui(bReset,"reset");

      radius =1;
  }
  
  void onDraw(){ 
    
    int base = 5;

    PointGroup3D<Vec> pg(5,3);
    
  //  for (auto& i : pg.ops){ Draw(i); }

    //cout << pg.ops.size() << endl;

    auto cir = CXY(1).trs(0,0,radius);
    vector<Circle> vcir;
       
    //FIRST
   // auto vec = Vec(.23333,.622115,1.3925).unit(); //<--random seed vector
   
    auto vec = Vec::z;
    auto ridx = Root::Reflections( vec, pg.ops );
    
    vector<Vec> vvec;
     
    for(auto& i : pg.ops){
      vvec.push_back(vec.reflect(i));
    }
    
  //  int iter =0;
    for (auto& i : ridx){
      auto tvec = vvec[ i.resIdx ].reflect( pg.ops[ i.opIdx] );
      vvec.push_back(tvec);
    }

 //   for (auto& i : pg.ops){
 //     vcir.push_back( cir.reflect(i) );
 //   }

 //   for (auto& i : rIdx){
 //     auto tcir = vcir[ i.resIdx ].reflect( pg.ops[ i.opIdx] );
 //     vcir.push_back(tcir);
 //   }

     for (auto& i : vvec){ Draw(i); }
  //   for (auto& i : vcir){ Draw(i); }



 //   //Keep doing it . . .
 //   int iter =0;
 //   bool keepgoing=true;
 //   while(keepgoing){
 //     
 //     keepgoing=false;
 //     int tn = vvec.size();
 //     for(int i =0; i<pg.ops.size(); ++i){
 //       for (int j=0;j<tn;++j){
 //     
 //         auto tvec = vvec[j].reflect( pg.ops[i] );
 //  
 //         bool exists = false;
 //         for (auto& k : vvec){
 //           exists = Root::Compare(tvec,k);
 //           if (exists) {
 //             break;
 //           }
 //          }
 //  
 //         if (!exists){
 //           vvec.push_back(tvec);
 //           cout << iter << " " << i << " " << j << endl;
 //           rIdx.push_back( {iter,i,j} );
 //           keepgoing=true;
 //         }
 //       }
 //     }
 //     iter++;
 //   }
    


//    bool keepgoing =true;
//    while(keepgoing){
//      keepgoing=false;
//      for(auto& i : pg.ops){
//      
//        for (auto& j : vvec){
//      
//          auto tvec = j.reflect(i);
//   
//          bool exists = false;
//          for (auto& k : vvec){
//            exists = Root::Compare(tvec.unit(),k.unit());
//            if (exists) {
//             // cout << k << "\n" << tvec << endl;
//              break;
//            }
//           }
//   
//          if (!exists){
//            vvec.push_back(tvec);
//            keepgoing=true;
//            //cout << "NEW" << tcir << endl;
//          }
//      }
//     }
//    }
     
   
    }
   
};


int main(){
                             
  MyApp app;
  app.start();
  return 0;

}


//    //FIRST
//    vector<Vec> res;
//    auto seed = Vec(.23,.621,1.5);
//    
//    for (auto& i : pg.ops ){
//      auto tvec = seed.reflect(i);
//      res.push_back(tvec);
//    }
//
//    //SECOND
//    for (auto& i : pg.ops )
//      for (auto& j : res ){
//        auto tvec = j.reflect(i);
//        bool exists;
//        for(auto& k : res){
//          exists = Root::Compare(k,tvec);
//          if (exists) break;
//        }
//        if (!exists){
//          res.push_back(tvec);
//          ops2.push_back(i);
//        }
//          
//        }
//      }
//    }
//
