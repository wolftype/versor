#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

//TangentFrame

struct MyApp : App
{

  //Some Variables
  bool bDrawCurves;
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  float amtU = 0;
  float amtV = 0;
  float amtV20 = 0;
  float amtV21 = 0;
  float amtWU = 0;
  float amtWV = 0;

  Frame fa;
  Frame fb;
  Frame fc;
  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (amtU, "amtU", -100, 100);
    gui (amtV, "amtV", -100, 100);
    gui (amtV20, "amtV20", -100, 100);
    gui (amtV21, "amtV21", -100, 100);
    gui (amtWU, "amtWU", -100, 100);
    gui (amtWV, "amtWV", -100, 100);
    gui (bToggle, "bToggle");
    gui (bSet, "bSet");
    gui (bDrawCurves, "bDrawCurves");

    objectController.attach (&fa);
    objectController.attach (&fb);
    objectController.attach (&fc);

    fa.pos (0, 0, 0);
    fb.pos (4, 4, -4);
    fc.pos (2, 6, -2);
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

    Point p000 = fa.pos ();
    Point p111 = fb.pos ();
    //Tangents in tu and tv directions
    //normal to su and sv surfaces and tangent to ku and kv curves
    Pair tu000 = fa.tx ();
    Pair tv000 = fa.ty ();
    Pair tw000 = -fa.tz ();
    Pair tu111 = -fb.tx ();
    Pair tv111 = -fb.ty ();
    Pair tw111 = fb.tz ();

    /*** SPHERES ***/
    //DualPlanes at origin recast as DualSpheres;
    DualSphere du0 = -tf.dyz ();
    DualSphere dv0 = tf.dxz ();
    DualSphere dw0 = tf.dxy ();

    //Curvature operators kmn of constant m in n direction
    Boost kuv = Gen::boost (tu000 * -amtU / 2.0);
    Boost kvu = Gen::boost (tv000 * -amtV / 2.0);

    //Coordinate spheres sigma u and sigma v
    DualSphere su0 = du0.spin (kuv);
    DualSphere sv0 = dv0.spin (kvu);

    //sv sphere at (1,1,1) orthogonal to su0
    DualSphere sv1 = (tv111.undual () ^ su0).dual ();
    //su sphere at (1,1,1) orthogonal to sv0
    DualSphere su1 = (tu111.undual () ^ sv0).dual ();

    /*** W Curves ***/
    //curve of constant w where su0 and sv0 meet
    Circle kw000 = (sv0 ^ su0).undual ();
    //curve of constant w where su0 and sv1 meet
    Circle kw010 = (su0 ^ sv1).undual ();
    //curve of constant w at b where su1 and sv1 meet
    Circle kw111 = (sv1 ^ su1).undual ();
    //curve of constant w where su1 and sv0 meet
    Circle kw100 = (su1 ^ sv0).undual ();

    /*** SIDE A ***/
    //p100: increasing u with constant v0 is the u-plunge from su0 at 000 into su1
    //we extract the point on su1, hit by the plunge, and the tangents there
    Circle ku000 = tu000 ^ su1;
    Point p100 = Round::split ((ku000.dual () ^ su1).undual (), false);
    Pair tu100 = Tangent::at (su1.dual (), p100).dual ();
    Pair tv100 = Tangent::at (sv0.dual (), p100).dual ();
    Pair tw100 = -Tangent::at (kw100, p100);

    //p010: increasing v with constant u0 is the v-plunge from sv0 at 000 into sv1
    //we extract the point on sv1, hit by the plunge, and the tangents there
    Circle kv000 = tv000 ^ sv1;
    Point p010 = Round::split ((kv000.dual () ^ sv1).undual (), false);
    Pair tu010 = Tangent::at (su0.dual (), p010).dual ();
    Pair tv010 = Tangent::at (sv1.dual (), p010).dual ();
    Pair tw010 = -Tangent::at (kw010, p010);

    //p110: increasing u with constant v1 is the u-plunge from su0 at 010 into su1
    //we extract the point on su1, hit by the plunge
    //and the tangents there (at su1 and sv1)
    Circle ku010 = tu010 ^ su1;
    Point p110 = Round::split ((ku010.dual () ^ su1).undual (), false);
    Pair tu110 = Tangent::at (su1.dual (), p110).dual ();
    Pair tv110 = Tangent::at (sv1.dual (), p110).dual ();
    Pair tw110 = -Tangent::at (kw111, p110);

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
    Pair tw011 = Tangent::at (kw010, p011);

