/*
 * =====================================================================================
 *
 *       Filename:  xCircleNetGeneric.cpp
 *
 *    Description:  n-sided cyclidic nets 
 *
 *        Version:  1.0
 *        Created:  06/22/2015 20:38:33
 *       Revision:  none
 *       Compiler:  gcc
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

   Bst ratio_( const DualSphere& a, const DualSphere& b, bool bFlip){
    Bst tbst = (b/a).runit();
    cout << "tbst[0] " << tbst[0] << endl;
    //if (tbst[0]<0)
    if (bFlip) tbst = -tbst; //restrict to positive <R>
    auto ss = 2 * (1+tbst[0]);     
    auto n = (ss >= 0 ? sqrt(ss) : -sqrt(-ss) );
    return FERROR(n) ? Bst() : (tbst+1)/n;
   }


  /*! atanh2 function for logarithm of general rotors*/
   Pair atanh2_( const Pair& p, VSR_PRECISION cs){
      VSR_PRECISION norm=1;

      auto tp = p.wt();
     // cout << "atan wt: " << tp << endl;
      auto sq = sqrt( fabs(tp) );
      if(tp>0) norm = asinh(sq)/sq;
      else if (tp<0) norm = atan2(sq, cs) / sq;
      return p * norm;
   }

    /*! Log of a simple rotor (uses atanh2) */
   Pair log_( const Bst& b){
     return atanh2_( Pair(b), b[0] );
   }

  template<class A>  
   auto bst_(const GAPar<A>& tp) -> decltype( tp + 1 ) { 

          VSR_PRECISION norm; VSR_PRECISION sn; VSR_PRECISION cn;

          VSR_PRECISION td = tp.wt(); 

          cout << td << endl;

          if (td < 0) { norm =  sqrt( - td );  sn = -sin(norm) / norm; cn = cos(norm); } //note, changed to cos from cosh
          else if (td > 0) { norm = sqrt(td); sn = -sinh(norm) / norm; cn = cosh(norm); }
          else if (td == 0) { norm = 0; sn = -1; cn = 1; }

          return (tp * sn) + cn;
    }

} }



/*-----------------------------------------------------------------------------
 *  Triply Orthogonal Curvilinear Coordinate System
 *-----------------------------------------------------------------------------*/
struct Ortho {

  Frame frame;
  
  float amtX=1;
  float amtY=1;
  float amtZX=1;
  float amtZY=1;

  bool bDrawFrame = true;
  bool bDrawX=true;
  bool bDrawY=true;
  bool bDrawZ=true;

  void set(float x, float y, float zx, float zy){
      amtX = x; amtY = y; amtZX = zx; amtZY = zy;
  }
 
  DualSphere sigmaX(){
    auto bst = Gen::bst( frame.tx() * -amtX );
    return Dls( frame.dyz() ).spin(bst);
  }

 
  DualSphere sigmaY(){
    auto bst = Gen::bst( frame.ty() * -amtY );
    return Dls( frame.dxz() ).spin(bst);
  }
  
  DualSphere sigmaZX(){
    auto bst = Gen::bst( frame.tz() * -amtZX );
    return Dls( frame.dxy() ).spin(bst);
  }

  DualSphere sigmaZY(){
    auto bst = Gen::bst( frame.tz() * -amtZY );
    return Dls( frame.dxy() ).spin(bst);
  }

  Circle cirZ(){
    return (sigmaX() ^ sigmaY()).dual();
  }

  Circle cirX(){
    return (sigmaY() ^ sigmaZX()).dual();
  } 

  Circle cirY(){
    return (sigmaX() ^ sigmaZY()).dual();
  } 

  
  void draw(){
    
    auto sx = sigmaX();
    auto sy = sigmaY();
    auto szx = sigmaZX();
    auto szy = sigmaZY();

    auto cx = cirX();
    auto cy = cirY();
    auto cz = cirZ();

    Draw( cx, 1,0,0);
 
    if (bDrawFrame) Draw(frame);
    if (bDrawX) Draw(sx,1,0,0,.2);
    Draw( cx, 1,0,0);
    if (bDrawY) Draw(sy,0,1,0,.2);
    Draw( cy, 0,1,0);
    if (bDrawZ) Draw(szx,0,0,1,.2);
    Draw( cz, 0,0,1);
    if (bDrawZ) Draw(szy,0,0,1,.2);
    
  }


};


