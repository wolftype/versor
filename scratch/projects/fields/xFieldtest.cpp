/*
 * =====================================================================================
 *
 *       Filename:  xFieldtest.cpp
 *
 *    Description:  sanity checks
 *
 *        Version:  1.0
 *        Created:  02/19/2014 14:25:55
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
  float amt;

  float time =0;

  Field<Vec> vf(20,10,1);

  virtual void setup(){
    bindGLV();
    gui(amt,"amt",-100,100);
    amt = .3;
  }

  virtual void onDraw(){

      mouse = calcMouse3D();
      time +=.01;
      auto pnt = Round::point( sin(time)*5, cos(time) * 5, 0);

      Par par;
      for (int i = 0; i < vf.num(); ++i){
        float dist = Round::sqd( vf.grid(i), pnt ) + .1;
        par = Par(Tnv(0,1,0)).trs(pnt) * 1./dist;
        Vec nv = Round::location( vf.grid(i).bst(par*amt) ) - vf.grid(i);
        vf[i] = nv;
      }

      Draw(pnt);
      Draw(vf);

  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
