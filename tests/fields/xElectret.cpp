/*
 * =====================================================================================
 *
 *       Filename:  xElectret.cpp
 *
 *    Description:  electric sources and sinks modelled as weighted flat points
 *
 *        Version:  1.0
 *        Created:  12/03/2013 11:45:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga3D;



struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      getMouse();
    
    //ELECTRET FIELD
    static Field<Flp> f(5,5,5,1);

    bool bPolarity = false;
    for (int i = 0; i < f.num(); ++i){
      bPolarity = !bPolarity;
      Touch(interface, f.grid(i) );
      f[i] = f.grid(i) ^ Inf(1);
      if (bPolarity) {
        f[i] *= -1;
        Draw(f[i],0,1,0);
      } else Draw(f[i], 1,0,0);
    }

    //FIELD THAT IT ACTS ON
    Field<Vec> v(20,20,20,.2);

    for (int i = 0; i < v.num(); ++i){

      Flp flp;
      //Sum Distances
      for (int j = 0; j < f.num(); ++j){
        VT wt = 1.0 / (.1 + Ro::sqd(v.grid(i), f.grid(j)) );
        flp += f[j] * wt;
      }
      auto tp = v.grid(i).spin( Gen::bst(flp * amt) );
      v[i] = tp / tp[3] - v.grid(i); 
    }

    DrawB(v);

    //EQUIPOTENTIALS
    
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
