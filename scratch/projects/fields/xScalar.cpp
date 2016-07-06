/*
 * =====================================================================================
 *
 *       Filename:  xScalar.cpp
 *
 *    Description:  scalar gradient
 *
 *        Version:  1.0
 *        Created:  08/01/2014 15:36:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:
 *
 * =====================================================================================
 */



#include "vsr_app.h"
#include "vsr_field.h"

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {

  Pnt mouse;

  float time;
  float amt;

  bool bReset;

  virtual void setup(){
      bindGLV();
      gui(amt,"amt",-100,100);
      gui(bReset,"reset");
  }

  virtual void onDraw(){

    mouse = calcMouse3D();

    Field<Sca> f(10,10,1);
    for (int i=0;i<f.num();++i){
      f[i][0] = (float)i/f.num();
    }
    Pnt pnt = Round::null(0,0,0);

    for (int i=0;i<f.num();++i){
      DrawAt( pnt, f.grid(i), f[i][0],0,1-f[i][0]);
    }

    for (int i=0;i<f.w();++i){
      for (int j=0;j<f.h();++j){
          auto a = f.at(i,j);
          auto b = f.at(i+1,j);
          auto c = f.at(i,j+1);

      }
    }
  }

};


int main(){
  MyApp app;
  app.start();
  return 0;
}
