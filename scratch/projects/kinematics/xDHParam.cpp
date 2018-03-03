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
  float length;


  Point mouse;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    
    gui(amt0,"amt0",-100,100);
    gui(length,"length",1,10);
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(bToggle,"bToggle")(bSet,"bSet");
    gui(amt3,"amt3",-100,100);
    gui(amt4,"amt4",-100,100);
    
    amt0 = amt1 = 1;
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    mouse = calcMouse3D();

    Chain chain(5);

    for (int i =0;i<chain.num();++i){  
     chain.link(i).pos(0,length,amt1);
     chain.link(i).rot() = Gen::rot( Biv::xz * PIOVERFOUR * amt0 );
     
    }

    chain.joint(0).rot() = Gen::rot( Biv::xy * amt2);
    chain.joint(1).rot() = Gen::rot( Biv::xy * amt3);
    chain.joint(2).rot() = Gen::rot( Biv::xy * amt4);

    chain.fk();


     //Draw circle of rotation of each joint
     for (int i=1;i<chain.num();++i){
        auto cira =  chain.prevCircle(i);
        auto cirb = chain.nextCircle(i);

        Draw ( Constrain::PointToCircle(mouse, cira), 1,1,0);
        Draw ( Constrain::PointToCircle(mouse, cirb), 0,1,1);

        Draw( chain.prevCircle(i),1,0,0); 
        Draw( chain.nextCircle(i),0,1,0); 

        if (i==2) {
          //rel xy rot
          auto xy = chain[i].xy();

          //ratio
          auto rot = Gen::ratio( Biv::xy.duale(), xy.duale());

          Draw( chain.prevPlane_(i), 0,1,1);


        } //Draw( chain.prevPlane_(i), 0,1,1);

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
