/*
 * =====================================================================================
 *
 *       Filename:  xFabrikChain.cpp
 *
 *    Description:  "FABRIK" Iterative Solver
                    (see paper Aristodou and Lasenby "Inverse Kinematic
 Solutions using Conformal Geometric Algebra")
 *
 *        Version:  1.0
 *        Created:  07/20/2015 19:56:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include <vsr/draw/vsr_chain_draw.h>
#include <vsr/form/vsr_chain.h>
#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
  // error threshold
  float err = 0;

  // Chain of 10 links
  Chain chain = Chain(10);

  void onSetup() { scene.camera.pos(0, 0, 10); }

  /// Add Variables to GUI
  void onDrawGui() { gui(err, "error", 0, 10); }

  void onDraw() {
    // intersection of ray cast by mouse into space and the xy plane
    calcMouse3D();
    Point point = Construct::meet(mMouseRay, Dlp(0, 0, 1, 0));

    // Draw point on xy plane as small red circle.  This is our "target"
    auto marker = Construct::sphere(point, .2);
    draw(marker, 1, 0, 0);

    // pass in beginning, ending index, adjust error threshold
    chain.fabrik(point, chain.num() - 1, 0, err);
    // recalculate forward kinematics
    chain.fk();

    for (int i = 0; i < chain.num(); ++i) {
      float t = 1.0 * i / chain.num();
      draw(chain[i]);
      draw(chain.nextLine(i), 1, 0, 0);
      draw(chain.nextSphere(i), t, 1, 1 - t, .3);
    }
  }
};

int main() {
  MyApp app;
  app.start();

  return 0;
}
