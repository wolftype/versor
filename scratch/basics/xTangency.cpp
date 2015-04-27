/*
 * =====================================================================================
 *
 *       Filename:  xTangency.cpp
 *
 *    Description:  tangents to circles and spheres
 *
 *        Version:  1.0
 *        Created:  02/10/2015 11:39:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D_app.h"   

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {

  DualSphere dls = sphere(0,0,0,1);

  //Some Variables
  float amt = 0;

  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100);
    ///Add circle to objectController
    objectController.attach(&dls);
  }

  void onDraw(){
    auto p = point(0,0,0);//calcMouse3D();

    cout << (p<=dls)[0] << endl;
    
    //tangent of direct round at point p gives a direction
    auto tangentToCircle = Tangent::at(dls.dual(),p);

    //line through point and sphere, intersected with sphere, gives point on sphere
    //closest to point p
    auto line = p ^ dls ^ Inf(1);
    auto pointOnCircle = Ro::split( (line.dual() ^ dls).dual(), false);
    
    draw(pointOnCircle,1,1,0);

    draw(tangentToCircle,0,0,1);

    draw(Ro::loc(tangentToCircle),0,1,0);

    draw(dls,1,0,0,.2); ///<-- draw red sphere

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
