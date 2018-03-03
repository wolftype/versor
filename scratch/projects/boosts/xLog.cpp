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

namespace vsr{ 
  
Boost con_( const vector<Pair>& log, float amtA){
    Par par = log[0] * amtA;
    if (log.size() > 1){
      par += log[1] * amtA;
    }
    return Gen::bst(par);
}


Boost con_( const vector<Pair>& log, float amtA, float amtB){
    Par par = log[0] * amtA;
    if (log.size() > 1){
      par += log[1] * amtB;
    }
    return Gen::bst(par);
}


    vector<Pair> split_(const Pair& par){
   
      typedef decltype(Sphere()+1) SqDeriv;
    
      vector<Pair> res;
      
      SqDeriv h2 = par*par;
      auto hh2 = Sphere(h2).wt();
      auto ipar = (-Sphere(h2) + h2[0]) /(h2[0]*h2[0] - hh2);  
    
      auto tmp2 = ( (h2*h2) - (h2*2*h2[0]) )[0] ;
      auto ff4 = FERROR(tmp2) ? 0 : pow( -tmp2, 1.0/4);
      auto wt = ff4 * ff4;
      
      if ( FERROR(wt) ) {
          if ( FERROR(h2[0]) ){
           // cout << h2 << endl;
            cout << "no real splitting going on" << endl; //<-- i.e. interpolation of null point pairs
            res.push_back(par);
           // res.push_back(Par());
            return res;
          } else {
            cout << "(adding random value and retrying)" << endl;
            static Pair dp(.001,.006,.004,.002,.008,.006,.003,.007,.001,.001);
           // return split( par + dp);
          }
      }
      
      auto iha = ipar * wt;
      auto fplus = iha + 1;
      auto fminus = -iha + 1;
    
      Pair pa = par * fplus * .5;
      Pair pb = par * fminus * .5;
    
      res.push_back(pa);
      res.push_back(pb);
      
      return res;
       
    }
    
   /*! Split Log of General Conformal Rotor */
   vector<Pair> log_( const Circle& a, const Circle& b){

    auto trot = (b/a).runit();
    auto sca = (1+trot[0]);
    auto sca2 = sca*sca;
    auto sph = Sphere(trot);
    auto dls = sph.dual();
    auto sphwt = sph.wt();

    if ( FERROR(sca2-sphwt) ){
      cout << "orthos!" << endl;
    }
          
     vector<Pair> res;
      
     //0. Some Terms for later on
     //R^2
     auto sqrot = trot*trot;
     //<R>2
     Pair rot2(sqrot);
    
     //1. Get Exterior Derivative
     Sphere quad(trot); //grade 4 part

     auto tmp = quad + (-trot[0]); //scalar + quadvector
     //half the exterior deriv is sinh(B+/-)
     auto deriv = Pair( tmp * Pair(trot) ) * 2; //quad parts are zero here.
     //find commuting split of that
     auto v = split_( deriv );
  
     //get cosh (see p96 of ref)
     auto sp = v[0].wt();//(v[0]<=v[0])[0];
     auto sm = v[1].wt();//(v[1]<=v[1])[0];

     VSR_PRECISION coshp = FERROR(sm) ? sqrot[0] : -(rot2 <= !v[1])[0];
     VSR_PRECISION coshm = FERROR(sp) ? sqrot[0] : -(rot2 <= !v[0])[0];

     //5.27 on p96 of Dorst ref
     res.push_back( Gen::atanh2(v[0],coshp,false) * -.5 );
     res.push_back( Gen::atanh2(v[1],coshm,false) * -.5 );

     return res;

   }

