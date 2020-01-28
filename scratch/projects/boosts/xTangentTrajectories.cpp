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
    // du of v direction (sweeps left to right BACK)
    Pair uvlog = tf.uvlog (uf);
    // du of w direction (sweeps left to right BOTTOM)
    Pair uwlog = tf.uwlog (uf);
    // dv of w direction (sweeps bottom to top LEFT)
    Pair vwlog = tf.vwlog (vf);
    // dv of u direction (sweeps bottom to top BACK)
    Pair vulog = tf.vulog (vf);
    // dw of u direction (sweeps back to front BOTTOM)
    Pair wulog = tf.wulog (wf);
    // dw of v direction (sweeps back to front LEFT)
    Pair wvlog = tf.wvlog (wf);

    // we need to calculate the other frames
    // to calculate the other surfaces
    TFrame uvf = vf.xf( Gen::boost(uvlog), true, false, false);
    TFrame vwf = wf.xf( Gen::boost(vwlog), false, true, false);
    TFrame wuf = uf.xf( Gen::boost(wulog), false, false, true);

    auto topPlane = vf.pos() ^ uvf.pos() ^ vwf.pos() ^ Inf(1);
    auto frontPlane = wf.pos() ^ vwf.pos() ^ wuf.pos() ^ Inf(1);
    auto rightPlane = uf.pos() ^ wuf.pos() ^ uvf.pos() ^ Inf(1);
    auto p = (topPlane.dual() ^ frontPlane.dual() ^ rightPlane.dual()).dual();
    p /= p[3];
    Point np = p.null();

    //these each have two defined surfaces
    uvf.suw = -np <= uvf.tu;
    uvf.svw = -np <= uvf.tv;
    vwf.svu = -np <= vwf.tv;
    vwf.swu = -np <= vwf.tw;
    wuf.swv = -np <= wuf.tw;
    wuf.suv = -np <= wuf.tu;

    //Draw (uvf.suw);

    //and with that, we have all the surfaces that can be defined with
    //nine coefficients -- 24 surfaces!  Which pair up into 12 Generators
    //
    //now we find the remaining 6 generators
    // du of v direction at w=1 (sweeps left to right FRONT)
    Pair uvwlog = wf.uvlog (wuf);
    // du of w direction at v=1 (sweeps left to right TOP)
    Pair uwvlog = vf.uwlog(uvf);
    // dv of w direction at u=1 (sweeps bottom to top RIGHT)
    Pair vwulog = uf.vwlog (uvf);
    // dv of u direction at w=1 (sweeps bottom to top FRONT)
    Pair vuwlog = wf.vulog (vwf);
    // dw of u direction at v=1 (sweeps back to front TOP)
    Pair wuvlog = vf.wulog (vwf);
    // dw of v direction at u=1 (sweeps back to front RIGHT)
    Pair wvulog = uf.wvlog (wuf);

    //There are few ways these can define a coordinate in the volume
    //A) Ratio of generators?
    //B) Sliding swvs, swus, suws, svws,

//    auto xf = [&](const Point& p, const Pair& logU,
//        const Pair &logV, const Pair& logW)
//    {
//     Boost bu = Gen::bst (logU);
//     Boost bv = Gen::bst (logV);
//     Boost bw = Gen::bst (logW);
//     Con K =  bu * bv;
//     Point tp = Round::location (p.spin (K));
//     return tp;
//    };

     auto split = Gen::log ( -( wvulog/wvlog).runit() );
     int res = 10;
     for (int i = 0; i <= res; ++i)
     {
        float ti = (float)i/res;
        Boost bu = Gen::bst (split[0] * ti);
        Boost bv = Gen::bst (split[1] * ti);
        Point pu = Round::location (tf.pos().spin(bu));
        Point pv = Round::location (tf.pos().spin(bv));
        Point pw = Round::location (tf.pos().spin(bv * bu));
        Draw (pu, 1,0,1);
        Draw (pv, 0,1,1);
        Draw (pw, 1,1,1);
     }

