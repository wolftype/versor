/*
 * =====================================================================================
 *
 *       Filename:  xBricard.cpp
 *
 *    Description:  bricard 6 bar linkage (see "Lost Jade" article by Y.Chen and Z.You, 2012)
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


#include "util/vsr_cga3D_app.h"   
#include "form/vsr_chain.h"
#include "draw/vsr_chain_draw.h"


using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt0,amt1,amt2,amt3 = 0;
  //axis translation offset
  float R0,R1,R2,R3,R4,R5;
  //skews
  float theta0,theta1,theta2,theta3,theta4,theta5;

  Chain chain;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt0,"amt0",-100,100)(bReset,"bReset");
    gui(amt1,"amt1",-100,100);
    gui(amt2,"amt2",-100,100);
    gui(amt3,"amt3",-100,100);

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
      auto rot =  Gen::rot( Biv::xz * theta[i]);
      chain.link(i).pos( Vec(0,3,0) + Vec(0,0,R[i]) );//.spin(rot) );
      chain.link(i).rot() = rot;
    }
    
    chain.joint(0).rot( Biv::xy * amt0 );
    chain.joint(1).rot( Biv::xy * amt1 );
    chain.joint(2).rot( Biv::xy * amt2 );
    chain.joint(3).rot( Biv::xy * amt3 );

    chain.fk();

    chain[5] = Frame( !chain.link(5).mot() * chain[0].mot() );
   // auto s2 = chain.lastDls(5);

    //chain.calcJoints();

  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
    DrawR(chain);
    Draw(chain);

    auto s1 = chain.nextSphere(1);
    auto p1 = chain.nextPlane(1);
    auto c1 = chain.nextCircle(1);
   
    auto c2 = chain.nextCircle(2);
    auto c3 = chain.nextCircle(3);
    auto c4 = chain.nextCircle(4);

   // Draw(c2,0,0,1);
   // Draw(c3,0,0,1);
   // Draw(c4,0,0,1);
    
    Draw( c1,0,1,0 );
   // Draw( s1,1,0,0,.2 );

    auto s5 = chain.prevSphere(5);
    auto p5 = chain.prevPlane(5);
    auto c5 = chain.prevCircle(5);

    auto dlp = Dlp(Round::sur(c5) - Round::sur(c1));
  //  Draw(dlp,1,1,0);

  //  auto con = Gen::ratio( c1, c5 );
  //  auto con2 = Gen::con( Gen::log(con), .5 );
  //  Draw( Line(c1.spin(con2)),0,1,0); 


  //  Draw( p5,1,1,0 );
    Draw( c5,0,1,0 );
  //  Draw( s5,1,0,0,.2 );

    //inverses

    //Rot r1 =  !chain.link(4).rot() * chain[5].rot();




  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
