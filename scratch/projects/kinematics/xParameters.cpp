/*
 * =====================================================================================
 *
 *       Filename:  xParameters.cpp
 *
 *    Description:  denavit harten
 *
 *        Version:  1.0
 *        Created:  11/10/2015 13:09:10
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
  float amt3 = 0;

  float offset;

  Point mouse;


    Chain chain = Chain(5);

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(amt3,"amt3",-100,100)(offset,"offset",0,10)(bToggle,"bToggle")(bSet,"bSet");
    
  }

  void reset(){

    for (int i =0;i<chain.num();++i){
        chain.link(i).pos(0,2,offset);
        chain.link(i).rot() = Gen::rot(Biv::xz * PIOVERFOUR);
    }

  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    if (bMouseSet) mouse = calcMouse3D();
    
    reset();

    chain.joint(1).rot() = Gen::rot(Biv::xy * amt1);
    chain.joint(2).rot() = Gen::rot(Biv::xy * amt2);
    chain.joint(3).rot() = Gen::rot(Biv::xy * amt3);
      

     chain.fk();

     Draw(chain);

     for (int i=1;i<chain.num();++i){
//        Draw(chain.prevCircle(i),1,0,0);

//        Rot localunlink = chain[i].rot() * !chain.link(i-1).rot();
//        Vec rj = Op::rj( Vec(chain[i].pos() - chain[i-1].pos()), Biv::xy.spin( localunlink) );
//        DrawAt( Vec::z.spin( chain.prevRot(i) ), chain[i].pos(),1,0,1);
//

         auto target = ( chain[i].vec() - chain[i-1].vec() ).unit();              //global target direction
         auto linkRot = Gen::ratio(Vec::y,  chain.link(i-1).vec().unit() );       //what link position contributes...
         auto trot = chain[i-1].rot() * linkRot;
        
        // mFrame[i].rot() = mFrame[i-1].rot() * mLink[i-1].rot();
         target = target.spin( !(trot) );
          
         // auto rot = Gen::ratio( Vec::y.spin(trot), target ) * chain[i-1].rot(); 
          auto adj = Gen::ratio( Vec::y, target );
     
       // DrawAt( Vec::z.spin(rot), chain[i].pos(), 0,1,1);
         DrawAt( Vec::y.spin( chain[i-1].rot ), chain[i-1].pos(), 0,1,0);
     }


  }


  /*-----------------------------------------------------------------------------
   *  Keyboard control (hit 's' to toggle mouse tracking)
   *-----------------------------------------------------------------------------*/
  //  void onKeyDown(const gfx::Keyboard& k){
//    switch (k.code){
//      case 's': bMouseSet = !bMouseSet;
//    }
//
//    /// Default
//    App:onKeyDown(k);
//  }

  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
