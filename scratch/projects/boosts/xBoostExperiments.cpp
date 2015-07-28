/*
 * =====================================================================================
 *
 *       Filename:  xBoostExperiments.cpp
 *
 *    Description:  a bunch of points twisting around a knot
 *
 *        Version:  1.0
 *        Created:  03/31/2014 18:59:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_knot.h"
#include "util/vsr_stat.h"

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,amt2,P,Q,iter;

  bool bReset;

  vector<Pnt> pnt;

  void setup(){
    
      Rand::Seed();

      bindGLV();
      gui(amt,"amt",-100,100)(amt2,"amt2",-100,100);
      gui(P,"P",0,10);
      gui(Q,"Q",0,10);
      gui(iter,"iter",0,10000);
      gui(bReset,"reset");

      P=3;Q=2; amt = .01;

      pnt = vector<Pnt>(10);
      reset();

  }
  

    Pnt random(){
      Vec v = Vec::x.sp( Gen::rot( (-1 + Rand::Num(2))*PI, -1 + Rand::Num(2) * PIOVERFOUR) ) * Rand::Num(.3,2);
      return Round::null(v);
    }

    void reset(){
      for (auto&i : pnt ) i = random();
    }

    virtual void onDraw(){ 
        
      mouse = calcMouse3D();
      TorusKnot tk(P,Q);

      //Generate Boost
      Par par = tk.par();
      Bst bst = Gen::bst( par * amt );

      //For each seeded point
      for(auto& tp : pnt ){
         
         //save temp
         Pnt tmp = tp;

         //Boost
         tp = Round::loc( tp.spin( bst ) );
         Draw(tp);  
         
         //make movement  
         Par tpar = tmp ^ tp; 
         Pnt cpnt = Round::pnt_cir( tpar.dual(), 0 );
         Mot mot = Gen::mot( ( tpar ^ Inf(1) ).dual().runit() * amt2 );          
         Pnt p = cpnt;//Round::loc( cpnt.spin( mot * bst ) ) ;
         Draw(p,1,1,0);
         //tp = p;
         for (int i=0;i<iter;++i){
           VSR_PRECISION t = (float)i/iter;
           p = Round::loc( p.spin( bst * mot ) );
           Draw(p,0,1,0,1-t);
         }
      }

      if (bReset) { bReset = 0; reset(); }
    
  }
   

};




int main(){
                             
  MyApp app;
  app.start();
  
  return 0;

}
