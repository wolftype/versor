#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

// Draw DualSphere as Plane if Flat
// @todo -- make this part of Regular Draw method
void Draw2 (const DualSphere &s, float r, float g, float b, float a = 1.0)
{

  auto c = Inf (1) <= s;
  float cc = (c * c)[0];
  if (cc == 0)
    Draw (DualPlane (s), r, g, b, a);
  else
    Draw (s, r, g, b, a);
}

// Draw Circle as Line if Flat
// @todo -- make this part of Regular Draw method
void Draw2 (const Circle &s, float r, float g, float b, float a = 1.0)
{

  auto c = Inf (1) <= s;
  float cc = (c * c)[0];
  if (cc == 0)
    Draw (Line (s), r, g, b, a);
  else
    Draw (s, r, g, b, a);
}

//TangentFrame

struct MyApp : App
{

  //Some Variables
  bool bDrawCurves;
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
    gui (bDrawCurves, "bDrawCurves");

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
    //calkvlate mouse position in world space
    if (bMouseSet)
      calcMouse3D ();

    //Copy in Frame and save
    TangentFrame tf (fa);
    tf.store ();

    Point p000 = fa.pos ();
    Point p111 = fb.pos ();
    //Tangents in tu and tv directions
    //normal to su and sv surfaces and tangent to ku and kv curves
    Pair tu000 = fa.tx ();
    Pair tv000 = fa.ty ();
    Pair tu111 = -fb.tx ();
    Pair tv111 = -fb.ty ();

    /*** SPHERES ***/
    //DualPlanes at origin recast as DualSpheres;
    DualSphere du0 = -tf.dyz ();
    DualSphere dv0 = tf.dxz ();

    //Curvature operators of constant ku and kv, respectively
    Boost ku = Gen::boost (tu000 * -amt1 / 2.0);
    Boost kv = Gen::boost (tv000 * -amt2 / 2.0);

    //Coordinate spheres sigma u and sigma v
    DualSphere su0 = du0.spin (ku);
    DualSphere sv0 = dv0.spin (kv);

    //sv sphere at (1,1,1) orthogonal to su
    DualSphere sv1 = (tv111.undual () ^ su0).dual ();
    //su sphere at (1,1,1) orthogonal to sv
    DualSphere su1 = (tu111.undual () ^ sv0).dual ();


    /*** SIDE A ***/
    //p100: increasing u with constant v0 is the u-plunge from su0 at 000 into su1
    //we extract the point on su1, hit by the plunge, and the tangents there
    Circle ku000 = tu000 ^ su1;
    Point p100 = Round::split ((ku000.dual () ^ su1).undual (), false);
    Pair tu100 = Tangent::at (su1.dual (), p100).dual ();
    Pair tv100 = Tangent::at (sv0.dual (), p100).dual ();

    //p010: increasing v with constant u0 is the v-plunge from sv0 at 000 into sv1
    //we extract the point on sv1, hit by the plunge, and the tangents there
    Circle kv000 = tv000 ^ sv1;
    Point p010 = Round::split ((kv000.dual () ^ sv1).undual (), false);
    Pair tu010 = Tangent::at (su0.dual (), p010).dual ();
    Pair tv010 = Tangent::at (sv1.dual (), p010).dual ();

    //p110: increasing u with constant v1 is the u-plunge from su0 at 010 into su1
    //we extract the point on su1, hit by the plunge
    //and the tangents there (at su1 and sv1)
    Circle ku010 = tu010 ^ su1;
    Point p110 = Round::split ((ku010.dual () ^ su1).undual (), false);
    Pair tu110 = Tangent::at (su1.dual (), p110).dual ();
    Pair tv110 = Tangent::at (sv1.dual (), p110).dual ();

    //increasing v with constant u1 is the v-plunge from sv0 at 100 into sv1
    Circle kv100 = tv100 ^ sv1;

    /*** SIDE B ***/
    //p011: decreasing u with constant v1 is the u-plunge from su1 at 111 into su0
    //we extract the point on su0, hit by the plunge
    //and the tangents there (at su0 and sv1)
    Circle ku111 = tu111 ^ su0;
    Point p011 = Round::split ((ku111.dual () ^ su0).undual (), false);
    Pair tu011 = Tangent::at (su0.dual (), p011).dual ();
    Pair tv011 = Tangent::at (sv1.dual (), p011).dual ();

