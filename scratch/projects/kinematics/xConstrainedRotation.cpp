/*
 * =====================================================================================
 *
 *       Filename:  xConstrainedRotation.cpp
 *
 *    Description:  debug
 *
 *        Version:  1.0
 *        Created:  08/29/2015 12:57:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_rigid.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt,amt2 = 0;
  bool bTrack=false;

    Point point;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100);
    gui(amt2,"amt2",-100,100);
    gui(bReset,"bReset");
    
  }

  void onKeyDown( const gfx::Keyboard& k){
    if (k.code == 's' ){
        bTrack = !bTrack;
        cout << "tracking" << endl;
    }
  }
  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    if (bTrack){
      point = calcMouse3D();
    } 
    
    Frame link;
    link.rot() = Gen::rot( Biv::xz * PIOVERFOUR/2.0 );
 
    Frame fa;
    fa.rot() =  Gen::rot( Biv::yz * .3) * link.rot() * Gen::rot ( Biv::xy * amt2 );

    Frame fb;
    fb.pos() = point;
    fb.rot() = Gen::rot( Biv::yz * .3);

    auto dir = Vec(fa.pos() - point).unit();

    //in socket
    Vec pj = Op::project( dir, fa.xy() ).unit();
    auto rota = Gen::ratio(fa.y(), pj );
    auto rotb = rota * fa.rot();
    auto rotc = Gen::ratio( Vec::y, fa.y().spin(!rotb) );

         
    Frame fc; fc.rot() = rotb * !link.rot();
   
    auto cir = fc.cxy();
    
    auto np = Construct::point(fa.bound(), -fc.y());//Constrain::PointToCircle( fb.pos(), cir);
  
    auto rot2 = Gen::ratio( fc.y(), Vec(fa.pos() - np).unit() );
    
    Frame fd; fd.pos() = np;
    fd.rot() = rot2*fc.rot(); 


    DrawB(fa);
    DrawB(fb);
    Draw(fc);
    Draw(cir,0,1,0);
    Draw(fd);



  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
