/*
 * =====================================================================================
 *
 *       Filename:  xCircleBlending.cpp
 *
 *    Description:  blending of circles after doran (linear)
 *
 *        Version:  1.0
 *        Created:  04/18/2015 17:35:03
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
  float amt = 0;


  Circle ca = Construct::circle(-3,0,0);
  Circle cb = Construct::circle(3,0,0);

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
    
    auto tca = ca.twist( cga::line(1,0,0).dual() * amt );
    auto tcb = cb.twist( cga::line(1,0,0).dual() * -amt );

    draw(tca); 
    draw(tcb); 
  
    
    //normalized
   // tca = tca.unit();
   // tcb = tcb.unit();


    float theta = ( (tca<=tcb)[0] );
    cout << theta << endl;

    for (int i=0;i<100;++i){
      float t = (float)i/100;
      //auto ncir = ( tca * sin((1-t)*theta) + tcb * sin(t*theta) )  / sin(theta);
      auto ncir = ( (tca * (1-t)) + (tcb * t) );
      draw(ncir,t,0,1-t);
    }
    
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
