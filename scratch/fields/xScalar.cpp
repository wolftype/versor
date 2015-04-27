/*
 * =====================================================================================
 *
 *       Filename:  xScalar.cpp
 *
 *    Description:  scalar gradient 
 *
 *        Version:  1.0
 *        Created:  08/01/2014 15:36:59
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
        
      getMouse();

      Field<Sca> f(10,10,1);
      for (int i=0;i<f.num();++i){
        f[i] = (float)i/f.num();
      }      
      Pnt pnt = Ro::null(0,0,0);

      for (int i=0;i<f.num();++i){
        DrawAt( pnt, f.grid(i), f[i][0],0,1-f[i][0]);
      }

 
      for (int i=0;i<f.w();++i){
        for (int j=0;j<f.h();++j){
            auto a = f.at(i,j);
            auto b = f.at(i+1,j);
            auto c = f.at(i,j+1);

        }
      }     

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
