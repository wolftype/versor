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
#include "form/vsr_cyclide.h"
#include "draw/vsr_cyclide_draw.h"

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {

  //Some Variables
  bool bA,bB, bC,bD,bE,bF = false;//bReset2,bFlip,bFlip2= false;

  Point point;
  bool bSetMouse=true;

   Frame f;


  float amt,amt1,amt2,radius,iter;

  Frame fa = Frame(-2,0,0);
  Frame fb = Frame(2,0,0);

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(bA)(bB)(bC)(bD)(bE)(bF);
    gui(amt,"amt",-10,10);
    gui(amt1,"amt1",-1000,1000);
    gui(amt2,"amt2",-1000,1000);
    gui(iter,"iter",-1000,1000);
    gui(radius,"radius",-10,1);

    objectController.attach(&fa);
    objectController.attach(&fb);
    ps.bShadedOutput = false;

    amt = 0;
    amt1 = 0;
    amt2 = 0;
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

    Circle circle = fa.cxz();
    auto ori = Construct::point( circle, amt );

 //   /// Theta in radians. @todo, does not consider quadrant!
 //   VSR_PRECISION theta( const Circle& circle, const Point& point){
     /// Tangent Element on circle at point
//     auto t = Tangent::at(circle, point);
//     /// Direction of point relative to circle center
//     auto dir = Vec( Round::location( t ) - Round::location(circle) ).unit();
//     /// Spin e1 by rotor which takes e3 to normal
//     auto rot = Gen::ratio( Vec::z, Round::carrier(circle).dual().unit());
//     auto tx = Vec::x.spin( rot );
//     /// Theta that takes tx to to dir
//     auto phi = atan2( (dir^tx).rnorm() * Op::sign(dir^tx, Biv::xy.spin(rot) ? 1 : -1, (dir<=tx)[0] );
 //   }
    


    Draw(fa.bound());
    cout << (point <= fa.bound()) << endl;
    

  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
