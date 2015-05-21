/*
 * =====================================================================================
 *
 *       Filename:  xConicInversion.cpp
 *
 *    Description:  conic inversion test
 *
 *        Version:  1.0
 *        Created:  02/10/2014 13:04:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_conic.h"
#include "vsr_frame.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt, beta;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(beta,"beta",-100,100);
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

      auto cir = CXY(1).dilate( beta );

      static auto cir2 = CXY(1);
      Touch(interface, cir2);
      Draw(cir2);


      Draw(cir);

      static Frame f;
      Touch(interface,f);
      DrawAt( f.y(), f.pos(), 0,1,0);
      auto ty = f.ty();

      DualLine dll = DLN(1,.4,0).trs(0,.2,0).runit();
      Pair par = ( dll ^ Ro::sur(cir) ).dual();
      Draw(par,0,0,1);

      auto idll = Conic::Transform( dll, Vec(mouse).unit(), amt ).dual();
      Draw(idll,0,1,1);
      auto ridll = Cir(idll).reflect( cir2 );
      Draw(ridll,0,1,1);

      auto pp = Ro::split(par);
      auto p0 = Conic::Transform( pp[0], Vec(mouse).unit() , amt );
      auto p1 = Conic::Transform( pp[1], Vec(mouse).unit() , amt );

      Draw(p0,1,1,0);
      Draw(p1,1,1,0);

      auto ip0 = Ro::loc( p0.reflect(cir2) );
      auto ip1 = Ro::loc( p1.reflect(cir2) );

      Draw( Ro::dls_pnt( ip0, .2 ),0,1,0 );
      Draw( Ro::dls_pnt( ip1, .2 ),0,1,0 );


      for (int i = 0; i < 100; ++i){
        auto p = Ro::pnt_cir( cir, PI * i/100);
        auto tp = Conic::Transform( p, Vec(mouse).unit() , amt );

        Draw( tp );

        auto itp = Ro::loc( tp.reflect(cir2) );
        Draw(itp,1,0,0);

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
