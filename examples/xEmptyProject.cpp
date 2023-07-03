#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App {

  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  float amt1 = 0;
  float amt2 = 0;


  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Add Variables to GUI
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100);
    gui(bToggle,"bToggle")(bSet,"bSet");
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw(){
    //calculate mouse position in world space
    if (bMouseSet) calcMouse3D();
    //use mouse position in world space to construct a circle
    Circle cxy = Construct::circle(mMouse3D, 1);
    Frame frame;
    //draw the circle
    draw (cxy);
    draw (frame);

   }

  /*-----------------------------------------------------------------------------
   *  Keyboard
   *-----------------------------------------------------------------------------*/
  void onKeyDown(const gfx::Keyboard &k){
//   if (k.code=='s') bMouseSet = !bMouseSet;

    App::onKeyDown(k);
  }
};


int main(){

  MyApp app;
  app.start();

  return 0;
}