//v     int res = 10;
//v     for (int i = 0; i <= res; ++i)
//v     {
//v       float ti = (float)i/res;
//v       Boost bu = Gen::bst (uvlog * ti);   //left right BACK
//v       Boost buw = Gen::bst (uvwlog * ti); //left right FRONT
//v       //apply it to const w in v direction
//v       DualSphere w0 = tf.swv.spin (bu).unit();
//v       DualSphere w1 = wf.swv.spin (buw).unit();
//v       //Ratio
//v       Pair wlog = Gen::log (-(w1/w0).runit()) / 2.0;
//v
//v       for (int j = 0; j <= res; ++j)
//v       {
//v         float tj = (float)j/res;
//v         Boost bv = Gen::bst (vulog * tj); //bottom up BACK
//v         Con K = bv * bu;
//v         Point p = Round::location (tf.pos().spin (K));
//v         DrawCurve (p, wlog, 20, 1.0, 0.0, 0.0);
//v       }
//v     }
//v
//v     for (int i = 0; i <= res; ++i)
//v     {
//v       float ti = (float)i/res;
//v       Boost bv = Gen::bst (vulog * ti);   //bottom up BACK
//v       Boost bvw = Gen::bst (vuwlog * ti); //bottom up FRONT
//v       //apply it to const w in u direction
//v       DualSphere w0 = tf.swu.spin (bv).unit();
//v       DualSphere w1 = wf.swu.spin (bvw).unit();
//v       //Ratio
//v       Pair wlog = Gen::log (-(w1/w0).runit()) / 2.0;
//v
//v       for (int j = 0; j <= res; ++j)
//v       {
//v         float tj = (float)j/res;
//v         Boost bu = Gen::bst (uvlog * tj); //left right BACK
//v         Con K = bu * bv;
//v         Point p = Round::location (tf.pos().spin (K));
//v         DrawCurve (p, wlog, 20, 0.0,1.0, 0.0);
//v       }
//v     }

//     for (int i = 0; i <= res; ++i)
//     {
//       float ti = (float)i/res;
//       Boost buv0 = Gen::bst (uwlog * ti);   //left right BOTTOM
//       Boost buv1 = Gen::bst (uwvlog * ti); //left right TOP
//       //apply it to const w in v direction
//       DualSphere v0 = tf.svw.spin (buv0);
//       DualSphere v1 = vf.svw.spin (buv1);
//       //Ratio
//       Pair vlog = Gen::log (-(v1/v0).runit()) / 2.0;
//
//       for (int j = 0; j <= res; ++j)
//       {
//         float tj = (float)j/res;
//         Boost bw = Gen::bst (wulog * tj); //back front BOTTOM
//         Con K = bw * buv0;
//         Point p = Round::location (tf.pos().spin (K));
//         DrawCurve (p, vlog, 20, 0.0, 1.0, 0.0);
//       }
//     }

//     for (int i = 0; i <= res; ++i)
//     {
//       float ti = (float)i/res;
//       Boost bv = Gen::bst (vulog * ti);   //bottom up BACK
//       Boost bvw = Gen::bst (vuwlog * ti); //bottom up FRONT
//       //apply it to const w in u direction
//       DualSphere w0 = tf.swu.spin (bv);
//       DualSphere w1 = wf.swu.spin (bvw);
//       //Ratio
//       Pair wlog = Gen::log (-(w1/w0).runit()) / 2.0;
//
//       for (int j = 0; j <= res; ++j)
//       {
//         float tj = (float)j/res;
//         Boost bu = Gen::bst (uvlog * tj); //left right BACK
//         Con K = bu * bv;
//         Point p = Round::location (tf.pos().spin (K));
//         DrawCurve (p, wlog, 20, 0.0,0.0, 1.0);
//       }
//     }


    // Draw 'em
    Draw (np, 1,1,0);

    DrawFrame(tf);
    DrawFrame(uf);
    DrawFrame(vf);
    DrawFrame(wf);
    DrawFrame(uvf);
    DrawFrame(vwf);
    DrawFrame(wuf);

    DrawCurve (tf.pos(), uvlog, 20, 1,0,0);
    DrawCurve (tf.pos(), vwlog, 20, 0,1,0);
    DrawCurve (tf.pos(), wulog, 20, 0,0,1);

    DrawCurve (vf.pos(), uvlog, 20, 1,0,0);
    DrawCurve (wf.pos(), vwlog, 20, 0,1,0);
    DrawCurve (uf.pos(), wulog, 20, 0,0,1);

    DrawCurve (wf.pos(), uwlog, 20, 1,0,0);
    DrawCurve (uf.pos(), vulog, 20, 0,1,0);
    DrawCurve (vf.pos(), wvlog, 20, 0,0,1);

    DrawCurve (vwf.pos(),uwvlog,20,1,0,0);
    DrawCurve (wuf.pos(),vuwlog,20,0,1,0);
    DrawCurve (uvf.pos(),wvulog,20,0,0,1);

//    DrawPoints (tf.pos(), uvlog, vulog, 20, 1,0,0);
//    DrawPoints (tf.pos(), vwlog, wvlog, 20, 0,1,0);
//    DrawPoints (tf.pos(), wulog, uwlog, 20, 0,0,1);

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
