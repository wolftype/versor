/*
 * =====================================================================================
 *
 *       Filename:  xMeet.cpp
 *
 *    Description:  meet of two spheres is a circle
 *                  meet of three is a point pair
 *
 *        Version:  1.0
 *        Created:  02/10/2015 13:11:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {

  /// DualSpheres with radius of 1        x  y  z  r
  DualSphere sphereA = Construct::sphere(-.5, 0, 0, 1);
  DualSphere sphereB = Construct::sphere(0.5, 0, 0, 1);
  DualSphere sphereC = Construct::sphere(0.0, .5, 0, 1);

  void onSetup() {

    /// Add spheres to objectController
    /// (hit "t" and "s" keys then click on them to translate and scale)
    objectController.attach(&sphereA);
    objectController.attach(&sphereB);
    objectController.attach(&sphereC);

    // Set background color
    mColor.set(.91, .886, .824);
    // Turn off immediate mode rendering
    mRenderGraph.immediate(false);
  }

  void onDraw() {

    /// intersection of two elements is the dual of the outer product of duals
    auto circle = (sphereA ^ sphereB).dual();
    auto pair = (circle.dual() ^ sphereC).dual();

    /// draw circle in green
    draw(circle, 0, 1, 0);
    /// draw pair in cyan
    draw(pair, 0, 1, 1);

    /// draw spheres in red
    draw(sphereA, 1, 0, 0);
    draw(sphereB, 1, 0, 0);
    draw(sphereC, 1, 0, 0);
  }
};

int main() {

  MyApp app;
  app.start();

  return 0;
}
