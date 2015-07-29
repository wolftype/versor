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

#include "form/vsr_field.h"
#include "form/vsr_knot.h"
#include "form/vsr_knot.h"
#include "form/vsr_shapes.h"


using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {    
   
  Point mouse;

  float time,speed;
  float amt;

  TorusKnot tk;

  float mP,mQ,theta,phi,radius;

  Field<Pair> f;

  bool bReset, bSetMouse, bMotion;

  void setup(){
      bindGLV();
      
      gui(amt,"amt",-100,100);
      gui(mP,"p",1,100);
      gui(mQ,"q",1,100);


      gui(radius,"radius",-100,100);
      gui(speed,"speed",-100,100);

      //theta phi of poles
      gui(theta,"theta",-PI,PI);
      gui(phi,"phi",-PIOVERTWO,PIOVERTWO);
      
      gui(bReset,"reset");
      f.resize(10,10,10);

      setField();
      scene.camera.pos(0,0,10);
  }

  void onKeyDown(const gfx::Keyboard& k){
    if (k.code=='s') bSetMouse = !bSetMouse;
    if (k.code=='g') bMotion = !bMotion;
  }

  void setField(){
      for (int i =0;i<f.num();++i){
        f[i] = Pair(Tnv(0,1,0)).translate( f.grid(i) );
      }

  }
  virtual void onDraw(){ 

    if (bReset) setField();

    auto rot = Gen::rot( theta, phi );

    auto rvec = Vec::x.spin(rot);

    auto pair = Round::produce( Construct::sphere(0,0,0), rvec );
    
    Draw( rvec );

    if (bSetMouse) mouse = calcMouse3D();
    
    tk.HF.cir() = CXZ(1).translate( mouse );
    tk.P = mP;
    tk.Q = mQ;
    
    auto& hf = tk.HF;
    tk.HF.vec() = rvec;
    auto bst = tk.bst(amt);
    for(int i =0;i<f.num();++i){
      if (bMotion) f[i] = f[i].spin(bst);//Round::location( f[i].spin(bst) );
      else f[i] = Pair(Tnv(0,1,0)).translate(f.grid(i)).spin(bst);// Round::location( f.grid(i).spin(bst) );
    }

    Draw( Construct::pointA(pair),1,0,0 );
    Draw( Construct::pointB(pair),0,1,0 );

    int slices = 20;
    int stacks = 20;
    for (int i = 0;i<slices;++i){
      for (int j = 0;j<stacks;++j){
    //    auto rot = Gen::rot( -PI + TWOPI * i/slices, -PIOVERTWO + PI * j/stacks);
    //    auto vec = Vec::x.spin(rot);

    //    auto rot2 = Gen::rot( time,  -PIOVERTWO + PI * j/stacks);
     //   hf.vec() = rvec;//.spin(rot2);
        auto tcirA = hf.fiberA().translate( rvec * radius );
        auto tcirB = hf.fiberB().translate( rvec * radius );
        
        if (Round::size(tcirA) < 10000) Draw(tcirA, (float)i/slices,1,(float)j/stacks);
        else Draw(Line(tcirA), 0,1,0);
        if (Round::size(tcirB) < 10000) Draw(tcirB, (float)i/slices,1,(float)j/stacks);
        else Draw(Line(tcirB), 0,1,0);
      }
    }


      for (int i=0;i<f.num();++i){
        auto dir = -Round::dir(f[i]).copy<Vec>().unit();
        auto pos = Round::location(f[i]);
        DrawAt(dir,pos,0,1,1);
      }
  }
   

  
};



int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
