//SURFACE PATCH

#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include <vsr/form/vsr_rigid.h>
#include <vsr/util/vsr_draw_util.h>

using namespace gfx;
using namespace vsr;
using namespace vsr::cga;


void DrawT (const TFrame_& tf, bool bFlip = false)
{
 Draw (tf.t[0] * (bFlip ? -1.0 : 1.0), 1,0,0);
 Draw (tf.t[1] * (bFlip ? -1.0 : 1.0), 0,1,0);
 Draw (tf.t[2] * (bFlip ? -1.0 : 1.0), 0,0,1);
}

struct TPatch {

  TFrame_ tf[2][2];

};

struct MyApp : App {

  float amt1 = 0;
  float amt2 = 0;

  vsr::util::Grid grid;

  Frame mFrame000;
  Frame mFrame100;
  Frame mFrame010;
  Frame mFrame110;

  double time = 0.0;
  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void onDrawGui(){
    ///Add Variables to GUI
    gui(amt1,"amt1",-100,100);
    gui(amt2,"amt2",-100,100);
  }

  void setup(){
    mFrame000.pos (0,0,0);
    mFrame100.pos (1,0,0);
    mFrame110.pos (1,1,0);
    mFrame010.pos (0,1,0);

    //top right patch
    objectController.attach(&mFrame110);

    grid.init (10,6);
  }

 
  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw(){
    //calculate mouse position in world space
    if (bSetMouse) calcMouse3D();

    time+=.01;

    //Lets design a surface you can pinch with your fingers
    //
    //Given: a Frame and three more points.

    Vec target (sin(time)*10,1,1);
    mFrame110.orient(target);

    Point pos100 = mFrame100.pos();
    Point pos010 = mFrame010.pos();
    Point pos110 = mFrame110.pos();
    Circle circle = pos010 ^ pos100 ^ pos110;

    Point pos000 = Constrain::PointToCircle (mMouse3D, circle);

    TFrame_ f110;

    f110.build (mFrame110);

    TFrame_ f010(pos010, f110, TDIR::u);
    TFrame_ f100(pos100, f110, TDIR::v);
    TFrame_ f000(pos000, f010, TDIR::v);


   // f100.buildV (pos100, f110);//, TDIR::v);
   // f010.buildU (pos010, f110);//, TDIR::u);
   // f000.buildV (pos000, f010);//, TDIR::v);

    f000.addSurfaces (f100, TDIR::u);

    Pair genU  = f000.gen (f100, TCS::uv);//TSX::UV());
    Pair genV  = f000.gen (f010, TCS::vu);//TSX::VU());

    grid.plot(pos000, genU, genV);
    grid.draw(true, 1.0);

//    DrawT (f000);
//    DrawT (f100);
//    DrawT (f010);
//    DrawT (f110);
    Draw(circle,0,1,0);

    Frame fa (pos000, f000.rotor());
    Frame fb (pos100, f100.rotor());
    Frame fc (pos110, f110.rotor());
    Frame fd (pos010, f010.rotor());

    GL::lightsOff();

    Draw(fa);
    Draw(fb);
    Draw(fc);
    Draw(fd);
//    // TMP find rotation
//    Vec vx = -Round::dir(f100.t[0]).copy<Vec>();
//    Vec vy = -Round::dir(f100.t[1]).copy<Vec>();
//    Vec vz = -Round::dir(f100.t[2]).copy<Vec>();
//
//    Rotor rz = Gen::ratio (Vec::z, vz);
//    Vec vxx = Vec::x.spin(rz);
//    Rotor rx = Gen::ratio (vxx, vx);
//
//    Rotor rot = rx*rz; 
//
//    Frame ftmp (PAO, rot);
//    Draw(ftmp);
     

    

    //Question, what about "any two circles"
    //thinking of two hands here, what does that afford us?
    //or, starting with a cube, how to modify with hands?
    //Key is circular controllers, and planar constraints
    //Wave Shaping -- a flow that your hands are sculpting,
    //like a pottery wheel
    //
    //A) The Rubber Band around four fingers
    //B) What happens when two points are collapsed (triangular simplex)
    //C) The Stream.  The River.  What is this?


   }

  /*-----------------------------------------------------------------------------
   *  Keyboard
   *-----------------------------------------------------------------------------*/
  void onKeyDown(const gfx::Keyboard &k){

    App::onKeyDown(k);
  }
};


int main(){

  MyApp app;
  app.mColor = Vec3f (0.2, 0.2, 0.2);
  app.start();

  return 0;
}
