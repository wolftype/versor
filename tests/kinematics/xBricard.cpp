/*
 * =====================================================================================
 *
 *       Filename:  xBricard.cpp
 *
 *    Description:  bricard 6 bar linkage
 *
 *        Version:  1.0
 *        Created:  02/25/2015 15:45:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D_app.h"   
#include "vsr_chain.h"
#include "vsr_chain_draw.h"


using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;
  float R0,R1,R2,R3,R4,R5;
  float theta0,theta1,theta2,theta3,theta4,theta5;

  Chain chain;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");

    gui(R0, "R0",-10,10);
    gui(R1, "R1",-10,10);
    gui(R2, "R2",-10,10);
    
    gui(theta0, "theta0",-PI,PI);
    gui(theta1, "theta1",-PI,PI);
    gui(theta2, "theta2",-PI,PI);

    chain.alloc(6);

  }

  /*-----------------------------------------------------------------------------
   *  Simulation
   *-----------------------------------------------------------------------------*/
  void onAnimate(){

    /// Line Symmetric Case
    R3=R0; R4=R1; R5=R2;
    theta3=theta0; theta4=theta1; theta5=theta2;

    float R[] = {R0,R1,R2,R3,R4,R5};
    float theta[] = {theta0,theta1,theta2,theta3,theta4,theta5};
    for (int i=0;i<6;++i){
      chain.link(i).pos(R[i], 1, 0);
      chain.link(i).rot() = Gen::rot( Biv::xz * theta[i]);
    }
    chain.joint(0).rot( Biv::xy * amt );


    //chain.calcJoints();
    chain.fk();

  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
    DrawR(chain);
    Draw(chain);
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
