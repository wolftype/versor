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

    
    auto pl = point ^ (Biv::xy * amt + Biv::xz * amt1) ^ Inf(1);

    Draw(pl);

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
