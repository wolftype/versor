#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  
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
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(bToggle,"bToggle")(bSet,"bSet");
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    if (bMouseSet) mouse = calcMouse3D();

  }


  /*-----------------------------------------------------------------------------
   *  Keyboard control (hit 's' to toggle mouse tracking)
   *-----------------------------------------------------------------------------*/
  void onKeyDown(const gfx::Keyboard& k){
    switch (k.code){
      case 's': bMouseSet = !bMouseSet;
    }

    /// Default
    App:onKeyDown(k);
  }

  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
