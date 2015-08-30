/*
 * =====================================================================================
 *
 *       Filename:  xOrient.cpp
 *
 *    Description:  example orientation keeping y axis as vertical as possible
 *
 *        Version:  1.0
 *        Created:  08/28/2015 12:55:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    scene.camera.lens.bOrtho = bReset;
    Vec target(2,2,2);
    //Draw( Construct::point(2,2,2),1,0,0 );

    Frame fa;
    Frame fb;
    Frame fc;
    
    fb.rot() = Gen::ratio( Vec::z, target.unit() );
    fc.orient(target);


    fa.pos(-2,0,0);
    fc.pos(2,0,0);
//    Draw(fa);
//    if (bReset) {
      DrawB(fa);
     // DrawAt(target.unit(),fa.pos(),0,0,1,.5);
      Draw(fb.xy(),.3,.3,.2);
      DrawB(fb);
      DrawAt( Vec(Op::project( Vec::y, fb.xy() )).unit(), fb.pos(), 0,1,0,.5 );
//    }
//    else {
      
      DrawB(fc);
//    }

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}

