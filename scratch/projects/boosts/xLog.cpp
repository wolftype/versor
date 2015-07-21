/*
 * =====================================================================================
 *
 *       Filename:  xLog.cpp
 *
 *    Description:  example use of dorst's logarithm with bivector splits
                    from "square roots" paper (dorst and valkenburg)

                    The conformal transformation between two (orthogonal) Circles
                    is split using exterior derivative dx ^ RxR 
                    into two orthogonal independent rotors
                    around which we wind.

                    the surface carved out by taking circle a to circle b
                    represents the lattice of area 1 -- one page in the book
                    of all harmonic relations 
                    
                    seg gets from one circle to another at 1 for every four periods
                    "                      "  back to itself at 2 for every two periods
                    "                      "  through itself and back to other circle
                    "                      " loops at 4
                    
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
#include "form/vsr_csg.h"   

using namespace vsr;
using namespace vsr::cga;

namespace vsr{ namespace gen{

Boost con_( const vector<Pair>& log, float amtA){
    Par par = log[0] * amtA;
    if (log.size() > 1){
      par += log[1] * amtA;
    }
    return gen::bst(par);
}


Boost con_( const vector<Pair>& log, float amtA, float amtB){
    Par par = log[0] * amtA;
    if (log.size() > 1){
      par += log[1] * amtB;
    }
    return gen::bst(par);
}

} }

struct MyApp : App {
 
  //Some Variables
  bool bReset, bDrawXf, bDrawPair, bDrawInterp,bDrawCoordinateGrid = false;
  float amt, P, Q, P2,Q2,wt,seg,seg2 = 0;
  float bivA,bivB,theta,phi,hopftheta,twistAmt,offset,iter;

  bool bRealA, bRealB, bTangentA, bTangentB, bAxis, bPoint;
  float numStart;
  Frame fa,fb;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100);

    gui(P,"P",-100,100);
    gui(Q,"Q",-100,100);

    gui(P2,"P2",-100,100);
    gui(Q2,"Q2",-100,100);
    
    gui(bReset,"bReset")(bDrawXf,"bDrawXf")(bDrawInterp,"bDrawInterp")(bDrawPair,"bDrawPair");
    gui(wt,"wt",-100,100);
   
    gui(seg,"seg",-100,100);           //<-- number of segments along "template"
    gui(seg2,"seg2",-100,100);           //<-- number of segments along "template"
    gui(theta,"theta",0,TWOPI);        //<-- theta position of starting orbit around circle ca 
    gui(phi,"phi",0,TWOPI);            //<-- phi spread of positions on 
    gui(hopftheta,"hopftheta",0,TWOPI);            //<-- phi spread of positions on 
    gui(twistAmt,"twistAmt",0,TWOPI);  //<-- twist around axis
    gui(numStart,"numStart",0,1000);   //<-- number of points around circle generator
    gui(iter,"iter",0,1000);           //<-- number of circles
  
    gui(bDrawCoordinateGrid,"bDrawCoordinateGrid");
    gui(bRealA,"realA");
    gui(bRealB,"realB");
    gui(bTangentA, "tangentA");
    gui(bTangentB, "tangentB");
    gui(bAxis, "bAxis")(bPoint, "bPoint");;
    gui(bivA, "bivA",-100,100)(bivB,"bivB",-100,100);

    
    fa.pos(-2,0,0);
    fa.rot() = gen::rot(0,PIOVERFOUR/2.0);
    
    //fb.pos() = fa.pos().reflect( Dlp(1,0,0) );
    fb.mot( fa.mot().reflect( Dlp(1,0,0) ) );
    //fb.pos(2,0,0);

    objectController.attach(&fa);
    objectController.attach(&fb);

    bRealA = bRealB = true;
    //defaults
    wt = 1; seg=4; amt = .01;

    P=3; Q=2;
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    //Circles A and B On Frames
    auto ca = bTangentA ? fa.tz().dual() : (bRealA ? fa.cxy() : fa.icxy());
    auto cb = bTangentB ? fb.tz().dual() : (bRealB ? fb.cxy() : fb.icxy());

    //tangents (null point pairs) in y direction
    auto ta = fa.ty();
    auto tb = fb.ty();

    //Orthogonal circle through frame fb
    auto caorth = fb.pos() ^ ca.dual();
  
    //use axis of circle a, orthogonal circle to it, or frame circle cb itself 
    auto cc = bAxis ? Cir(fa.lz().unit()) : bPoint ? caorth : cb; 

    //Transformation from circle a to circle b, and log
    //.................................flip?  theta  
    auto fullxf =  gen::ratio( ca, cc, false, hopftheta); 

    auto log = gen::log( fullxf );

    //conformal motion (small differential)
    float amtP = (P==0) ? 0 : seg*amt/P;
    float amtQ = (Q==0) ? 0 : seg*amt/Q;

    auto conf =  gen::con(log, amtP, amtQ);

    //Just the Split (for drawing purposes)
    auto split = gen::split( fullxf );
    
    //numStart Points around a circle starting at theta with spread of phi,
    auto pnt = points(ca, (int)numStart, theta, phi);

    for (auto& i : pnt){
      i = i.twist( (round::par_cir(ca,0)^Inf(1)).dual() * twistAmt );
    }
 
    //common product
    float harmonic = ((P==0) || (Q==0)) ? 1 : P*Q;  
    float harmonic2 = ((P2==0) || (Q2==0)) ? 1 : P2*Q2;  

    int numIter = harmonic / amt;
 

    //Relative, recursive, Transformations (dconf)
    for (int j=0;j<numStart;++j){
     float t = (float)(j/numStart);
     auto start = pnt[j];
     for (int i=0;i<numIter;++i){
      
       vector<Circle> coord;

       start = start.spin(conf);
       auto tp = round::loc(start);
       Draw(tp , t, 1, 1-t);

       float titer =0;
        if (bDrawCoordinateGrid){
           for (auto& s : split){
            auto cir = tp^s;
            coord.push_back(cir);
            Draw(cir,t,titer,1-t);
            Draw( tangent::at(cir,tp),t,1-titer,1-t);
            titer+=1;
           }

           for (int k=0;k<iter;++k){

              float tk = seg2 * harmonic2 * (float)k/iter;
              auto conf2 = gen::con( coord[0].dual(), split[1].dual(), tk/P2, tk/Q2 );
              Draw( round::location( tp.spin(conf2) ), 0, 1, 0 );

           }
           
         }
     }



    }

 
   //Absolute
   for (int i=0;i<=(int)iter;++i){
     
       float t = (float)i/iter ;

       //Absolute Circles      
       auto fxf = gen::con( log, t);
       auto nc = ca.spin(fxf);
     //  Draw(nc);


       //Absolute points along orbit
   //    float tamt = seg * harmonic * t;
   //    auto txf = gen::con( log, tamt/P, tamt/Q );

    //  for (auto& j : pnt ){
    //     auto tmp = round::location( j.spin(txf) );
    //     Draw(tmp,1,0,0);
    //   }
 
   }
   
    //Draw Original Circle 
    Draw(ca);
    if (bPoint) Draw(caorth,1,0,1);
    
    
    //Draw Transformed Circle
    if (!bAxis) Draw( ca.spin(fullxf),0,1,1);
    else Draw( Line( ca.spin(fullxf) ),0,1,1);
 

    //Draw frames    
   // Draw(fa);
  //  Draw(fb);

    // Draw Split
    bool bBlue = false;
    for (auto& i : split){
     Draw(i.dual(),0,1,bBlue ? 1 : 0);
     bBlue = !bBlue;
    }
 
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
