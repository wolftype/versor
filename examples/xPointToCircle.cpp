/*
 * =====================================================================================
 *
 *       Filename:  xPointToCircle.cpp
 *
 *    Description:  constrain a point to a circle
 *
 *        Version:  1.0
 *        Created:  07/24/2015 01:24:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include <vsr/vsr_app.h>
#include <vsr/form/vsr_rigid.h>

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App
{

  //Some Variables
  bool bReset = false;
  float amt = 0;


  Frame frame;
  Point point;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (amt, "amt", -1.0, 1.0) (bReset, "bReset");

    objectController.attach (&frame);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {

    auto circle = frame.cxy ();

    draw(frame);
    draw (circle,1,0,1);                      //draw circle in white

    //plane and plane2 are the same:
    auto plane = Round::carrier (circle);
    auto plane2 = frame.dxy().undual();

    //if amt = 0, point is at camera plane
    //hit 'x' to enable / disable mouse setting
    if (bSetMouse) point = calcMouse3D(amt);
    draw (point, 0, 1, 0);

    auto constrainedPoint = Constrain::PointToCircle (point, circle);
    draw (constrainedPoint, 1, 1, 0);

    //Done.  Now here we illustrate the geometry of How it works:

    // imaginary circle contraction -
    // imagine the intersection of the plane with the sphere bisected by plane through point
    auto icirc = point <= plane;
    draw(icirc,1,0,0);

    //projection of point onto plane
    Point p = ((icirc) / plane).null();

    //line through point and point p
    Line line = point ^ p ^ Inf(1);
    draw (line, 0,0,1);

    //circle at p
    Frame fp (p, frame.rot());
    fp.scale(.2);
    draw (fp.cxy(), 0, 0, 0);

    // line through coplanar point and circle center
    auto sur = Round::sur(circle);
    auto line2 = p ^ sur ^ Inf (1);
    draw (line2, 1, 0, 0);
    auto meet = (line2.dual() ^ sur ).dual();
    draw (meet, 0, 1, 1);

//    draw (plane, 1, 1, 0);
//    draw (coplanar, 0, 1, 0);
//    draw (Round::sphere(Round::center(coplanar), .5), 0, 1, 0);
//    draw (line, 1, 1, 1);
//    draw (meet,0,0,0);

    draw (point, 0,1,1);
    draw (p, 0,0,0);

    // then calculate meet of line and surround of circle...
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
