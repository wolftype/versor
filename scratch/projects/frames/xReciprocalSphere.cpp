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


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;


struct Recip {


};


struct MyApp : App {    

  Point mouse;
     
  float time;
  float amt, amtA,amtB,amtC;


  Frame fa, fb, fc, fd;

  void setup(){
      bindGLV();
      gui(amt,"amt",-100,100);
      gui(amtA,"amtA",-1,1);
      gui(amtB,"amtB",-1,1);
      gui(amtC,"amtC",-1,1);

      objectController.attach(&fa);
      objectController.attach(&fb);
      objectController.attach(&fc);
      objectController.attach(&fd);

      fa.pos() = PT(-1,-1,0);
      fb.pos() = PT(-1,1,0);
      fc.pos() = PT(1,1,0);
      fd.pos() = PT(1,-1,0);

  }
 
    virtual void onDraw(){ 
        
      mouse = calcMouse3D();
            
      //Draw
      Draw(fa); Draw(fb); Draw(fc); Draw(fd);

      auto pa = fa.pos();
      auto pb = fb.pos();
      auto pc = fc.pos();
      auto pd = fd.pos();

      //SPHERE Tangent Space
      Sph sphere = pa ^ pb ^ pc ^ pd;

      VSR_PRECISION sph_norm = sphere.rnorm();
      VSR_PRECISION sph_sqnorm = sph_norm*sph_norm;

      auto sph_rpa = ( ( pb ^ pc ^ pd ) / sphere); //<= sphere )/ (-sph_sqnorm);
      auto sph_rpb = ( ( pc ^ pd ^ pa ) / sphere); //<= sphere )/ (-sph_sqnorm);
      auto sph_rpc = ( ( pa ^ pb ^ pd ) / sphere); //<= sphere )/ (-sph_sqnorm);
      auto sph_rpd = ( ( pa ^ pb ^ pc ) / sphere); //<= sphere )/ (-sph_sqnorm);

      sphere.print();

      Draw(sph_rpa,1,0,0,.2); // Draw(sph_rpb); // Draw(sph_rpc); Draw(sph_rpd);

      Draw(sphere,0,1,0,.3 );

  }
   

  
};



int main(){

  MyApp app;
  app.start();
  
  return 0;

}
