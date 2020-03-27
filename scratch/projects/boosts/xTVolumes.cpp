
#include <vsr/vsr_app.h>

#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include "vsr/form/vsr_graph.h"
#include "vsr/draw/vsr_graph_draw.h"

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;


struct MyApp : App {

  bool bInverse;
  //Some Variables
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
    gui (bInverse, "bInverse");


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

    TVolume tv (kVU, kWU,
                kUV, kWV,
                kUW, kVW,
                kV1U, kU1W, kW1V,
                uSpacing, vSpacing, wSpacing);

    int num = 5;

//    for (auto& i : cmap.mCon)
//    {
//       Point pw = Round::location(tv.tf().pos().spin(i));
//       Draw (pw, 1,1,0);
//    }

    DrawFrame(tv.tf());
    DrawFrame(tv.uf());
    DrawFrame(tv.vf());
    DrawFrame(tv.wf());
    DrawFrame(tv.uvf());
    DrawFrame(tv.vwf());
    DrawFrame(tv.uwf());
    DrawFrame(tv.uvwf());

    TVolume tv2 (tv, TVolume::Face::RIGHT);

    DrawFrame(tv2.tf());
    DrawFrame(tv2.uf());
    DrawFrame(tv2.vf());
    DrawFrame(tv2.wf());
    DrawFrame(tv2.uvf());
    DrawFrame(tv2.vwf());
    DrawFrame(tv2.uwf());
    DrawFrame(tv2.uvwf());

    TVolume::Mapping cmap = tv.calcMapping(num);
    TVolume::Mapping cmap2= tv2.calcMapping(num);

    for (auto& i : cmap.mCon)
      Draw (Round::location(tv.tf().pos().spin(i)));

    auto cdraw = [=](const Point &origin, const TVolume::Mapping& mapping){
      for (int i = 0; i < num; ++i)
      {
       glColor4f (0, .3, 0, 1);
       glBegin (GL_LINE_STRIP);
         for (int j = 0; j < num; ++j)
         {
           Con c = mapping.at(j,i,num-1);
           Point pw = Round::location(origin.spin(c));
           GL::vertex (pw);
          }
       glEnd();
      }
    };

    cdraw(tv.tf().pos(), cmap);


   for (int i = 0; i < num; ++i)
   {
     float ti = (float)i/(num-1);
     Con c = cmap.at(num-1,i,num-1);
     Point pw = Round::location(tv.tf().pos().spin(c));
     Draw (Construct::sphere (pw, .05), 1,0,0);

     TVolume::Coord tc = tv2.inverseMapping (pw, TVolume::Face::FRONT);

     glColor4f (0, .5, .5, 1);
     glBegin (GL_LINE_STRIP);
     for (int j = 0; j < num; ++j){
       float tj = (float)j/(num-1);
       Con c2 = bInverse ? tv2.calcMapping (tj, tc.v, 1.0) : tv2.calcMapping (tj, ti, 1.0);
       Point pw2 = Round::location (tv2.tf().pos().spin(c2));
       GL::vertex (pw2);
     }
     glEnd();
   }


   }

};


int main(){

  MyApp app;
  app.start();

  return 0;
}
