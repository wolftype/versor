/*
 * =====================================================================================
 *
 *       Filename:  xFrameKnot.cpp
 *
 *    Description:  a frame around a knot
 *
 *        Version:  1.0
 *        Created:  05/21/2015 15:51:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_knot.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;


  TorusKnot tk = TorusKnot(3,2);
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
  
    Frame frame(3,0,0);
    
    Pnt pnt = frame.pos();
    Rot rot = frame.rot();

    Mot mot = frame.mot();

    for (int i = 0;i < tk.iter();++i){
      auto pair = tk.par() * amt;
     // pnt = Round::loc( pnt.boost(pair) );
      mot = mot.boost( pair );
      Frame frame(mot);
      frame.pos() = Round::loc( frame.pos() );
      frame.rot() = frame.rot().unit();
     // mot = mot.runit();
      //rot = frame.mot().boost( pair );
     // cout << rot << endl;
      Draw( frame );
    }

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
