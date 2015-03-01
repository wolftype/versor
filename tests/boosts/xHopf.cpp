/*
 * =====================================================================================
 *
 *       Filename:  xHopf.cpp
 *
 *    Description:  good fibration
 *
 *        Version:  1.0
 *        Created:  03/21/2014 15:30:21
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

#include "vsr_fiber.h"
#include "vsr_knot.h"
#include "vsr_shapes.h"


using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  HopfFiber hf;
  TorusKnot tk;

  float mP,mQ,theta;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(mP,"p",1,100);
      gui(mQ,"q",1,100);
      gui(theta,"theta",-PI,PI);
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

  virtual void update(){

  }

  virtual void onDraw(){ 

   glPolygonMode( GL_FRONT, GL_LINE );    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    vector<Cir> cir;

    tk.P = mP;
    tk.Q = mQ;  
    tk.HF.vec() = -Vec::y.rot( Biv::xy * theta );
    tk.calc0( Ro::null(amt,0,0) );

    for (auto& i: tk.pnt ){
       Vec v = Vec(i).unit();
       Draw(v.null(),1,0,0);
       hf.vec() = v;
       cir.push_back(hf.fiberA());      
    }

    /* for (int i=0; i<100; ++i){ */

    /*   double theta = (double)i/100; */
    /*   double phi = sin(theta*PI*10) * amt; */

    /*   Vec v = Vec::x.sp( Gen::rot( PI * theta, PIOVERFOUR * phi )  ); */
    /*   hf.vec() = v; */

    /*   Draw( v.null(),1,0,0 ); */
      
    /*   cir.push_back( hf.fiberA() ); */
    
    /* } */

    auto m = Shape::Skin(cir, cir.size(), 15);
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
