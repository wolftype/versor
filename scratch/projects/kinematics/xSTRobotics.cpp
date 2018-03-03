/*
 * =====================================================================================
 *
 *       Filename:  xSTRobotics.cpp
 *
 *    Description:  model of st robotics
 *
 *        Version:  1.0
 *        Created:  11/19/2015 13:15:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_chain.h"
#include "draw/vsr_chain_draw.h"
#include "form/vsr_knot.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  
  float amt1 = 0;
  float amt2 = 0;
  float knotSpeed =0;
  float linewidth;

   Chain chain = Chain(5);
  bool bFollowMouse;

  Point mouse;
  bool bDrawCircle;

  float lightx,lighty,lightz, err;

  
  Frame frame = Frame(0,1,0);

  Rot adjust;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(err,"err",0,1);
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(bToggle,"bToggle")(bSet,"bSet");
    gui(knotSpeed,"knotSpeed",-100,100);
    gui(bFollowMouse,"bFollowMouse");
    gui(bDrawCircle,"bDrawCircle");
    gui(linewidth,"linewidth",1,100);
    gui(lightx,"lightx",-10,10);
    gui(lighty,"lighty",-10,10);
    gui(lightz,"lightz",-10,10);
    amt1 = .001; 

    lightx = lighty = lightz = 1;
    objectController.attach(&frame);

        mColor.set(.8,.8,.8);

  }


  void reset(){

    chain = Chain(5);

    for (int i=0;i<chain.num();++i){
      chain[i].scale() = .2;  
    }

    chain.baseFrame().rot() = Gen::rot( Biv::yz * -PIOVERFOUR );
    chain.link(0).pos(0,amt1,2);
    chain.link(0).rot() = Gen::rot( Biv::yz * PIOVERFOUR );

    chain.link(1).pos(0,1,amt1);
    chain.link(2).pos(0,1,-amt1);
    chain.link(3).pos(0,.5,amt1);
    chain.link(3).rot() = Gen::rot( Biv::yz * -PIOVERFOUR );
    chain.link(4).pos(0,0,.5);
   
    chain.fk();

    Vec pj = Op::pj( chain[ chain.num()-1 ].vec(), chain.baseFrame().xy() ).unit();
    adjust =  Gen::rot( Biv::xy * -PIOVERFOUR);// Gen::ratio( pj.spin( !chain.baseFrame().rot()), Vec::y );
    Draw( pj,0,1,0);



  }
  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    //if (bMouseSet) 
    mouse = calcMouse3D();

    glLineWidth( linewidth );
    GL::lightPos(lightx,lighty,lightz);

    Point tmp =  Construct::point(1.5,0,0);
    static Point point = tmp;
    TorusKnot tk(3,2);

    point = Round::location( point.spin( tk.bst(knotSpeed) ) );

    tk.HF.cir() = frame.cxz();

    tk.calc(tmp);

   // 
    if (!bFollowMouse) {
    static int iter = 0;
     if (iter > tk.pnt.size()-1) iter = 0;
     mouse = tk.pnt[iter];
     glColor4f(0,0,0,.5);
     glBegin(GL_LINE_STRIP);
     for (auto& i : tk.pnt){
      GL::vertex(i.begin());
     }
     glEnd();
    // Draw(tk.pnt[iter],1,0,0);
     iter++;

    }
    if (bSet) reset();

   
    chain.joint(0).rot() = Gen::ratio( Vec::y, Vec(Op::pj( Vec(mouse).unit(), chain.baseFrame().xy() ) ).unit().spin( adjust * !chain.baseFrame().rot()) );    
    chain.fk();

    
    Draw(mouse,1,0,0);
    //chain.constrainedFabrik(mouse, chain.num()-1, 1,err);
    chain.constrainedFabrik(mouse, chain.num()-1, 1,err);
    chain.fk();
    
    Draw(chain,false, false, 0,0,0);  
    DrawR(chain,0,1,1);

    for (int i=0;i<chain.num();++i){

      auto cir = chain.nextCircle(i);

      
       if (bDrawCircle) Draw(chain.nextCircle(i),0,.3,0);
   //   if (i>0) Draw(chain.prevCircle(i),1,0,0);

      //auto vec = chain[i].y();
      auto log = Gen::log( chain.joint(i).rot() );
      //if ( !FERROR( log.rwt() ) ){
      for (int j=0;j<10;++j){
        float t = (float)j/10;
        auto rot = chain[i].rot() * !Gen::rot( log * -t ); 
        DrawAt( Vec::y.spin(rot), chain[i].pos(), .5*t,.2,.5-.5*t);
      }
     // }
    }


  }


  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
