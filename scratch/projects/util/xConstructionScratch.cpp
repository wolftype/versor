  /*
 * =====================================================================================
 *
 *       Filename:  xConstructions.cpp
 *
 *    Description:  scratch pad for constructions
 *
 *        Version:  1.0
 *        Created:  07/20/2015 19:13:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"  
#include <vsr/form/vsr_twist.h> 
#include "form/vsr_rigid.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset,bFlip = false;

  Point point;
  bool bSetMouse=true;

   Frame f;


  float amt,amt1,amt2,radius,iter;


  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(bReset)(bFlip);
    gui(amt,"amt",-10,10);
    gui(amt1,"amt1",-1000,1000);
    gui(amt2,"amt2",-1000,1000);
    gui(iter,"iter",-1000,1000);
    gui(radius,"radius",-10,1);
    
    objectController.attach(&f);
    bShadedOutput = false;

    amt = -PI;
    amt1 = 1;
    amt2 = 1;
    iter=100;

    
  }


  void onKeyDown(const gfx::Keyboard& k){
    
    switch (k.code){
      case 's':
        bSetMouse = !bSetMouse;
    }

    App::onKeyDown(k);
   }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    point = calcMouse3D();
//      auto sa = Construct::sphere(-3,0,0,1); 
//      auto sb = Construct::sphere(2,0,0,.2);
//      Draw(sa);
//      //Draw(sb);
//      
//      auto ratio = Gen::ratio(sa,sb, bReset);//.runit();
//      Pair log = Gen::log(ratio, bFlip);
//    //  Draw(sa.spin(ratio) );
//     // auto log = Gen::log( (sb/sa).runit(), bReset) * -.5;
//      
//      for (int i=0;i<=10;++i){
//        float t= (float)i/10;
//        //Pair log = Pair(Tnv(0,1,0)) * t;
//        auto bst = Gen::bst(log*-t); 
//        Draw( sa.spin(bst),t,0,1-t,.2);
//      }

        auto c = f.cxy();
        Vec pj = Op::pj( Vec(Round::location(c) - point), Round::carrier(c) ).unit() ;

        Draw (Constrain::PointToCircle(point, c), 1,0,0 );

        Draw( Construct::point( Round::surround(c), -pj ), 0, 1,0);

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
