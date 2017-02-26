/*
 * =====================================================================================
 *
 *       Filename:  xMeet.cpp
 *
 *    Description:  meet of two spheres is a circle
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


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {

  ///DualSpheres centered at x=-.5 and x=.5, each with radius of 1
  DualSphere sphereA = Construct::sphere(-.5,0,0,1);
  DualSphere sphereB = Construct::sphere(.5,0,0,1);

  void setup(){
    
    ///Add spheres to objectController
    ///(hit "tab" and then "t" and "s" keys to translate and scale)
    objectController.attach(&sphereA);
    objectController.attach(&sphereB);
  }

  void onDraw(){

    /// intersection of two spheres is the dual of their outer product
    auto meet = (sphereA ^ sphereB).dual();

    draw(meet,0,1,0);                   ///<-- draw meet (circle) in green
    
    draw(Round::surround(meet),0,0,1,.5);  ///<-- draw surround of meet (a sphere) in blue with half alpha

    draw(sphereA,1,0,0,.5);             ///<-- draw spheres in red, with half alpha
    draw(sphereB,1,0,0,.5);             ///<-- draw spheres in red, with half alpha

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
