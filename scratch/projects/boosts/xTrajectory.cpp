/*
 * =====================================================================================
 *
 *       Filename:  xTrajectory.cpp
 *
 *    Description:  elliptical, parabolic, hyperbolic paths by radius or weight adjustment of particle
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


#include "vsr_app.h"   
#include "form/vsr_knot.h"

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float radius,weight, P, Q, iter,bDraw2;

  vector<Pnt> rpnts;
  vector<Pnt> wpnts;

  bool bReset;

  Frame f;
  
  Pnt sPnt = Round::null(0,0,0);
  Pnt rPnt; 
  Pnt wPnt = Round::null(0,0,0).trs(-2,0,0); 
  

  void setup(){
      bindGLV();
      gui(radius,"radius",-100,100);
      gui(weight,"weight",-100,100);
      
      gui(P,"P",0,10);
      gui(Q,"Q",0,10);
      gui(iter,"iter",0,1000);
      gui(bDraw2,"draw2");

      P = 3; Q = 2; iter = 100;

      gui(bReset,"reset");

      objectController.attach(&f);
  }

    virtual void onDraw(){ 
        
      mouse = calcMouse3D();

      time +=1;

      //Knot A
      TorusKnot tk(P,Q);
      tk.HF.cir() = f.cxz().unit();
      
      //Knot B (orthogonal to it)
      TorusKnot tk2(P,Q);
      tk2.HF.cir() = f.cxz().unit();
      tk2.HF.vec() = Vec::x;

      //Generate a Compound Boost
      Bst tbst  = tk.bst();//tk2.bst() * tk.bst();


      //Point set by radius
      rPnt = Round::dls( radius,0,0,0 ).trs(-2,0,0);
      //Point set by weight
      wPnt[3] = weight;
     
      
      //Set Pnt positions  
      rpnts.clear();
      wpnts.clear();
      Pnt rp = rPnt;
      Pnt wp = wPnt; 
      for (int i = 0; i < iter; ++ i ){
         rp = rp.sp(tbst);  
         wp = wp.sp(tbst);  
         rpnts.push_back( Round::loc(rp) );
         wpnts.push_back( Round::loc(wp) );
      }   

      //increment orbit
      sPnt = Round::loc( sPnt.sp( tbst ) );

      //Frame
      DrawAt( f.y(), f.pos(), 1,0,0 );
      Draw(f.cxz() );     
      //Fibers
    //  Draw(tk2.HF.fiberA(),0,1,0 );
    //  Draw(tk2.HF.fiberB(),0,1,0 );     
      //Orbit  
   //   Draw(Round::dls(sPnt,.2),0,1,1 );
      //Orbits
      for (auto& i : rpnts) Draw(i,1,0,0); //Draw radius-based points in red
      for (auto& i : wpnts) Draw(i,1,1,0); //Draw weight-based points in yellow
      Draw( sphere( wpnts[ (int) time % wpnts.size() ], .2) , 1, 0, 1);


  }
   

  
};



int main(){
  
  MyApp app;
  app.start();

  return 0;

}
