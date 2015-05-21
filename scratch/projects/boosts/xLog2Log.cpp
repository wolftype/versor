/*
 * =====================================================================================
 *
 *       Filename:  xLog2Log.cpp
 *
 *    Description:  bilinear interp of 4 frame curves
 *
 *        Version:  1.0
 *        Created:  03/23/2015 19:07:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
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
  float amt, num, amtB = 0;
  float bivA,bivB;


  Frame fa, fb, fc, fd;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    gui(amtB,"amtB",-100,100);
    gui(num,"num",0,100);
    gui(bivA, "bivA",-100,100)(bivB,"bivB",-100,100);
    
    fa.pos(-2,-0,0);
    fb.pos(-1,1,0);
    fc.pos(2,0,0);
    fd.pos(1,-1,0);

    objectController.attach(&fa);
    objectController.attach(&fb);
    objectController.attach(&fc);
    objectController.attach(&fd);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    auto ca = fa.cyz();
    auto cb = fb.cxz();
    auto cc = fc.cyz();
    auto cd = fd.cxz();

    GenRot rotAB = (cb / ca).runit();
    GenRot rotDC = (cc / cd).runit();

    auto squareRootAB = sqroot(rotAB);
    auto logOfRootAB = log( squareRootAB.runit() );

    auto squareRootDC = sqroot(rotDC);
    auto logOfRootDC = log( squareRootDC.runit() );
 
    for (int i=0;i<=num;++i){
      float t=(float)i/num * amt;
      auto xfAB = Gen::bst( logOfRootAB[0] * t ) * Gen::bst( logOfRootAB[1] * t );
      auto xfDC = Gen::bst( logOfRootDC[0] * t ) * Gen::bst( logOfRootDC[1] * t );

      auto tmpA = ca.spin(xfAB);
      auto tmpB = cd.spin(xfDC);

      GenRot rotFinal = (tmpB/tmpA).runit();
      auto squareRootFinal = sqroot( rotFinal );
      auto logOfRootFinal = log( squareRootFinal.runit() );
      for(int j=0;j<=num;++j){
        float tv = (float)j/num * amtB;
        auto xfFinal = Gen::bst(logOfRootFinal[0] * tv) * Gen::bst( logOfRootFinal[1] * tv );
        Draw( ca.spin(xfFinal*xfAB) ,tv,t,1-tv);
      }
    }
    
    Draw(ca); 
    Draw(cb);
    Draw(cc);
    Draw(cd);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
