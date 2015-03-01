/*
 * =====================================================================================
 *
 *       Filename:  xOrbits.cpp
 *
 *    Description:  conformal orbits
 *
 *        Version:  1.0
 *        Created:  12/03/2013 15:27:49
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
#include "vsr_knot.h"
#include "vsr_conic.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;
  

  float P, Q;
  bool bRealA, bRealB;
  float amt2,wt;
  float num;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100)(amt2,"amt2",-100,100)(num,"num",1,10000)(wt,"wt",-100,100);
      gui(bRealA)(bRealB);
      gui(P,"P", 0,100)(Q,"Q",0,100);

      P = 0; Q = 1;
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

      
      TorusKnot tk(P,Q);

      //tk.HF.vec() = Vec::x.rotate( Biv::xy * PI * amt );

      Draw( tk.HF.fiberA(),1,0,0 );
      Draw( tk.HF.fiberB() );

    
      Par pa, pb;

       Par ta = tk.HF.fiberA().dual();
       Par tb = tk.HF.fiberB().dual();


      if (bRealA) pa = Ro::real(ta);
      else pa = ta; 

      if (bRealB) pb = Ro::real(tb);
      else pb = tb; 


      Pnt p = PT(1,0,0);
      //Touch(interface, p);
      Draw(p,0,0,1);

      static Frame frame(-1,0,0);
      Touch(interface,frame);
      DrawAt( frame.y(), frame.pos() );
      Par tpar = frame.ty();

      Par par = tk.par();//pa * PI/P + pb * PI/Q;
     
      auto ppar = p ^ tpar;
      Draw( ppar,0,1,0 ); 

      for(int i=0;i<100;++i){
        Pnt tcp = Ro::pnt_cir( ppar, PI * (float)i/100);
        Draw ( Conic::Transform(tcp, Vec::x, -1+wt), 0,0,1 );
      }

      Pnt tp = p;
      tp[3] = wt;

      Cir tcir = tp ^ tpar;
      Draw(tcir);

      cout << Ro::size(tcir,false) << " " << Ro::size(ppar,false) << endl;

      for (int i = 0; i < tk.iter(); ++i){
        tp = tp.bst( tpar * amt2 );
        Draw( Ro::loc(tp),1,0,0);
      }

      Pnt tp2 = p;
      for (int i=0;i<num;++i){
        VT t = -PI + TWOPI*(float)i/num;
        tp2 =  p.bst( tpar * t );
        Draw (Ro::loc( tp2) );
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
