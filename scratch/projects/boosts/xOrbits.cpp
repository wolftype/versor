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



#include "vsr_app.h"   
#include "form/vsr_knot.h"
#include "form/vsr_conic.h"

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;
  

  float P, Q;
  bool bRealA, bRealB;
  float amt2,wt;
  float num;

  Frame frame;

  void setup(){
      bindGLV();
      gui(amt,"amt",-100,100)(amt2,"amt2",-100,100)(num,"num",1,10000)(wt,"wt",-100,100);
      gui(bRealA)(bRealB);
      gui(P,"P", 0,100)(Q,"Q",0,100);

      P = 0; Q = 1;

      frame.pos(-1,0,0);
      objectController.attach(&frame);
  }
  

    virtual void onDraw(){ 
        
      mouse = calcMouse3D();
      TorusKnot tk(P,Q);

      Draw( tk.HF.fiberA(),1,0,0 );
      Draw( tk.HF.fiberB() );

    
      Par pa, pb;

      Par ta = tk.HF.fiberA().dual();
      Par tb = tk.HF.fiberB().dual();


      if (bRealA) pa = Round::real(ta);
      else pa = ta; 

      if (bRealB) pb = Round::real(tb);
      else pb = tb; 


      Pnt p = PT(1,0,0);
      Draw(p,0,0,1);

      DrawAt( frame.y(), frame.pos() );
      Par tpar = frame.ty();

      Par par = tk.par();
     
      auto ppar = p ^ tpar;
      Draw( ppar,0,1,0 ); 

      for(int i=0;i<100;++i){
        Pnt tcp = Round::pnt_cir( ppar, PI * (float)i/100);
        Draw ( Conic::Transform(tcp, Vec::x, -1+wt), 0,0,1 );
      }

      Pnt tp = p;
      tp[3] = wt;

      Cir tcir = tp ^ tpar;
      Draw(tcir);

      cout << Round::size(tcir,false) << " " << Round::size(ppar,false) << endl;

      for (int i = 0; i < tk.iter(); ++i){
        tp = tp.bst( tpar * amt2 );
        Draw( Round::loc(tp),1,0,0);
      }

      Pnt tp2 = p;
      for (int i=0;i<num;++i){
        VT t = -PI + TWOPI*(float)i/num;
        tp2 =  p.bst( tpar * t );
        Draw (Round::loc( tp2) );
      }

    
  }
   

  
};



int main(){
     
  MyApp app;
  app.start();                        
  return 0;

}
