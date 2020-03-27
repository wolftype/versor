#include <vsr/vsr_app.h>

#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include "vsr/form/vsr_graph.h"
#include "vsr/draw/vsr_graph_draw.h"

//now written into TVolumes class in vsr_tangent.h

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;


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
    TFrame uf = tf.xf (uc, true, false, false);
    TFrame vf = tf.xf (vc, false, true, false);
    TFrame wf = tf.xf (wc, false, false, true);

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

    // we need to calculate the other frames
    // to calculate the other surfaces
    TFrame uvf = vf.xf( Gen::boost(duvw0), true, false, false);
    TFrame vwf = wf.xf( Gen::boost(dvwu0), false, true, false);
    TFrame wuf = uf.xf( Gen::boost(dwuv0), false, false, true);

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

//    auto sp = -uf.tu ;
//    auto mult = sp * (Inf(-1) <= uf.tu);
//    auto mult2= (Inf(-1) <= uf.tu) * sp;


    // DONE
    //
    //
    // some tests on inverse mapping:
    //
    //use amt1
//    auto pt = Round::loc(tf.pos().spin (Gen::bst (dvuw0*amt1) * Gen::bst (duvw0 * amt1)));
//
//    Draw(pt);
//
//    auto sph = -pt <= duvw0;
//    auto sph2= -pt <= dvuw0;
//
//    Draw (sph, 1,0,0,amt2);
//    Draw (sph2, 0,1,0,amt2);
//
//    auto tpair = Gen::log ( -(sph/tf.suv).runit()) * .5;
//    auto tpairB = Gen::log (-(uf.suv/tf.suv).runit()) * .5;
//
//    auto tvalA = tpair <= !tpairB;
//    auto tvalB = tpair / tpairB;

    //cout << " T " << tvalA << endl;
    //cout << " TB " << tvalB << endl;


    // and HERE WE RATION

    // indexable (back, front, bottom, top, left, right,
    // back, front, bottom, top, left, right):
//    vector<Pair> gen = {duv, duvw, duw, duwv, dvw, dvwu, dvu,
//      dvuw, dwu, dwuv, dwv, dwvu};
     //
    //There are few ways these can define a coordinate in the volume
    //A) Sliding swvs, swus, suws, svws,
    //B) Ratio of generators? (NO)
    //C) Sliding tensors themselves
    //
    //In A, we can chaange both direction x y or y x then z
    //


     //ds: first is direction, second is the curve
     //d_direction_curve
     int res = 5;
     int resp = 6;
     vector<Triple> results (resp*resp*resp);;
     for (int i = 0; i <= res; ++i)
     {
       float ti = (float)i/res;

       // OPTION A (left to right)//
       //back and front, sweep right
       //bottom and top, sweep right
       Boost uvw0 = Gen::bst (duvw0 * ti);
       Boost uvw1 = Gen::bst (duvw1 * ti);
       Boost uwv0 = Gen::bst (duwv0 * ti);
       Boost uwv1 = Gen::bst (duwv1 * ti);

       //osculating spheres as we move left to rightA
       //those osc spheres turn into generators in the two other directions
       DualSphere sw0v = tf.swv.spin (uvw0);
       DualSphere sw1v = wf.swv.spin (uvw1);
       DualSphere sv0w = tf.svw.spin (uwv0);
       DualSphere sv1w = vf.svw.spin (uwv1);
     
       auto tmp = sv1w/sv0w; 
       cout << " HELLO : " << tmp.wt() <<  " " << tmp.rwt() << endl;

       Pair dvw = Gen::log (-(sv1w/sv0w).runit())/2.0;
       Pair dwv  = Gen::log (-(sw1v/sw0v).runit())/2.0;

       // OPTION B (back to front)//
       Boost wvu0 = Gen::bst (dwvu0 * ti);
       Boost wvu1 = Gen::bst (dwvu1 * ti);
       Boost wuv0 = Gen::bst (dwuv0 * ti);
       Boost wuv1 = Gen::bst (dwuv1 * ti);

       DualSphere su0v = tf.suv.spin (wvu0);
       DualSphere su1v = uf.suv.spin (wvu1);
       DualSphere sv0u = tf.svu.spin (wuv0);
       DualSphere sv1u = vf.svu.spin (wuv1);

       Pair duv  = Gen::log (-(su1v/su0v).runit())/2.0;
       Pair dvu  = Gen::log (-(sv1u/sv0u).runit())/2.0;

       // OPTION C //
       Boost vwu0 = Gen::bst (dvwu0 * ti);
       Boost vwu1 = Gen::bst (dvwu1 * ti);
       Boost vuw0 = Gen::bst (dvuw0 * ti);
       Boost vuw1 = Gen::bst (dvuw1 * ti);

       DualSphere su0w = tf.suw.spin (vwu0);
       DualSphere su1w = uf.suw.spin (vwu1);
       DualSphere sw0u = tf.swu.spin (vuw0);
       DualSphere sw1u = wf.swu.spin (vuw1);

       Pair duw  = Gen::log (-(su1w/su0w).runit())/2.0;
       Pair dwu  = Gen::log (-(sw1u/sw0u).runit())/2.0;

       for (int j = 0; j <= res; ++j)
       {
         float tj = (float)j/res;
  //       Boost vw = Gen::bst(dvw * tj);
         Boost uv = Gen::bst(duv * tj);
   //      Boost uw = Gen::bst(duw * tj);


        for (int k = 0; k <= res; ++k)
        {
          float tk = (float)k/res;
 //         Boost wv = Gen::bst(dwv * tk);
          Boost vu = Gen::bst(dvu * tk);
 //         Boost wu = Gen::bst(dwu * tk);

      //    Con Ku = wv * vw * uvw0;
          Con Kw = vu * uv * wvu0;
     //     Con Kv = wu * uw * vwu0;

   //       Point pu = Round::location(tf.pos().spin(Ku));
          Point pw = Round::location(tf.pos().spin(Kw));
    //      Point pv = Round::location(tf.pos().spin(Kv));

//          Draw (pu, 1.0, 0.0, 0.0);
//          Draw (pw, 0.0, 0.0, 1.0);
 //         Draw (pv, 0.0, 1.0, 0.0);

//          int resp = res +1;
//          int idxu = i * resp * resp + j * resp + k;
//          int idxw = j * resp * resp + k * resp + i;
//          int idxv = j * resp * resp + i * resp + k;
//          results[idxu].point[0] = pu;
//          results[idxw].point[1] = pw;
//          results[idxv].point[2] = pv;
//
        }

       }

     }

     for (auto& i : results){
//          Draw (i.point[0], 1.0, 0.0, 0.0);
//          Draw (i.point[1], 0.0, 0.0, 1.0);
//          Draw (i.point[2], 0.0, 1.0, 0.0);
     }

