/*
 * =====================================================================================
 *
 *       Filename:  xFieldtest.cpp
 *
 *    Description:  sanity checks
 *
 *        Version:  1.0
 *        Created:  02/19/2014 14:25:55
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
#include "vsr_field.h"

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
      time +=.01;
      auto pnt = Ro::point( sin(time)*5, cos(time) * 5, 0);
    
      static Field<Vec> vf(20,10,1);
      //static Field<Vec> ortho(20,10,1);

      Par par;
      for (int i = 0; i < vf.num(); ++i){
        float dist = Ro::sqd( vf.grid(i), pnt ) + .1;
        par = Par(Tnv(0,1,0)).trs(pnt) * 1./dist;
        Vec nv = Ro::loc( vf.grid(i).bst(par*amt) ) - vf.grid(i);
        vf[i] = nv;
      }

      Draw(pnt);

      Draw(vf);
      //Draw(ortho);

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
