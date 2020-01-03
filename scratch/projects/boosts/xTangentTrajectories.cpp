#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>

#include <vsr/form/vsr_graph.h>
#include <vsr/draw/vsr_graph_draw.h>

struct MyApp : App {

  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  float amt1 = 0;
  float amt2 = 0;

  float kVU = 0;
  float kWU = 0;
  float kUV = 0;
  float kWV = 0;
  float kUW = 0;
  float kVW = 0;
  float kV1U = 0;
  float kU1W = 0;
  float kW1V = 0;
  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup(){
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

    scene.camera.pos(0,0,10);
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw(){
    //calculate mouse position in world space
    if (bMouseSet){
    }

    TFrame tf;

    // Conformal Rotors along u,v,w curves, passing in two curvatures
    float vSpacing = 3.0;
    Con uc = tf.uc (kVU, kWU, vSpacing);
    Con vc = tf.vc (kUV, kWV, vSpacing);
    Con wc = tf.wc (kUW, kVW, vSpacing);

    //make the actual surfaces and store them
    tf.surfaces (kVU, kWU, kUV, kWV, kUW, kVW);

    // New frames in respective directions, bools specify whether to "flip"
    TFrame uf = tf.xf (uc, true, false, false);
    TFrame vf = tf.xf (vc, false, true, false);
    TFrame wf = tf.xf (wc, false, false, true);

    // Here we can make three surfaces by bending
    // "Top Going Right"
    vf.svu = vf.vsurf (kV1U);
    // "Right Going Forward"
    uf.suw = uf.usurf (kU1W);
    // "Front Going Up"
    wf.swv = wf.wsurf (kW1V);

    //Now all other surfaces must be orthogonal to these
    //"Back Right Edge Going Up"
    //At u1, const u1 and const w0 in v dir are both ortho to const v1 in u dir
    uf.suv = -vf.svu <= uf.tu;
    uf.swv = -vf.svu <= uf.tw;
    //"Front Bottom Edge Going Over"
    //At w1, const w1 and const v0 in u dir are both ortho to const u1 in w dir
    wf.swu = -uf.suw <= wf.tw;
    wf.svu = -uf.suw <= wf.tv;
    //"Top Left Edge Going Forward"
    //At v1, const v1 and const u0 are both ortho to const w1 in v dir
    vf.svw = -wf.swv <= vf.tv;
    vf.suw = -wf.swv <= vf.tu;

    //now, what about the other surfaces to match the three bends?
    //"Bottom Going Forward"
    uf.svw = -wf.swu <= uf.tv;
    //"Back Going Right"
    vf.swu = -uf.suv <= vf.tw;
    //"Left Going Up"
    wf.suv = -vf.svw <= wf.tu;

    // Get log that takes one surface to another.
    // surfaces must have been created already.
    // du of v direction (sweeps back left to back right)
    Pair uvlog = tf.uvlog (uf);
    // du of w direction (sweeps bottom left to bottom right)
    Pair uwlog = tf.uwlog (uf);
    // dv of w direction (sweeps bottom left to top left)
    Pair vwlog = tf.vwlog (vf);
    // dv of u direction (sweeps back bottom to back top)
    Pair vulog = tf.vulog (vf);
    // dw of u direction (sweeps back bottom to front bottom)
    Pair wulog = tf.wulog (wf);
    // dw of v direction (sweeps back left to front left)
    Pair wvlog = tf.wvlog (wf);

    // Dont' even need to transform the frames to the corners

    // Draw 'em
    DrawFrame(tf);
    DrawFrame(uf);
    DrawFrame(vf);
    DrawFrame(wf);

    DrawCurve (tf.pos(), uvlog, 20, 1,0,0);
    DrawCurve (tf.pos(), vwlog, 20, 0,1,0);
    DrawCurve (tf.pos(), wulog, 20, 0,0,1);

    DrawCurve (vf.pos(), uvlog, 20, 1,0,0);
    DrawCurve (wf.pos(), vwlog, 20, 0,1,0);
    DrawCurve (uf.pos(), wulog, 20, 0,0,1);

    DrawCurve (wf.pos(), uwlog, 20, 1,0,0);
    DrawCurve (uf.pos(), vulog, 20, 0,1,0);
    DrawCurve (vf.pos(), wvlog, 20, 0,0,1);

    DrawPoints (tf.pos(), uvlog, vulog, 20, 1,0,0);
    DrawPoints (tf.pos(), vwlog, wvlog, 20, 0,1,0);
    DrawPoints (tf.pos(), wulog, uwlog, 20, 0,0,1);

//    DrawCurve (tv.pos(), uvlog, 20, 1,0,0);

//    DrawCurve (tu.pos(), vulog, 20, 0,1,0);
//    DrawCurve (tu.pos(), wulog, 20, 0,0,1);
//
//
//    DrawCurve (w.pos(), vulog, 20, 0,1,0);
//    DrawCurve (w.pos(), uvlog, 20, 1,0,0);
//

//    DrawCurve (w.pos(), vwlog, 20, 0,1,0);
   }

  /*-----------------------------------------------------------------------------
   *  Keyboard
   *-----------------------------------------------------------------------------*/
  void onKeyDown(const gfx::Keyboard &k){
   if (k.code=='s') bMouseSet = !bMouseSet;
  }
};


int main(){

  MyApp app;
  app.start();

  return 0;
}
