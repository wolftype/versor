/*
 * =====================================================================================
 *
 *       Filename:  xAnapole.cpp
 *
 *    Description:  toroidal rotation of one circle around another
 *
 *        Version:  1.0
 *        Created:  06/01/2015 15:17:41
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
  float amtB = 0;

  float timer = 0;

  Frame frame;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    gui(amtB,"amtB",-100,100);

    objectController.attach(&frame);
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    auto cir = frame.cxz();

    auto tcir = CXZ(2).rot( Biv(amtB,0,0) ).trs(amt,0,0);


    Draw(cir,1,0,0);
    
    timer += .01;

    Draw( tcir.spin( Gen::bst( cir.dual() * timer ) ), 0, 1, 0 );

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