   /*!  Generate Conformal Transformation from circle a to circle b
        uses square root method of Dorst et Valkenburg, 2011
   */
   Con ratio_( const Circle& a, const Circle& b, bool bFlip, float theta){
    
    Con trot = (b/a).runit();
    //planar?
  //  float planarity = (Round::carrier(a).dual().unit() ^ Round::carrier(b).dual().unit()).wt();
    if ( bFlip && trot[0] < 0 ) { //fabs(planarity)<=.000009 )  {
      trot = -trot;      //restrict to positive <R> only if coplanar
    }
      
    auto rotone = trot + 1;

    VSR_PRECISION sca = 1 + trot[0];
    VSR_PRECISION sca2 = sca*sca;

    Sphere sph(trot);
    auto sph2 = sph.wt();


    //orthogonal circles have infinity of roots
    if ( FERROR( sca2 - sph2) ) {
     // printf("infinity of roots . . .  \n");
      auto rotneg = (-trot) + 1;

      Vec vec;

      Biv dir = Round::dir(b).copy<Biv>().runit();
      auto sizeA = nga::Round::size(a,false);
      auto sizeB = nga::Round::size(b,false);
      //if circle is orthogonal
      if ( sizeB < 1000 && !FERROR(sizeB) ) vec = Vec( Round::location(a) - Round::location(b) ).unit().rotate( dir * theta );
      //or if one is axis of the other
      else vec = -Round::vec(a,theta).unit();    

      auto dls = sph.dual();
      
      auto biv = ( Pair( vec.copy<Tnv>() ).trs( Round::location(a) ) ^ dls ).dual();//.trs(1,0,0);

      biv = biv.runit();
        
      auto test = (biv*sph - sph*biv).wt();

      if ( !FERROR( (biv<=biv)[0] +1 ) || (!FERROR(test) ) ) {
        printf("HEY NOW NOT COMMUTING\n");
      }

      auto ret = rotone/2.0 + (biv*(rotneg/2.0));
      return ret;
    }

    auto sca3 = sca2 - sph2;
    auto sqsca3 = sqrt(sca3);

 //   cout << sca2 << " " << sph2 << " " << sca << " " << sqsca3 << endl;
 //   sca = fabs(sca);  //<--* added this fabs in
    auto v1 = ( -sph + sca ) / (2*sca3);
    auto v2 = (sph+(sca+sqsca3))/sqrt( sca+sqsca3 ); 
     
    return rotone * v1 * v2;      
   }
//vector<Pair> log_( const Circle& a, const Circle& b){

    
    //test orthogonality

//    if ( FERROR(sca2-sphwt) ){
//      cout << "orthos!" << endl;
//      Vec vec;
//      float theta = 0;
//
//      auto sizeB = nga::Round::size(b,false);
//      //if circle is orthogonal
//      if ( sizeB < 1000 && !FERROR(sizeB) ) vec = Vec( Round::location(b) - Round::location(a) ).unit();
//      //or if one is axis of the other
//      else vec = Round::vec(a,theta).unit();  
//      // bivector component commutes with quadvector
//      auto biv = ( Pair( vec.copy<Tnv>() ).trs( Round::location(a) ) ^ dls ).undual();
//
//      biv = biv.runit();
//      trot += biv;
//      return Gen::log(trot);
//    }
      
 //   return Gen::log ( trot );    
//   }


} 

struct MyApp : App {
 
  //Some Variables
  bool bReset, bDrawXf, bDrawSplit, bDrawPair, bDrawInterp,bDrawCoordinateGrid = false;
  float amt, P, Q, P2,Q2,wt,seg,seg2 = 0;
  float bivA,bivB,theta,phi,hopftheta,twistAmt,offset,iter;

  bool bRealA, bRealB, bTangentA, bTangentB, bAxis, bPoint, bFlow;
  float numStart,linewidth;
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
    gui(seg2,"seg2",-100,100);         //<-- number of segments along "template"
    gui(theta,"theta",0,TWOPI);        //<-- theta position of starting orbit around circle ca 
    gui(phi,"phi",0,TWOPI);            //<-- phi spread of positions on 
    gui(hopftheta,"hopftheta",0,TWOPI);            //<-- phi spread of positions on 
    gui(twistAmt,"twistAmt",0,TWOPI);  //<-- twist around axis
    gui(numStart,"numStart",0,1000);   //<-- number of points around circle generator
    gui(iter,"iter",0,1000);           //<-- number of circles
  
    gui(bDrawCoordinateGrid,"bDrawCoordinateGrid");
    gui(bDrawSplit, "bDrawSplit");
    gui(bRealA,"realA");
    gui(bRealB,"realB");
    gui(bTangentA, "tangentA");
    gui(bTangentB, "tangentB");
    gui(bAxis, "bAxis")(bPoint, "bPoint");
    gui(bFlow, "bFlow");
    gui(bivA, "bivA",-100,100)(bivB,"bivB",-100,100);
    gui(linewidth,"linewidth",.5,10);
    
    fa.pos(-2,0,0);
    //fa.rot() = Gen::rot(0,PIOVERFOUR/2.0);
    
    //fb.pos() = fa.pos().reflect( Dlp(1,0,0) );
    fb.mot( fa.mot().reflect( Dlp(1,0,0) ) );
    //fb.pos(2,0,0);

