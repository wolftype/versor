#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App {

  //Some Variables
  bool bToggle = false;
  float amt1 = 0;

  void setup(){}

  void onDrawGui(){
    ///Add Variables to GUI
    gui(amt1,"amt1",-1,1);
    gui(bToggle,"bToggle");
  }

  void onDraw(){
    //calculate mouse position in world space
    if (bSetMouse) calcMouse3D(.99);
    //use mouse position in world space to construct a circle
    Circle cxy = Construct::circle(mMouse3D, Biv::xy, 1);
    Frame frame (mMouse3D);
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
