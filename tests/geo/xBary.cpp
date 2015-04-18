/*
 * =====================================================================================
 *
 *       Filename:  xBary.cpp
 *
 *    Description:  barycentric coordinates of n-simplices
 *
 *        Version:  1.0
 *        Created:  07/21/2014 12:00:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_cga3D_app.h"
#include "vsr_simplex.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Point mouse;
  Dls mysphere = sphere(0,0,0);

  float time=0;
  float amtA, amtB, amtC;

  bool bReset;

  void setup(){
      
      bindGLV();
      gui(amtA,"amtA",-100,100);
      gui(amtB,"amtB",-100,100);
      gui(amtC,"amtC",-100,100);
      gui(bReset,"reset");

      objectController.attach(&mysphere);
  }
  
  void getMouse(){
     mouse = calcMouse3D();
  }

    virtual void onDraw(){ 
        
      getMouse();

      //a triangle
      Simplex<2> n;

      //make simplex vertices null points
      auto pa = cga3D::point( n.verts[0][0], n.verts[0][1], 0);//.translate(5,0,0);
      auto pb = cga3D::point( n.verts[1][0], n.verts[1][1], 0);//.translate(5,0,0);
      auto pc = cga3D::point( n.verts[2][0], n.verts[2][1], 0);//.translate(5,-5,0);

      //points along hlines
      for (int i = 0;i<=10;++i){
        float t = (float)i/10;

        auto iab = hspin(pa, pb, t);
        auto iac = hspin(pa, pc, t);
        
        auto iba = hspin(pb, pa, t);
        auto ibc = hspin(pb, pc, t);

        auto ica = hspin(pc, pa, t);
        auto icb = hspin(pc, pb, t);

        Draw(iab,0,1,0); Draw(iac,0,1,0); Draw(ibc,0,1,0);
        
      }      
 
      /*-----------------------------------------------------------------------------
       *  OKAY, above works, now, experiment
       *-----------------------------------------------------------------------------*/    
      //hyperbolic tangent space
      auto pss = pa ^ pb ^ pc ^ EP;
      auto norm = pss.rnorm();
      auto sqnorm = norm * norm;
      pss /= -sqnorm; 

      //circle edges
      auto cirA = pb ^ pa ^ mysphere;
      auto cirB = pc ^ pa ^ mysphere;

      //surrounds
      auto sa = Ro::surround(cirA);
      auto sb = Ro::surround(cirB);

      //distance
      double dist = (sa<=sb)[0];
      //now add in 2x what?
      double hdist = dist - 2*sa[4] - 2*sb[4];
     // cout << hdist << endl;

      //centers
      auto ca = Ro::loc(cirA);
      auto cb = Ro::loc(cirB);

      auto trace = sa ^ sb ^ mysphere;
      auto vers = (mysphere<=trace).runit();

      /* for (int j=0;j<=20;++j){ */
      /*   Draw( hspin(ca,cb,(float)j/20), 1,0,0); */
      /* } */
     
      //plunge
      auto plunge = sa ^ sb ^ mysphere;
      Draw(plunge);
      auto versor = (mysphere<=plunge).runit();
      auto sdist = 1.0/Ro::size(plunge,true);

      /* auto pairA = meet(plunge,sa); */
      /* auto pairB = meet(plunge,sb); */

      /* auto a = Ro::loc( Ro::split(pairA,true) ); */
      /* auto b = Ro::loc( Ro::split(pairB,false) ); */   

     // Draw(a,0,0,1);  
     // Draw(b,0,0,1);  
      
      //auto gen = hgen(a,b,amtA);
     // double dist = hdist(ca,cb);

     // Draw( cirA.boost(gen) );
     // Draw( cirA.boost( versor * amtB * (1-dist)*.5 ),0,0,1 );
      Draw( cirA.boost( versor * sdist * amtC * .5), 0,1,0 ) ;

      //Draw(mysphere,0,1,.2,.3);
      //Draw(plunge,1,0,1,.3);

     /* //centers */
     /*  auto ca = Ro::loc(sa); */
     /*  auto cb = Ro::loc(sb); */

     /*  //transformation */
     /*  auto gen = hgen(ca,cb, amtA); */
     /*  Draw(gen,1,1,0); */
     /*  auto xf = cirA.boost(gen); */
     /*  Draw(xf,1,0,0,.5); */

     // auto cirC = eab ^ EP;

    //  reciprocal frames (dual spheres)
    //  auto rcpA = cirA <= pss;
    //  auto rcpB = cirB <= pss;

    //  rcpA /= rcpA[3];
    //  rcpB /= rcpB[3];
    //  auto rcpC = cirC <= pss;

      //candidate transformation lines
     // auto rta = rcpA ^ pa ^ EP;
     // auto rtb = rcpB ^ pb ^ EP;
    //  auto rtc = rcpC ^ pa ^ EP;
    //
  
  //  double dist = acosh(1-(hnorm(rcpA)<=hnorm(rcpB))[0]);
  //  auto hline = rcpA ^ rcpB ^ EP;
  //  auto hversor = EP<=hline; 
  //  hversor /=hversor.rnorm();
  //  Draw(hversor,1,1,0);

   }

  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
