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

  Frame frame;

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

    gui (ps.bShadedOutput, "bShadedOutput");  ///< default for output
    gui (ps.bSortOutput, "bSortOutput");      ///< default
    gui (ps.bSimpleSort, "bSimpleSort");      ///< default
    gui (ps.bOffsetOutput, "bOffsetOutput");
    gui (ps.bOccludeOutput, "bOccludedOutput");
    gui (ps.bTightBounds, "bTightBounds");
    gui (ps.bPDF, "bPDF");  ///< pdf or eps
    objectController.attach(&frame);
  }

  void onDraw(){

    //Draw(frame);

    //NOTES:
    // Generally
    // point <= sphere  is POSITIVE if point is INSIDE sphere
    // Inf(1) <= tangent makes a plane, with norml
    // point <= plane is POSITIVE if point is ABOVE plane
    Point pos = Construct::point (-1,0,0);
    Point tpos = Construct::point (0,0,0);
    DualSphere dls = Construct::sphere (1,0,0,1);

    Pair tp1 = Pair (Tnv(0,-1,0)).trs (-1,0,0);
    Pair tp2 = Pair (Tnv(0,1,0)).trs (1,0,0);
    Pair tp3 = Pair (Tnv(1,0,0)).trs (1.5,0,0);

    auto normalize_p= [](const DualSphere& a){
      float flip = (a[3] < 0) ? -1.0 : 1.0;
      return ((a[3] == 0) ? a : a/a[3] ) * flip;
    };

    float signA = bFlipSphereA ? -1.0 : 1.0;
    float signB = bFlipSphereB ? -1.0 : 1.0;
    DualSphere tds1 = (Inf(signA) <= tp1) * (tp1 * k1 + 1);
    DualSphere tds2 = (Inf(signB) <= tp2) * (tp2 * k2 + 1);
    DualSphere tds3 = tds2 <= tp3;

    DualSphere pds1 = normalize_p (tds1);
    DualSphere pds2 = normalize_p (tds2);
    DualSphere pds3 = normalize_p (tds3);

    Pair pair = Gen::log ((pds2/pds1).tunit() * (bFlipRotor ? -1.0 : 1.0), bCW, bTWOPI) * .5;

    DualSphere pds4 = normalize_p (tpos <= (pds1 ^ pds2));
    Pair tp4 = pds4 ^ tpos;
    //Draw(tp4);
    
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

    Pair tp = (pds2/pds1).tunit();
  
    //Pair ntp = TFrame::NormalizePair(pds2 ^ Round::location (pos.spin(Gen::boost(pair))));
    Pair ntpA = TFrame::NormalizePair(pds1 ^ pos);
    Pair ntpB = TFrame::NormalizePair(pds2 ^ Round::location (pos.spin(Gen::boost(pair))));

    float fA = (pds1 <= ntpA)[3];
    float fB = (pds2 <= ntpB)[3];
    //Draw (tp1,1,0,0);
    Draw (ntpA,1,0,1);
    Draw (ntpB,1,0,1);
    Draw(tp1,1,0,0);
    Draw(tp2,1,0,0);

    DrawCurve (pos, pair, 50, 0,1,0);
    DrawRound (pds1, 1.0, 0.0,0.0,alpha);
    Draw ((pds1^pds2).undual(), 1.0, 0.0,1.0);
    DrawRound (pds2, 1.0, 0.0,0.0,alpha);
    //for (int i = 0; i <= num; ++ i)
    //{
    //  float t = (float)i/num;

      Boost tb = Gen::boost (pair * amt);
      DualSphere ds = -TFrame::Normalize(pds1.spin(tb));
      Draw(ds,0, 0, 1, .5);

      cout << pds1 << endl;
      cout << pds2 << endl;
//      cout << ds <<endl;
      //Point tp = Round::location(pos.spin (tb));
      //auto sp = normalize_p(-tp <= pair);
      //auto pair1 = tmp(sp, pds1);
      //auto pair2 = tmp(pds2, pds1);
      //float tv = (pair1 <= !pair2)[0];
//    cout << "WEIGHT" << tp.wt() <<  " " << (-tp).wt() << endl;
//    cout << "SPHERE DOT ITS OWN TANGENT" << endl;
//    cout << flipA << " " << flipB << endl;
//    cout << fA << " " << fB << endl;
//    cout << "POINT POSITION" << endl;
//    cout <<  p2 << endl;


//    }


  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
