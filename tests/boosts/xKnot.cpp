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



#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_knot.h"
#include "vsr_twist.h"
#include "vsr_stat.h"

using namespace vsr;
using namespace vsr::cga3D;


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

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
    init();
  }

  void init(){
    Rand::Seed();
    int n = 30;
    frame = vector<Frame>(n);
    tframe = vector<Frame>(n);
    tk = vector<TorusKnot>(n);
    bst = vector<Bst>(n);
  }

  void initGui(){
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
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }
  
  void setFrame(){
    for(int i=0;i<frame.size();++i){
      float t= -5.0 + 10.0*i/frame.size();
      frame[i].set( Ro::null(t,0,0), Gen::rot( Biv::xy * Rand::Num() + Biv::xz * Rand::Num() ) );
    }
  }

  void weights(){

      for(int i=0;i<frame.size();++i){
        Par par;
        for(int j=0;j<frame.size();++j){
          if (i!=j){
            auto odist = 1.0 / (.01 + Ro::dist( frame[i].pos(), frame[j].pos() ) );
            par += tk[j].par() * odist; 
          }
        }
        bst[i] = Gen::bst( par * amt3 );
      }
   }

  void knots(){
      for(int i=0;i<frame.size();++i){
        tk[i].HF.cir() = frame[i].cxz();//Ro::round( frame[i].ibound(), Biv::xz);
      }
  }

    virtual void onDraw(){ 
        
      getMouse();
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
      auto target = Ro::null( v/(float)frame.size() );
      Frame cam( Vec(target) + Vec(0,5,15) ); cam.orient(target);       
      scene.camera.set( cam.vec(), cam.quat() );
      
  }
   

  
};


MyApp * app;


int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(500,500,"Versor",&glv);    
  app = new MyApp( win ); 
  app -> initGui();
  
  
  glv << *app;

  Application::run();

  return 0;

}
