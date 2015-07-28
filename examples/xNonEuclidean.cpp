/*
 * =====================================================================================
 *
 *       Filename:  xNonEuclidean.cpp
 *
 *    Description:  non euclidean lines
 *
 *        Version:  1.0
 *        Created:  05/09/2014 09:23:17
 *       Revision:  none
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  wolftype 
 *
 * =====================================================================================
 */


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {    
   
  float time=0;
  float amt;

  bool bNonEuclidean, bHyperbolic;//, bSpherical;

  void setup(){
      bindGLV();
      gui(amt,"amt",-100,100);
      gui(bNonEuclidean,"non_euclidean");
      gui(bHyperbolic,"hyperbolic");//(bSpherical,"spherical");
  }
  

  void onDraw(){ 
        
      auto mouse = calcMouse3D();
      gfx::GL::disablePreset();


      auto hyp = EP;      // Hyperbolic Infinity
      auto sph = EM;      // Spherical Infinity
      auto inf = Inf(1);  // Euclidean Infinity   
      
      double spacing = 5;
      int num = 20;
      for (int i=0; i<=num; ++i){
        double t=-spacing/2.0 + spacing*(double)i/num;
        auto tan = Round::null(t,0,0) ^ Vec::y;
        Line line = tan ^ inf;
        draw(line,0,0,1);

        if (bNonEuclidean){
           Cir circle = tan ^ (bHyperbolic ? hyp : sph );
           draw(circle,1,0,0);
        }

      }

      for (int i=0; i<=num; ++i){
        double t=-spacing/2.0 + spacing*(double)i/num;
        auto tan = Round::null(0,t,0) ^ Vec::x;
        
        Line line = tan ^ inf;
        draw(line,0,0,1);

        if (bNonEuclidean){
           Cir circle = tan ^ (bHyperbolic ? hyp : sph );
           draw(circle,1,0,0);
        }

      }


    
  }
   

  
};


int main(){
                             
  MyApp app;
  app.start();
  return 0;

}