//       Boost buw0 = Gen::bst (duv * amt1);  //left to right BACK vertical
//       Boost buw1 = Gen::bst (duvw * amt1); //left to right FRONT vertical
//
//       Boost buv0 = Gen::bst (duw * amt2);  //left to right BOTTOM depth
//       Boost buv1 = Gen::bst (duwv * amt2); //left to right TOP depth
//
//       //osculating spheres as we move left to rightA
//       //back and front
//       DualSphere sw0 = tf.swv.spin (buw0);
//       DualSphere sw1 = wf.swv.spin (buw1);
//       //bottom and top
//       DualSphere sv0 = tf.svw.spin (buv0);
//       DualSphere sv1 = vf.svw.spin (buv1);
//       Draw (sw0,1.0, 0.0,0.0,.3);
//       Draw (sw1,1.0, 0.0,0.0,.3);
//
//       Draw (sv0,0.0, 1.0,0.0,.3);
//       Draw (sv1,0.0, 1.0,0.0,.3);

     for (int i = 0; i <= res; ++i)
     {
       float ti = (float)i/res;
       Boost bvw0 = Gen::bst (dvuw0 * ti);   //bottom to top BACK
       Boost bvw1 = Gen::bst (dvuw1 * ti);  //bottom to top FRONT
       //apply it to const w in u direction
       DualSphere w0 = tf.swu.spin (bvw0);
       DualSphere w1 = wf.swu.spin (bvw1);
       //Ratio
       Pair wlog = Gen::log (-(w1/w0).runit()) / 2.0;

       for (int j = 0; j <= res; ++j)
       {
         float tj = (float)j/res;
         Boost buvw0 = Gen::bst (duvw0 * tj); //left right BACK
         Con K = buvw0 * bvw0;
         DualSphere ds = tf.pos().spin (K);
         Point p = Round::location (ds);
         //DrawCurve (p, wlog, 20, 0.0,1.0, 0.0);
       }
     }


