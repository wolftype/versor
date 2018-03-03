/*
 * =====================================================================================
 *
 *       Filename:  xRotorRatio.cpp
 *
 *    Description:  ratio of rotors
 *
 *        Version:  1.0
 *        Created:  04/23/2015 18:17:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt,amt2,amt3,amt4= 0;


  
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(amt2,"amt2",-10,10)(amt3,"amt3",-10,10)(amt4,"amt4",0,10)(bReset,"bReset");
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    Frame fa(-2,1,0); fa.rot() = cga::gen( Biv::xz * amt) * cga::gen( Biv::xy * amt2);
    Frame fb(0,2,0); fb.rot() =  fa.rot() * cga::gen( Biv::xz * amt3) * cga::gen( Biv::xy * amt4 );
    fb.pos() = (fa.pos() + fa.y()).null();

    Draw(fa); Draw(fb);


    auto yratio = Gen::ratio( fa.y(), fb.y() );

    auto ratio = fb.rot() / fa.rot(); // rotor to take 

    auto comp = !fa.rot() * !yratio * fb.rot(); //SEQUENCE MATTERS!//!fa.rot() * 

    Frame f; f.rot() = comp;// * fa.rot();
    Draw ( f );


  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
