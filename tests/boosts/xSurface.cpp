/*
 * =====================================================================================
 *
 *       Filename:  xSurface.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/06/2014 16:43:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


//Get mouse position as an "unprojected" point in 3D space

#include "vsr_cga3D_app.h"   

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time=0;
  float amt;

  void setup(){
      bindGLV();
      gui(amt,"amt",-100,100);
  }
 
  void onDraw(){ 
        
      mouse = calcMouse3D();
      
      time += .1;

      auto line = LN(1,0,0);

      Draw(line,0,0,1);

      auto boost = Gen::boost( Pair(TangentVector(0,1,0)).rotate( Biv::xy * time ) * amt );

      auto cir = Circle(line).spin(boost);


      Draw( Round::pnt_cir( cir,  PIOVERFOUR * .5 ),1,0,0 );

      Draw(cir,0,1,0);
    
  }
   
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