    //p101: decreasing v with constant u1 is the v-plunge from sv1 at 111 into sv0
    //we extract the point on sv0, hit by the plunge
    //and the tangents there (at su1 and sv0)
    Circle kv111 = tv111 ^ sv0;
    Point p101 = Round::split ((kv111.dual () ^ sv0).undual (), false);
    Pair tu101 = Tangent::at (su1.dual (), p101).dual ();
    Pair tv101 = Tangent::at (sv0.dual (), p101).dual ();
    Pair tw101 = Tangent::at (kw100, p101);

    //p001: decreasing u with constant v0 is the v-plunge from su1 at 101 into su0
    //we extract the point on su0, hit by the plunge
    //and the tangents there (at su0 and sv0)
    Circle ku101 = tu101 ^ su0;
    Point p001 = Round::split ((ku101.dual () ^ su0).undual (), false);
    Pair tu001 = Tangent::at (su0.dual (), p001).dual ();
    Pair tv001 = Tangent::at (sv0.dual (), p001).dual ();
    Pair tw001 = Tangent::at (kw000, p001);

    //decreasing v with constant u0 is the u-plunge from sv1 at 011 into sv0
    Circle kv011 = tv011 ^ sv0;

    /**W Sphere***/
    DualSphere sw00 = (tw000.undual () ^ p010).dual ();
    DualSphere sw10 = (tw100.undual () ^ p110).dual ();
    DualSphere sw01 = (tw001.undual () ^ p011).dual ();
    DualSphere sw11 = (tw101.undual () ^ p111).dual ();

    /*** Rotors ***/
    Pair logU = Gen::log (su0, su1, 1);
    Pair logV = Gen::log (sv0, sv1, 1);
    Pair logW = Gen::log (sw00, sw01, 1);

    int numU = 10;
    int numV = 10;
    int numW = 10;

    for (int i = 0; i <= numW; ++i)
      {
        float w = (float) i / numW;
        Boost bW = Gen::bst (logW * -w);
        for (int j = 0; j <= numV; ++j)
          {
            float v = (float) j / numV;
            Boost bV = Gen::bst (logV * -v);

            for (int k = 0; k <= numU; ++k)
              {
                float u = (float) k / numU;
                Boost bU = Gen::bst (logU * -u);

                Con con = bU * bV * bW;
                auto p = Round::location (p000.spin (con));
                Draw (Construct::sphere (p, .1), 1, 0, 0);
                //auto p1 = fa.cxy ().dilate(fa.pos(), -.9);  //Construct::sphere (p000, .1);
                //auto p2 = fa.cxz ().dilate(fa.pos(), -.9);  //Construct::sphere (p000, .1);
                //Draw (p1.spin (con), u, v, w, .5);
                //Draw (p2.spin (con), u, v, w, .5);
              }
          }
      }

    for (int i = 0; i <= numW; ++i)
      {
        float w = (float) i / numW;
        Boost bW = Gen::bst (logW * -w);
        for (int j = 0; j <= numV; ++j)
          {
            float v = (float) j / numV;
            Boost bV = Gen::bst (logV * -v);

            for (int k = 0; k <= numU; ++k)
              {
                float u = (float) k / numU;
                Boost bU = Gen::bst (logU * -u);

                Con con = bU * bV * bW;
                auto p = Round::location(fa.pos ().spin(con));
                GL::vertex (p);
              }
          }
      }


    // Extrude in V direction
    DualSphere dv2 = fc.dxz ();
    // Curve
    DualSphere sv20 = dv2.spin (Gen::bst (fc.ty() * - amtV20/2.0));
    DualSphere sv21 = dv2.spin (Gen::bst (fc.ty() * - amtV20/2.0));

    //Get coordinate curve plunge into its system from front
    Circle nkv010 = -tv010 ^ sv20;
    Circle nkv110 = -tv110 ^ sv20;
    //Get coordinate curve plunge into its system from back
    Circle nkv011 = -tv011 ^ sv21;
    Circle nkv111 = -tv111 ^ sv21;

    //Get intersection there
    Pair tpairV010 = (nkv010.dual () ^ sv20).undual ();
    Pair tpairV110 = (nkv110.dual () ^ sv20).undual ();
    Pair tpairV011 = (nkv011.dual () ^ sv21).undual ();
    Pair tpairV111 = (nkv111.dual () ^ sv21).undual ();
    Point npv020 = Round::split (tpairV010, false);
    Point npv120 = Round::split (tpairV110, false);
    Point npv021 = Round::split (tpairV011, false);
    Point npv121 = Round::split (tpairV111, false);

    //Get tangent there
    Pair ntv020 = Tangent::at (nkv010, npv020);
    Pair ntv120 = Tangent::at (nkv110, npv120);
    Pair ntv021 = Tangent::at (nkv011, npv021);
    Pair ntv121 = Tangent::at (nkv111, npv121);

