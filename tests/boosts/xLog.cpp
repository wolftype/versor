/*
 * =====================================================================================
 *
 *       Filename:  xLog.cpp
 *
 *    Description:  example use of dorst's logarithm with bivector splits
                    from "square roots" paper (dorst and valkenburg)
 *
 *        Version:  1.0
 *        Created:  03/18/2015 16:48:57
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "util/vsr_cga3D_app.h"   

using namespace vsr;
using namespace vsr::cga;


vector<Pair> resplit( const Pair& a, const Pair& b){
  return gen::split( a + b );
}



//how to interpolate between two split pairs...?
Con conTest2(const vector<Pair>& a, const vector<Pair>& b, float t){
  return gen::bst( a[0] * t + b[0]*(1-t) ) * gen::bst( a[1] * t + b[1]*(1-t) );
}

Con conTest(const vector<Pair>& a, const vector<Pair>& b, float t){
  auto ra = gen::split( (a[0]+a[1])*(1-t) + (b[0]+b[1])*t );
  return gen::con(ra, 1 );
}

struct MyApp : App {
 
  //Some Variables
  bool bReset, bDrawXf, bDrawPair, bDrawInterp = false;
  float amt, amtB, wt = 0;
  float bivA,bivB;

  bool bRealA, bRealB;
  Frame fa,fb;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset")(bDrawXf,"bDrawXf")(bDrawInterp,"bDrawInterp")(bDrawPair,"bDrawPair");
    gui(wt,"wt",-100,100);
    gui(amtB,"amtB",-100,100);
    gui(bRealA,"realA");
    gui(bRealB,"realB");
    gui(bivA, "bivA",-100,100)(bivB,"bivB",-100,100);
    
    fa.pos(-2,0,0);
    fb.pos(2,0,0);

    objectController.attach(&fa);
    objectController.attach(&fb);

    //defaults
    wt = 1; amt = 1;
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    auto base = CXZ(1);

    //Circles A and B
    auto ca = fa.cxz();// : fa.icxz();
    auto cb = fb.cxy();// : fb.icxy();

    //Transformation from circle a to circle b, and log    
    auto xf = gen::ratio( ca, cb ); 
    auto log = gen::log( xf );

 
//    //Transformations from base to circles 
//    auto xfA = gen::ratio( base, ca );
//    auto xfB = gen::ratio( base, cb );
//    
//    //Logarithm
//    auto logA = gen::log( xfA );
//    auto logB = gen::log( xfB );

    static vector<Point> pnt(20);
    if (bReset){
      for(int j=0;j<20;++j){
        pnt[j] = round::pnt_cir(ca,TWOPI*j/20);
      }
    }

    auto con = gen::con(log, amtB);
    for (auto& i : pnt ){
      i = round::loc( i.spin(con) );
      Draw(i,1,0,0);
    }

    for (int i=0;i<=20;++i){
      float t=(float)i/20 * amt;
      
      auto txf = gen::con( log, t );
//      auto txf2 = conTest( logA, logB, t);
//      if (bDrawInterp) Draw( base.spin(txf2), 0, 1, 0 );   //xf inter of base
//      else 
      Draw( ca.spin(txf), t, 1, 1-t);                      //xf of a circle

      if (bDrawPair) {
        auto pair = ca.spin(txf).dual();
        Draw ( cga::pointA(pair).null() , 1,t,1-t);
        Draw ( cga::pointB(pair).null() , 1,t,1-t);
      }
      //POINTS
      for(int j=0;j<20;++j){
        auto tpnt = round::pnt_cir(ca, .25 + TWOPI*j/20);
        tpnt = tpnt.spin(txf);                          //xf of a point
        Draw( round::loc( tpnt ),0,1,1);
      }
      
      //LINEAR (For comparison)
//      if (bReset) Draw( ca * (1-t) + (cb * t), 1,0,0);
    }
    
    Draw(ca);

//    if (bDrawXf) Draw( round::loc( ca.spin(xf) ) );
    Draw(cb);
   // else Draw(ca.spin( squareRoot ),0,1,0 );
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
