/*
 * =====================================================================================
 *
 *       Filename:  xCoupledMotors.cpp
 *
 *    Description:  coupled motors from
 *
 *        Version:  1.0
 *        Created:  10/13/2015 17:00:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_twist.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  
  float amt1 = 0;
  float amt2 = 0;
  float amt3 = 0;
  float ext = 1;
  float periodA,periodB,periodC,pitchA,pitchB,pitchC;
  Point mouse;

  Frame frame;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(ext,"ext",-100,100)(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(amt3,"amt3",-100,100)(bToggle,"bToggle")(bSet,"bSet");
    gui(periodA, "periodA",-100,100);
    gui(periodB, "periodB",-100,100);
    gui(periodC, "periodC",-100,100);
    gui(pitchA, "pitchA",-100,100);
    gui(pitchB, "pitchB",-100,100);
    gui(pitchC, "pitchC",-100,100);

    objectController.attach(&frame);
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    if (bMouseSet) mouse = calcMouse3D();

    auto dla = Twist::Along( DLN(1,0,0), periodA, pitchA); 
    auto dlb = Twist::Along( DLN(0,1,0), periodB, pitchB); 
    auto dlc = Twist::Along( DLN(0,0,1), periodC, pitchC); 

    Draw(dla,0,0,0);
    Draw(dlb,0,0,0);
    Draw(dlc,0,0,0);

    glColor3f(.1,.3,.1);
    glBegin(GL_LINE_STRIP);
    for (int i =0;i<1000;++i){
      float t = ext * (float)i/1000;
     // auto tdll = dla * amt1 + dlb * amt2 + dlc * amt3;
      auto mot = Gen::mot( dlc * amt3 * t) * Gen::mot( dlb * amt2 * t) * Gen::mot( dla * amt1 * t);
      Frame f( mot * frame.mot() );
      f.scale( .5);
     //DrawAt( f.x(), f.pos(),0,.3,.3);
     // DrawAt( f.z(), f.pos(),.3,.3,0);
      //Draw(f.cxz(),0,.3,.3);
      GL::vertex(f.pos());
    }
    glEnd();

  }


//  /*-----------------------------------------------------------------------------
//   *  Keyboard control (hit 's' to toggle mouse tracking)
//   *-----------------------------------------------------------------------------*/
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
