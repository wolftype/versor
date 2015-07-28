/*
 * =====================================================================================
 *
 *       Filename:  xConformalVersusTwist.cpp
 *
 *    Description:  attempt to show benefits of conformal vs twist based tranforms
 *
 *        Version:  1.0
 *        Created:  06/26/2015 14:37:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */




#include "vsr_app.h"   
#include "form/vsr_twist.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt,theta = 0;

  bool bDrawMotor, bDrawBoost;

  Frame f;
  Field<Pair> p;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(theta,"theta",-TWOPI, TWOPI)(amt,"amt",-100,100)(bReset,"bReset");
    gui(bDrawMotor,"bDrawMotor")(bDrawBoost,"bDrawBoost");
   
   
    objectController.attach(&f); 

    p.resize(10,1,10,1);
    for (int i =0;i<p.num();++i){
        p[i] = Pair(Tnv(0,1,0)).translate(p.grid(i));
    }

    bDrawMotor = bDrawBoost = 0;

    f.pos(0,1,0);
  }



  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){


     

    for (int i = 0;i < p.num();++i ){
      
      auto dist = 1.0 / (.01 + Round::dist(p.grid(i),f.pos()));

     auto motor = Gen::mot( Twist::Along( f.dlz(), theta, amt) * dist );
     auto boost = Gen::boost( f.tz() * amt * dist );

      auto pa = Round::loc( p.grid(i).spin( motor ));
      auto pb = Round::loc ( p.grid(i).spin( boost ));

      auto ta = p[i].spin(motor);
      auto tb = p[i].spin(boost);

      if (bDrawMotor) DrawAt( -Round::dir(ta), pa );
      if (bDrawBoost) DrawAt( -Round::dir(tb), pb );
    }

    Draw(f); 
    Draw(p,0,1,0);

  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
