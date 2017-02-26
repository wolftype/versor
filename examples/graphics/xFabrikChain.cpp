/*
 * =====================================================================================
 *
 *       Filename:  xFabrikChain.cpp
 *
 *    Description:  "FABRIK" Iterative Solver
                    (see paper Aristodou and Lasenby "Inverse Kinematic Solutions using Conformal Geometric Algebra")
 *
 *        Version:  1.0
 *        Created:  07/20/2015 19:56:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include <vsr/vsr_app.h>
#include <vsr/form/vsr_chain.h>
#include <vsr/draw/vsr_chain_draw.h>

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {

  //Some Variables
  bool bReset = false;
  float amt = 0;


  //Chain of 10 links
  Chain chain = Chain(10);


  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");

  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines
   *-----------------------------------------------------------------------------*/
  void onDraw(){
    auto point = calcMouse3D();

    //adjust amt variable to adjust error threshold
    chain.fabrik(point,chain.num()-1,0,amt);
    chain.fk();

   // Draw(chain);

    for (int i = 0;i<chain.num(); ++i){
      Draw( chain[i]);
      Draw( chain.nextLine(i),1,0,0);
      Draw( chain.nextSphere(i),0,1,1,.3);
    }


  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
