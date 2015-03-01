/*
 * =====================================================================================
 *
 *       Filename:  xBots.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/04/2014 10:25:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vsr_cga3D.h"   
#include "vsr_cga3D_frame.h"
#include "vsr_GLVimpl.h"
#include "vsr_stat.h"
#include "vsr_twist.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  bool bMove, bSpin;


  static const int numAgents = 30;
  float spacing = 3;
  vector<Frame> frame;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
    
   // frame = new Frame[numAgents];

    frame = vector<Frame>(numAgents);

    Rand::Seed();
    for (auto& f : frame ){
      Vec v( Rand::Num(), Rand::Num(), Rand::Num());
      f.pos() = Ro::null(v * spacing );
      f.rot() = Gen::rot( Biv(  Rand::Num(), Rand::Num(), Rand::Num() ) );
      f.scale() = .5;
    }
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(bMove,"bMove");
      gui(bSpin, "bSpin");
  
      bMove =true; bSpin=true;
  }

  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      getMouse();

     // Dlp dlp(1,0,0);

     // ( mouse <= dlp ).print();

      for (auto & f : frame ){
        Draw(f);
        f.move();
        f.spin();
      }


      


      //swarm -- find nearest neighbors in z direction (within halfspace of xyplane)
      float acc = .02;
      float rotAcc = .02; 

     // Line line = mouse ^ Vec::z ^ Inf(1);
     //
      float period = 1;
      float pitch = 1;
      
      DualLine dll = Twist::Along( (mouse ^ Vec::z ^ Inf(1)).dual(), period , pitch );

      int numNeighbors = 3;
      for (auto& fa : frame){

        float dist = Ro::sqd(fa.pos(), mouse);
        float famt = 1.0/(.01 + (dist*dist) );

        vector<Frame> nearest;
        vector<Frame> toonear;
        float thresh = spacing;
        float min = .75;
        for (auto& fb : frame){
          float halfplane = (fb.pos() <= fa.dxy())[0];
          if ( halfplane > 0 ){
            float dist = Ro::sqd( fa.bound(), fb.bound() );
            if (dist < thresh) nearest.push_back(fb);
            if (dist < min) toonear.push_back(fb);
            if (nearest.size() == numNeighbors) break;
         }
        }
        
        Biv db; // Amount to orient
        Vec dx; // Amount to move
       
     //   Vec tv = fa.pos().mot( dll ) - fa.pos();;
      //  Vec tv( fa.pos() - mouse );
       // tv[2] = 0;
       // tv *= famt * 10;
        
        dx += tv;

        if (!toonear.empty()){
           db += fa.xz();
           dx += fa.z(); 
        } else {

         for (auto& neigh : nearest){
           gfx::Glyph::Line( fa.pos(), neigh.pos() );          
           db += Gen::log( neigh.rot() ) / nearest.size();
           dx += Vec( neigh.pos() - fa.pos() ) / nearest.size();
         }

         if (nearest.empty()){
           db += fa.xz() * .1;
           dx += fa.z() * .1;
         }
        }

         dx += -Vec(fa.pos()) * .01;

         fa.db() = db * rotAcc; 
         fa.dx() = dx * acc;

      }
    
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

