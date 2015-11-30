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


#include <vsr/vsr_app.h>   
#include <vsr/form/vsr_rigid.h>
#include <vsr/form/vsr_chain.h>
#include <vsr/draw/vsr_chain_draw.h>

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {
 
  //Some Variables
  bool bReset = true;
  float amt,amt2,amt3,linewidth = 0;
  float frameScale =.5;
  float linkLength = 2;
  float linkOffset = 2;

  Chain chain = Chain(5);
  Point point;

  bool bTrack = true;
  bool bDrawChainR,bDrawChainRot,bDrawChainProj,bFK = false;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100);
    //gui(amt2,"amt2",-100,100)(amt3,"amt3",-100,100);
    gui(bReset,"bReset")(linewidth,"linewidth",1,10);
    gui(linkLength,"linkLength",0,10);
    gui(linkOffset,"linkOffset",0,10);
    gui(frameScale,"frameScale",0,10);
    gui(bDrawChainR,"bDrawChainR");
    gui(bDrawChainRot,"bDrawChainRot");
    gui(bDrawChainProj,"bDrawChainProj");
    gui(bFK);

    //reset chain
    reset();    

    amt = .001;
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
      chain.link(i).pos(0,linkLength,linkOffset);
      chain[i].scale() = frameScale;
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
      if (bFK)  chain.fk();
     }
    
     Draw( Construct::sphere( Frame( chain[chain.num()-1].mot() * chain.link(chain.num()-1).mot() ).pos(),.2),1,0,0); 
     Draw(chain,false,true,.2,.2,.2);

     if(bDrawChainR){
      DrawR(chain,0,1,1);
     }

     //Draw circle of rotation of each joint
     for (int i=1;i<chain.num();++i){
       // Draw( chain.prevCircle(i),1,0,0); 
       // Draw( chain.nextCircle(i),0,1,0); 

       auto s = chain[i].z() <= chain[i-1].z();
       if ( !FERROR(s[0])) cout << i << " " << s[0] << endl;
     }


     for (int i=1;i<chain.num();++i){

        //rotation in terms of prev frame
        auto dir = Vec(chain[i].pos()-chain[i-1].pos()).unit();
        //unit projection of direction onto xy
        Biv txy = chain[i].xy();
        Vec pj = Op::project( dir, txy ).unit();

        if (bDrawChainProj) DrawAt(pj, chain[i].pos(),.3,.3,0);

        // joint rotation
        Vec ty = chain[i].y();
        Rot rot = Gen::ratio(pj,ty);
      
        if (bDrawChainRot){
         auto biv = Gen::log(rot);
         for (int j=0;j<10;++j){
          float t = (float)j/10;
          DrawAt( pj.rotate(-biv*t), chain[i].pos(), 1-t,t,1-t);
         }
        }                

     }
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
