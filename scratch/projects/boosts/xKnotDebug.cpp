/*
 * =====================================================================================
 *
 *       Filename:  xKnotDebug.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/19/2015 13:46:01
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

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;

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
    gui(bReset);
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

    
    if (bSetMouse) point = calcMouse3D();


    auto cir = Round::produce( Construct::sphere(0,0,0,-radius), Biv::xz );//CXZ(1);
    auto dll = (Inf(1) <= cir).runit();

    Draw(cir,.6,1,.3);
    Draw(dll,.2,.2,.2);  //  gl2psLineWidth(1);

    gl2psPointSize(10);
    Draw(point,.6,.1,.1);
    Point p = point;

    vector<Cir> cirs;
    vector<Pnt> pnts;

    float mult = (amt1*amt2) / (amt*Round::size(cir,false) );
    for (int i =1;i <(int)iter;++i){
       float t = PI * amt / iter;
       auto par = (cir.dual().runit() * t * amt2) + (dll*t * amt1);
       p = Round::loc( p.spin( Gen::bst(par) ) );// / (!FERROR(rad)?rad:1) ) );
       Draw( p,.6*(1-t),.1,.1 );

    }

//  for (int i =0;i <(int)iter;++i){
//     float t = (float)i/iter * PI *amt;// * (amt1*amt2/iter);
//     auto parA = (cir.dual() * t * amt2);
//     auto parB = (dll*t * amt1);
//     p = ( point.spin( Gen::bst(parB) * Gen::bst(parA) ) );// / (!FERROR(rad)?rad:1) ) );
//     p = p/p[3];
//     Draw( p,0,1,0 );
//  }


    if (bReset) {
      DrawB(cir.dual(),0,.3,.8,.3);
      if (Round::size(cir)<0) Draw( Construct::sphere(0,0,0,-radius),0,3,.8,.3);
    }

   // auto tmp = Round::axis(cir);
   // cir.print();

//
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
