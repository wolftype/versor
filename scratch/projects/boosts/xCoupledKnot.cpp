/*
 * =====================================================================================
 *
 *       Filename:  xCoupledKnot.cpp
 *
 *    Description:  coupled knot orbits
 *
 *        Version:  1.0
 *        Created:  10/19/2015 11:55:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
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
   
  Point mouse = Construct::point(0,0,0);

  float time,iter;
  float amt,amt2;

  TorusKnot tk,tk2;

  float mP,mQ,theta,phi;
  float mP2,mQ2,theta2,phi2;

  float linewidth,pointsize;

  bool bDrawKnot, bDrawHopf,bDrawPreImage;

  void setup(){
      bindGLV();
      
      gui(iter,"iter",0,10000);
      gui(amt,"amt",-100,100);
      gui(mP,"p",0,100);
      gui(mQ,"q",0,100);
      gui(theta,"theta",0,1);
      gui(phi,"phi",-1,1);
      gui(amt2,"amt2",-100,100);
      gui(mP2,"p2",0,100);
      gui(mQ2,"q2",0,100);
      gui(theta2,"theta2",0,1);
      gui(phi2,"phi2",-1,1);
      gui(bDrawKnot,"bDrawKnot");
      gui(bDrawHopf,"bDrawHopf");
      gui(bDrawPreImage,"bDrawPreImage");
      gui(bSortOutput,"bSort");
      gui(bShadedOutput,"bShade");
      gui(linewidth,"linewidth",0,10);
      gui(pointsize,"pointsize",0,20);

      scene.camera.pos(0,0,10);

      iter=100;

      phi = phi2 = -1;
      mP=mQ=mP2=mQ2=0;
  }

  virtual void onDraw(){ 

    if (bSetMouse) mouse = calcMouse3D();

    tk.P = mP; tk.Q = mQ; tk.amt=amt;
    tk2.P = mP2; tk2.Q = mQ2; tk2.amt=amt2;

    tk2.HF.cir() = CXZ(1).trs(2,0,0);
    tk.HF.vec() = Vec::x.spin( Gen::rot(theta*PI,phi * PIOVERTWO) );
    tk2.HF.vec() = Vec::x.spin( Gen::rot(theta2*PI,phi2 * PIOVERTWO) );

    
    auto bst = tk2.bst() * tk.bst();
    
    vector<Pnt> pnt;
    vector<Cir> cir;

    Pnt np = mouse;
    for (int i =0;i<iter; ++i){
      np = Round::location( np.spin( bst ) );
      pnt.push_back(np);
    }

    for (int i=0;i<iter-1;++i){
      int next= (i<iter-1) ? i+1:0;
      auto tcir = (pnt[i]^pnt[next]).dual();
      cir.push_back(tcir);
    }


    if (bDrawHopf){
      
     gl2psPointSize((int)pointsize);
     gl2psLineWidth((int)linewidth);
     

     Draw(tk.HF.fiberA(),.6,.2,.6);
     Draw(tk.HF.fiberB(),.2,.4,.2);

     Draw(tk2.HF.fiberA(),.2,.6,.2);
     Draw(tk2.HF.fiberB(),.2,.2,.6);


     auto pair = Construct::pair( Dls(0,0,0,1,-.5), tk.HF.vec() );

    }

  
    if (bDrawKnot){

     gl2psPointSize(1);
     gl2psLineWidth(1);

     auto m = Shape::Skin(cir, cir.size(), false, 50);
     m.drawElementsColor();
    }


  
  }

  /*-----------------------------------------------------------------------------
   *  Keyboard control (hit 's' to toggle mouse tracking)
   *-----------------------------------------------------------------------------*/
//  void onKeyDown(const gfx::Keyboard& k){
//    switch (k.code){
//      case 's': bMouseSet = !bMouseSet;
//    }
//
//    /// Default
//    App:onKeyDown(k);
//  }   

  
};



int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
