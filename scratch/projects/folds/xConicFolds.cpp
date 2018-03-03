/*
 * =====================================================================================
 *
 *       Filename:  xConicFolds.cpp
 *
 *    Description:  conic orbits
 *
 *        Version:  1.0
 *        Created:  07/17/2014 15:48:26
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

#include "vsr_cga3D_frame.h"
#include "vsr_field.h"
#include "vsr_conic.h"


using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,ecc, iter;

  bool bReset,bBst;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(ecc,"ecc",-10,10);
      gui(iter,"iter",1,1000);
      gui(bReset,"reset");
      gui(bBst,"bst");
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

      static Frame frame;
      static Field<Point> f(10,10,1);

      auto cir = frame.cxy();

      Touch(interface,frame);
      Draw(cir);

      for (int i = 0; i<100;++i){
        auto p = Ro::pnt_cir(cir, TWOPI*i/100.0);
        auto np = Conic::Transform(p, Vec::y, ecc);
        Draw(np,0,1,0);
      }

    
      auto bst = Gen::bst( cir.dual() * amt );

      for (int i=0; i<f.num(); ++i){
        for (int j=0;j<iter;++j){
          float t = (float)j/iter;
          auto p = Conic::ITransform( f[i], Vec::y, ecc);
          auto tcir = p ^ cir.dual();
          
          auto dll = (Ta::at(cir,p)^Inf(1)).dual().runit();
          auto mot = Gen::mot( Conic::Transform(dll,Vec::y,ecc)*amt);
          auto tp = Ro::loc( p.spin( bst ) );
          auto bp = Conic::Transform( tp, Vec::y, ecc);
          auto mp = Ro::loc( f[i].spin(mot) );
           
          f[i] = bBst ? bp : mp; 
          Draw(f[i], t,0,1-t);
        }
      }

      Draw(f);

      if (bReset) f.reset();

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
