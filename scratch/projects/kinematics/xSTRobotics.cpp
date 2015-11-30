/*
 * =====================================================================================
 *
 *       Filename:  xSTRobotics.cpp
 *
 *    Description:  model of st robotics
 *
 *        Version:  1.0
 *        Created:  11/19/2015 13:15:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_chain.h"
#include "draw/vsr_chain_draw.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  
  float amt1 = 0;
  float amt2 = 0;

   Chain chain = Chain(6);


  Point mouse;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(bToggle,"bToggle")(bSet,"bSet");
   
    amt1 = .001; 
  }


  void reset(){

    chain = Chain(6);

  }
  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    //if (bMouseSet) 
    mouse = calcMouse3D();

    if (bSet) reset();

    chain.baseFrame().rot() = Gen::rot( Biv::yz * -PIOVERFOUR );
    chain.link(0).pos(0,.2,2);
    chain.link(0).rot() = Gen::rot( Biv::xz * PIOVERFOUR );

    chain.link(1).pos(0,1,amt1);
    chain.link(2).pos(0,1,amt1);
    chain.link(3).pos(0,.5,.5);
    chain.link(3).rot() = Gen::rot( Biv::yz * -PIOVERFOUR );
    chain.link(4).pos(0,.2,.2);
   
    chain.joint(0).rot() = Gen::ratio( Vec::y, Vec(Op::pj( Vec(mouse).unit(), chain.baseFrame().xy() ) ).unit().spin( !chain.baseFrame().rot()) );
  
  
    chain.fk();

    chain.constrainedFabrik(mouse, chain.num()-1, 1,.01);
    chain.fk();
   // Draw(chain.baseFrame());
    Draw(chain);

    for (int i=0;i<chain.num();++i){
    //  Vec rj = Op::rj( link(i).vec(), Biv::xy );
     
      Draw(chain.nextCircle(i),1,0,0);
      if (i>1) Draw(chain.prevCircle(i),0,1,0);

    }


  }

//
//  /*-----------------------------------------------------------------------------
//   *  Keyboard control (hit 's' to toggle mouse tracking)
//   *-----------------------------------------------------------------------------*/
//  void onKeyDown(const gfx::Keyboard& k){
//    switch (k.code){
//      case 's': bMouseSet = !bMouseSet;
//    }
//
//    /// Default
//    App:onKeyDown(k);
//  }
//
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
