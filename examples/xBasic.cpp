#include "vsr_app.h"

using namespace vsr::cga;

struct MyApp : App {  

  //DualSphere at 2,0,0 with radius .5
  DualSphere sphere = Construct::sphere(2,0,0,.5);;

  //Circle through 3 Points
  Circle circle = Construct::point(1,0,0) ^ Construct::point(0,1,0) ^ Construct::point(-1,0,0) ; 
    
  //Dual plane with Normal in Y direction
  DualPlane dualplane = DualPlane(0,1,0);

  void setup(){
   
    //bind glv gui 
    bindGLV();

    //set renderer to use programmable pipeline
    mSceneRenderer.immediate(false);

    //Enable Mouse Control with 't' 'r' and 's' keys
    objectController.attach(&circle);
    objectController.attach(&sphere);
  }

  void onDraw(){

    //Calculate  Intersection (which is the dual of the outer product of duals . . . )
    auto pair_meet = Construct::meet(circle,dualplane);  //<---- i.e. (circle.dual() ^ dualplane).dual()
    auto circle_meet = Construct::meet(sphere,dualplane);

    //Draw 'em with colors
    draw(circle,0,1,0);          //<-- Draw Green Circle
    draw(dualplane,0,0,1);  //<-- Draw Blue Plane
    draw(pair_meet,1,0,0);       //<-- Draw Red Point Pair (intersection)

    draw(sphere,1,0,0);          //<-- Draw Red Sphere
    draw(circle_meet,0,1,0);        //<-- Draw circle (intersection)

//    text("use t, r and s keys and drag with mouse to Translate, Rotate and Scale the Circle");
  } 

};
                        

int main(){
  
  MyApp app;
  app.start();
  
  return 0;
  
}
