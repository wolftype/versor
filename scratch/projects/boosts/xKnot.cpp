/*
 * =====================================================================================
 *
 *       Filename:  xKnot.cpp
 *
 *    Description:  knots again
 *
 *        Version:  1.0
 *        Created:  11/06/2014 19:56:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_app.h"   
#include "form/vsr_knot.h"
#include "form/vsr_twist.h"
#include "util/vsr_stat.h"

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt, amt2, amt3, amt4, speed;

  bool bReset, bKnot;

  vector<Frame> frame;
  vector<Frame> tframe;
  vector<TorusKnot> tk;// tka, tkb, tkc;
  vector<Bst> bst;

  float P, Q;


  void setup(){
    
    bindGLV();
    
    Rand::Seed();
    int n = 30;
    frame = vector<Frame>(n);
    tframe = vector<Frame>(n);
    tk = vector<TorusKnot>(n);
    bst = vector<Bst>(n);
    scene.camera.pos(0,0,10);

    gui(amt,"amt",-100,10000);
    gui(amt2,"amt2",-100,10000);
    gui(amt3,"amt3",-100,10000);
    gui(amt4,"amt4",-100,10000);
    gui(speed,"speed",-100,10000);
    gui(P,"P",-100,100);
    gui(Q,"Q",-100,100);
    gui(bReset,"reset")(bKnot,"knot");
    P = 3; Q=2;
    bReset = true;
  }

  void setFrame(){
    for(int i=0;i<frame.size();++i){
      float t= -5.0 + 10.0*i/frame.size();
      frame[i].set( Round::null(t,0,0), Gen::rot( Biv::xy * Rand::Num() + Biv::xz * Rand::Num() ) );
    }
  }

  void weights(){

     for(int i=0;i<frame.size();++i){
       Par par;
       for(int j=0;j<frame.size();++j){
         if (i!=j){
           auto odist = 1.0 / (.01 + Round::dist( frame[i].pos(), frame[j].pos() ) );
           par += tk[j].par() * odist; 
         }
       }
       bst[i] = Gen::bst( par * amt3 );
     }
   }

  void knots(){
      for(int i=0;i<frame.size();++i){
        tk[i].HF.cir() = frame[i].cxz();
      }
  }

    virtual void onDraw(){ 
        
      mouse = calcMouse3D();
      time++;
     
      for(auto& i : tk) { i.P=P; i.Q=Q; }

      if (bReset){
        setFrame();
      }

      for(int i=0;i<frame.size();++i){
        Draw(frame[i]);
        tframe[i] = frame[i];
      }

      knots();
      weights();

      for (int j=0;j<amt;++j){
        for (int i=0;i<frame.size();++i){
          tframe[i].boost( bst[i] );
          Draw(tframe[i]);
        }        
      }

      for (int j=0;j<amt2;++j){
        for (int i=0;i<frame.size();++i){
          frame[i].boost( bst[i] );
          Draw(frame[i]);
        }
        knots();
        weights();
      }

      for (int i=0;i<frame.size();++i){
        frame[i] = tframe[i];
      }


      
      /*-----------------------------------------------------------------------------
       *  Camera
       *-----------------------------------------------------------------------------*/
      Vec v;
      for (auto& i : frame){
        v += i.vec();
      }
      auto target = Round::null( v/(float)frame.size() );
      Frame cam( Vec(target) + Vec(0,5,15) ); cam.orient(target);       
      scene.camera.set( cam.vec(), cam.quat() );
      
  }
   

  
};


int main(){
                             
  MyApp app;
  app.start();
  return 0;

}
