/**
  * Try to find percentage of transformation, given a point
  */

#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App
{

  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  float amt1 = 0;
  float amt2 = 0;


  Frame frame;
  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (amt1, "amt1", -100, 100) (amt2, "amt2", -100);
    gui (bToggle, "bToggle") (bSet, "bSet");

    frame.pos (1, 2, 0);

    objectController.attach (&frame);
  }



  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {
    //calculate mouse position in world space
    if (bMouseSet)
      calcMouse3D ();

    auto s1 = Construct::sphere (-2.0, 0.0, 0.0, 0.0);
    auto s2 = Construct::sphere (2.0, 0.0, 0.0, 0.0);

    auto sy0 = Construct::sphere (0.0, -2.0, 0.0, 0.0);
    auto sy1 = Construct::sphere (0.0, 2.0, 0.0, 0.0);


    //s1 = s1.unit();
    //s2 = s2.unit();
    //auto log = Gen::log (Gen::ratio (s1, s2, bToggle));
    auto log = Gen::log (-(s2 / s1).runit ()) * .5;
    //auto rlog = Gen::log (-(s1 / s2).runit ());

    //get begin and end points, and circle and line between them
    auto pt = frame.pos ();
    auto p = Tangent::at (s1.undual (), pt).dual ();
    auto cp = Round::carrier (p);
    auto begin = Round::split ((s1 ^ cp.dual ()).dual (), false);
    auto end = Round::loc (begin.bst (log));
    auto cir = Tangent::at (s1.undual (), begin).dual () ^ s2;
    auto lin = begin ^ end ^ Inf (1);


    auto tat = CXY (.1).trs (begin);

    for (int i = 0; i <= 10; ++i)
      {
        float t = (float) i / 10.0;
        auto bst = Gen::bst (log * t);
        //    draw (tat.spin (bst), t, 0, 1 - t);
      }


    draw (cir);
    draw (lin);

    //    draw (tat);

    //curved trajectory generator
    auto log_c = log * amt1;
    //    auto log_rc = rlog * amt1 * .5;
    //straight trajectory generator (as the crow flies)
    auto log_s = Vec (end - begin).copy<Drv> () * amt1;
    //    auto log_rs = Vec (begin - end).copy<Drv> () * amt1;

    //general rotation ratio
    //    Bst tbst = Gen::bst (log_c) / Gen::trs (log_s);
    //    auto log_r = Gen::log (Bst(tbst));

    auto np = s1.trs ( Vec(s2-s1) * amt1 + Vec(sy1-sy0) * amt2);//mMouse3D;//Round::loc (begin.bst (log_c));
    auto sp = begin.trs (log_s);
    //    auto spr = end.trs (log_rs);
    draw (sp, 1, 1, 0);

//    auto dlp = sp <= log_s;
//    draw (dlp);

    for (int i = 0; i < 10; ++i)
      {
        //        float t = (float) i / 10.0;
        //        auto tb = Gen::bst (log_r * t);
        //        auto tbr = Gen::bst (log_rr * t);
        //        draw (Round::loc (sp.spin (tb)));
        //        draw (Round::loc (spr.spin (tbr)));
      }

    auto tpss = !(s1 ^ s2);
    auto tre1 = s2 <= tpss;
    auto tre2 = s1 <= tpss;

    //contract with s1 ^ s2 to get surface there
//    auto tproj = np <= (log);//tpss;
    auto tproj = np <= ((s1 ^ s2).runit());//tpss;
    auto tprojY = np <= ((sy0 ^ sy1).runit());//tpss;

    //auto tproj2 = np <= (Round::loc (s1) ^ Round::loc (s2));
    //draw (tproj2);

    // Tangent at p relative to s1 and d2 (i.e. p <= (p^B))
    auto tan = Tangent::at (tproj.undual(), np).dual();//np ^ log, np);
    auto tanY = Tangent::at (tprojY.undual(), np).dual();//np ^ log, np);

    //In fact let's get surface gradients at begin and end and make recip frame from them
    auto bproj = begin <= log;
    auto eproj = end <= log;
    auto btan = Tangent::at (bproj.undual(), begin).dual();
    auto etan = Tangent::at (eproj.undual(), end).dual();

    draw( btan, 1,0,0);
    draw( etan, 1,0,0);

    auto rpss = !(btan ^ etan ^ Inf(1));
    auto rtan1= (etan ^ Inf(1)) <= rpss;
    auto rtan2= (btan ^ Inf(1)) <= rpss;

    draw (rtan1, 0, 1, 0);
    draw (rtan2, 0, 1, 2);

    // should be 1 and 0
    cout << "a: " << (btan <= rtan1) << endl;
    cout << "b: " << (btan <= rtan2) << endl;
    cout << "wt: " << btan.wt() << endl;
    cout << "twt: " << nga::Tangent::wt (btan) << endl;


    //Q: how to extract curvature here?


    //d . tproj + d ^ tproj (shape tensor??)
    auto dA = tre1 * tproj;
    auto dB = tre2 * tproj;

    //there's also (p <= B) / B (project) and (p ^ B) / B (reject)
    auto proj = DualSphere (Op::project (np, log));
    auto rej = DualSphere (Op::reject (np, log));

    draw (tan);
    draw (tanY);


    //given np, how to find surface s'?
    //point p, reflected over tensor
    //can coordinate it be simplified from?
    // (log (s'/s1))/(log (s2/s1))
    auto atn = np.reflect (s1 ^ s2);
    auto atnp = Round::loc (atn);
    auto s1p1 = tan.dual () ^ atnp;
    draw (atnp, 1, 0, 0);

    //hmmm, use to generate a transformation?
    draw (Cir (tan ^ Inf (1)).bst (tan * amt2));

    auto s1p = s1.bst (log * amt1);
    auto sublog = Gen::log (-(s1p / s1).runit ());

    for (int i = 0; i < 100; ++i)
      {
        float t = (float) i / 100.0;
        auto tp = begin.spin (Gen::bst (log * t));
        draw (Round::loc (tp), 0, 1, 1);
      }

    auto tmp = Gen::log ((s1p / s1).runit ()) <= !Gen::log ((s2 / s1).runit ());

    auto pss = !(begin ^ s1 ^ s2);
    auto rn = (s1 ^ s2) <= pss;
    auto rn1 = (s2 ^ begin) <= pss;
    auto rn2 = (s1 ^ begin) <= pss;

    auto rx = rn;
    auto nx = (s2 ^ np) <= (pss);

    //draw (Round::sphere (np, .1), 1, 0, 1);
    draw (p, 0, 1, 0);
    draw (begin, 0, 1, 1);
    draw (cp, 0, 1, 0);
    draw ((tproj ^ Dlp(0,0,1)).undual(), 0,1,0);
    draw ((tproj ^ Dlp(0,0,1) ^ tprojY).undual(), 0,1,1);
    draw ((tprojY ^ Dlp(0,0,1)).undual(), 1,1,1);
    draw (s1, 1, 0, 0, .1);
    draw (s2, 1, 0, 0, .1);
    draw (tproj, 0, 1, 0, .1);
  }

  /*-----------------------------------------------------------------------------
   *  Keyboard
   *-----------------------------------------------------------------------------*/
  void onKeyDown (const gfx::Keyboard &k)
  {
    if (k.code == 's')
      bMouseSet = !bMouseSet;
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
