//
// A single cyclidic volume
// used to warp an underlying shape
//

#include <array>
#include <vsr/vsr_app.h>

#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include "vsr/form/vsr_graph.h"
#include "vsr/draw/vsr_graph_draw.h"
#include "vsr/util/vsr_draw_util.h"
#include "vsr/util/vsr_gui_util.h"

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;
using namespace vsr::util;

struct MyApp : App {

  // The Six Faces of a Hexahedral Lattice
  std::array<vsr::util::Grid, 6> faces;

  // A Cylinder inside it
  Cylinder cylinder;

  // Resolution and spacing of the Lattice
  float resU = 10;
  float resV = 10;
  float resW = 10;
  float uSpacing = 3;
  float vSpacing = 3;
  float wSpacing = 3;

  float alpha = 1.0;

  float uc = 0;
  float vc = 0;
  float wc = 0;

  float preset = 0;
  float decay = 0.0;
  float timer = 0;

  // buttons
  bool bDrawOne = false;
  bool bDrawVol1 = true;
  bool bDrawVol2 = false;
  bool bDrawFrame = false;
  bool bDrawIMapSurface = false;
  bool bDrawVolumeFrames = false;
  bool bDrawEdges = false;
  bool bDrawSection = false;
  bool bDrawCircle = false;
  bool bDrawCylinder = false;
  bool bDrawBaseSpheres = false;
  bool bDrawUVSpheres = false;
  bool bDrawLidSpheres = false;
  bool bDrawUSpheres = false;
  bool bDrawVSpheres = false;
  bool bDrawWSpheres = false;
  bool bDrawVDirSpheres = false;

  // Curvature values
  // OParam from gui_util creates an oscillating parameter value
  OParam kVU = OParam("kVU");
  OParam kWU = OParam("kWU");
  OParam kUV = OParam("kUV");
  OParam kWV = OParam("kWV");
  OParam kUW = OParam("kUW");
  OParam kVW = OParam("kVW");
  OParam kV1U = OParam("kV1U");
  OParam kU1W = OParam("kU1W");
  OParam kW1V = OParam("kW1V");

  void guiElement(OParam &cp) {
    gui(cp.val, cp.name, -1, 10);
    gui(cp.frq, "", 0, .1);
    gui(cp.osc, "");
  }

  void onDrawGui() {
    // Add Variables to GUI
    gui(preset, "preset", 0, 10);
    gui(decay, "decay", -10, 10);
    guiElement(kVU);
    guiElement(kWU);
    guiElement(kUV);
    guiElement(kWV);
    guiElement(kUW);
    guiElement(kVW);
    guiElement(kV1U);
    guiElement(kU1W);
    guiElement(kW1V);

    gui(alpha, "alpha", -1, 1);

    gui(uc, "uc", 0, 1);
    gui(vc, "vc", 0, 1);
    gui(wc, "wc", 0, 1);

    gui(uSpacing, "uSpacing", 1, 10);
    gui(vSpacing, "vSpacing", 1, 10);
    gui(wSpacing, "wSpacing", 1, 10);

    gui(bDrawOne, "bDrawOne");
    gui(bDrawVol1, "bDrawVol1");
    gui(bDrawFrame, "bDrawFrame");
    gui(bDrawIMapSurface, "bDrawIMapSurface");
    gui(bDrawVolumeFrames, "bDrawVolumeFrames");
    gui(bDrawEdges, "bDrawEdges");
    gui(bDrawSection, "bDrawSection");
    gui(bDrawCircle, "bDrawCircle");
    gui(bDrawCylinder, "bDrawCylinder");
    gui(bDrawBaseSpheres, "bDrawBaseSpheres");
    gui(bDrawUVSpheres, "bDrawUVSpheres");
    gui(bDrawLidSpheres, "bDrawLidSpheres");
    gui(bDrawUSpheres, "bDrawUSpheres");
    gui(bDrawVSpheres, "bDrawVSpheres");
    gui(bDrawWSpheres, "bDrawWSpheres");
    gui(bDrawVDirSpheres, "bDrawVDirSpheres");

    gui(ps.bShadedOutput, "bShadedOutput"); ///< default for output
    gui(ps.bSortOutput, "bSortOutput");     ///< default
    gui(ps.bSimpleSort, "bSimpleSort");     ///< default
    gui(ps.bOffsetOutput, "bOffsetOutput");
    gui(ps.bOccludeOutput, "bOccludedOutput");
    gui(ps.bTightBounds, "bTightBounds");
    gui(ps.bPDF, "bPDF"); ///< pdf or eps
  }

