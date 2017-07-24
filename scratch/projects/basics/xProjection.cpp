/*
 * =====================================================================================
 *
 *       Filename:  xProjection.cpp
 *
 *    Description:  projection of elements onto circles, etc.
 *
 *        Version:  2.0
 *        Created:  11/25/2013 10:49:16
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *
 * =====================================================================================
 */


#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {

  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  Frame frame;

  void setup(){
      gui(amt,"amt",-100,100);
      objectController.attach(&frame);
  }

  void onDraw(){

      mouse = calcMouse3D();

      Cir cir = frame.cxy();
      Dls dls = frame.bound();

      //RELATIONSHIP BETWEEN PROJECTION AND PLUNGE
      //The projection of a tangent element onto a round element
      //Is the meet of that round element and the plunge between them

      //Tangent "up" vector at mouse position
      auto tangent = Pair( Tnv(0,1,0) ).trs(mouse);

      //Projection of tangent onto frame's xy circle
      auto projCir = Op::project(tangent, cir);
      //Extract Point pair portion of it
      Pair projCir2 = projCir;

      //Projection of tangent onto frame's surround
      auto projSph = Op::project(tangent, dls.dual());
      //Extract Point pair portion of it
      Pair projSph2 = projSph;

      //Circle Plunge Outer product of tangent with surround
      Cir tanSph = tangent^dls;
      //Sphere Plunge Outer product of tangent with dual circle
      Sph tanCir = tangent^cir.dual();

      Draw(mouse);

      Draw(projSph2,1,0,0);
      Draw(tanSph,0,1,0);
//      Draw(projCir2,1,0,1);
//      Draw(tanCir,0,1,1);
      Draw(dls,0,0,1,.2);


  }

};

int main(){
  MyApp app;
  app.start();
  return 0;
}
