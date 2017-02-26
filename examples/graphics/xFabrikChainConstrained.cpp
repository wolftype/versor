/*
 * =====================================================================================
 *
 *       Filename:  xConstrainedFabrik.cpp
 *
 *    Description:  Inverse Kinematics Constrained to R (rotation) joints only
 *
 *        Version:  1.0
 *        Created:  08/29/2015 18:39:41
 *       Revision:  none
 *       Compiler:  gcc or clang with c++11 support
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include <vsr/vsr_app.h>   
#include <vsr/form/vsr_rigid.h>
#include <vsr/form/vsr_chain.h>
#include <vsr/draw/vsr_chain_draw.h>

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt,amt2,amt3,linewidth = 0;

  Chain chain = Chain(10);
  Point point;

  bool bTrack = true;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100);
    gui(bReset,"bReset")(linewidth,"linewidth",1,10);

    //reset chain
    reset();    

    amt = .001;

    scene.camera.pos(0,0,13);
  }

  void onKeyDown( const gfx::Keyboard& k){
    //hit 's' to start/stop tracking of mouse position
    if (k.code == 's' ){
        bTrack = !bTrack;
    }
    App::onKeyDown(k);
  }

  void reset(){
    chain.reset();
    
    //set up relative links (90 degree rotation)
    for (int i =0;i<chain.num();++i){
      chain.link(i).rot() = Gen::rot( Biv::xz * PIOVERFOUR);
    }
    chain.fk();
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    glLineWidth(linewidth);

     if (bTrack){
      if (bReset) reset();

      point = calcMouse3D();
     
      //adjust amt variable to adjust error threshold (default is .001)
      chain.constrainedFabrik(point,chain.num()-1,0,amt);
      chain.fk();
     } 
          
     //Draw the chain
     Draw(chain);

     //Draw circle of rotation of each joint
     for (auto& i : chain.frame()){
        Draw( i.cxy(),1,0,0); 
     }

     //Draw the in-socket rotation of each joint
     for (int i=1;i<chain.num();++i){

        auto tyb = chain[i].y();
        auto tya = Vec::y.spin( chain[i].rot() * !chain.joint(i).rot() );
        auto biv = Gen::log( Gen::ratio(tya,tyb) );
        for (int j=0;j<10;++j){
          float t = (float)j/10;
          DrawAt(tya.rotate(-biv*t), chain[i].pos(), 1,t,1-t);
        }

     }
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
