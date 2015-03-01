#include "vsr_cga3D_app.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {  

  //DualSphere
  DualSphere s = sphere(2,0,0,.5);//Ro::dls(.5,2,0,0);

  //Circle through 3 Points
  Circle c = point(1,0,0) ^ point(0,1,0) ^ point(-1,0,0) ; //Ro::point(1,0,0) ^ Ro::point(0,1,0) ^ Ro::point(-1,0,0);
    
  //Dual plane with Normal in Y direction
  DualPlane dualplane = DualPlane(0,1,0);

  void setup(){
   
    //bind glv gui 
    bindGLV();

    //set renderer to use programmable pipeline
    mSceneRenderer.immediate(false);

    //Enable Mouse Control with 't' 'r' and 's' keys
    objectController.attach(&c);
    objectController.attach(&s);
  }

  void onDraw(){

    //Calculate  Intersection (which is the dual of the outer product of duals . . . )
    auto pair = meet(c,dualplane);  //<---- i.e. (circle.dual() ^ dualplane).dual()
    auto cir = meet(s,dualplane);

    //Draw 'em with colors
    draw(c,0,1,0);          //<-- Draw Green Circle
    draw(dualplane,0,0,1);  //<-- Draw Blue Plane
    draw(pair,1,0,0);       //<-- Draw Red Point Pair (intersection)

    draw(s,1,0,0);          //<-- Draw Red Sphere
    draw(cir,0,1,0);        //<-- Draw circle (intersection)

//    text("use t, r and s keys and drag with mouse to Translate, Rotate and Scale the Circle");
  } 

};
                        

int main(){
  
  MyApp app;
  app.start();
  
  return 0;
  
}
