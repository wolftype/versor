/*
 * =====================================================================================
 *
 *       Filename:  xSphereBlend.cpp
 *
 *    Description:  sphere blending using point pair generators
 *    
 *    Q: given two spheres sa and sb, how to weigh the bivector sa^sb so
 *    that it boosts sa to sb?
 *
 *        Version:  1.0
 *        Created:  03/02/2015 15:45:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D_app.h"   

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;

  float wt, size,dist;

  //Two Spheres
  DualSphere sa,sb;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    gui(wt,"wt",-100,100);
    gui(size,"size",-100,100);
    gui(dist,"dist",-100,100);
    
    sa = sphere(-3,-1,0);
    sb = sphere(4,0,0);

    objectController.attach(&sa);
    objectController.attach(&sb);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    Pair par = sa ^ sb;

    auto dst = (sa<=sb)[0]*dist;

    float w = 1.0/par.rnorm() * wt;
    float sz = 1.0/Ro::size(par,true) * size;

    for (int i=0;i<10;++i){
      float t = (float)i/10;
      auto ts = sa.bst(par.runit() * t * (amt+w+sz+dst)/2.0 );
      auto cir = meet(ts,Dlp(0,0,1));
      Draw(cir,t,0,1-t,.5);
    }

    Draw(sa,0,0,1,.5);
    Draw(sb,1,0,0,.5);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
