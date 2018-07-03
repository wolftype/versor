#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

void Draw2 (const DualSphere &s, float r, float g, float b, float a = 1.0)
{

  auto c = Inf (1) <= s;
  float cc = (c * c)[0];
  if (cc==0)
    Draw (DualPlane (s), r, g, b, a);
  else
    Draw (s, r, g, b, a);
}

void Draw2 (const Circle &s, float r, float g, float b, float a = 1.0)
{

  auto c = Inf (1) <= s;
  float cc = (c * c)[0];
//  cout << s << endl;
  if (cc==0)
    Draw (Line (s), r, g, b, a);
  else
    Draw (s, r, g, b, a);
}

struct MyApp : App
{

  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  float amt1 = 0;
  float amt2 = 0;

  Frame fa;
  Frame fb;
  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (amt1, "amt1", -100, 100);
    gui (amt2, "amt2", -100, 100);
    gui (bToggle, "bToggle");
    gui (bSet, "bSet");

    objectController.attach (&fa);
    objectController.attach (&fb);

    fa.pos (-3, 0, 0);
    fb.pos (3, 3, 0);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {
    GL::lightsOff ();
    //calculate mouse position in world space
    if (bMouseSet)
      calcMouse3D ();

    //Copy in Frame and save
    TangentFrame tf (fa);
    tf.store ();

    //Tangents in tu and tv directions (normal to su and sv surfaces)
    Pair tua = fa.tx ();
    Pair tva = fa.ty ();

    Pair tub = fb.tx ();
    Pair tvb = fb.ty ();

    //DualPlanes cast as DualSpheres;
    DualSphere du = tf.dyz ();
    DualSphere dv = tf.dxz ();

    //curvature operators of constant ku and kv, respectively
    Boost ku = Gen::boost (tua * -amt1 / 2.0);
    Boost kv = Gen::boost (tva * -amt2 / 2.0);

    //coordinate spheres sigma u and sigma v
    DualSphere sua = du.spin (ku);
    DualSphere sva = dv.spin (kv);

    //sv sphere at b orthogonal to su
    DualSphere svb = (tvb.undual () ^ sua).dual ();

    //su sphere at b orthogonal to sv
    DualSphere sub = (tub.undual () ^ sva).dual ();

    //"Front"
    //plunge along sua into svb
    Circle cua = tva ^ svb;
    //plunge along sva into sub
    Circle cva = tua ^ sub;

    Point pu1 = Round::split((cua.dual() ^ svb).undual(), false);
    Point pv1 = Round::split((cva.dual() ^ sub).undual(), false);

    Pair tua1 = Tangent::at (sua.dual(), pu1).dual();
    //plunge from sua, along svb, into sub
    Circle cva1 = tua1 ^ sub;

    Pair tva1 = Tangent::at (sva.dual(), pv1).dual();
    //plunge from sva, along sub, into svb
    Circle cua1 = tva1 ^ svb;

    //"Back"
    //plunge along sub into sva
    Circle cub = tvb ^ sva;
    //plunge along svb into sua
    Circle cvb = tub ^ sua;

    Point pu2 = Round::split((cub.dual() ^ sva).undual(), true);
    Point pv2 = Round::split((cvb.dual() ^ sua).undual(), true);

    Pair tub2 = Tangent::at (sub.dual(), pu2).dual();
    //plunge from sub, along sva, into sua
    Circle cvb1 = tub2 ^ sua;

    Draw (Construct::sphere (pu1,.2));
    Draw (Construct::sphere (pv1,.2));
    Draw (Construct::sphere (pu2,.2));
    Draw (Construct::sphere (pv2,.2));
    //curve of constant w where sua and sva meet
    Circle vaua = (sva ^ sua).undual ();
    //curve of constant w where sua and svb meet
    Circle uavb = (sua ^ svb).undual ();
    //curve of constant w at b where sub and svb meet
    Circle vbub = (svb ^ sub).undual ();
    //curve of constant w where sub and sva meet
    Circle ubva = (sub ^ sva).undual ();

    Draw (fa);
    Draw (fb);

    Draw2 (cua, 1, 1, 0);
    Draw2 (cva, 1, 1, 0);
    Draw2 (cua1, 1, 1, 0);
    Draw2 (cva1, 1, 1, 0);

    Draw2 (cvb, 0, 1, 1);
    Draw2 (cub, 0, 1, 1);
    Draw2 (cvb1, 0, 1, 1);

    Draw2 (vaua,0,0,1);
    Draw2 (uavb,0,0,1);
    Draw2 (vbub,0,0,1);
    Draw2 (ubva,0,0,1);

    Draw2 (sua, 1, .3, .1, .4);
    Draw2 (sva, .1, 1, .3, .4);
    Draw2 (svb, 0, 1, 0, .4);
    Draw2 (sub, 1, 0, 0, .4);

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
