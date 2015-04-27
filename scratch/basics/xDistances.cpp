/*
 * =====================================================================================
 *
 *       Filename:  xDistances.cpp
 *
 *    Description:  scratch for distance metrics
 *
 *        Version:  1.0
 *        Created:  08/12/2014 12:28:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_cga3D_app.h"   

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {

   Frame frame;

  //Some Variables
  bool button = false;
  float amt = 0;

  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(button,"bReset");
    ///Add circle to objectController
    objectController.attach(&frame);
  }

  void onDraw(){ 

      App::calcMouse3D();

      auto p3d = mMouse3D;
      draw(p3d,1,0,0);

      auto p =  mMouse2D;

      draw( frame.cxy() );
      draw( p );

      float dist = ( p <= frame.cxy() ).wt();
      cout << "circle: " << dist << endl;  
      cout << Round::radius( frame.cxy() ) << " radius" << endl;
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}


