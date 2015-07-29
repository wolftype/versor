/*
 * =====================================================================================
 *
 *       Filename:  xConformalDeformation.cpp
 *
 *    Description:  (quasi?) conformal deformations of a surface
 *
 *        Version:  1.0
 *        Created:  04/29/2015 11:05:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_field.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt, radius = 0;


  Field<Point> f;

  Frame frame = Frame(0,0,5);
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(radius,"radius",-100,100)(bReset,"bReset");

    f.resize(10,10,1);

    objectController.attach(&frame);
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
 
 
    Draw(f);
    Draw(frame);
 

    for (int i=0;i<f.num();++i){
      auto circle = Construct::circle( f[i], radius );
      Draw(circle,1,0,0);

      auto nc =frame.cxy().reflect(circle);

      auto transform = Gen::con(nc, frame.cxy(), amt);
      auto tc = nc.spin(transform);
      if (bReset) Draw( tc, 0,1,0);
      Draw( Round::location(tc), 0,1,1);

    }
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
