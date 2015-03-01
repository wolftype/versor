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

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      getMouse();

      time += .1;

      auto line = LN(1,0,0);

      Draw(line,0,0,1);

      auto boost = Gen::boost( Pair(TangentVector(0,1,0)).rotate( Biv::xy * time ) * amt );

      auto cir = Circle(line).spin(boost);


      Draw( Round::pnt_cir( cir,  PIOVERFOUR * .5 ),1,0,0 );

      Draw(cir,0,1,0);
    
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
