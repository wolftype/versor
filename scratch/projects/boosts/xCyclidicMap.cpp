/**
  * Find the simple rotor at x
  */

#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/form/vsr_conic.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App
{

  //Some Variables
  bool bToggleA = false;
  bool bToggleB = false;
  bool bSetA = false;
  bool bSetB = false;

  bool bMouseSet = false;
  float amt1 = 0;
  float amt2 = 0;
  float amt3 = 0;

  Frame ta;
  Frame tb;
  Frame tc;
  Frame td;

  Frame frame;

  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    mColor.set (0, 0, 0);
    ///Add Variables to GUI
    gui (amt1, "amt1", -100, 100);
    gui (amt2, "amt2", -100, 100);
    gui (amt3, "amt3", -100, 100);
    gui (bToggleA, "bToggleA");
    gui (bToggleB, "bToggleB");
    gui (bSetA, "bSetA");
    gui (bSetB, "bSetB");

    //Top Left
    ta.pos () = PT (-5, 2, 0);
    //Bottom Left
    tb.pos () = PT (-5, -2,0);
    //Top Right
    tc.pos () = PT (5, 2, -1);
    //Bottom Right
    td.pos () = PT (5, -2, 1);

    frame.pos () = PT (0, 0, -10);

    ta.rot () = Gen::rot (Biv::xy * PI / 8.0);
    tb.rot () = Gen::rot (Biv::xy * -PI / 8.0);
    tc.rot () = Gen::rot (Biv::xy * 3 * PI / 8.0);
    td.rot () = Gen::rot (Biv::xy * -3 * PI / 8.0);
    //    ta.orient (Vec(1,1,0), 1, true);
    //    tb.orient (Vec(1,-1,0), 1, true);
    //    tc.orient (Vec(-1,1,0), 1, true);
    //    td.orient (Vec(-1,-1,0), 1, true);
    //
    objectController.attach (&ta);
    objectController.attach (&tb);
    objectController.attach (&tc);
    objectController.attach (&td);
    objectController.attach (&frame);

    scene.camera.pos (0, 0, 10);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {
    //calculate mouse position in world space
    if (bMouseSet)
      calcMouse3D ();

    // Points
    Point a = ta.pos ();
    Point b = tb.pos ();
    Point c = tc.pos ();
    Point d = td.pos ();

    // Tangents
    Pair pa = ta.tx ();
    Pair pb = tb.tx ();
    Pair pc = tc.tx ();
    Pair pd = td.tx ();

    // Common Spheres
    // wedge is shared sphere (const coord)
    // paths should be on this surface
    auto sab = pa ^ pb;
    auto scd = pc ^ pd;
    auto sac = pa ^ pc;
    auto sbd = pb ^ pd;

    //sphere through four points
    auto sph = a ^ b ^ c ^ d;

    // Gen taking s1 to s2
    // Par genA = Gen::log (s1, s2, amt3);
    // Gen taking s1 to s2
    // Par genB = Gen::log (s2, s1, amt3);

    //Sphere with normal ta through tb
    auto sna = pa.undual () ^ tb.pos ();
    //Sphere with normal tb through ta
    //    auto snb = pb.undual () ^ ta.pos ();
    //Sphere with normal tc through td
    //    auto snc = pc.undual () ^ td.pos ();
    //Sphere with normal td through tc
    //    auto snd = pd.undual () ^ tc.pos ();

    //Tangents of _ at _
    //Pair tnab = Tangent::at (sna, b).dual ();
    //Pair tncd = Tangent::at (snc, d).dual();

    //Circle where they meet (through points and orthogonal to common sphere)
    auto cirab = a ^ b ^ sab.dual ();  //(sna.dual () ^ snb.dual ()).dual ();
    auto circd = c ^ d ^ scd.dual ();  //(snc.dual () ^ snd.dual ()).dual ();
    auto cirac = a ^ c ^ sac.dual ();  //(snc.dual () ^ snd.dual ()).dual ();
    auto cirbd = b ^ d ^ sbd.dual ();  //(snc.dual () ^ snd.dual ()).dual ();

    // Circles through each other on sphere
    auto cab = pa ^ b;
    auto cba = pb ^ a;
    auto cbd = pb ^ d;
    auto cdb = pd ^ b;

    //Tangents on circles
    auto pba = -Tangent::at (cab, b);
    auto pab = -Tangent::at (cba, a);

    //planes
    auto da = Inf (1) <= pa;
    auto dba = Inf (1) <= pba;

    auto db = Inf (1) <= pb;
    auto dab = Inf (1) <= pab;

    auto genab = Gen::log (-(dba / da).runit (), bToggleA, bSetA) * amt3/2.0;
    auto genba = Gen::log (-(dab / db).runit (), bToggleB, bSetB) * amt3/2.0;

    draw (da, 0, 1, 0);
    draw (dba, 0, 1, 1);
    draw (genab, 1, 1, 1);

    draw (pa.spin (Gen::bst (genab)));
    draw (pb.spin (Gen::bst (genba)));

    draw (cab);
    draw (cba);
    draw (cbd);
    draw (cdb);
    draw (pab, 0, 1, 0);
    draw (pba, 0, 1, 0);


    //Transformation from sna to snc
    //Pair genAC = Gen::log (-(snc / sna).runit (), bToggleA, bSetA) * amt3;

    //Transformation from snb to snd
    //Pair genBD = Gen::log (-(snd / snb).runit (), bToggleB, bSetB) * amt3;

    //Transformation from circle to circle
    //    for (int i = 0; i < 100; ++i)
    //      {
    //        Circle c1 = cirab;
    //        Circle c2 = circd;
    //        float t = (float) i / 100.0;
    //        auto con = Gen::con (c1, c2, amt1 * t);
    //        auto con2 = Gen::con (c2, c1, amt1 * t);
    //        draw (pa.spin (con), 1, abs(t), 1);
    //        draw (pc.spin (con2), 1, abs(t), 1);
    //      }

    //Transformation from sna to snb
    //    Par genAB = Gen::log (-(snb / sna).runit (), bToggle, bSet) * amt3;
    // sphere
    //    auto sc = sa.spin (Gen::bst (genAB));
    // Tangents on sphere
    //    auto tna = Tangent::at (sc, ta.pos()).dual();
    //    auto tnb = Tangent::at (sc, tb.pos()).dual();

    // Shared Circles
    //    draw (cirab, 1, 0, 0);
    //    draw (circd, 1, 0, 0);
    //    draw (cirac, 1, 0, 0);
    //    draw (cirbd, 1, 0, 0);

    // Tangents
    draw (pa);
    draw (pb);
    draw (pc);
    draw (pd);

    //motion of tangents across x surface tensor  (top and bottom)
    //    draw (pa.spin (Gen::bst (genAC)), 0, 1, 0);
    //    draw (pb.spin (Gen::bst (genBD)), 0, 1, 0);

    //motion of osc spheres across top x surface tensor (left and right)
    //    draw (s1.spin (Gen::bst (genAC)), 0, 1, 1, .1);
    //    draw (s2.spin (Gen::bst (-genAC)), 0, 1, 1, .1);

    //motion of osc spheres across bottom x surface tensor (left and right)
    //   draw (s1.spin (Gen::bst (genBD)), .3, 1, .6, .1);
    //   draw (s2.spin (Gen::bst (-genBD)), .3, 1, .6, .1);

    //   normals of common spheres
    //    draw (Tangent::at (sab, b).dual().runit(), .5,0,1);
    //    draw (Tangent::at (sbd, b).dual().runit(), 1,0,.5);

    //pseudoscalar as circle intersection ??
    //    auto join = (sab.dual () ^ sbd.dual ());
    //    auto pss = join.undual ().inv ();
    //    auto r1 = pa <= pss;  //sab.dual () <= pss;
    //    auto r2 = pb <= pss;  //sbd.dual () <= pss;

    //    auto r3 = frame.x () <= join;  //sbd.dual () <= pss;

    //    auto r3 = sab.dual () * pss;
    //    r3.print();
    //    draw (r1, 1,0,0);
    //draw (r1, 0, 1, 0);
    //draw (r2, 0, 1, 0);
    //    draw (r4, 0, 1, 0);
    //    draw (r2, 1,0,0,.2);

    draw (frame);

    //common spheres (cyan)
    draw (sab, 0, 1, 1, .2);
    //    draw (scd, 0, 1, 1, .2);
    //    draw (sac, 0, 1, 1, .2);
    //draw (sbd, 0, 1, 1, .2);

    //draw (r3, 0, 1, 0, .4);
    //    draw (sph, 1, 1, 1, .1);

    //picked spheres (red and orange)
    //draw (sna, 1, 0, 0, .1);
    //    draw (snc, 1, 0, 0, .1);
    //    draw (snb, 1, .2, 0, .1);
    //    draw (snd, 1, .2, 0, .1);
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
