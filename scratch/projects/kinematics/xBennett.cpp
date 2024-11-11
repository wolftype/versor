/*
 * =====================================================================================
 *
 *       Filename:  xBennett.cpp
 *
 *    Description:  bennett 4 bar linkage (linked)
 *
 *        Version:  1.0
 *        Created:  01/17/2014 14:18:12
 *       Revision:  none
 *       Compiler:  gcc4.7 or clang3.2 or higher
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */

#include "vsr/vsr_app.h"
#include "vsr/form/vsr_linkages.h"
#include "vsr/draw/vsr_chain_draw.h"

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App {

  Pnt mouse;
  Lin ray;

  float time;

  float numiter; // number of iterations to sweep single instance
  float numx, numy;

  float d1, d2; // root linkage lengths
  float theta;  // skew of root linkage
  float thetax,
      thetay; // skew of subsequent linkage, in terms of pecentage of previous
  float rotPhase;     // phase of mechanism
  float amtx1, amtx2; // percentage along length of xcoord link a and b
  float amty1, amty2; // percentage along length of ycoord link a and b
  float link;         // which link to link with

  bool bReset = false;
  bool bSubBennett = false;
  bool bDrawOutline = false;
  bool bDrawMeet = false;
  bool bDrawPlane = false;

  void onDrawGui() {
    gui(bReset, "bReset");
    gui(numiter, "numiter", 1, 100);
    gui(numx, "numx", 0, 100);
    gui(numy, "numy", 0, 100);
    gui(d1, "d1", 0, 100);
    gui(d2, "d2", 0, 100);
    gui(theta, "theta", -TWOPI, TWOPI);

    gui(thetax, "thetax", -TWOPI, TWOPI);
    gui(thetay, "thetay", -TWOPI, TWOPI);

    gui(rotPhase, "rotPhase", 0, TWOPI);

    gui(amtx1, "amtx1", -1, 1);
    gui(amtx2, "amtx2", -1, 1);
    gui(amty1, "amty1", -1, 1);
    gui(amty2, "amty2", -1, 1);

    gui(link, "link", 0, 3);
    gui(bSubBennett, "subbennett");
    gui(bDrawOutline, "bDrawOutline");
    gui(bDrawMeet, "bDrawMeet");
    gui(bDrawPlane, "bDrawPlane");
  }

  void onSetup() {
    reset();
    mColor.set(1, 1, 1);
  }

  void reset() {
    link = 2;
    d1 = 3;
    d2 = 3;
    theta = PIOVERFOUR;
    rotPhase = .5;
    amtx1 = amtx2 = amty1 = amty2 = .5;
    thetax = -1;
    thetay = -1;
  }

  virtual void onDraw() {

    if (bReset)
      reset();

    auto mouse = mMouse3D;

    Bennett b(theta, d1, d2);
    for (int i = 0; i < b.num(); ++i) {
      b[i].scale() = .3;
    }

    b(rotPhase);

    // Draw it
    Draw((Chain)b, true, false, 0, 0, 0);

    // alt drawing
    bDrawOutline ? DrawR_((Chain)b, 0, .5, 1) : DrawR((Chain)b, 0, .5, 1);

    if (bDrawMeet) {
      auto sphA = Construct::sphere(b[1].pos(), b.lengthB());
      auto sphB = Construct::sphere(b[3].pos(), b.lengthA());

      // intersect xy plane of frame with circle
      auto cir = (sphA ^ sphB);
      Draw(cir.dual(), 1, 0, 1);
      auto pair = (b[1].dxy() ^ cir).dual();
      Draw(b[1].dxy(), 0, 0, 0);
      Draw(pair, 1, 0, 0);
      Draw(Construct::pointA(pair), 1, 0, 0);
      Draw(Construct::pointB(pair), 1, 0, 0);

      Draw(sphA, 1, 0, 0, .2);
      Draw(sphB, 1, 0, 0, .2);
    }

    Bennett b2 = b.linkAt((int)link, thetax, amtx1, amtx2);

    for (int i = 0; i < b2.num(); ++i) {
      b2[i].scale() = .3;
    }

    Bennett balt = b.linkAt_((int)link, thetax, amtx1, amtx2);
    for (int i = 0; i < balt.num(); ++i) {
      balt[i].scale() = .3;
    }

    // sub bennett

    bool bSwitch = true;
    int itx = numx;
    int ity = numy;
    for (int i = 0; i < itx; ++i) {

      Draw((Chain)b2, true, false, 0, 0, 0);
      bDrawOutline ? DrawR_((Chain)b2, 0, .5, 1) : DrawR((Chain)b2, 0, .5, 1);

      b2 = b2.linkAt((int)link % 3, thetax, amtx1, amtx2);

      bSwitch = !bSwitch;

      if (bSubBennett) {
        if (bSwitch) {

          Bennett b3 = b2.linkAt(((int)link - 1) % 3, thetay, amty1, amty2);
          bool bSwitchB = true;
          for (int j = 0; j < ity; ++j) {

            Draw((Chain)b3, true, false, 0, 0, 0);
            bDrawOutline ? DrawR_((Chain)b3, 0, .5, 1)
                         : DrawR((Chain)b3, 0, .5, 1);

            b3 = b3.linkAt(((int)link) % 3, thetay, amty1, amty2);

            /* //these alternations twist the linkage */
            /* if (bSwitchB){ */
            /*   b3 = b3.linkAt(((int)link)%3,thetay, 1-amty1, 1-amty2,
             * b3.lengthB(), b3.lengthA() ); */
            /* } else { */
            /*   b3 = b3.linkAt(((int)link)%3, thetay, amty1, amty2 ); */
            /* } */

            bSwitchB = !bSwitchB;
          }
        }
      }
    }
  }
};

int main() {
  MyApp app;
  app.start();
  return 0;
}
