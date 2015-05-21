/*
 * =====================================================================================
 *
 *       Filename:  xConicCurves.cpp
 *
 *    Description:  parabolic, elliptic, hyperbolic deformations
 *
 *        Version:  1.0
 *        Created:  02/24/2014 17:26:23
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
#include "vsr_conic.h"
#include "vsr_cga3D_frame.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;
  float rad;
  float ori;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(rad,"rad",-100,100);
      gui(ori,"ori",-100,100);
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

      int numPoints = 100;

      static Frame f; 
      Touch(interface,f); 
      DrawAt(f.y(), f.pos(),0,1,0);
      
      auto bend = f.ty() * amt;

      for (int i = 0; i < numPoints; ++i){
        //original
        VT x = (float)i/numPoints;
        auto pt = Ro::pnt_cir( CXY(1), TWOPI * x );
        Draw(pt);
        
        //blue option a (radius)
        VT dist = Ro::sqd( pt, f.pos() ) + .1;
        auto s = Ro::dls_pnt(pt,rad);
        auto dls = s.boost(bend);
        Draw(Ro::loc(dls),0,0,1);

        //green option b (weighted and renormalized)
        auto s2 = pt; s2[3] = ori;
       // s2 /= (s2 <= Inf(1))[0];
        auto dls2 = s2.boost(bend);
        Draw(Ro::loc(dls2),0,1,0);

        //red option c (conic transform)
        auto s3 = Conic::Transform( pt, f.y(), amt);
        Draw(s3, 1,0,0);

      }


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
