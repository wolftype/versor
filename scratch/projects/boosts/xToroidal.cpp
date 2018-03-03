/*
 * =====================================================================================
 *
 *       Filename:  xToroidal.cpp
 *
 *    Description:  simple anapole
 *
 *        Version:  1.0
 *        Created:  06/19/2015 18:47:56
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


//goal of function is to take a point to another point along a circle with tangent t
Point pnorm(const Pnt& p, const Pnt& rel){
  cout << (hnorm(p)) << endl;
  
  return -(p/ (rel<=p));

  
}

double pdist(const Pnt& pa, const Pnt& pb, const Pnt& rel){

  return acosh( 1-(pnorm(pa,rel)<=pnorm(pb,rel))[0]);
}

Pair pgen(const Pnt& pa, const Pnt& pb, const Pnt& rel, VSR_PRECISION amt){
   double dist = pdist(pa,pb,rel);         //<-- p distance
   auto pline = pa ^ pb ^ rel;             //<-- p line (circle)
   auto par_versor = (rel <= pline).runit();      //<-- p trans generator (pair)
   return par_versor * dist * amt * .5;    //<-- and ready to be applied
}

//a null pair pa to pb along circle c
Point pspin(const Point& pa, const Point& pb, const Dls& rel, double amt){
  //auto rel = Round::surround( PAO ^ c.dual() ); //sphere
  return Round::location(pa.boost( pgen(pa, pb, rel, amt) ));
}

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt,theta1,theta2 = 0;


  Frame fa;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(theta1,"theta1",-100,100)(theta2,"theta2",-100,100)(bReset,"bReset");

    objectController.attach(&fa);
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    auto tcir = Round::produce( fa.ibound(), Biv::xz);

    auto cir = point(-3,0,0) ^ tcir.dual();

    auto pnt1 = point(cir,theta1);
    auto pnt2 = point(cir,theta2);
    
    auto trans = Round::loc( pnt2.boost( tcir.dual() * acosh(amt) ) );

    auto trans2 = pspin( pnt2, pnt1, fa.ibound(), amt );

    Draw (tcir);
    Draw (cir,1,0,0);
    Draw (pnt1);
    Draw (pnt2,0,1,0);
    Draw (trans2,1,1,0);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
