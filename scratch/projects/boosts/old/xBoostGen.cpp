/*
 * =====================================================================================
 *
 *       Filename:  xBoostGen.cpp
 *
 *    Description:  test of implementation of wareham, cameron, and lasenby's algorithm for move-rotate-scale
 *
 *        Version:  1.0
 *        Created:  12/02/2013 10:40:46
 *       Revision:  none
 *       Compiler:  gcc4.7 or clang3.2 or higher
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_twist.h"

using namespace vsr;
using namespace vsr::cga;


typedef decltype( Bst() * Mot() ) Mtt; 

/*! Generate General Exponential from Point Pair*/
Mtt mtt( const Par& p){
   Biv thetaP = p; // Bivector Part
   VT theta = thetaP.norm();
   Biv P = ( theta != 0 ) ? thetaP / theta : thetaP; // Normalized BIV (what if theta == 0? )

   Drv tn = p; // Direction Vector Part
   Vec vd = tn.copy<Vec>();
   VT t = vd.norm(); //Drv weight
   Drv n = (t != 0 ) ? tn / t : tn;  //Normalized DRV

   Mnk N = p; // EPlane part
   VT w = N[0]; //Eplane value

   VT ct = cos(theta);
   VT st = sin(theta);
   VT chw = cosh(w);
   VT shw = sinh(w);
   VT shcw = Math::sinhc(w);

   VT wt = w * w + theta * theta;
   VT stchw = st*chw;
   VT ctshw = ct*shw;
   

   Vec tpara = Op::pj( vd, P );
   Vec tperp = Op::rj( vd, P );

   Rot r = (P * st) + ct;
   auto d = ( Mnk(shw) + tperp.copy<Drv>() * shcw ) + chw;

   Biv bp = P * ( (-w*stchw + theta * ctshw ) / wt ) ;
   Drv dp = tpara.copy<Drv>() * ( ( w*ctshw + theta * stchw ) / wt ) ;


   auto rd = r * d + bp + dp;

   // cout << thetaP.norm() << endl;
  //  r.print();

   //rd.print();    
   
   return rd; 

}

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt, period,pitch, mnk;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100)(period,"period",-TWOPI,TWOPI)(pitch,"pitch",-TWOPI,TWOPI)(mnk,"mnk",-100,100);

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

      static Frame frame(2,0,0);
      Par par = Twist::Along( frame.dly(), period, pitch) + Tnv(0,amt,0) + Mnk(mnk);// frame.pos() ^ mouse;

      par.print();

      mtt(par).print();

      Draw(frame); Draw(mouse);
      Touch(interface,frame);

      static Cir cxy = CXY(1).trs(-2,0,0);
      Touch(interface,cxy);

      for (int i = 0; i < 100; ++i){
        float t = (float)i/100;
        Mtt m = mtt(par * t);
        Draw( cxy.spin( m ), t,0,1-t );
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