    //p101: decreasing v with constant u1 is the v-plunge from sv1 at 111 into sv0
    //we extract the point on sv0, hit by the plunge
    //and the tangents there (at su1 and sv0)
    Circle kv111 = tv111 ^ sv0;
    Point p101 = Round::split ((kv111.dual () ^ sv0).undual (), false);
    Pair tu101 = Tangent::at (su1.dual (), p101).dual ();
    Pair tv101 = Tangent::at (sv0.dual (), p101).dual ();

    //p001: decreasing u with constant v0 is the v-plunge from su1 at 101 into su0
    //we extract the point on su0, hit by the plunge
    //and the tangents there (at su0 and sv0)
    Circle ku101 = tu101 ^ su0;
    Point p001 = Round::split ((ku101.dual () ^ su0).undual (), false);
    Pair tu001 = Tangent::at (su0.dual (), p001).dual ();
    Pair tv001 = Tangent::at (sv0.dual (), p001).dual ();

    //decreasing v with constant u0 is the u-plunge from sv1 at 011 into sv0
    Circle kv011 = tv011 ^ sv0;

    //curve of constant w where su0 and sv0 meet
    Circle kw000 = (sv0 ^ su0).undual ();
    //curve of constant w where su0 and sv1 meet
    Circle kw010 = (su0 ^ sv1).undual ();
    //curve of constant w at b where su1 and sv1 meet
    Circle kw111 = (sv1 ^ su1).undual ();
    //curve of constant w where su1 and sv0 meet
    Circle kw101 = (su1 ^ sv0).undual ();


    Draw (Construct::sphere (p010, .2));
    Draw (Construct::sphere (p100, .2));
    Draw (Construct::sphere (p110, .2));
    Draw (Construct::sphere (p101, .2));
    Draw (Construct::sphere (p011, .2));
    Draw (Construct::sphere (p001, .2));

    DrawAt (-Round::dir (tu000).copy<Vec> ().unit (), p000, 1, 0, 0);
    DrawAt (-Round::dir (tv000).copy<Vec> ().unit (), p000, 0, 1, 0);
    DrawAt (-Round::dir (tu100).copy<Vec> ().unit (), p100, 1, 0, 0);
    DrawAt (-Round::dir (tv100).copy<Vec> ().unit (), p100, 0, 1, 0);
    DrawAt (-Round::dir (tu010).copy<Vec> ().unit (), p010, 1, 0, 0);
    DrawAt (-Round::dir (tv010).copy<Vec> ().unit (), p010, 0, 1, 0);
    DrawAt (-Round::dir (tu110).copy<Vec> ().unit (), p110, 1, 0, 0);
    DrawAt (-Round::dir (tv110).copy<Vec> ().unit (), p110, 0, 1, 0);

    DrawAt (-Round::dir (tu111).copy<Vec> ().unit (), p111, 1, 0, 0);
    DrawAt (-Round::dir (tv111).copy<Vec> ().unit (), p111, 0, 1, 0);
    DrawAt (-Round::dir (tu101).copy<Vec> ().unit (), p101, 1, 0, 0);
    DrawAt (-Round::dir (tv101).copy<Vec> ().unit (), p101, 0, 1, 0);
    DrawAt (-Round::dir (tu011).copy<Vec> ().unit (), p011, 1, 0, 0);
    DrawAt (-Round::dir (tv011).copy<Vec> ().unit (), p011, 0, 1, 0);
    DrawAt (-Round::dir (tu001).copy<Vec> ().unit (), p001, 1, 0, 0);
    DrawAt (-Round::dir (tv001).copy<Vec> ().unit (), p001, 0, 1, 0);

    Draw (fa);
    Draw (fb);

    if (bDrawCurves)
      {
        Draw2 (kv000, 1, 1, 0);
        Draw2 (ku000, 1, 1, 0);
        Draw2 (kv100, 1, 1, 0);
        Draw2 (ku010, 1, 1, 0);
        Draw2 (ku111, 1, 1, .2);
        Draw2 (kv111, 1, 1, .2);
        Draw2 (ku101, 1, 1, .2);
        Draw2 (kv011, 1, 1, .2);

        //w-curves
        Draw2 (kw000, 0, 1, 1);
        Draw2 (kw010, 0, 1, 1);
        Draw2 (kw111, 0, 1, 1);
        Draw2 (kw101, 0, 1, 1);
      }

    //    Draw2 (su0, 1, .3, .1, .4);
    //    Draw2 (sv0, .1, 1, .3, .4);
    //    Draw2 (sv1, 0, 1, 0, .4);
    //    Draw2 (su1, 1, 0, 0, .4);
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
