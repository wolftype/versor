#include <vsr/vsr_app.h>

#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include "vsr/form/vsr_graph.h"
#include "vsr/draw/vsr_graph_draw.h"

//now written into TVolumes class in vsr_tangent.h

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

#define FLIP false


struct Triple {
   Point point[3];
};

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
  float uSpacing = 3;
  float vSpacing = 3;
  float wSpacing = 3;

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
    gui (amt1, "amt1", -1, 1);
    gui (amt2, "amt2", 0, 1);
    gui (uSpacing, "uSpacing", 1, 10);
    gui (vSpacing, "vSpacing", 1, 10);
    gui (wSpacing, "wSpacing", 1, 10);


    gui (ps.bShadedOutput, "bShadedOutput");  ///< default for output
    gui (ps.bSortOutput, "bSortOutput");      ///< default
    gui (ps.bSimpleSort, "bSimpleSort");      ///< default
    gui (ps.bOffsetOutput, "bOffsetOutput");
    gui (ps.bOccludeOutput, "bOccludedOutput");
    gui (ps.bTightBounds, "bTightBounds");
    gui (ps.bPDF, "bPDF");  ///< pdf or eps

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
    Con uc = tf.uc (kVU, kWU, uSpacing);
    Con vc = tf.vc (kUV, kWV, vSpacing);
    Con wc = tf.wc (kUW, kVW, wSpacing);

    //make the actual surfaces and store them
    tf.surfaces (kVU, kWU, kUV, kWV, kUW, kVW);


    // New frames in respective directions, bools specify whether to "flip"
    TFrame uf = tf.xf (uc, FLIP, false, false);
    TFrame vf = tf.xf (vc, false, FLIP, false);
    TFrame wf = tf.xf (wc, false, false, FLIP);

    //TFrame uf = tf.xf (uc, false, false, false);
    //TFrame vf = tf.xf (vc, false, false, false);
    //TFrame wf = tf.xf (wc, false, false, false);
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
    uf.suv = TFrame::Normalize(-vf.svu <= uf.tu);
    uf.swv = TFrame::Normalize(-vf.svu <= uf.tw);
    //"Front Bottom Edge Going Over"
    //At w1, const w1 and const v0 in u dir are both ortho to const u1 in w dir
    wf.swu = TFrame::Normalize(-uf.suw <= wf.tw);
    wf.svu = TFrame::Normalize(-uf.suw <= wf.tv);
    //"Top Left Edge Going Forward"
    //At v1, const v1 and const u0 are both ortho to const w1 in v dir
    vf.svw = TFrame::Normalize(-wf.swv <= vf.tv);
    vf.suw = TFrame::Normalize(-wf.swv <= vf.tu);

    //now, what about the other surfaces to match the three bends?
    //"Bottom Going Forward"
    uf.svw = TFrame::Normalize(-wf.swu <= uf.tv);
    //"Back GTFrame::Normalize(oing Right"
    vf.swu = TFrame::Normalize(-uf.suv <= vf.tw);
    //"Left GTFrame::Normalize(oing Up"
    wf.suv = TFrame::Normalize(-vf.svw <= wf.tu);

    vf.svu = TFrame::Normalize(-uf.suv <= vf.tv);
    uf.suw = TFrame::Normalize(-wf.swu <= uf.tu);
    wf.swv = TFrame::Normalize(-vf.svw <= wf.tw);

    //(uf.suv <= vf.tv).print();


    // Get log that takes one surface to another.
    // surfaces must have been created already.
    // first letter indicates direction of sweep.
    // second letter indicates curvature line that is swept.
    // TODO change convenction:
    // duvw0 = sweep vertical line right at w=0 (i.e. back patch)
    // duwv0 = sweep depth line right at v=0 (i.e. bottom patch)
    // duv is a ratio of suvs, so sweeps right
    //
    // du of v direction (sweeps left to right BACK vertical) |
    Pair duvw0 = tf.duv (uf);
    // du of w direction (sweeps left to right BOTTOM depth) /
    Pair duwv0 = tf.duw (uf);
    // dv of w direction (sweeps bottom to top LEFT depth) /
    Pair dvwu0 = tf.dvw (vf);
    // dv of u direction (sweeps bottom to top BACK horizontal) _
    Pair dvuw0 = tf.dvu (vf);
    // dw of u direction (sweeps back to front BOTTOM horizontal) _
    Pair dwuv0 = tf.dwu (wf);
    // dw of v direction (sweeps back to front LEFT veritical) |
    Pair dwvu0 = tf.dwv (wf);

    cout << (tf.svu[3]) << " " << (vf.svu[3]) << endl;
    cout << (tf.pos() <= vf.svu) << endl;


   // we need to calculate the other frames
    // to calculate the other surfaces
    TFrame uvf = vf.xf( Gen::boost(duvw0), FLIP, false, false);
    TFrame vwf = wf.xf( Gen::boost(dvwu0), false, FLIP, false);
    TFrame wuf = uf.xf( Gen::boost(dwuv0), false, false, FLIP);

    //TFrame uvf = vf.xf( Gen::boost(duvw0), false, false, false);
    //TFrame vwf = wf.xf( Gen::boost(dvwu0), false, false, false);
    //TFrame wuf = uf.xf( Gen::boost(dwuv0), false, false, false);

    auto topPlane = vf.pos() ^ uvf.pos() ^ vwf.pos() ^ Inf(1);
    auto frontPlane = wf.pos() ^ vwf.pos() ^ wuf.pos() ^ Inf(1);
    auto rightPlane = uf.pos() ^ wuf.pos() ^ uvf.pos() ^ Inf(1);
    auto p = (topPlane.dual() ^ frontPlane.dual() ^ rightPlane.dual()).dual();
    p /= p[3];
    Point np = p.null();

    //these each have two defined surfaces
    uvf.suw = TFrame::Normalize(-np <= uvf.tu);
    uvf.svw = TFrame::Normalize(-np <= uvf.tv);
    vwf.svu = TFrame::Normalize(-np <= vwf.tv);
    vwf.swu = TFrame::Normalize(-np <= vwf.tw);
    wuf.swv = TFrame::Normalize(-np <= wuf.tw);
    wuf.suv = TFrame::Normalize(-np <= wuf.tu);

    //Draw (uvf.suw);

    //and with that, we have all the surfaces that can be defined with
    //nine coefficients -- 24 surfaces!  Which pair up into 12 Generators

    //now we find the remaining 6 generators
    // du of v direction at w=1 (sweeps left to right FRONT) |
    Pair duvw1 = wf.duv(wuf);
    // du of w direction at v=1 (sweeps left to right TOP) /
    Pair duwv1 = vf.duw(uvf);
    // dv of w direction at u=1 (sweeps bottom to top RIGHT) /
    Pair dvwu1 = uf.dvw(uvf);
    // dv of u direction at w=1 (sweeps bottom to top FRONT) _
    Pair dvuw1 = wf.dvu(vwf);
    // dw of u direction at v=1 (sweeps back to front TOP) _
    Pair dwuv1 = vf.dwu(vwf);
    // dw of v direction at u=1 (sweeps back to front RIGHT) |
    Pair dwvu1 = uf.dwv(wuf);

    // DONE
     // Draw 'em
    Draw (np, 1,1,0);

    DrawFrame(tf);
    DrawFrame(uf);
    DrawFrame(vf);
    DrawFrame(wf);
    DrawFrame(uvf);
    DrawFrame(vwf);
    DrawFrame(wuf);

    int numCurves = 5;
    for (int i = 0; i <= numCurves; ++i){
       float ti = (float)i/numCurves;
       Boost tbst = Gen::boost (dvuw0 * ti);
       Point tpos = Round::location(tf.pos().spin(tbst));
       DrawCurve (tpos, duvw0, 20, 1,0,0);

       Boost w1bst = Gen::boost (dvuw1 * ti);
       Point wpos = Round::location(wf.pos().spin(w1bst));
       DrawCurve (wpos, duvw1, 20, 1,0,0);

       Boost bwuv0 = Gen::boost (dwuv0 * ti);
       Point u0pos = Round::location(tf.pos().spin(bwuv0));
       Point u1pos = Round::location(uf.pos().spin(bwuv0));
       DrawCurve (u0pos, dvwu0, 20, 0,1,0);
       DrawCurve (u1pos, dvwu1, 20, 0,1,0);

       Boost buvw0= Gen::boost (duvw0 * ti);
       Point v0pos = Round::location (tf.pos().spin(buvw0));
       Point v1pos = Round::location (vf.pos().spin(buvw0));
       DrawCurve (v0pos, dwuv0, 20, 0,0,1);
       DrawCurve (v1pos, dwuv1, 20, 0,0,1);

    }

