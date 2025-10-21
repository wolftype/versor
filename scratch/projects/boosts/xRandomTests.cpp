// SURFACE PATCH

#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include <vsr/form/vsr_rigid.h>
#include <vsr/util/vsr_draw_util.h>
#include <GLFW/glfw3.h>

using namespace gfx;
using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {

  Frame mFrame00;
  Frame mFrame10;
  Frame mFrame01;
  Frame mFrame11;

  Frame mControl;

  void resetFrames() {
    mFrame00.pos(-5, -5, 0).rot(Rot(1));
    mFrame10.pos(5, -5, 0).rot(Rot(1));
    mFrame11.pos(5, 5, 0).rot(Rot(1));
    mFrame01.pos(-5, 5, 0).rot(Rot(1));
    mControl.pos(3,0,0);
  }

  void onDrawGui() {

  }

  void onSetup() {
    resetFrames();

    objectController.attach(&mFrame00);
    objectController.attach(&mControl);
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw() {

    GL::lightsOff();

    draw(mFrame00);
    draw(mFrame10);
    draw(mFrame01);
    draw(mFrame11);



    Circle cir = mFrame00.pos() ^ mFrame10.pos() ^ mFrame01.pos();
    DualSphere _s = Round::surround(cir);
    draw(cir);

    Pair tan = Tops::NormalizePair(mFrame00.pos() <= cir) ;
    Pair bitan =  Tops::NormalizePair(tan <= _s.dual());
    Pair normal = Tops::NormalizePair(bitan <= (Inf(1) <= tan).dual());

    Point p = mMouse3D;
    DrawPoint(p);

    DualSphere _s2 = Round::sphere(p, 2.0);


    //equiplane in drv direction
    auto tmp = _s2 <= Drv(1,1,0);
    draw(tmp);
    //point pair in Drv direction
    auto tan2 = (_s2 ^(_s2 <= Drv(1,1,0)));

    Point pp = Round::split(tan2, true);
    Draw(pp,0,0,1);
    pp.print();
    Point ppn = Round::location(pp); //divideed by no
    cout << "ppn: " << endl;
    ppn.print();

    //Tangent to _s2 in direction
    Pair tan3 =ppn ^ _s2;
    Draw(tan3, 1, 1, 0);

    //tan2.print();

    Draw(tan2, 1, 0, 0);

    DrawRound(_s2, 0, 1, 0, .3);

    // DrawPoint(mControl.pos());




    // // draw( tan, 1, 0, 0);
    // // draw( bitan, 0, 0, 1);
    // // draw( normal, 0, 1, 0);
    // //draw(pj);

    // auto wt =  (Ori(1) <= Round::direction(tan) ).wt();

    // draw(tan.undual(), 1, 0, 0);
    // draw(bitan, 1, 0, 1);
    // draw(normal.undual(), 0, 1, 0);

    // cout << bitan.wt() << endl;
    // for (int i = 0; i < 20; ++i) {
    //   Bst bi = Gen::bst(tan * i / 20.0);
    //   for (int j = 0; j < 20; ++j) {
    //     Bst bj = Gen::bst(bitan * j / 20.0);
    //     for (int k = 0; k < 20; ++k) {
    //       Bst bk = Gen::bst(normal * k / 20.0);
    //       Con con = bj;
    //       Point np = p.spin(con);
    //       Draw(Round::location(np));
    //       np /= np[3];
    //       Draw(np, 1, 0, 0);
    //     }
    //   }
    // }
  }
  /*-----------------------------------------------------------------------------
   *  Keyboard
   *-----------------------------------------------------------------------------*/
  void onKeyDown(const gfx::Keyboard &k) { App::onKeyDown(k); }
};

int main() {

  MyApp app;
  app.mColor = Vec3f(0.2, 0.2, 0.2);
  app.start();

  return 0;
}
