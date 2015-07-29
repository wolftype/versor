/*
 * =====================================================================================
 *
 *       Filename:  xTransforms.cpp
 *
 *    Description:  pair transform
 *
 *        Version:  1.0
 *        Created:  04/21/2015 12:48:20
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
 
    auto ca = Construct::circle(0,0,0);
  
    auto pa = cga::point(ca,PIOVERTWO);
    auto pb = cga::point(ca,-PIOVERTWO);

    auto pair = pa^pb;

    auto bst = Gen::boost( pair * amt );
    auto tp = Round::location( cga::point(1,0,0).spin( bst ) );

    Draw( ca ); Draw( cga::sphere(tp,.1), 1,0,0 );

  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
