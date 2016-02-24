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
#include <vsr/form/vsr_group.h>

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
    ps.bShadedOutput = false;

    amt = -PI;
    amt1 = 3;
    amt2 = 2;
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



//      //0. a and c are at 90 degrees, must find b...
//      auto a = Vec::x;
//      auto c = Vec::z;
//      
//      //1. employ reduced version of good old spherical trig cosine rule ...
//      double tp = PI/(int)amt1;
//      double tq = PI/(int)amt2;
//
//      double ca = cos(tq);
//      double sa = sin(tq);
//      double cc = cos(tp);
//      double sc = sin(tp);
//
//      //reduced (because tb is contrained to PIOVERTWO)
//      double tA = acos( ca/sc );
//      double tC = acos( cc/sa );
//
//      //2. ... to rotate the yx plane ...
//      auto bivA = (a ^ c).rot( a.unduale() * -tA / 2.0 );//changed
//      auto bivC = (a ^ c).rot( c.unduale() * tC / 2.0 );
//
//
//      auto b = (bivA.duale() ^ bivC.duale()).duale().unit(); //note neg!

//
//       auto pair = Construct::point(0,-2,0) ^ Construct::point(0,2,0);
//
//      // Draw(pair.undual(),0,1,0);
//
//       auto circle = CXZ(1);
//
//       auto pair2 = circle.dual();
//
//       Draw( Construct::pointA(pair2),0,1,0);
//       Draw( Construct::pointB(pair2),1,0,0);
//
//       Draw(circle,1,0,0);

         Biv b(.2,3,.4);

         auto v = b.duale() * 2;

         auto vb = v*b;
         auto bv = b*v;

         cout << vb << endl;
         cout << bv << endl;

       
//

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
