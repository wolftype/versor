  /*
 * =====================================================================================
 *
 *       Filename:  xKnotMesh.cpp
 *
 *    Description:  knot tube
 *
 *        Version:  1.0
 *        Created:  10/14/2015 18:51:35
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

  float time;
  float amt;

  TorusKnot tk;

  float mP,mQ,theta,phi;

  float linewidth,pointsize;

  bool bDrawKnot, bDrawHopf,bDrawPreImage;

  void setup(){
      bindGLV();
      
      gui(amt,"amt",-100,100);
      gui(mP,"p",1,100);
      gui(mQ,"q",1,100);
      gui(theta,"theta",0,1);
      gui(phi,"phi",-1,1);
      gui(bDrawKnot,"bDrawKnot");
      gui(bDrawHopf,"bDrawHopf");
      gui(bDrawPreImage,"bDrawPreImage");
      gui(bSortOutput,"bSort");
      gui(bShadedOutput,"bShade");
      gui(linewidth,"linewidth",0,10);
      gui(pointsize,"pointsize",0,20);

      scene.camera.pos(0,0,10);
  }

  virtual void onDraw(){ 

    if (bSetMouse) mouse = calcMouse3D();

    tk.P = mP; tk.Q = mQ; tk.amt=amt;

    tk.calc( mouse );

    vector<Cir> cir;

    for (int i=0;i<10;++i){
      auto tcir = CXZ(1).trs(0,3*(float)i/10,0);
      cir.push_back(tcir);
    }


    if (bDrawHopf){
     gl2psPointSize((int)pointsize);
     gl2psLineWidth((int)linewidth);
     
     tk.HF.vec() = Vec::x.spin( Gen::rot(theta*PI,phi * PIOVERTWO) );

     Draw(tk.HF.fiberA(),.6,.2,.6);
     Draw(tk.HF.fiberB(),.2,.4,.2);

     auto pair = Construct::pair( Dls(0,0,0,1,-.5), tk.HF.vec() );

     if (bDrawPreImage){
     Draw( tk.HF.vec(), .5,.2,0);
     Draw( Construct::pointA(pair), .6,.2,.6);
     Draw( Construct::pointB(pair), .2,.4,.2);
     Draw( Dls(0,0,0,1,-.5), 0,1,1,.4);
     }

    }

  
    if (bDrawKnot){
    // Draw(Line(tk.HF.fiberA()),.6,.2,.6);
    // Draw(tk.HF.fiberB(),.2,.6,.2);
     gl2psPointSize(1);
     gl2psLineWidth(1);

     auto m = Shape::Skin(tk.cir, tk.cir.size(), true, 50);
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
