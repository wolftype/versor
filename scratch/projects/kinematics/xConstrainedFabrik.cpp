/*
 * =====================================================================================
 *
 *       Filename:  xConstrainedFabrik.cpp
 *
 *    Description:  rotational constraints added to spherical fabrik
 *
 *        Version:  1.0
 *        Created:  08/09/2015 18:11:33
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
#include "form/vsr_chain.h"
#include "draw/vsr_chain_draw.h"

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
    gui(amt,"amt",-100,100)(amt2,"amt2",-100,100)(amt3,"amt3",-100,100)(bReset,"bReset")(linewidth,"linewidth",1,10);

    //reset chain
    reset();    
  }

  void onKeyDown( const gfx::Keyboard& k){
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
      //adjust amt variable to adjust error threshold
      chain.constrainedFabrik(point,chain.num()-1,0,amt);
      chain.fk();
     } else {
       chain.joint(0).rot() = Gen::rot( Biv::xy * amt );
       chain.joint(1).rot() = Gen::rot( Biv::xy * amt2);
       chain.joint(2).rot() = Gen::rot( Biv::xy * amt3 );
       if (bReset) chain.fk();

     }
     
     Draw(chain);

     //Draw circle of rotation of each joint
     for (auto& i : chain.frame()){
        Draw( i.cxy(),1,0,0); 
     }
     for (int i=1;i<chain.num();++i){
     //   Draw( chain.prevCircle_(i),0,1,0);
     }
     
     for (int i=1;i<chain.num();++i){

        //rotation in terms of prev frame
        auto dir = Vec(chain[i].pos()-chain[i-1].pos()).unit();
        //unit projection of direction onto xy
        Biv txy = chain[i].xy();
        Vec pj = Op::project( dir, txy ).unit();

        // possible joint rotation
        Vec ty = chain[i].y();
        Rot rot = Gen::ratio(  pj,ty );

        auto biv = Gen::log(rot);
        for (int j=0;j<10;++j){
          float t = (float)j/10;
          DrawAt( pj.rotate(-biv*t), chain[i].pos(), 1,t,1-t);
        }

        //get bxy of frame (minus link)
        auto adjustedRot = chain.prevRot(i);// !rot * chain[i].rot() * !chain.link(i-1).rot();
        Frame tmp; tmp.rot() = adjustedRot;

        auto normal = tmp.z();//Vec::y.spin( adjustedRot); 
        auto y = tmp.y();

      //  DrawAt( normal, chain[i].pos(),0,0,1 );
      //  DrawAt( y, chain[i].pos(),0,1,1 );
                

     }
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
