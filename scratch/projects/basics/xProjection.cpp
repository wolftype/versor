/*
 * =====================================================================================
 *
 *       Filename:  xProjection.cpp
 *
 *    Description:  projection of elements onto circles, etc.
 *
 *        Version:  2.0
 *        Created:  11/25/2013 10:49:16
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *
 * =====================================================================================
 */


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  Frame frame;

  void setup(){
      gui(amt,"amt",-100,100);
  
      objectController.attach(&frame);
  }

  void onDraw(){ 
        
      mouse = calcMouse3D();
 
      Cir cir = frame.cxy();
      Dls dls = frame.bound();

      auto flatmouse = mouse ^ Inf(1);

      auto tangent = Pair( Tnv(0,1,0) ).trs(mouse);

      auto projCir = Op::project(tangent, cir);
      auto projSph = Op::project(tangent, dls.dual());

     // Pair flatPointCircle = projCir;
      Pair flatPointSphere = projSph;
      
      projSph.print();
      
      Draw(mouse);

      Draw(flatPointSphere,1,0,0);

      Draw(tangent^dls,0,1,0);
      

//      Draw(projSph,0,1,0);
//
      Draw(dls,0,0,1,.2);
      
  }

  
};




int main(){
                             
  MyApp app;

  app.start();
  
  return 0;

}
