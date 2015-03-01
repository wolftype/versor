
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

#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga2D;



struct MyApp : App {    
   
  Point mouse;

  float time;
  float amt;

  const int numPoint = 1000;
  const int numPair = 100;

  Point point[numPoint];

  Pair pair;//[numPair];

  MyApp(Window * win ) : App(win),
  box(8,8)
  {
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      amt = 50;
  }
  
  void getMouse(){
    auto tm = interface.mouse.projectMid;     
    mouse = Round::null( tm[0] * amt, tm[1] * amt  ); 

  }

  virtual void update(){
  
    
 
  }


   virtual void onDraw(){         
      getMouse();

      pair = Ro::null(-5,0) ^ Ro::null(5,0);


      
  }
   
  
};


MyApp * app;


int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(500,500,"Versor",&glv);    
  app = new MyApp( win ); 
  app -> initGui();
  
  
  glv << *app;

  Application::run();

  return 0;

}
