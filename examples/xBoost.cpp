
/*-----------------------------------------------------------------------------
 *  General Conformal Transformation from one Circle to another

 *  This example generates a conformal transformation between two circles,
 *  each twisting around the x axis
 *-----------------------------------------------------------------------------*/


#include "vsr_app.h"  

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
      
  float amt,dist;
  bool bPos,bFlop;

  Circle ca, cb;
   
  virtual void setup(){
    
    bindGLV();
    gui(amt,"amt",-100,100);
    gui(dist,"dist",-100,100);
    gui(bPos, "bPos");
    gui(bFlop, "bFlop");

    amt = .01;
    dist = 1;
    
    ca = Construct::circle(-3,0,0);                           //<-- A circle at coordinate -3,0,0
    cb = Construct::circle(3,0,0);                            //<-- A circle at coordinate 3,0,0
  }
  

  virtual void onDraw(){       

    auto tca = Construct::circle(-dist/2.0,0,0).twist(Construct::line(1,0,0).dual() * -amt );   //<-- twist circle b around x axis dualline
    auto tcb = Construct::circle(dist/2.0,0,0).twist(Construct::line(1,0,0).dual() * amt );     //<-- twist circle b around x axis dualline
  
    //log of transformation from ca to cb
    auto ratio = Gen::ratio(tca,tcb);
    auto log = Gen::log( ratio );

    auto trot = (tcb/tca).runit();
    auto angle = ( Round::dir(tcb).copy<Biv>().runit() / Round::dir(tca).copy<Biv>().runit() )[0];
    
   // cout << trot[0] << " " << angle << endl;
    float theta = acos( trot[0] );

    float planarity = (Round::carrier(tca).dual().unit() ^ Round::carrier(tcb).dual().unit()).wt();
    if (FERROR(planarity)) cout << "PLANAR" << endl;
         
    int num = 100;
    for (int i = 0; i < num; ++i ){
      float t = 1.0 * i/num;
      
      auto nc = tca.spin( Gen::con(log, t * ( bPos ? 1 : -1) ) );            //<- general rotation ("spin") by general conformal transformation
      

    //  auto nnc = (tca * sin((1-t)*theta) + (tcb * sin(t*theta)) ) / sin(theta);
      Draw ( nc );
    //  Draw( nnc, 1,0,0);
    }

    Draw(tca,1,0,1);
    Draw(tcb,1,1,0);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}  


//namespace vsr{ namespace gen {
//   Con ratio_( const Circle& a, const Circle& b, bool flop){
//    
//    Con trot = (b/a).runit();
//    //planar?
//    float planarity = (Round::carrier(a).dual().unit() ^ Round::carrier(b).dual().unit()).wt();
//
//    auto ftest = (b.unit() <= a.unit() )[0];
//    if ( flop && trot[0] < 0 ) {//fabs(planarity)<=.000009 )  {
//      trot = -trot;//(-b/a).runit();//-trot; //restrict to positive <R> only if coplanar
//    }
//      
//    auto rotone = trot + 1;  //<-- we are going to "normalize" this by polar decomposition
//
//    VSR_PRECISION sca = 1 + trot[0];
//    VSR_PRECISION sca2 = sca*sca;
//
//    Sphere sph(trot);
//    auto sph2 = sph.wt();
//
//
//    if (sca2 == sph2) {
//      printf("infinity of roots -- need to program this...\n");
//      auto rotneg = -trot + 1;
//      //random tangent vector
//      auto biv = sph.dual() ^ PAO.trs(1,0,0);
//
//     // printf("f\n");
//     // //std::cout << "A" << endl;
//     // (biv*biv).print();
//     // (biv*sph).print();
//     // (sph*biv).print();
//      return (rotone + (biv*rotneg))/2.0;
//    }
//
//    auto sca3 = sca2 - sph2;
//    auto sqsca3 = sqrt(sca3);
//
// //   cout << sca2 << " " << sph2 << " " << sca << " " << sqsca3 << endl;
//
// //   sca = fabs(sca);  //<--* added this fabs in
//    auto v1 = ( -sph + sca ) / (2*sca3);
//    auto v2 = (sph+(sca+sqsca3))/sqrt( sca+sqsca3 ); 
//     
//    return rotone * v1 * v2;      
//   }
//}}
