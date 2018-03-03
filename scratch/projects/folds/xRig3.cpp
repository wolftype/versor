/*
 * =====================================================================================
 *
 *       Filename:  xRig3.cpp
 *
 *    Description:  rig3 net
 *
 *        Version:  1.0
 *        Created:  07/21/2015 18:12:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_rigid.h"
#include "form/vsr_field.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt, theta,idxU,idxV = 0;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt_translate",-100,100);
    gui(theta,"theta",-100,100);
    gui(idxU,"idxU",0,3);
    gui(idxV,"idxV",0,3);

    gui(bReset,"bReset");
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

     int w = 3;
     int h = 3;
      
     /// STARTING FIELD
     Field<Point> f(w,h,1);
     for (int i=0;i<w;++i){
      for (int j=0;j<h;++j){
        if ( (i>0) && (i<w) && (j&1) ) f.at(i,j) = f.at(i,j).translate(amt,0,0);
      }
     }

     Draw(f);

     /// Rigids (1 per point)
     vector<Rig4> rigid(f.num());

     /// initialize
     for(int i =0;i<f.num();++i){
        rigid[i].initResult( f[i] ); 
     }

     for (int i=0;i<w;++i){
      for (int j=0;j<h;++j){
        
        
      
      }
     }
     

     // For three 
//     for (int i=0;i<w;++i){
//      for (int j=0;j<h;++j){
//          
//        int a,b,c;
//        if (i>0){
//         if (j>0){
//           a = f.idx(i-1,j);
//           b = f.idx(i-1,j-1);
//           c = f.idx(i,j-1);
//
//         } else {
//           a = f.idx(i,j+1);
//           b = f.idx(i-1,j+1);
//           c = f.idx(i-1,j);
//         }
//        } else {
//         if (j>0){
//           a = f.idx(i, j-1);
//           b = f.idx(i+1,j-1);
//           c = f.idx(i+1,j);
//          } else {  
//           a = f.idx(i+1,j);
//           b = f.idx(i+1,j+1);
//           c = f.idx(i,j+1);
//          }
//
//        }
//        
//        rigid[ f.idx(i,j) ].set( &rigid[a],&rigid[b],&rigid[c], false, true );
//      }
//     }

     int idx = f.idx(2,0);

     rigid[idx].result = Constrain::Double( Constrain::Distance(f.at(1,0), f.at(2,0) ), Constrain::Distance(f.at(1,1), f.at(2,0) ), theta );
     rigid[idx].bCalc = false;
      
     rigid[idx].up();
     
     //eval();
     //rigid[t2].eval();

      for (auto& i : rigid){
        Draw( i.result,0,1,0);
        Draw( i.meet(),1,0,0);
       // cout << i.mChild.size() << endl;
      }

      //Draw ( Round::dls( f.at( (int)idxU, (int)idxV), .2), 0,1,0);
    //  Draw( rigid[ f.idx( (int)idxU, (int)idxV) ].meet(),1,0,0);

    //  Draw( rigid[ f.idx( (int)idxU, (int)idxV) ].mDistance[2](),0,1,0,.2);


  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
