/*
 * =====================================================================================
 *
 *       Filename:  xAmbientOcclusion.cpp
 *
 *    Description:  test of ambient occlusion on a mesh
 *
 *        Version:  1.0
 *        Created:  04/03/2014 12:56:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

#include "vsr_knot.h"
#include "vsr_shapes.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,num;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(num,"num",0,100);
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

      static Pnt pnt = Ro::null(2,0,0);
      Touch(interface,pnt); Draw(pnt,1,0,0);

      TorusKnot tk(3,2,amt);

      tk.HF.vec() = mouse;
      tk.calc(pnt);

      for(auto& i : tk.cir) {
        i = Ro::real(i);
        Draw(i,0,0,1);
      }

      Mesh m = Shape::Skin( tk.cir, tk.cir.size() - num, 10 );

      for(auto& i : m.vertex()){
          


      }

      m.drawElements();
    
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
