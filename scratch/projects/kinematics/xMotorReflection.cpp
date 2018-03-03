/*
 * =====================================================================================
 *
 *       Filename:  xMotorReflection.cpp
 *
 *    Description:  reflection of motors in a plane
 *
 *        Version:  1.0
 *        Created:  09/19/2015 22:08:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  bool bSetMouse  = false;

  float amt1 = 0;
  float amt2 = 0;

  Point mouse;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(bReset,"bReset")(bSetMouse,"bSetMouse");
    
  }

  void onKeyDown(const gfx::Keyboard& k){
    if (k.code=='s') bSetMouse = !bSetMouse;

    App::onKeyDown(k);
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    if (bSetMouse) mouse = calcMouse3D();
    
    Frame frame( mouse, Gen::rot( Biv::xy * amt1 + Biv::xz*amt2 ) );


    DualPlane dlp(1,0,0,2);

    Draw(dlp,1,1,0);
    Draw(frame);

    auto rot = frame.rot().reflect(dlp);
    auto pt = frame.pos().reflect(dlp);

    Frame refFrame(pt, rot);

    Frame newFrame( frame.mot().reflect(dlp) );
    //newFrame.translate(1,0,0);

   // if (bReset) { 
    Draw(refFrame);
    Draw(pt);
    Draw(newFrame); 

    auto dlla = frame.dly();
    auto dllb = refFrame.dlx();

    auto dlsA = Construct::sphere(mouse,.2);
    auto dlsB = Construct::sphere(3,0,0,.7);
  //  Draw(dlsA);
  //  Draw(dlsB);

    for (int i =0;i<100;++i){
      float t = (float)i/100;
      //auto log = Gen::log(dlla,dllb,t);
      //Draw( dlla.spin(Gen::mot(log)),t,0,1-t);

      auto rat = dlsB/dlsA; //rat /= rat.rnorm();
      auto rat2 = Gen::ratio(dlsA, dlsB);
      auto log = Gen::log(rat) * t/2.0;
      auto log2 = Gen::log(rat2) * -t;
      auto log3 = Gen::log(dlsA,dlsB,t);
      
      Draw( Round::produce(dlsA,Biv::xy).spin( Gen::bst( bReset ? log : log2) ),t,0,1-t);

    }


  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
