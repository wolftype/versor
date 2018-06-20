/*
 * =====================================================================================
 *
 *       Filename:  xLinkages.cpp
 *
 *    Description:  Hmmm, looks like this needs work.
 *
 *        Version:  1.0
 *        Created:  01/13/2014 12:50:12
 *       Revision:  none
 *       Compiler:  clang++3.2 or higher or gcc4.7 or higher
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include <vsr/vsr_app.h>
#include <vsr/form/vsr_chain.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App
{

  Pnt mouse;
  Lin ray;

  float amt, pinRatio, decayRate;

  int num = 20;
  Chain chainA = Chain (num);
  Chain chainB = Chain (num);

  Point pnt = PT (.5, -2, 0);


  void setup ()
  {
    gui (amt, "amt", -100, 100);
    gui (pinRatio, "pinRatio", 0, 100) (decayRate, "decayRate", -100, 100);

    objectController.attach (&pnt);

    pinRatio = 1;
    decayRate = 0;
  }

  virtual void onDraw ()
  {

    mouse = calcMouse3D ();


    chainA[0].pos () = pnt;

    for (int i = 0; i < num; ++i)
      {
        draw (chainA[i]);
        draw (chainB[i]);
      }

    double tpinRatio = pinRatio;
    bool flip = false;

    for (int i = 0; i < num; i += 2)
      {

        //Next chain element connects A and B (pivot)
        Dls dlsA = chainA.nextSphere (i);
        Dls dlsB = chainB.nextSphere (i);

        //Alternate dilation of A and B
        Dls tdlsa = flip ? dlsA : dlsA.dil (dlsA.null (), log (tpinRatio));
        Dls tdlsb = !flip ? dlsB : dlsB.dil (dlsB.null (), log (tpinRatio));

        //Intersection
        Par p = (tdlsa ^ tdlsb ^ Dlp (0, 0, 1, 0)).dual ();
        Pnt pnt = Round::split (p, flip);

        //Set Chain Position of Next Element
        chainA[i + 1].pos () = pnt;
        chainB[i + 1].pos () = pnt;

        //Alternate next position ratio
        double a = (1 / tpinRatio);
        double b = tpinRatio;

        //chain + direction * ratio
        chainA[i + 2].pos () = Round::null (
          chainA[i].vec () + ((chainA[i + 1].vec () - chainA[i].vec ())
                              * (1 + ((flip) ? b : a))));
        chainB[i + 2].pos () = Round::null (
          chainB[i].vec () + ((chainB[i + 1].vec () - chainB[i].vec ())
                              * (1 + ((flip) ? a : b))));

        flip = !flip;

        tpinRatio *= (1 - decayRate);

        draw (pnt);
      }

    chainA.calcJoints ();
    chainB.calcJoints ();
  }
};


int main ()
{
  MyApp app;
  app.start ();
  return 0;
}
