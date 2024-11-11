#include <vsr/vsr_app.h>
#include <vsr/form/vsr_chain.h>

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {

  float amt, linewidth, distA;
  Chain k = Chain(5);
  Pnt targetPos;

  void onDrawGui() {
    gui(distA, "LinkLength", 1, 10);
    gui(linewidth, "linewidth", 0, 10);
  }

  void onSetup() {
    distA = 5.0;
    linewidth = 3;
    scene.camera.pos(0, 0, 30);

    //    mRenderGraph.immediate(false);
  }

  void onDraw() {

    // We are looking for the "elbow"

    // A baseframe -- default constructor places it at the origin
    Frame baseFrame;

    // Line projected from mouse into infinity
    auto line = mMouseRay;

    // Point on the line closest to the origin
    targetPos = Construct::point(line, Ori(1));

    // targetFrame is the "finger tip""
    Frame targetFrame(targetPos);

    // DualPlane of End Effector Target
    DualPlane targetXZ = targetFrame.dxz();

    // Make distance contraint sphere from a point and a radius
    // this calls Round::dls( Pnt,float )
    DualSphere targetSphere = Round::sphere(targetPos, distA);

    // secondFrame is the "shoulder"
    Frame secondFrame(0, distA, 0);

    // Make another Sphere constraint
    DualSphere firstSphere = Round::sphere(secondFrame.pos(), distA);

    // Plane of Rotation formed by line axis of base and target point
    Plane rotationPlane = baseFrame.ly() ^ targetPos;

    // Line of Target is the meet of the two planes
    DualLine tline = targetXZ ^ rotationPlane.dual();

    // Point Pairs of Final joint position
    Pair fjoint = (tline ^ targetSphere).dual();

    // Of the two points, pick the point closest to the base frame
    Frame finalFrame(Round::split(fjoint, false), Rot(1, 0, 0, 0));

    // Sphere around fframe
    auto ffsphere = Round::sphere(finalFrame.pos(), distA);

    // A circle of possible positions
    Circle cir = (ffsphere ^ firstSphere).dual();

    // TWo points where the middle joint could be
    Pair fpair = (rotationPlane.dual() ^ cir.dual()).dual();

    // Pick One and put the middle frame there
    Frame middleFrame(Round::split(fpair, true));

    // We can store the positions in a chain class which will sort out relative
    // orientations for us
    k[0] = baseFrame;
    k[1] = secondFrame;
    k[2] = middleFrame;
    k[3] = finalFrame;
    k[4] = targetFrame;

    // Base Frame will rotate to plane defined by its yaxis and target point
    Rot r1 = Gen::ratio(Vec::z, Vec(rotationPlane.dual().unit()));
    k[0].rot(r1);

    // for all the other frames, calculate joint rotations and link lengths from
    // current positions
    k.calcJoints(1);
    k.links();

    for (int i = 0; i < 4; ++i) {

      glColor3f(0, 1, 0);
      gfx::Glyph::Line(k[i].pos(), k[i + 1].pos());

      draw(k[i]);
    }

    draw(rotationPlane, 0, 1, 0);
    draw(targetXZ, 0, .5, 1);
    draw(tline, 1, 1, 0);
    draw(fjoint);
    draw(cir, .5, 1, 1);
    draw(fpair, 1, .5, .5);
    draw(targetPos, 1, 0, 0);
    draw(ffsphere, 1, 0, 0, .4);
    draw(firstSphere, 1, 0, 0, .4);
  }
};

int main() {

  MyApp app;
  app.start();

  return 0;
}

