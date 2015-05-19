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



#include "vsr_app.h"   

#include "form/vsr_fiber.h"
#include "form/vsr_knot.h"
#include "form/vsr_shapes.h"


using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  HopfFiber hf;
  TorusKnot tk;

  float mP,mQ,theta;


  void setup(){
      bindGLV();
      
      gui(amt,"amt",-100,100);
      gui(mP,"p",1,100);
      gui(mQ,"q",1,100);
      gui(theta,"theta",-PI,PI);

      scene.camera.pos(0,0,10);
  }


  virtual void onDraw(){ 

   glPolygonMode( GL_FRONT, GL_LINE );    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    vector<Cir> cir;

    tk.P = mP;
    tk.Q = mQ;  
    tk.HF.vec() = -Vec::y.rot( Biv::xy * theta );
    tk.calc0( round::null(amt,0,0) );

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



int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
