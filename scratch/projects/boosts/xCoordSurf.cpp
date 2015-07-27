/*
 * =====================================================================================
 *
 *       Filename:  xCoordSurf.cpp
 *
 *    Description:  coordinate surfaces
 *
 *        Version:  1.0
 *        Created:  07/13/2015 14:14:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_differential.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;


  Frame fa,fb,fc;

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

    Point pa = make::point(0,0,0);
    Point pb = make::point(1,0,0):
    Point pc = make::point( Vec::x.rotate( Biv::xy * amt ) );

    auto tx = fa.tx();
    auto tx = fa.tx();
    auto tx = fa.tx();
    
    Draw(pa);Draw(pb);Draw(pc);
    
    auto vecA =  Vec(pb-pa);
    auto vecB = Vec(pc-pa);

  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
