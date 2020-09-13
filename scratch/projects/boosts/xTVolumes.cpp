#include <array>
#include <vsr/vsr_app.h>

#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include "vsr/form/vsr_graph.h"
#include "vsr/draw/vsr_graph_draw.h"

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

void DrawGraph (const HEGraph<Point>& graph, int resU, int resV, bool bStart, 
    float red=1.0, float green=1.0, float blue=1.0, float alpha=1.0)
{
   glBegin(GL_TRIANGLES);
   bool bC= bStart;
   bool bS= false;
   int iter =0;
   //black and white
   for (auto& i : graph.face()){
       auto& a = i->a();
       auto& b = i->b();
       auto& c = i->c();
       //glColor4f(.2,1,.2,.7);
       Vec normal = (Vec(b-a) ^ Vec(c-a)).duale().unit();
       GL::normal( normal.begin() );

       glColor4f(bC * red,bC * green,bC * blue, alpha);
       GL::vertex( a.begin() );
       GL::vertex( b.begin() );
       GL::vertex( c.begin() );
       if (bS) bC = !bC;
       bS = !bS;
   }

   glEnd();

}

struct Cylinder {

  HEGraph<Point> graph;
  std::vector<Point> pnts;
  std::vector<Vec> coords;

  int resU, resV;
  float height, radius;

  void init(int _resU, int _resV) {
    resU = _resU;
    resV = _resV;
    pnts.resize(resU * resV);
    coords.resize(resU * resV);

    graph.UV (resU, resV, pnts.data (), true, false );

    //makeCoords();
    diagCoords();
  }

  void makeCoords(){

    for (int i = 0; i < resU; ++i)
    {
      for (int j = 0; j < resV; ++j)
      {
            auto tu = (float) i / resU * PI;
            auto tv = (float) j / (resV-1);

            auto rot = Gen::rot (Biv::yz * tu);
            auto vecx = Vec::y.spin (rot) * 0.5 + Vec(0.0, 0.5, 0.5);
            int idx = i * resV + j;
//            coords[idx] = vecx + Vec(0,tv,0);
            coords[idx] = vecx + Vec(tv,0,0);
      }
    }
  }

  void diagCoords(){

    Vec vec (1.0, 1.0, 1.0);
    Rot rotax = Gen::ratio (Vec::y, vec.unit());

    for (int i = 0; i < resU; ++i)
    {
      for (int j = 0; j < resV; ++j)
      {
            int idx = i * resV + j;

            auto tu = (float) i / resU;
            auto tv = (float) j / (resV-1);

            auto rot = Gen::rot (Biv::xz * tu * PI);
            auto vecx = Vec::x.spin (rot)  * tv * .2;
            Vec tvec = vecx + Vec(0,tv * ROOT2,0);
            coords[idx] = tvec.spin(rotax);
      }
    }
  }
  void coneCoords(){

    Vec vec (1.0, 1.0, 1.0);
    Rot rotax = Gen::ratio (Vec::y, vec.unit());

    for (int i = 0; i < resU; ++i)
    {
      for (int j = 0; j < resV; ++j)
      {
            int idx = i * resV + j;

            auto tu = (float) i / resU;
            auto tv = (float) j / (resV-1);

            auto rot = Gen::rot (Biv::xz * tu * PI);
            auto vecx = Vec::x.spin (rot) * tv * .5;
            Vec tvec = vecx + Vec(0,tv,0);
            coords[idx] = tvec.spin(rotax);
      }
    }
  }

  void draw(bool bStart){
    DrawGraph (graph, resU, resV, bStart, 1.0, .3, .3, 1.0);
  }
};

struct TFace {

  HEGraph<Point> graph;
  std::vector<Point> pnts;

  int resU, resV;

  void init(int _resU, int _resV) {
    resU = _resU;
    resV = _resV;
    pnts.resize(resU * resV);
    graph.UV (resU, resV, pnts.data (), false, false);
  }

  void draw(bool bStart, float alpha){
    DrawGraph (graph, resU, resV, bStart, 1.0, 1.0, 1.0, alpha);
  }

};

/// Curvature Parameter
struct CParam {

  float val = 0;
  float frq = .02;
  bool osc = false;
  string name;

  CParam (string _name) : name(_name) {}

  float eval(float timer)
  {
    return osc ? val * (sin(timer * frq)+1)/2.0 : val;
  }

};


struct MyApp : App {

  Cylinder cylinder;
  Cylinder cylinderB;
  std::array<TFace, 12> faces;

  float resU = 10;
  float resV = 10;
  float resW = 10;

  bool bInverse = true;
  //Some Variables
  float alpha = 1.0;

  float uc = 0;
  float vc = 0;
  float wc = 0;

