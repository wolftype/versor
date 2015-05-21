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
   
  Point mouse;

  float time,speed;
  float amt;

  HopfFiber hf;

  float mP,mQ,theta,phi,radius;

  void setup(){
      bindGLV();
      
      gui(amt,"amt",-100,100);
      gui(mP,"p",1,100);
      gui(mQ,"q",1,100);

      gui(radius,"radius",-100,100);
      gui(speed,"speed",-100,100);
      gui(theta,"theta",-PI,PI);
      
      gui(phi,"phi",-PIOVERTWO,PIOVERTWO);

      scene.camera.pos(0,0,10);
  }


  virtual void onDraw(){ 

  //  glPolygonMode( GL_FRONT, GL_LINE );  

    time += speed;
    vector<Cir> cir;

    auto rot = gen::rot( theta, phi );

    Draw( Vec::x.spin(rot) );

    int slices = 20;
    int stacks = 20;
    for (int i = 0;i<slices;++i){
      for (int j = 0;j<stacks;++j){
        auto rot = gen::rot( -PI + TWOPI * i/slices, -PIOVERTWO + PI * j/stacks);
        auto vec = Vec::x.spin(rot);

        auto rot2 = gen::rot( time,  -PIOVERTWO + PI * j/stacks);
        hf.vec() = Vec::x.spin(rot2);
        auto tcirA = hf.fiberA().translate( vec * radius );
        auto tcirB = hf.fiberB().translate( vec * radius );
        
        Draw(tcirA, (float)i/slices,(float)j/stacks,1);
        Draw(tcirB, (float)i/slices,(float)j/stacks,1);
      }
    }
//    for (auto& i : pnt){
//       Vec v = Vec(i).unit();
//       Draw(v.null(),1,0,0);
//       hf.vec() = v;
//       cir.push_back(hf.fiberA());      
//    }
//
//    auto m = Shape::Skin(cir, cir.size(), 15);
//    m.drawElements();
  
  }
   

  
};



int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
