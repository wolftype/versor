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

struct MyApp : App {

  // A position and orientation in space
  Frame frame;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void onSetup() {
    objectController.attach(&frame);
    mRenderGraph.immediate(false);
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines
   *-----------------------------------------------------------------------------*/
  void onDraw() {

    // you mouse as a point in space
    Point point = mMouse3D;

    // The XY Unit Circle of our Frame
    auto circle = frame.cxy();

    // The point on the circle closest to the mouse position
    auto constrainedPoint = Constrain::PointToCircle(point, circle);

    // draw 'em
    draw(point, 0, 1, 0);
    draw(frame);
    draw(circle, 1, 0, 1);
    draw(constrainedPoint, 1, 1, 0);

    // Done.  Now here we illustrate the geometry of their relationships;

    // plane the circle is in
    auto plane = Round::carrier(circle);
    //(altnernative formulation of the same plane)
    // auto plane2 = frame.dxy().undual();

    // imaginary circle contraction -
    // (red) intersection of the plane with the sphere bisected by plane
    auto icirc = point <= plane;
    draw(icirc, 1, 0, 0);

    //(white) projection of point onto plane
    Point p = ((icirc) / plane).null();
    draw(p, 1, 1, 1);

    // (red) line through coplanar point and circle center
    auto sur = Round::surround(circle);
    auto line2 = p ^ sur ^ Inf(1);
    draw(line2, 1, 0, 0);

    // calculate meet of line and surround of circle...
    auto meet = (line2.dual() ^ sur).dual();
    draw(meet, 0, 1, 1);

    // Let's draw a few things to clarify relationships
    // draw a small (green) circle at p in plane
    draw(Construct::circle(p, Flat::dir(plane).copy<Biv>(), .2), 0, 1, 0);

    // draw (blue) line through point and point p (orthogonal to plane)
    Line line = point ^ p ^ Inf(1);
    draw(line, 0, 1, 1);

    draw(plane, .8, .2, 0);
    draw(point, 0, 1, 1);
    draw(p, 0, 0, 0);
  }
};

int main() {

  MyApp app;
  app.start();

  return 0;
}
