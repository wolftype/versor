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

#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;


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
  float bivA,bivB,exponent,offset;

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
    gui(exponent,"exponent",0,100);
    gui(offset,"offset",-100,100);
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
    auto cb = fb.cxz();// : fb.icxy();

    //line thrugh centers
    auto line = fa.pos() ^ fb.pos() ^ Infinity(1);
    

    //Transformation from circle a to circle b, and log    
    auto xf =  gen::ratio( ca, cb ); 
    auto log = gen::log( xf );
    auto con = gen::con(log, amtB);

    int num = 20;
    //starting positions
    static vector<Point> pnt(num);
    if (bReset){
      for(int j=0;j<num;++j){
        pnt[j] = cga::affine(fa.pos(), fb.pos(),(float)j/num);
      }
    }

    //points along orbit
    for (auto& i : pnt ){
      i = round::location( i.spin(con) );
   //   Draw(i,1,0,0);
    }



    for (int i=0;i<=num;++i){
      float t=(float)i/num * amt;
      
      auto txf = gen::con( log, t );
      auto nc = ca.spin(txf);
      auto pair = cga::point( nc, 0) ^ cga::point(nc,PI);

      Draw( nc, t, 1, 1-t);  
  

      float tneg = 2*(t-.5);  //-1 to 1
      float tmp = tneg;
      for (int i=0;i<exponent;++i){
        tneg *= tmp;
      }

      float ta = t * .5;
      
      
      auto pa =  round::location( cga::point(ca,PIOVERTWO).spin(txf) ); 
      auto tbst = gen::bst( pair * amtB * (tneg + ((tneg>0?1:-1) *offset) ) );
      auto tpa = round::location( pa.spin( tbst ) ) ;
   //   
      Draw(pa,1,0,0);

      //auto pb=  cga::point(cb, PI*tt); //< starting point    
      //auto tpa = round::location( pa.spin(txf) );
      //auto tpb = round::location( pb.spin(!txf) );
            
      Draw(tpa,0,1,0);     
      //Draw(line,0,1,0);   
    //  Draw(tpb,1,0,0);        

      //POINTS
//      for(int j=0;j<20;++j){
//        auto tpnt = round::pnt_cir(ca, .25 + TWOPI*j/20);
//        tpnt = tpnt.spin(txf);                          //xf of a point
//        Draw( round::loc( tpnt ),0,1,1);
//      }
      
      //LINEAR (For comparison)
//      if (bReset) Draw( ca * (1-t) + (cb * t), 1,0,0);


//      if (bDrawPair) {
//        auto pair = ca.spin(txf).dual();
//        Draw ( cga::pointA(pair).null() , 1,t,1-t);
//        Draw ( cga::pointB(pair).null() , 1,t,1-t);
//      }

    }
    
    Draw(ca);
    Draw(cb);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