    objectController.attach(&fa);
    objectController.attach(&fb);

    bRealA = bRealB = true;
    //defaults
    wt = 1; seg=4; amt = .01;

    P=3; Q=2;

    mColor.set(.8,.8,.8);

  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    glLineWidth(linewidth);
    gl2psPointSize(5);
    
    //Circles A and B On Frames
    auto ca = bTangentA ? fa.ty().dual() : (bRealA ? fa.cxz() : fa.icxz());
    auto cb = bTangentB ? fb.ty().dual() : (bRealB ? fb.cxz() : fb.icxz());

    //Dls dtmp = ca/(-Inf(1) <= ca);
    //Draw(dtmp);
    //tangents (null point pairs) in y direction
  //  auto ta = fa.ty();
  //  auto tb = fb.ty();

    //Orthogonal circle through frame fb
    auto caorth = fb.pos() ^ ca.dual();

  
  
    //use axis of circle a, orthogonal circle to it, or frame circle cb itself 
    auto cc = bAxis ? Cir(fa.ly().unit()) : bPoint ? caorth : cb; 

    //Transformation from circle a to circle b, and log
    //.................................flip?  theta  
    auto fullxf = ratio_( ca, cc, false, hopftheta); 

    //vector
    auto tbiv = Round::dir(cc).copy<Biv>().runit();
    auto dir = bAxis? Round::vec(cc, -hopftheta).unit() : Vec( Round::location(ca) - Round::location(cc) ).unit().rotate(tbiv*hopftheta);
    DrawAt(dir, Round::location(ca),0,.3,0);

    auto log = Gen::log( fullxf );
   // auto log = log_(ca,cc);
   // log[0] *= .5;
   // log[1] *= .5;

    //conformal motion (small differential)
    float amtP = (P==0) ? 0 : seg*amt/P;
    float amtQ = (Q==0) ? 0 : seg*amt/Q;

    auto conf =  Gen::con(log, amtP, amtQ);

    //Just the Split (for drawing purposes)
  //  auto split = Gen::split( fullxf );
    
    //numStart Points around a circle starting at theta with spread of phi,
    auto pnt = points(ca, (int)numStart, theta, phi);

    for (auto& i : pnt){
      i = i.twist( (Round::pair(ca,0)^Inf(1)).dual() * twistAmt );
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
       auto tp = Round::loc(start);
       Draw(tp , t*.4, .3, (1-t)*.4);

       float titer =0;
     //  if (bDrawCoordinateGrid){
     //      for (auto& s : split){
     //       auto cir = tp^s;
     //       coord.push_back(cir);
     //       Draw(cir,t,titer,1-t);
     //       Draw( Tangent::at(cir,tp),t,1-titer,1-t);
     //       titer+=1;
     //      }

     //      for (int k=0;k<iter;++k){

     //         float tk = seg2 * harmonic2 * (float)k/iter;
     //         auto conf2 = Gen::con( coord[0].dual(), split[1].dual(), tk/P2, tk/Q2 );
     //         Draw( Round::location( tp.spin(conf2) ), 0, 1, 0 );

     //      }
     //      
     //  }
     }



    }

 
   //Absolute
   for (int i=0;i<=(int)iter;++i){
     
       float t = (float)i/iter ;

       //Absolute Circles      
       auto fxf = Gen::con( log, t);
       auto nc = ca.spin(fxf);
       Draw(nc,.2,.2,.2);


       //Absolute points along orbit
   //    float tamt = seg * harmonic * t;
   //    auto txf = Gen::con( log, tamt/P, tamt/Q );

    //  for (auto& j : pnt ){
    //     auto tmp = Round::location( j.spin(txf) );
    //     Draw(tmp,1,0,0);
    //   }
 
   }
   
    //Draw Original Circles
    Draw(ca,0,0,1);
    if (bPoint) Draw(caorth,1,0,1);
    else if(!bAxis) Draw(cb,0,0,1);
 
    //Draw Transformed Circle
    if (!bAxis) Draw( ca.spin(fullxf),0,1,1);
    else Draw( Line( ca.spin(fullxf) ),0,1,1);
 

    //Draw frames    
  //  Draw(fa.pos(),0,0,1);
  //  Draw(fb.pos(),0,0,1);

    // Draw Split
//    if (bDrawSplit){
//     bool bBlue = false;
//     for (auto& i : split){
//       Draw(i.dual(),0,1,bBlue ? 1 : 0);
//      bBlue = !bBlue;
//      }
//    }
 
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