  float uSpacing = 3;
  float vSpacing = 3;
  float wSpacing = 3;

  CParam kVU = CParam("kVU");
  CParam kWU = CParam("kWU");;
  CParam kUV =  CParam("kUV");
  CParam kWV =  CParam("kWV");
  CParam kUW =  CParam("kUW");
  CParam kVW =  CParam("kVW");
  CParam kV1U = CParam("kV1U");
  CParam kU1W = CParam("kU1W");
  CParam kW1V = CParam("kW1V");

  float preset = 0;

  bool bDrawVol2 = false;
  bool bDrawBorderPoints= false;
  bool bDrawFrames = false;
  bool bDrawEdges = false;
  bool bDrawCylinder = false;
  bool bDrawSpheres= false;

  float timer = 0;

  void guiElement(CParam& cp)
  {
    gui (cp.val, cp.name, -100,100);
    gui (cp.frq, "frq", 0,10);
    gui (cp.osc, "bOsc");
  }

  void setup(){
    ///Add Variables to GUI
    gui(preset,"preset",0,10);
    guiElement (kVU);
    guiElement (kWU);
    guiElement (kUV);
    guiElement (kWV);
    guiElement (kUW);
    guiElement (kVW);
    guiElement (kV1U);
    guiElement (kU1W);
    guiElement (kW1V);

    gui (alpha, "alpha", -1, 1);

    gui (uc, "uc", 0, 1);
    gui (vc, "vc", 0, 1);
    gui (wc, "wc", 0, 1);

    gui (uSpacing, "uSpacing", 1, 10);
    gui (vSpacing, "vSpacing", 1, 10);
    gui (wSpacing, "wSpacing", 1, 10);
    gui (bInverse, "bInverse");
    gui (bDrawVol2, "bDrawVol2");
    gui (bDrawBorderPoints, "bDrawBorderPoints");
    gui (bDrawFrames, "bDrawFrames");
    gui (bDrawEdges, "bDrawEdges");
    gui (bDrawCylinder, "bDrawCylinder");
    gui (bDrawSpheres, "bDrawSpheres");

    gui (ps.bShadedOutput, "bShadedOutput");  ///< default for output
    gui (ps.bSortOutput, "bSortOutput");      ///< default
    gui (ps.bSimpleSort, "bSimpleSort");      ///< default
    gui (ps.bOffsetOutput, "bOffsetOutput");
    gui (ps.bOccludeOutput, "bOccludedOutput");
    gui (ps.bTightBounds, "bTightBounds");
    gui (ps.bPDF, "bPDF");  ///< pdf or eps

    ps.bOffsetOutput = false;
    ps.bOccludeOutput = false;
    ps.bTightBounds= true;

    scene.camera.pos(0,0,10);

    for (auto& i: faces)
      i.init (resU, resV);

    cylinder.init(20, 20);
    cylinderB.init(20, 20);
  }

  void calcPreset()
  {
    switch ((int)preset)
    {
      case 0:
        break;

      case 1: 
        kVU.val = kVW.val = kUV.val = kUW.val = kWU.val = kWV.val = -.8;
        kVU.osc = kVW.osc = kUV.osc = kUW.osc = kWU.osc = kWV.osc = true;
        kVU.frq = kVW.frq = kUV.frq = kUW.frq = kWU.frq = kWV.frq = .02;

      default:
        break;
    }

  }

  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    calcPreset();

    timer++;

    // Two volumes,
    // one is attached to the RIGHT face of the first to the RIGHT face
    TVolume vol (kVU.eval(timer), kWU.eval(timer),
                 kUV.eval(timer), kWV.eval(timer),
                 kUW.eval(timer), kVW.eval(timer),
                 kV1U.eval(timer), kU1W.eval(timer), kW1V.eval(timer),
                 uSpacing, vSpacing, wSpacing);

    TVolume vol2 (vol, TVolume::Face::RIGHT);

    int citer = 0;
    for (auto& i : cylinder.coords)
    {
      cylinder.pnts[citer] = vol.pointAt (i[0], i[1], i[2]);
      citer++;
    }

    citer = 0;
    for (auto& i : cylinder.coords)
    {
      Point tp = vol.pointAt (1.0, i[1], i[2]);
      auto coord = vol2.inverseMapping (tp, TVolume::Face::LEFT);
      
      cylinderB.pnts[citer] = vol2.pointAt (i[0], coord.v, coord.w);
      citer++;
    }

    if (bDrawCylinder)
    {
      cylinder.draw(true);
      if (bDrawVol2)
        cylinderB.draw(false);
    }

    TVolume::Mapping mapping = vol.calcMapping(resU, resV, resW);
    TVolume::Mapping mapping2 = vol2.calcMapping(resU, resV, resW);

