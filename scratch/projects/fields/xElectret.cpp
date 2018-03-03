/*
 * =====================================================================================
 *
 *       Filename:  xElectret.cpp
 *
 *    Description:  electric sources and sinks modelled as weighted flat points
 *
 *        Version:  1.0
 *        Created:  12/03/2013 11:45:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_app.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {

  Pnt mouse;

  float time=0;
  float amt;

  //ELECTRET FIELD
  Field<Flp> f(5,5,5,1);

  virtual void setup(){
      bindGLV();
      gui(amt,"amt",-100,100);
      scene.camera.pos( 0,0,10 );

  }


  virtual void onDraw(){

    mouse = calcMouse3D();



    bool bPolarity = false;
    for (int i = 0; i < f.num(); ++i){
      bPolarity = !bPolarity;

      f[i] = f.grid(i) ^ Inf(1);
      if (bPolarity) {
        f[i] *= -1;
        Draw(f[i],0,1,0);
      } else Draw(f[i], 1,0,0);
    }

    //FIELD THAT IT ACTS ON
    Field<Vec> v(10,10,10,.1);

    for (int i = 0; i < v.num(); ++i){

      Flp flp;
      //Sum Distances
      for (int j = 0; j < f.num(); ++j){
        float wt = 1.0 / (.1 + Round::sqd(v.grid(i), f.grid(j)) );
        flp += f[j] * wt;
      }
      auto tp = Round::location( v.grid(i).spin( Gen::bst(flp * amt) ) );
      v[i] = tp - v.grid(i);
    }

    Draw(v);

    //EQUIPOTENTIALS

  }



};


int main(){

  MyApp app;
  app.start();

  return 0;

}