    Draw (fc);
    Draw (npv020);
    Draw (npv120);
    Draw (npv021);
    Draw (npv121);
    Draw (nkv010, 0, 1, 0);
    Draw (nkv110, 0, 1, 0);
    Draw (nkv011, 0, 1, 0);
    Draw (nkv111, 0, 1, 0);

    Draw (p010 ^ p110 ^ npv020);
    Draw (p110 ^ p111 ^ npv120);
    Draw (npv020 ^ npv021 ^ npv120);

    Draw (sv20, 0,1,0,.2);
    Draw (sv21, 0,1,1,.2);

    Draw (Construct::sphere (p010, .2));
    Draw (Construct::sphere (p100, .2));
    Draw (Construct::sphere (p110, .2));
    Draw (Construct::sphere (p101, .2));
    Draw (Construct::sphere (p011, .2));
    Draw (Construct::sphere (p001, .2));

    DrawAt (-Round::dir (tu000).copy<Vec> ().unit (), p000, 1, 0, 0);
    DrawAt (-Round::dir (tv000).copy<Vec> ().unit (), p000, 0, 1, 0);
    DrawAt (-Round::dir (tw000).copy<Vec> ().unit (), p000, 0, 0, 1);
    DrawAt (-Round::dir (tu100).copy<Vec> ().unit (), p100, 1, 0, 0);
    DrawAt (-Round::dir (tv100).copy<Vec> ().unit (), p100, 0, 1, 0);
    DrawAt (-Round::dir (tw100).copy<Vec> ().unit (), p100, 0, 0, 1);
    DrawAt (-Round::dir (tu010).copy<Vec> ().unit (), p010, 1, 0, 0);
    DrawAt (-Round::dir (tv010).copy<Vec> ().unit (), p010, 0, 1, 0);
    DrawAt (-Round::dir (tw010).copy<Vec> ().unit (), p010, 0, 0, 1);
    DrawAt (-Round::dir (tu110).copy<Vec> ().unit (), p110, 1, 0, 0);
    DrawAt (-Round::dir (tv110).copy<Vec> ().unit (), p110, 0, 1, 0);
    DrawAt (-Round::dir (tw110).copy<Vec> ().unit (), p110, 0, 0, 1);

    DrawAt (-Round::dir (tu111).copy<Vec> ().unit (), p111, 1, 0, 0);
    DrawAt (-Round::dir (tv111).copy<Vec> ().unit (), p111, 0, 1, 0);
    DrawAt (-Round::dir (tw111).copy<Vec> ().unit (), p111, 0, 0, 1);
    DrawAt (-Round::dir (tu101).copy<Vec> ().unit (), p101, 1, 0, 0);
    DrawAt (-Round::dir (tv101).copy<Vec> ().unit (), p101, 0, 1, 0);
    DrawAt (-Round::dir (tw101).copy<Vec> ().unit (), p101, 0, 0, 1);
    DrawAt (-Round::dir (tu011).copy<Vec> ().unit (), p011, 1, 0, 0);
    DrawAt (-Round::dir (tv011).copy<Vec> ().unit (), p011, 0, 1, 0);
    DrawAt (-Round::dir (tw011).copy<Vec> ().unit (), p011, 0, 0, 1);
    DrawAt (-Round::dir (tu001).copy<Vec> ().unit (), p001, 1, 0, 0);
    DrawAt (-Round::dir (tv001).copy<Vec> ().unit (), p001, 0, 1, 0);
    DrawAt (-Round::dir (tw001).copy<Vec> ().unit (), p001, 0, 0, 1);

    Draw (fa);
    Draw (fb);

    if (bDrawCurves)
      {
        DrawRound (kv000, 1, 1, 0);
        DrawRound (ku000, 1, 1, 0);
        DrawRound (kv100, 1, 1, 0);
        DrawRound (ku010, 1, 1, 0);
        DrawRound (ku111, 1, 1, .2);
        DrawRound (kv111, 1, 1, .2);
        DrawRound (ku101, 1, 1, .2);
        DrawRound (kv011, 1, 1, .2);

        //w-curves
        DrawRound (kw000, 0, 1, 1);
        DrawRound (kw010, 0, 1, 1);
        DrawRound (kw111, 0, 1, 1);
        DrawRound (kw100, 0, 1, 1);
      }

    //    DrawRound (sw00, 0, 1, 1, .3);
    //    DrawRound (sw10, 0, 1, 1, .3);
    //    DrawRound (sw01, 0, 1, 0, .3);
    //    DrawRound (sw11, 0, 1, 0, .3);
    //    DrawRound (su0, 1, .3, .1, .4);
    //    DrawRound (sv0, .1, 1, .3, .4);
    //    DrawRound (sv1, 0, 1, 0, .4);
    //    DrawRound (su1, 1, 0, 0, .4);
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
