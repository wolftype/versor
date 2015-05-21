/*
 * =====================================================================================
 *
 *       Filename:  xConformalCoord.cpp
 *
 *    Description:  conformal coordinate systems a la dorst and valkenburg
 *
 *        Version:  1.0
 *        Created:  08/11/2014 12:29:15
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
#include "vsr_cga3D_frame.h"
#include "vsr_fiber.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,amt2,iter,P,Q;

  bool bReset,bRealA,bRealB,bNorm,bDrawCoords;
  float weight, scaleA, scaleB, radius;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100)(amt2,"amt2",-100,100);
      gui(scaleA,"scaleA",-100,100)(scaleB,"scaleB",-100,100);
      gui(P,"P",0,100)(Q,"Q",0,100);
      gui(iter,"iter",1,10000);
      gui(bReset,"reset");
      gui(weight,"weight",-100,100)(radius,"radius",-100,100);
      gui(bNorm, "normalize");
      gui(bRealB,"realA");
      gui(bRealA,"realB");
      gui(bDrawCoords,"drawcoords");

      amt = .03; amt2 = .02;
      iter = 1000;
      scaleA = scaleB = 1;

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

      //Frame
      static Frame fa(0,0,0);
      Touch(interface, fa);
      //Draw(fa);

      //set fiber to frame
      HopfFiber hopf;
      hopf.vec() = fa.y();
      Draw( fa.cxz() );

      /* auto px = fa.py(); */
      /* auto ipx = fa.ipy(); */
      /* auto dll = fa.dly(); */
      auto ra = Ro::real( hopf.fiberA().dual() );
      auto rb = Ro::real( hopf.fiberB().dual() );

      auto pa = bRealA ? ra : hopf.fiberA().dual();
      auto pb = bRealB ? rb : hopf.fiberB().dual();

      Draw(pa,1,0,0); Draw(pb,0,0,1);

      auto bst = Gen::bst( pa.dilate( scaleA ) * amt + pb.dilate( scaleB ) * amt2 );
      auto p = mouse;
      p[3] += weight;
      p = Ro::dls_pnt(p, radius);
      for (int i = 0; i < iter; ++i){
        auto np = bNorm ? Ro::loc( p.spin(bst) ) : p.spin(bst);
        p = np;
        float t = (float) i/iter;
        auto pp = bNorm ? p : Ro::loc(p);
        Draw( pp,t,0,1-t);
        if (bDrawCoords){
          Draw( pp ^ pa, 1,0,1);
          Draw( pp ^ pb, 0,1,0);
        }
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

