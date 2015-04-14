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
#include "vsr_cga3D_log.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;

  float radiusA, radiusB,wt,size,dist;

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
    gui(radiusA,"radiusA",-100,100);
    gui(radiusB,"radiusB",-100,100);
    gui(wt,"wt",-100,100);
    gui(size,"size",-100,100);
    gui(dist,"dist",-100,100);
    
    radiusB =1; radiusA=1;
    wt=1; size=dist=0; amt=-.5;

    objectController.attach(&sa);
    objectController.attach(&sb);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    sa = sphere(-10,0,0,radiusA);
    sb = sphere(1,0,0,radiusB);


    auto rot = Gen::ratio( sa, sb ); //(root)
    auto par = Gen::log(rot);

    for (int i=0;i<=10;++i){

      float t = (float)i/10;
      
      auto ts = sa.bst(par * t * amt );
      auto cir = meet(ts,Dlp(0,0,1));
      Draw(cir,t,0,1-t,.5);
      
      //if (Ro::size(cir,false)>500) Draw(Line(cir),0,1,0);
      if(bReset) Draw( Ro::pnt_cir(cir, wt), 0, 1, 0, .2 );
    }

//    Draw( sa.reflect(sb),0,1,0,.3);
//    Draw( sb.reflect(sa),0,1,0,.3);

    Draw( sa^sb );
    Draw(sa,0,0,1,.5);
    Draw(sb,1,0,0,.5);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
