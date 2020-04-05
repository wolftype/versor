
#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include "vsr/form/vsr_graph.h"
#include "vsr/draw/vsr_graph_draw.h"

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App
{


  float kVU = 0;
  float kWU = 0;
  float kUV = 0;
  float kWV = 0;
  float kUW = 0;
  float kVW = 0;
  float kV1U = 0;
  float kU1W = 0;
  float kW1V = 0;
  float amt1 = 0;
  float amt2 = 0;

  void setup () {
    ///Add Variables to GUI
    gui (kVU, "kVU", -100, 100);
    gui (kWU, "kWU", -100, 100);
    gui (kUV, "kUV", -100, 100);
    gui (kWV, "kWV", -100, 100);
    gui (kUW, "kUW", -100, 100);
    gui (kVW, "kVW", -100, 100);
    gui (kV1U, "kV1U", -100, 100);
    gui (kU1W, "kU1W", -100, 100);
    gui (kW1V, "kW1V", -100, 100);
    gui (amt1, "amt1", 0, 1);
    gui (amt2, "amt2", 0, 1);
  }

  void onDraw () {

    TFrame tf;

    float uSpacing = 3;
    float vSpacing = 3;
    float wSpacing = 3;

    // Conformal Rotors along u,v,w curves, passing in two curvatures
    Con uc = tf.uc (kVU, kWU, uSpacing);
    Con vc = tf.vc (kUV, kWV, vSpacing);
    Con wc = tf.wc (kUW, kVW, wSpacing);

    //make the actual surfaces and store them
    tf.surfaces (kVU, kWU, kUV, kWV, kUW, kVW);

    // New frames in respective directions, bools specify whether to "flip"
    TFrame uf = tf.xf (uc, false, false, false);
    TFrame vf = tf.xf (vc, false, false, false);
    TFrame wf = tf.xf (wc, false, false, false);

    // Here we can make three surfaces by bending
    // "Top Going Right"
    vf.svu = vf.vsurf (kV1U);
    // "Right Going Forward"
    uf.suw = uf.usurf (kU1W);
    // "Front Going Up"
    wf.swv = wf.wsurf (kW1V);



  }

};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
