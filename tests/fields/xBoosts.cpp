
/*
 * =====================================================================================
 *
 *       Filename:  xWavePropagation.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/22/2014 14:02:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */

//#include "vsr_generic_op.h"  
#include "vsr_cga2D_types.h"
#include "vsr_cga2D_draw.h" 

#include "vsr_cga3D_app.h"

using namespace vsr;
using namespace vsr::cga2D;


struct MyApp : App {    
   
  Point mouse;

  float time=0;
  float amt=0;

  const int numPoint = 1000;
  const int numPair = 100;

  Point point[numPoint];

  Pair pair;//[numPair];

  void initGui(){
  void setup(){
      bindGLV():

      gui(amt,"amt",-100,100);
      amt = 50;
  }
  
  void getMouse(){
    auto tm = calcMouse3D();//jinterface.mouse.projectMid;     
    mouse = Round::null( tm[0] * amt, tm[1] * amt  ); 

  }

   virtual void onDraw(){         
      getMouse();

      pair = Ro::null(-5,0) ^ Ro::null(5,0);

  }
   
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;
}
