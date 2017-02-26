/*
 * =====================================================================================
 *
 *       Filename:  xField.cpp
 *
 *    Description:  a 3d field of vectors
 *
 *        Version:  1.0
 *        Created:  02/09/2015 14:18:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:
 *
 * =====================================================================================
 */


#include "vsr_app.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {

  Field<Vec> f;

  //Some Variables
  bool button = false;
  float amt = 0;

  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(button,"bImmediate");

    //allocate field
    f.resize(10,10,10);

  }

  void onDraw(){

    draw(f);
   // mSceneRenderer.immediate(button);
  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