//     for (int i = 0; i <= res; ++i)
//     {
//       float ti = (float)i/res;
//       Boost buw0 = Gen::bst (duv * ti);  //left to right BACK vertical
//       Boost buw1 = Gen::bst (duvw * ti); //left to right FRONT vertical
//
//       for (int j = 0; j <= res; ++j)
//       {
//         float tj = (float)j/res;
//         Boost bvw0 = Gen::bst (dvu * tj);  //bottom up BACK horizontal
//         Boost bvw1 = Gen::bst (dvuw * tj); //bottom up FRONT horizontal
//
//         Con Kw0 = bvw0 * buw0;
//         Con Kw1 = bvw1 * buw1;
//
//         DualSphere dsw0 = tf.pos().spin (Kw0);
//         DualSphere dsw1 = wf.pos().spin (Kw1);
//
//         Pair wlog = Gen::log (-(dsw1 / dsw0).runit()) / 2.0;
//
//         Point p0 = Round::location (dsw0);
//         Point p1 = Round::location (dsw1);
//
//         DrawCurve (p0, wlog, 20, 1.0, 1.0, 0.0);
//         Draw (p0);
//         Draw (p1);
//       }
//     }
//
//     for (int i = 0; i <= res; ++i)
//     {
//       float ti = (float)i/res;
//       Boost buw0 = Gen::bst (duv * ti);  //left to right BACK vertical
//       Boost buw1 = Gen::bst (duvw * ti); //left to right FRONT vertical
//       //apply it to const w in v direction
//       DualSphere w0 = tf.swv.spin (buw0);
//       DualSphere w1 = wf.swv.spin (buw1);
//       //Ratio
//       Pair wlog = Gen::log (-(w1/w0).runit()) / 2.0;
//
//       for (int j = 0; j <= res; ++j)
//       {
//         float tj = (float)j/res;
//         Boost bv = Gen::bst (dvu * tj); //bottom up BACK
//         Con K = bv * buw0;
//         DualSphere ds = tf.pos().spin (K);
//         Point p = Round::location (ds);
//         DrawCurve (p, wlog, 20, 0.0, 0.0, 1.0);
//       }
//     }
//
//     // FLOOR, SWEPT UPWARD
//    for (int i = 0; i <= res; ++i)
//    {
//      float ti = (float)i/res;
//      Boost buv0 = Gen::bst (duw * ti);   //left to right BOTTOM depth
//      Boost buv1 = Gen::bst (duwv * ti);  //left to right TOP depth
//      //apply it to const w in v direction
//      DualSphere v0 = tf.svw.spin (buv0);
//      DualSphere v1 = vf.svw.spin (buv1);
//      //Ratio
//      Pair vlog = Gen::log (-(v1/v0).runit()) / 2.0;
//
//      for (int j = 0; j <= res; ++j)
//      {
//        float tj = (float)j/res;
//        Boost bw = Gen::bst (dwu * tj);   //back to front BOTTOM
//        Con K = bw * buv0;
//        DualSphere ds = tf.pos().spin(K);
//        Point p = Round::location (ds);
//        DrawCurve (p, vlog, 20, 0.0, 1.0, 0.0);
//      }
//    }
//   // LEFT WALL, SWEPT RIGHT
//    for (int i = 0; i <= res; ++i)
//    {
//      float ti = (float)i/res;
//      Boost bwu0 = Gen::bst (dwv * ti);   //back to front LEFT vertical
//      Boost bwu1 = Gen::bst (dwvu * ti);  //back to front RIGHT vertical
//      //apply to const v in u direction
//      DualSphere v0 = tf.suv.spin (bwu0);
//      DualSphere v1 = uf.suv.spin (bwu1);
//      //Ratio
//      Pair ulog = Gen::log (-(v1/v0).runit()) / 2.0;
//
//      for (int j = 0; j <= res; ++j)
//      {
//        float tj = (float)j/res;
//        Boost bv = Gen::bst (dvw * tj);   //bottom to top LEFT horizontal
//        Con K = bv * bwu0;
//        DualSphere ds = tf.pos().spin(K);
//        Point p = Round::location (ds);
//        DrawCurve (p, ulog, 20, 1.0, 0.0, 0.0);
//      }
//    }

    // Draw 'em
    Draw (np, 1,1,0);

    DrawFrame(tf);
    DrawFrame(uf);
    DrawFrame(vf);
    DrawFrame(wf);
    DrawFrame(uvf);
    DrawFrame(vwf);
    DrawFrame(wuf);

    DrawCurve (tf.pos(), duvw0, 20, 1,0,0);
    DrawCurve (tf.pos(), dvwu0, 20, 0,1,0);
    DrawCurve (tf.pos(), dwuv0, 20, 0,0,1);

    DrawCurve (vf.pos(), duvw0, 20, 1,0,0);
    DrawCurve (wf.pos(), dvwu0, 20, 0,1,0);
    DrawCurve (uf.pos(), dwuv0, 20, 0,0,1);

    DrawCurve (wf.pos(), duwv0, 20, 1,0,0);
    DrawCurve (uf.pos(), dvuw0, 20, 0,1,0);
    DrawCurve (vf.pos(), dwvu0, 20, 0,0,1);

    DrawCurve (vwf.pos(),duwv1,20,1,0,0);
    DrawCurve (wuf.pos(),dvuw1,20,0,1,0);
    DrawCurve (uvf.pos(),dwvu1,20,0,0,1);

//    DrawPoints (tf.pos(), duv, dvu, 20, 1,0,0);
//    DrawPoints (tf.pos(), dvw, dwv, 20, 0,1,0);
//    DrawPoints (tf.pos(), dwu, duw, 20, 0,0,1);

//    DrawCurve (tv.pos(), duv, 20, 1,0,0);

//    DrawCurve (tu.pos(), dvu, 20, 0,1,0);
//    DrawCurve (tu.pos(), dwu, 20, 0,0,1);
//
//
//    DrawCurve (w.pos(), dvu, 20, 0,1,0);
//    DrawCurve (w.pos(), duv, 20, 1,0,0);
//

//    DrawCurve (w.pos(), dvw, 20, 0,1,0);
//
       //float ti = (float)i;
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


   }

};


int main(){

  MyApp app;
  app.start();

  return 0;
}
