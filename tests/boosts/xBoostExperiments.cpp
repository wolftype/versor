/*
 * =====================================================================================
 *
 *       Filename:  xBoostExperiments.cpp
 *
 *    Description:  
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


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_knot.h"
#include "vsr_cga3D_frame.h"
#include "vsr_stat.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,amt2,P,Q,iter;

  bool bReset;

  vector<Pnt> pnt;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;

    Rand::Seed();

    init();
  }

  void initGui(){
      gui(amt,"amt",-100,100)(amt2,"amt2",-100,100);
      gui(P,"P",0,10);
      gui(Q,"Q",0,10);
      gui(iter,"iter",0,10000);
      gui(bReset,"reset");

      P=3;Q=2; amt = .01;

  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    Pnt random(){
      Vec v = Vec::x.sp( Gen::rot( (-1 + Rand::Num(2))*PI, -1 + Rand::Num(2) * PIOVERFOUR) ) * Rand::Num(.3,2);
      return Ro::null(v);
    }

    void init(){
      pnt = vector<Pnt>(10);
      reset();
    }

    void reset(){
      for (auto&i : pnt ) i = random();
    }

    virtual void onDraw(){ 
        
      getMouse();

      TorusKnot tk(P,Q);

      Par par = tk.par();
      Bst bst = Gen::bst( par * amt );

      for(auto& tp : pnt ){
         Pnt tmp = tp;
         tp = Ro::loc( tp.spin( bst ) );
         Draw(tp);  
          
         Par tpar = tmp ^ tp; 
         Pnt cpnt = Ro::pnt_cir( tpar.dual(), 0 );
         Mot mot = Gen::mot( ( tpar ^ Inf(1) ).dual().runit() * amt2 );          
         Pnt p = cpnt;//Ro::loc( cpnt.spin( mot * bst ) ) ;
         Draw(p,1,1,0);
         //tp = p;
         for (int i=0;i<iter;++i){
           VT t = (float)i/iter;
           p = Ro::loc( p.spin( bst * mot ) );
           Draw(p,0,1,0,1-t);
         }
      }

      if (bReset) { bReset = 0; reset(); }
    
  }
   

  
};


MyApp * app;


int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(500,500,"Versor",&glv);    
  app = new MyApp( win ); 
  app -> initGui();
  
  
  glv << *app;

  Application::run();

  return 0;

}
