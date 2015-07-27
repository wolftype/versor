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
  float amt, theta = 0;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100);
    gui(theta,"theta",-100,100);
    gui(bReset,"bReset");
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

     Field<Point> f(3,3,1);

     f.at(1,0) = f.at(1,0).translate(amt,0,0);
     
     Draw(f);

     vector<Rig3> rigid(f.num());

     for(int i =0;i<f.num();++i){
        rigid[i].setResult( f[i] ); 
     }

     rigid[ f.idx(1,1) ].set( f.at(1,1), &rigid[ f.idx(0,0) ], &rigid[ f.idx(1,0) ], &rigid[ f.idx(2,0) ], true, false);

     rigid[ f.idx(2,0) ].result = Constrain::Double( Constrain::Distance(f.at(1,0), f.at(2,0) ), Constrain::Distance(f.at(1,1), f.at(2,0) ), theta );
     
     //Draw(f.at(2,0));
      
      Draw( rigid[f.idx(2,0)].result,0,1,0);
      Draw( rigid[f.idx(1,1)].eval(), 1,0,0);  

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
