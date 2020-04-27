#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include "vsr/form/vsr_graph.h"
#include "vsr/draw/vsr_graph_draw.h"


using namespace vsr::cga;
using namespace gfx;

struct MyApp : App {


  float alpha;
  float amt;
  float k1 = 0;
  float k2 = 0;

  bool bFlipSphereA = false;
  bool bFlipSphereB = false;
  bool bFlipRotor = false;
  bool bCW= false;
  bool bTWOPI= false;
  bool bLights = true;

  Frame frame;
  Frame light;

  void setup(){

    gui(alpha, "alpha", 0, 1);
    gui(amt, "amt", -1, 1);
    gui(k1, "k1", -100, 100);
    gui(k2, "k2", -100, 100);
    gui(bFlipSphereA, "flip sphere A");
    gui(bFlipSphereB, "flip sphere B");
    gui(bFlipRotor, "flip rotor");
    gui(bCW, "flip ccw");
    gui(bTWOPI, "flip TWOPI");
    gui(bLights, "lights");

    gui (ps.bShadedOutput, "bShadedOutput");  ///< default for output
    gui (ps.bSortOutput, "bSortOutput");      ///< default
    gui (ps.bSimpleSort, "bSimpleSort");      ///< default
    gui (ps.bOffsetOutput, "bOffsetOutput");
    gui (ps.bOccludeOutput, "bOccludedOutput");
    gui (ps.bTightBounds, "bTightBounds");
    gui (ps.bPDF, "bPDF");  ///< pdf or eps
    objectController.attach(&frame);
    objectController.attach(&light);

    light.pos(2,2,0);
  }

  void onDraw(){

    //NOTES:
    // Generally
    // point <= sphere  is POSITIVE if point is INSIDE sphere
    // Inf(1) <= tangent makes a plane, with norml
    // point <= plane is POSITIVE if point is ABOVE plane
    Point pos = Construct::point (-1,0,0);
    Point tpos = Construct::point (0,0,0);
    DualSphere dls = Construct::sphere (1,0,0,1);

    Pair tp1 = Pair(Tnv(1,0,0)).trs (-1,0,0);
    Pair tp2 = Pair (Tnv(1,0,0)).trs (1,0,0);
    Pair tp3 = Pair (Tnv(1,0,0)).trs (1.5,1.5,0);

    auto normalize_p= [](const DualSphere& a){
      float flip = (a[3] < 0) ? -1.0 : 1.0;
      return ((a[3] == 0) ? a : a/a[3] ) * flip;
    };

    float signA = bFlipSphereA ? -1.0 : 1.0;
    float signB = bFlipSphereB ? -1.0 : 1.0;
    DualSphere tds1 = (Inf(signA) <= tp1) * (tp1 * k1 + 1);
    DualSphere tds2 = (Inf(signB) <= tp2) * (tp2 * k2 + 1);
    DualSphere tds3 = -tds1 <= tp3;

    DualSphere pds1 = normalize_p (tds1);
    DualSphere pds2 = normalize_p (tds2);
    DualSphere pds3 = normalize_p (tds3);

    Pair pair = Gen::log ((pds2/pds1).tunit() * (bFlipRotor ? -1.0 : 1.0), bCW, bTWOPI) * .5;

    DualSphere pds4 = normalize_p (-tpos <= pair);
    Pair tp4 = pds4 ^ tpos;

    float flipA = ((pds1 <= tp1)[3] < 0) ? -1.0 : 1.0;
    float flipB = ((pds2 <= tp2)[3] < 0) ? -1.0 : 1.0;
    float flipC = ((pds3 <= tp3)[3] < 0) ? -1.0 : 1.0;
    float flipD = ((pds4 <= tp4)[3] < 0) ? -1.0 : 1.0;

    float p1 = (pos <= pds1)[0];
    float p2 = (pos <= pds2)[0];
    float p3 = (pos <= pds3)[0];
    float p4 = (pos <= pds4)[0];

    auto tmp = [=](const DualSphere& a, const DualSphere& b){
      auto rat = (a / b).tunit();
      return Ori(1) <= (Gen::log (rat) * .5);
    };

    int num = 10;

//    cout << pds1[3] << " " << pds2[3]<< " "  << pds3[3] << " " << pds4[3] << endl;
//
   // cout << pds1 << endl;
    //((Inf(-1) <= pds1.undual()) ^ Inf(1)).print();
    //
//    cout << (pds2 <= tp1)[3] << endl;
    cout << (pos <= pds2) << endl;

    for (int i = 0; i < 10; ++i){

      float t = (float)i/10.0;
      Boost b= Gen::boost (pair * t);
      Point np = ( Round::location (pos.spin(b)));
      Draw (np);

    }
//    Draw (tp1);
//    Draw (tp2);

    Draw(pds1 ^ pos);
    DrawRound (pds1, 1,0,0,.5);
    DrawRound (pds2, 1,0,0,.5);
//    DrawRound (pds3, 1,0,0,.5);
//    DrawRound (pds4, 1,0,0,.5);


  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