    if (bDrawFrames)
    {
      DrawVolumeFrames (vol);
      DrawVolumeFrames (vol2);
      Con con = vol.calcMappingAt(uc,vc,wc);
      Pair tu = vol.tf().tu.spin (con);
      Pair tv = vol.tf().tv.spin (con);
      Pair tw = vol.tf().tw.spin (con);

      Draw (tu, 1,0,0);
      Draw (tv, 0,1,0);
      Draw (tw, 0,0,1);
    }

    if (bDrawEdges)
    {
      DrawCurve(vol.pos(),vol.duvw0(),20,1,.3,.3); 
      DrawCurve(vol.pos(),vol.dvuw0(),20,.3,1,.3); 
      DrawCurve(vol.pos(),vol.dwvu0(),20,.3,.3,1); 
    }

     //Draw Faces
    for (int i = 0 ; i < resU; ++i)
    {
      for (int j = 0 ; j < resV; ++j)
      {
       for (int k = 0 ; k < resW; ++k)
       {

         Point pt = vol.pointAt(i,j,k,mapping);

         if (i==0)
          faces[0].pnts [j*resW +k] = pt; 
         if (i==resU-1)
          faces[1].pnts [j*resW +k] = pt; 
         if (j==0)
          faces[2].pnts [i*resW +k] = pt; 
         if (j==resV-1)
          faces[3].pnts [i*resW +k] = pt; 
         if (k==0)
          faces[4].pnts [i*resV +j] = pt; 
         if (k==resW-1)
          faces[5].pnts [i*resV +j] = pt; 
       }
      }
    }

    if (bDrawBorderPoints)
    {
      for (int i = 0 ; i < resU; ++i)
      {
        for (int j = 0 ; j < resV; ++j)
        {
         for (int k = 0 ; k < resW; ++k)
         {
           if (i==resU-1)
           {
             DrawPoint (vol.pointAt(i,j,k,mapping),0,1,0);
             DrawPoint (vol2.pointAt(0,j,k,mapping2),1,0,0);
           }
         }
        }
      }
    }

      //Draw Faces Vol2
      for (int i = 0 ; i < resU; ++i)
      {
        float ti = (float )i/(resU-1);
        for (int j = 0 ; j < resV; ++j)
        {
         float tj = (float )j/(resV-1);
         Point ptBackRight = vol.pointAt(1.0,tj,0.0);
         Point ptFrontRight = vol.pointAt(1.0,tj,1.0);
         for (int k = 0 ; k < resW; ++k)
         {
           float tk = (float )k/(resW-1);
           Point ptBottomRight = vol.pointAt(1.0,0.0,tk);
           Point ptTopRight = vol.pointAt(1.0,1.0,tk);

           //Front
           if (k==resW-1)
           {
            auto coord = vol2.inverseMapping (ptFrontRight, TVolume::Face::FRONT);
            faces[6].pnts [i*resV +j] = vol2.pointAt(ti, bInverse ? coord.v : tj, 1.0); 
           }
           //Top
           if (j==resV-1)
           {
            auto coord = vol2.inverseMapping (ptTopRight, TVolume::Face::LEFT);
            faces[7].pnts [i*resW +k] = vol2.pointAt(ti, 1.0, bInverse ? coord.w : tk); 
           }
           //Bottom
           if (j==0)
           {
            auto coord = vol2.inverseMapping (ptBottomRight, TVolume::Face::LEFT);
            faces[8].pnts [i*resW +k] = vol2.pointAt(ti, 0.0, bInverse ? coord.w : tk); 
           }
           //Back
           if (k==0)
           {
            auto coord = vol2.inverseMapping (ptBackRight, TVolume::Face::BACK);
            faces[9].pnts [i*resV +j] = vol2.pointAt(ti, bInverse ? coord.v : tj, 0.0); 
           }
           //Right
           if (i==resU-1)
           {
            auto coordV = vol2.inverseMapping (ptBackRight, TVolume::Face::BACK);
            auto coordW = vol2.inverseMapping (ptBottomRight, TVolume::Face::LEFT);
            faces[10].pnts [j*resW +k] = vol2.pointAt(1.0, bInverse ? coordV.v : tj, bInverse ? coordW.w : tk); 
           }
         }
        }
      }

    int iter = 0;
    for (auto& i : faces)
    {
      if (iter < 6)
         i.draw(true, alpha);
      else if (bDrawVol2)
         i.draw(false, alpha);
      iter++;
    }

    if (bDrawSpheres)
    {
      DrawRound(vol.uf().suw,1,0,0,.2);
      DrawRound(vol.vf().svu,0,1,0,.2);
      DrawRound(vol.wf().swv,0,0,1,.2);
    }

   }

};


int main(){

  MyApp app;
  app.start();

  return 0;
}
