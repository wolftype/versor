/*
 * =====================================================================================
 *
 *       Filename:  xTrajectory.cpp
 *
 *    Description:  elliptical, parabolic, hyperbolic paths by radius adjustment of particle
 *
 *        Version:  1.0
 *        Created:  02/10/2014 13:19:46
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
#include "vsr_cga3D_frame.h"
#include "vsr_knot.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt, P, Q, iter,bDraw2;

  vector<Pnt> pnt;

  bool bReset;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      
      gui(P,"P",0,10);
      gui(Q,"Q",0,10);
      gui(iter,"iter",0,1000);
      gui(bDraw2,"draw2");

      P = 3; Q = 2; iter = 100;

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

      static Frame f( PT(0,0,0) );
      Touch(interface, f);

      static Pnt sPnt = Ro::null(0,0,0);

      DrawAt( f.y(), f.pos(), 1,0,0 );
      Draw(f.cxz() );

      auto par = f.ipy();

      /* int num = iter; */
      /* for (int j = 1; j <= num; ++j){ */

       // VT col = (float)j/num; 
        auto cp = Ro::dls( amt,0,0,0 ).trs(-2,0,0);
        
        TorusKnot tk(P,Q);
        tk.HF.cir() = f.cxz().unit();

        TorusKnot tk2(P,Q);
        tk2.HF.cir() = f.cxz().unit();
        tk2.HF.vec() = Vec::x;
        Draw(tk2.HF.fiberA(),0,1,0 );
        Draw(tk2.HF.fiberB(),0,1,0 );

        Bst tbst  = tk2.bst() * tk.bst();

        pnt.clear();
        Pnt np = cp; 
        for (int i = 0; i < iter; ++ i ){
           np = np.sp( tbst ) ;  
           pnt.push_back( Ro::loc( np) );
        }   
        for (auto& i : pnt) Draw(i);

        sPnt = Ro::loc( sPnt.sp( tbst ) );
        Draw( Ro::dls(sPnt,.2),0,1,1 );

        /* tk.calc0( cp ); */
        /* for (auto i : tk.pnt ){ */
        /*     auto tp = Ro::loc(i); */
        /*     Draw( tp,col,0,1-col); */
        /*     if(bDraw2){ */
        /*       tk2.calc0(tp); */
        /*       for (auto h : tk2.pnt ){ */
        /*          auto tp2 = Ro::loc(h); */
        /*         Draw( tp2,col,0,1-col); */   
        /*       } */
        /*     } */
        /* } */
    //  }




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
