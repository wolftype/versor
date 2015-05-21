/*
 * =====================================================================================
 *
 *       Filename:  xReciprocalSphere.cpp
 *
 *    Description:  reciprocal of four points
 *
 *        Version:  1.0
 *        Created:  03/30/2014 16:37:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


/*
 * =====================================================================================
 *
 *       Filename:  xReciprocal.cpp
 *
 *    Description:  reciprocal frames for facet / ray intersection
 *
 *    implementation of an algorithm from "Covarient Approach to Geometry using Geometric Algebra", A. Lasenby, J. Lasenby, R. Wareham, p52)
 *
 *        Version:  1.0
 *        Created:  11/22/2013 13:33:48
 *       Revision:  none
 *       Compiler:  gcc4.7 or clang3.2 or greater (C++11)
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  pretty awesome
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
  float amt, amtA,amtB,amtC;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(amtA,"amtA",-1,1);
      gui(amtB,"amtB",-1,1);
      gui(amtC,"amtC",-1,1);

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

      //Four Points
      Point pa, pb, pc, pd;

      //Init
      pa = PT(-1,1,1);
      pb = PT(2,3,.2);
      pc = PT(5,-3,-2);
      pd = PT(0,4,-5);

      //Draw
      Draw(pa); Draw(pb); Draw(pc); Draw(pd);

      //SPHERE Tangent Space
      Sph sphere = pa ^ pb ^ pc ^ pd;

      VT sph_norm = sphere.rnorm();
      VT sph_sqnorm = sph_norm*sph_norm;

      
      auto sph_rpa = ( ( pb ^ pc ^ pd ) / sphere);//<= sphere )/ (-sph_sqnorm);
      auto sph_rpb = ( ( pc ^ pd ^ pa ) / sphere); //<= sphere )/ (-sph_sqnorm);
      auto sph_rpc = ( ( pa ^ pb ^ pd ) / sphere);///<= sphere )/ (-sph_sqnorm);
      auto sph_rpd = ( ( pa ^ pb ^ pc ) / sphere);///<= sphere )/ (-sph_sqnorm);

      sph_rpa.print();

      Draw(sph_rpa); // Draw(sph_rpb); // Draw(sph_rpc); Draw(sph_rpd);

      Draw(sphere,0,1,0,.3 );

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
