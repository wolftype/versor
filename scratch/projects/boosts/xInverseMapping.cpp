
#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App
{

  float amt1 = 0;
  float amt2 = 0;

  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup () { gui (amt1, "amt1", -100, 100) (amt2, "amt2", -100, 100); }


  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw () {
    
     
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
