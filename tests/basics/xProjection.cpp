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
 
      Draw(mouse);

      static Cir c1 = CXY(1).trs(2,0,0);
      static Cir c2 = CXZ(1).trs(-1,-1,0);

      Touch(interface, c1); Touch(interface,c2);

      Draw(c1); Draw(c2);

      auto s = c1.dual() ^ c2.dual();

      Draw(s);
      
      /* Dls c = Ro::dls(1.0, 0,0,0); */

      /* auto a = Point( Op::project( mouse, c  ) ); */
      /* auto b = Point ( Op::reject( mouse, c ) ); */

      /* Draw(c,1,1,1,.1); */ 

      /* Draw( mouse <= (c ^ Inf(1)) ); */
    
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
