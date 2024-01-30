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

    //if amt = 0, point is at camera plane
    //hit 'x' to enable / disable mouse setting
    if (bSetMouse) point = calcMouse3D(amt);

    // The XY Unit Circle of our Frame
    auto circle = frame.cxy ();
    // The point on the circle closest to the mouse position
    auto constrainedPoint = Constrain::PointToCircle (point, circle);
   
    // draw 'em
    draw (point, 0, 1, 0);
    draw(frame);
    draw (circle,1,0,1);
    draw (constrainedPoint, 1, 1, 0);

    //Done.  Now here we illustrate the geometry of How it works:

    //plane
    auto plane = Round::carrier (circle);
    //(altnernative formulation of the same plane)
    auto plane2 = frame.dxy().undual();


    // imaginary circle contraction -
    // (red) intersection of the plane with the sphere bisected by plane
    auto icirc = point <= plane;
    draw(icirc,1,0,0);

    //(white) projection of point onto plane
    Point p = ((icirc) / plane).null();
    draw (p, 1,1,1);
    //(green) circle at p
    Frame fp (p, frame.rot());
    fp.scale(.2);
    draw (fp.cxy(), 0, 1, 0);


    //(blue) line through point and point p (orthogonal to plane)
    Line line = point ^ p ^ Inf(1);
    draw (line, 0,0,1);

    // (red) line through coplanar point and circle center
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
