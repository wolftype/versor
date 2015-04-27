/*
 * =====================================================================================
 *
 *       Filename:  xWatt.cpp
 *
 *    Description:  watt linkage with c++14
 *
 *        Version:  1.0
 *        Created:  10/07/2014 12:13:59
 *       Revision:  none
 *       Compiler:  clang 3.6 or above
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *
 * =====================================================================================
 */



#include "vsr_app.h"
//#include "vsr_functional.h"

using namespace vsr;
using namespace vsr::cga3D;

auto samples = [](int num, float start){
  return concat( vsr::list(start), num==0 ? vsr::list() : samples(num-1,start+.1) ); 
};

auto pnt = [](Circle c){

  return vsr::list();

};



struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  bool bReset;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(bReset,"reset");
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 


      static Frame fa(-1,0,0);
      static Frame fb(1,0,0);

      Circle ca = fa.cxy();
      Circle cb = fb.cxy();
      
     auto c = samples(10,0.0);
     //:c( fmap(prnt) );

        
      getMouse();
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
