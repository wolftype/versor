/*
 * =====================================================================================
 *
 *       Filename:  xDHParam.cpp
 *
 *    Description:  denavit hartenberg parameters encoded
 *
 *        Version:  1.0
 *        Created:  11/28/2015 22:25:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "form/vsr_chain.h"
#include "draw/vsr_chain_draw.h"


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  
  float amt0 = 0;
  float amt1 = 0;
  float amt2 = 0;
  float amt3 = 0;
  float amt4 = 0;


  Point mouse;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt0,"amt0",-100,100);
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(bToggle,"bToggle")(bSet,"bSet");
    gui(amt3,"amt3",-100,100);
    gui(amt4,"amt4",-100,100);
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    mouse = calcMouse3D();

    Chain chain(5);

    for (int i =0;i<chain.num();++i){  
     chain.link(i).pos(0,1,amt1);
     chain.link(i).rot() = Gen::rot( Biv::xz * PIOVERFOUR * amt0 );
     
    }

    chain.joint(0).rot() = Gen::rot( Biv::xy * amt2);
    chain.joint(1).rot() = Gen::rot( Biv::xy * amt3);
    chain.joint(2).rot() = Gen::rot( Biv::xy * amt4);

    chain.fk();

//    Rot ry(1);
//    for (int i = 0; i< chain.num(); ++i){
//      
//      int next = i < (chain.num()-1) ? i+1 : 0;    
//      
//      auto target = ( chain[next].vec() - chain[i].vec() ).unit();         //global target direction
//      target = Op::pj( target, chain[i].xy() ).unit();
//
//     // auto linkRot = Gen::ratio(Vec::y,  chain.link(i).vec().unit() );            //what link position contributes...
//        
//      target = target.spin(  !(ry) );//!(ry*linkRot) );//*linkRot) );
//
//      DrawAt(target, chain[i].pos(), 0,1,1);
//
//      auto adjustedRot = Gen::ratio( Vec::y, target );
//
//      chain.joint(i).rot() = adjustedRot;
//
//      ry = ry * chain.joint(i).rot() * chain.link(i).rot();   //* mLink[i].rot()                          //compound: last * current * next
//    }


    if (bSet) {
      chain.calcJoints();
      chain.fk();
    }

     //Draw circle of rotation of each joint
     for (int i=1;i<chain.num();++i){
        Draw( chain.prevCircle(i),1,0,0); 
        Draw( chain.nextCircle(i),0,1,0); 

       auto s = chain[i].z() <= chain[i-1].z();
       if ( !FERROR(s[0])) cout << s[0] << endl;
     }



    Draw(chain);

    for (int i =0;i<chain.num();++i){
      Draw( chain.nextCircle(i), 0,1,0);
      if (i>0) Draw( chain.prevCircle(i), 1,0,0);
    //  if (i>0) Draw( Frame(chain[i].pos(), chain[i].rot() * !chain.link(i-1).rot()) );
    }
    

  }




  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
