/*
 * =====================================================================================
 *
 *       Filename:  xCSG.cpp
 *
 *    Description:  er, constructive solid geometry
 *
 *        Version:  1.0
 *        Created:  02/25/2015 18:59:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D_app.h"   
#include "vsr_cga3D_frame.h"

using namespace vsr;
using namespace vsr::cga3D;


/*-----------------------------------------------------------------------------
 *  some test functions
 *-----------------------------------------------------------------------------*/

namespace topo {

  ///take a thing and a function

}

    void Immediate( const Cir& s, float th, float phi )  {  
    VT rad = Ro::rad( s );
      bool im = Ro::size(s, false) > 0 ? 1 : 0;  
     
    //Op::AA(s).vprint();
    gfx::GL::translate( Op::Pos(s).begin() );
    gfx::GL::rotate( Op::AA(s).begin() ); 

      im ? gfx::Glyph::Circle( rad ) :  gfx::Glyph::DashedCircle( rad );            
  }  

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;

  Frame frame;
  Circle cir = CXY(1);

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    
    objectController.attach(&cir);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){


    int s = amt;

    draw(cir);

    auto pair = meet(cir, plane(0,1,0));
    draw(pair,1,0,0);

    auto s1 = Ro::split(pair,true);
    auto s2 = Ro::split(pair,false);
    draw( sphere(s1,.2),0,1,0);
    draw( sphere(s2,.2),0,1,0);

    //segment from s1 to s2 along circle
    //theta of s1 relative to 0 degrees
    auto cen = Ro::loc(cir);
    auto tp = Ro::pointOnCircle(cir, 0)-cen;
    draw(tp,0,1,1);
    auto theta1 = acos( ( Vec(tp).unit() <= Vec(s1-cen).unit() )[0] );
    auto theta2 = acos( ( Vec(tp).unit() <= Vec(s2-cen).unit() )[0] );

    for (int i=0;i<100;++i){
      float t=(float)i/100 * (theta1 - theta2)/2.0;
      draw( sphere( s1.spin( Gen::mot( axis(cir).runit()*(TWOPI-t) ) ), .1), 0,0,1);
    }
  
    auto wt = pair.wt() * PI;

    cout << "theta: " << theta1 - theta2 << " " << theta2-theta1 << endl;
    cout << wt << endl;


    /* switch(s){ */

    /*   case 0: */
    /*     auto a = topo::spin(cir, frame.px(), 100); */
    /*     draw(a); */

    /* } */

  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