  void onSetup() {

    mColor.set(.8, .8, .8);
    ps.bOffsetOutput = false;
    ps.bOccludeOutput = false;
    ps.bTightBounds = true;

    scene.camera.pos(0, 0, 10);

    for (auto &i : faces)
      i.init(resU, resV);

    cylinder.init(20, 20);
  }

  void calcPreset() {
    switch ((int)preset) {
    case 0:
      kVU.bAbs = kVW.bAbs = kUV.bAbs = kUW.bAbs = kWU.bAbs = kWV.bAbs = false;
      kV1U.bAbs = kU1W.bAbs = kW1V.bAbs = false;
      break;

    case 1:
      kVU.bAbs = kVW.bAbs = kUV.bAbs = kUW.bAbs = kWU.bAbs = kWV.bAbs = true;
      kV1U.bAbs = kU1W.bAbs = kW1V.bAbs = true;
      break;

    case 2:
      kVU.val = kVW.val = kUV.val = kUW.val = kWU.val = kWV.val = 0.0;
      kVU.osc = kVW.osc = kUV.osc = kUW.osc = kWU.osc = kWV.osc = false;
      break;
    case 3:
      kVU.val = kVW.val = kUV.val = kUW.val = kWU.val = kWV.val = -.8;
      kVU.osc = kVW.osc = kUV.osc = kUW.osc = kWU.osc = kWV.osc = true;
      kVU.frq = kVW.frq = kUV.frq = kUW.frq = kWU.frq = kWV.frq = .02;
      kVU.bAbs = kVW.bAbs = kUV.bAbs = kUW.bAbs = kWU.bAbs = kWV.bAbs = true;
      kV1U.bAbs = kU1W.bAbs = kW1V.bAbs = true;

    case 4:
      kVU.osc = kVW.osc = kUV.osc = kUW.osc = kWU.osc = kWV.osc = true;
      kVU.bAbs = kVW.bAbs = kUV.bAbs = kUW.bAbs = kWU.bAbs = kWV.bAbs = false;

      break;

    case 5:
      cylinder.makeCoordsX();
      break;
    case 6:
      cylinder.makeCoordsZ();
      break;
    case 7:
      cylinder.diagCoords();
      break;
    case 8:
      decay = 2;
      bDrawVol1 = false;
      bDrawCylinder = true;
      break;
    default:
      break;
    }

    preset = 9;
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw() {

    calcPreset();

    timer++;

    TVolume vol(kVU.eval(timer), kWU.eval(timer), kUV.eval(timer),
                kWV.eval(timer), kUW.eval(timer), kVW.eval(timer),
                kV1U.eval(timer), kU1W.eval(timer), kW1V.eval(timer), uSpacing,
                vSpacing, wSpacing);

    int citer = 0;
    for (auto &i : cylinder.coords) {
      cylinder.pnts[citer] = vol.pointAt(i[0], i[1], i[2]);
      citer++;
    }

    if (bDrawCylinder) {
      cylinder.draw(true);
    }

    TVolume::Mapping mapping = vol.calcMapping(resU, resV, resW);

    // one little example
    if (bDrawOne) {
      Draw(vol.tf().tu, 1, 0, 0);
      DrawRound(vol.tf().suw, 1, 0, .3, .2);
    }

    if (bDrawVolumeFrames) {
      DrawVolumeFrames(vol);
    }

    if (bDrawFrame) {
      Con con = vol.calcMappingAt(uc, vc, wc);
      Pair tu = vol.tf().tu.spin(con);
      Pair tv = vol.tf().tv.spin(con);
      Pair tw = vol.tf().tw.spin(con);

      Draw(tu, 1, 0, 0);
      Draw(tv, 0, 1, 0);
      Draw(tw, 0, 0, 1);
    }

    if (bDrawEdges) {
      DrawCurve(vol.pos(), vol.duvw0(), 20, 1, .3, .3);
      DrawCurve(vol.pos(), vol.dvuw0(), 20, .3, 1, .3);
      DrawCurve(vol.pos(), vol.dwvu0(), 20, .3, .3, 1);

      Draw(Tops::NormalizePair(vol.tf().tu.spin(vol.ruvw0())), 1, .3, .3);
      Draw(Tops::NormalizePair(vol.tf().tv.spin(vol.rvuw0())), .3, 1, .3);
      Draw(Tops::NormalizePair(vol.tf().tw.spin(vol.rwvu0())), .3, .3, 1);
    }

    // calc Faces
    for (int i = 0; i < resU; ++i) {
      for (int j = 0; j < resV; ++j) {
        for (int k = 0; k < resW; ++k) {

          Point pt = vol.pointAt(i, j, k, mapping);

          if (i == 0)
            faces[0].pnts[j * resW + k] = pt;
          if (i == resU - 1)
            faces[1].pnts[j * resW + k] = pt;
          if (j == 0)
            faces[2].pnts[i * resW + k] = pt;
          if (j == resV - 1)
            faces[3].pnts[i * resW + k] = pt;
          if (k == 0)
            faces[4].pnts[i * resV + j] = pt;
          if (k == resW - 1)
            faces[5].pnts[i * resV + j] = pt;
        }
      }
    }

    if (bDrawVol1) {
      DrawVolumeEdges(vol);
    }

    if (bDrawSection) {

      Point pt = vol.pointAt(0.0, 0.0, wc);
      TSection tsec = vol.tensorAt(wc);

      for (int i = 0; i <= 9; ++i) {
        float ti = (float)i / 9;
        DrawCurve(Round::location(pt.boost(tsec.logV * ti)), tsec.logU, 10, 0,
                  0, 0);
        DrawCurve(Round::location(pt.boost(tsec.logU * ti)), tsec.logV, 10, 0,
                  0, 0);
      }
      Draw(tsec.tu, .3, 0.0, 0.0);
      Draw(tsec.tv, 0, .3, 0);
      Draw(tsec.tu1, .3, 0, 0);
      Draw(tsec.tv1, 0, .3, 0);

      if (bDrawCircle) {
        glColor3f(.3, .3, .3);
        glBegin(GL_TRIANGLE_STRIP);

        for (int i = 0; i <= 100; ++i) {
          float ti = (float)i / 100 * PI;
          Vec v = Vec::x.spin(Gen::rot(Biv::xy * ti)) * .5 + Vec(.5, .5, 0.0);
          Vec v2 = Vec::x.spin(Gen::rot(Biv::xy * ti)) * .4 + Vec(.5, .5, 0.0);
          Point p = vol.pointAt(v[0], v[1], wc);
          Point p2 = vol.pointAt(v2[0], v2[1], wc);
          GL::vertex(p.begin());
          GL::vertex(p2.begin());
        }

        glEnd();
      }

      int iter = 0;
      for (auto &i : faces) {
        if (iter < 6) {
          if (bDrawVol1)
            i.draw(true, alpha);
        } else if (bDrawVol2)
          i.draw(false, alpha);
        iter++;
      }

      if (decay > 0) {
        DrawRound(tsec.su0, 1, 0, 0, decay);
        DrawRound(tsec.su1, 1, 0, 0, decay);
        DrawRound(tsec.sv0, 0, 1, 0, decay);
        DrawRound(tsec.sv1, 0, 1, 0, decay);
      }

    }

    else {

      int iter = 0;
      for (auto &i : faces) {
        if (iter < 6) {
          if (bDrawVol1)
            i.draw(true, alpha);
        } else if (bDrawVol2)
          i.draw(false, alpha);
        iter++;
      }
    }

    if (bDrawIMapSurface) {
      Point tp = vol.pointAt(uc, vc, 0.0);

      DualSphere su = tp <= vol.duvw0();
      DualSphere sv = tp <= vol.dvuw0();

      DrawRound(su, 1, .3, 0, .2);
      DrawRound(sv, .3, 1, 0, .2);
    }

    if (bDrawBaseSpheres) {
      DrawRound(vol.tf().suw, 1, 0, .3, .2);
      DrawRound(vol.tf().suv, 1, .3, 0, .2);
      DrawRound(vol.tf().svu, 0.3, 1, 0.3, .2);
      DrawRound(vol.tf().svw, 0.0, 1, 0.3, .2);
      DrawRound(vol.tf().swu, .3, 0, 1, .2);
      DrawRound(vol.tf().swv, 0, .3, 1, .2);
    }
    if (bDrawUVSpheres) {
      DrawRound(vol.tf().suv, 1, .3, 0, .2);
      DrawRound(vol.tf().svu, 0.3, 1, 0.3, .2);
      DrawRound(vol.vf().svu, .3, 1, 0, .2);
      DrawRound(vol.uf().suv, 1.0, 0.3, 0, .2);
    }
    if (bDrawLidSpheres) {
      DrawRound(vol.uf().suw, 1, 0, 0, .2);
      DrawRound(vol.vf().svu, 0, 1, 0, .2);
      DrawRound(vol.wf().swv, 0, 0, 1, .2);
    }
    if (bDrawUSpheres) {
      DrawRound(vol.tf().suw, 1, 0, .3, .2);
      DrawRound(vol.tf().suv, 1, .3, 0, .2);

      DrawRound(vol.uf().suw, 1, 0, .3, .2);
      DrawRound(vol.uf().suv, 1, .3, 0, .2);

      DrawRound(vol.vf().suw, 1, 0, .3, .2);
      DrawRound(vol.wf().suv, 1, .3, 0., .2);

      DrawRound(vol.uvf().suw, 1, 0, .3, .2);
      DrawRound(vol.uwf().suv, 1, .3, 0, .2);
    }
    if (bDrawVSpheres) {
      DrawRound(vol.tf().svu, 0.3, 1, 0.3, .2);
      DrawRound(vol.tf().svw, 0.0, 1, 0.3, .2);

      DrawRound(vol.vf().svu, 0.3, 1, 0.0, .2);
      DrawRound(vol.vf().svw, 0.0, 1, 0.3, .2);

      DrawRound(vol.wf().svu, 0.3, 1, 0.0, .2);
      DrawRound(vol.uf().svw, 0.0, 1, 0.3, .2);

      DrawRound(vol.vwf().svu, 0.3, 1, 0.0, .2);
      DrawRound(vol.uvf().svw, 0.0, 1, 0.3, .2);
    }
    if (bDrawWSpheres) {
      DrawRound(vol.tf().swu, .3, 0, 1, .2);
      DrawRound(vol.tf().swv, 0, .3, 1, .2);

      DrawRound(vol.wf().swu, .3, 0, 1, .2);
      DrawRound(vol.wf().swv, .0, .3, 1, .2);

      DrawRound(vol.vf().swu, .3, 0, 1, .2);
      DrawRound(vol.uf().swv, 0, .3, 1, .2);

      DrawRound(vol.vwf().swu, .3, 0, 1, .2);
      DrawRound(vol.uwf().swv, 0, .3, 1, .2);
    }
    if (bDrawVDirSpheres) {
      DrawRound(vol.tf().suv, 1, 0, .3, .2);
      DrawRound(vol.tf().swv, 0, .3, 1, .2);
    }
  }
};

int main() {

  MyApp app;
  app.start();

  return 0;
}
