#include <vsr/vsr_app.h>

using namespace vsr::cga;
using namespace gfx;

struct MyApp : App {

  float time = 0.0;
  //DualSphere at 2,0,0 with radius .5
  DualSphere sphere = Construct::sphere(2,0,0,.5);;

  //Circle through 3 Points
  Circle circle = Construct::point(1,0,0) ^ Construct::point(0,1,0) ^ Construct::point(-1,0,0) ;

  //Dual plane with Normal in Y direction
  DualPlane dualplane = DualPlane(0,1,0);

  void setup(){

    //Enable Mouse Control with 't' 'r' and 's' keys
    objectController.attach(&circle);
    objectController.attach(&sphere);

    time = 0.0;
  }

  void onDraw(){

    //Calculate  Intersection (which is the dual of the outer product of duals . . . )
    //i.e. (circle.dual() ^ dualplane).dual()
    auto pair_meet = Construct::meet(circle,dualplane);
    auto circle_meet = Construct::meet(sphere,dualplane);

    //Draw 'em with colors
    draw(circle,0,1,0);          //<-- Draw Green Circle
    draw(dualplane,0,0,1);       //<-- Draw Blue Plane
    draw(pair_meet,1,0,0);       //<-- Draw Red Point Pair (intersection)

    draw(sphere,1,0,0);          //<-- Draw Red Sphere
    draw(circle_meet,0,1,0);     //<-- Draw circle (intersection)

    auto rot = Gen::rot(Biv(-PI * time,0,0));
    draw(Vec(1,0,0).spin(rot), 1,0,1);
    draw(Vec(1,0,0), 1,1,1);

    time += .01;


  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