//    DrawCurve (tf.pos(), dwuv0, 20, 0,0,1);
//
////    DrawCurve (vf.pos(), duwv1, 20, 1,0,0);
//    DrawCurve (wf.pos(), dvwu0, 20, 0,1,0);
//    DrawCurve (uf.pos(), dwuv0, 20, 0,0,1);
//
//    DrawCurve (wf.pos(), duvw1, 20, 1,0,0);
//    DrawCurve (uf.pos(), dvuw0, 20, 0,1,0);
//    DrawCurve (vf.pos(), dwvu0, 20, 0,0,1); //
//
//    DrawCurve (vwf.pos(),duwv1,20,1,0,0);
//    DrawCurve (wuf.pos(),dvuw1,20,0,1,0);
//    DrawCurve (uvf.pos(),dwvu1,20,0,0,1);

    Boost wvu0 = Gen::bst (dwvu0 * amt1);
    Boost wvu1 = Gen::bst (dwvu1 * amt1);
    Boost wuv0 = Gen::bst (dwuv0 * amt1);
    Boost wuv1 = Gen::bst (dwuv1 * amt1);

    DualSphere su0v = tf.suv.spin (wvu0);
    DualSphere su1v = uf.suv.spin (wvu1);
    DualSphere sv0u = tf.svu.spin (wuv0);
    DualSphere sv1u = vf.svu.spin (wuv1);

   float alpha = amt2;
       //DrawRound (su0v,1.0,0.0,0.0,alpha);
       //DrawRound (su1v,1.0,0.0,0.0,alpha);
       //DrawRound (sv0u,0.0,1.0,0.0,alpha);
       //DrawRound (sv1u,0.0,1.0,0.0,alpha);


//       DrawRound (vf.suw.unit(), 1.0,0,0,.2);
//       DrawRound (uvf.suw.unit(), 1.0,0,0,.2);
//
//       DrawRound (tf.svu.unit(), 0.0,1.0,0,.2);
//       DrawRound (vf.svu.unit(), 0.0,1.0,0,.2);
   }

};


int main(){

  MyApp app;
  app.start();

  return 0;
}