//    auto cir = net.cxy();
//
//    for (int i = 0; i < (int)amt; ++i){
//      
//      auto theta = TWOPI * i/(int)amt;
//
//      auto pnt = point( cir, theta );
//      
//    }
//

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amtX1,amtY1,amtZ1,amtX2,amtY2,amtZ2 = 0;

  float amtTU, amtTV;
  
  Frame net;
  Frame fa;

  Ortho ortho1, ortho2;

  bool bDrawX, bDrawY, bDrawZx, bDrawZy, bDrawSpan,bDrawPnts;
  float dist;
  
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(dist,"dist",0,100);
    gui(amtX1,"amtX1",-100,100);
    gui(amtY1,"amtY1",-100,100);
    gui(amtX2,"amtX2",-100,100);
    gui(amtY2,"amtY2",-100,100);
    gui(amtZ1,"amtZ1",-100,100);
    gui(amtZ2,"amtZ2",-100,100);

    gui(amtTU,"amtTU",-100,100);
    gui(amtTV,"amtTV",-100,100);
    gui(bReset,"bReset")(bDrawPnts,"bDrawPnts");
    gui(bDrawX)(bDrawY)(bDrawZx)(bDrawZy)(bDrawSpan);

    objectController.attach(&ortho1.frame);
    objectController.attach(&ortho2.frame);
 
    bShadedOutput = false;

    amtX1 = amtY1 = amtX2 = amtY2 = amtZ1 = amtZ2= .5;

    amtTU = amtTV = 1;
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    GL::enablePreset();

    bShadedOutput = bReset;

    ortho1.frame.set( point(-dist,0,0) );//, Gen::rot(PIOVERFOUR,PIOVERFOUR) );
    ortho2.frame.set( point(dist,0,0) );//, Gen::rot(-PIOVERFOUR,-PIOVERFOUR) );

    ortho1.set(amtX1,amtY1,amtZ1,amtZ2);
    ortho2.set(amtX2,amtY2,amtZ1,amtZ2);
   
    auto logX = Gen::log( Gen::ratio( ortho1.sigmaX(), ortho2.sigmaX() ) );
    auto logY = Gen::log( Gen::ratio( ortho1.sigmaY(), ortho2.sigmaY() ) );

    
   // cout << ortho1.sigmaX() * ortho2.sigmaX() << endl;
   // cout << (ortho1.sigmaX() * ortho2.sigmaX()).runit() << endl;


 //   for (int i =0;i< 10;++i){
 //       float tu = .5 * (float)i/10;
 //       auto bst = Gen::bst( logX * tu * amtTU);
 //       Draw( ortho1.sigmaX().spin(bst),tu,0,1-tu,.2);
 //       for (int j =0;j<10;++j){
 //         float tv = (float)j/10;

 //         auto con = Gen::bst(logY * tv * amtTU ) * Gen::bst(logX * tu * amtTV );

 //         auto npnt = Round::loc(ortho1.frame.pos().spin(con));
 //         Draw (npnt);

 //       }
 //   }

    ortho1.draw();

        
    
 //    Draw(ortho1.sigmaX(),0,0,1,.2);

 //    bool bColorFlip = true;

 //    auto sphere = ortho1.sigmaX();
 //    auto circle = (ortho1.sigmaX() ^ Dlp(0,0,1) ).dual();
 //    auto pnts = points( circle, 10 );

 //    for (int i =0;i<= 10;++i){
 //       float tu =  (float)i/10;
 //       auto bst = Gen::bst_( logX * tu * -amtTU);
 //       auto sph = ortho1.sigmaX().spin(bst);
 //       auto cir = (sph^Dlp(0,0,1) ).dual();
 //       auto cirB = (sph^Dlp(0,1,0)).dual();
 //       auto cirC = (sph^Dlp(1,0,0).translate(Round::loc(sph))).dual();

 //       Draw( cir, .5,.2,.2,2);
 //       Draw( cirB, .2,.5,.2,2);
 //       Draw( cirC, .2,.2,.5,2);

 //       if (bDrawPnts){
 //       for (auto& j : pnts){
 //         auto p = j;//
 //         auto p2 = Tangent::at(sphere,j).dual();
 //         Draw( Round::loc( p.spin(bst)),0,.2,0 );   
 //         
 //       }
 //       }

 //    }

 //    Draw(ortho2.sigmaX(),1,0,1,.2);
    

  
//   auto pa = point(4,0,0);
//   auto pb = point(0,4,0);
//   auto pc = point(3,6,0);
//   
//   Draw( fa.pos() );
//   Draw( pa,1,0,0);
//   Draw( pb,1,0,0);
//    
//   auto sphX  = (fa.tx().dual() ^ pb).dual(); 
//   auto sphY  = (fa.ty().dual() ^ pa).dual();
//   auto sphZx = (fa.tz().dual() ^ pb).dual();
//   auto sphZy = (fa.tz().dual() ^ pa).dual();
//    
//   auto cirX = (sphZx^sphX).dual();
//   auto cirY = (sphZy^sphY).dual();
//
//   if (bDrawZx) {
//     Draw(sphZx,0,0,1,.2);
//     Draw( cirX, 1,1,0);
//   }
//   if (bDrawZy) {
//     Draw(sphZy,0,0,1,.2);
//     Draw( cirY, 1,1,0);
//   }
//   
//   if (bDrawX) Draw(sphX,1,0,0,.2);
//   if (bDrawY) Draw(sphY,0,1,0,.2);
//   
//   
//   auto log = Gen::log( Gen::ratio( sphX, sphY ) );
//
//   auto log2 = Gen::log( Gen::ratio( cirX, cirY ) );
//   for (int i =0;i<=10;++i){
//     double t = (float)i/10 * amt;
//     auto rot = Gen::bst( log * t );
//     auto rot2 = Gen::con( log2, t );
//
//     auto ncir = cirX.spin( rot2 * amt );
//     
//     if (bDrawSpan) Draw(ncir,0,1,1);
//
//   }
//
//    Draw( fa );



     
 
   //recips?
//   auto pss = fa.pos() ^ pa ^ pb;
//
//   auto rca = (pa ^ pb) <= !pss;
//   auto rcb = (fa.pos() ^ pa) <= pss;
//   auto rcc = (fa.pos() ^ pb) <= pss;
//
//   //recips?
//   auto pss2 = fa.pos() ^ pa ^ pb ^ pc;
//
//   auto rca2 = (pa ^ pb ^ pc) <= !pss2;
//   auto rcb2 = (fa.pos() ^ pa ^ pb) <= pss2;
//   auto rcc2 = (fa.pos() ^ pb ^ pc) <= pss2;
//   auto rcd2 = (fa.pos() ^ pa ^ pc) <= pss2;
//



  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
