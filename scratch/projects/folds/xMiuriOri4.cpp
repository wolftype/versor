/*
 * =====================================================================================
 *
 *       Filename:  xMiuriOri4.cpp
 *
 *    Description:  miuri ori pattern
 *
 *        Version:  1.0
 *        Created:  06/30/2014 18:26:47
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
#include "vsr_fold.h"
#include "vsr_field.h"

using namespace vsr;
using namespace vsr::cga;



struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,theta;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100)(theta,"theta",-TWOPI,TWOPI);
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
      
      //grid
      int w = 3; int h =10;
      Field<Pnt> f(w,h,1);
      for(int i=0;i<w;i++){
        for(int j=0;j<h;j+=2){
          int idx = f.idx(i,j);
          f[idx] = (f[idx] + Vec(amt,0,0)).null();
        }
      }

      //constraints
      vector<Truss> truss;
      for (int i=0;i<w-2;i+=2){
        for (int j=0;j<h-1;++j){
          
          auto tl = f.at(i,j+1);
          auto tm = f.at(i+1,j+1);
          auto tr = f.at(i+2,j+1);

          auto bl = f.at(i,j); 
          auto bm = f.at(i+1,j);
          auto br = f.at(i+2,j);

          Truss tv(tl, tm, tr,
                   bl, bm, br, false);

          truss.push_back(tv);
        }
      }

     Rigid2 rigid(f.at(2,0), f.at(1,0), f.at(1,1), true);
     auto np = rigid.orbit(theta);
     truss[0].update( f.at(0,0), f.at(1,0), np );

      for (int i=1;i<truss.size();++i){
        truss[i].update(truss[i-1]);
        Draw(truss[i].p.tl);
        Draw(truss[i].p.tm);
        Draw(truss[i].p.tr);
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
