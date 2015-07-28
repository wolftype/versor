/*
 * =====================================================================================
 *
 *       Filename:  xAgents_control.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/09/2015 20:44:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "Cuttlebone/cuttlebone.hpp"

#include "vsr/util/vsr_cga3D_simple_app.h"

#include "vsr/util/vsr_stat.h"
#include "vsr/form/vsr_twist.h"

using namespace vsr;
using namespace vsr::cga;


#define NUMPOSE 60
struct State {
  gfx::Pose pose[NUMPOSE];
  float speed[NUMPOSE];
};


struct Member : Frame {

  struct Neighbor : Frame {
    Neighbor(const Frame& f, float d) : Frame(f), dist(d) {}
    float dist=0;
  };

  vector<Neighbor> nearest;
  vector<Neighbor> toonear;

  void clear() { nearest.clear(); toonear.clear(); }

};


struct MyApp : App {    

  cuttlebone::Maker<State> maker;
  State * state;
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  bool bMove, bSpin, bReset, bDrawConnections, bDrawAversions;

  int numAgents = NUMPOSE;

  float initialspacing;
  float nearestspacing;
  float toonearspacing;
  float sourceweight;
  float followweight;
  float aversionweight;
  float aversionspeed;
  float acc = .02;
  float rotAcc = .02; 

  vector<Member> frame;

  MyApp() : App(), maker("10.0.1.255"){}


  /*-----------------------------------------------------------------------------
   *  Setup
   *-----------------------------------------------------------------------------*/
  virtual void setup(){

     state = new State;
     memset(state, 0, sizeof(State) );

     initFrames();
      /// set some parameters in the gui
      bindGLV();
      gui(amt,"amt",-100,100);
      gui(bReset,"reset");
      gui(bDrawConnections,"draw connections");
      gui(bDrawAversions,"draw aversions");
     // gui(bMove,"bMove");
     // gui(bSpin, "bSpin");
      gui(initialspacing, "intitialspacing",0,100);
      gui(nearestspacing, "nearestspacing",0,100);
      gui(toonearspacing, "toonearspacing",0,100);
      gui(sourceweight, "sourceweight",0,100);
      gui(followweight, "followweight",0,100);
      gui(aversionweight, "aversionweight,",0,100);
      gui(aversionspeed, "aversionspeed,",0,100);
      gui(acc, "acc",0,100);
      gui(rotAcc, "rotAcc",0,100);
  
      bMove =true; bSpin=true;
      toonearspacing = .75;  // anything less than this is unacceptably close, agents fly away
      initialspacing = 3;    // initial spacing 
      nearestspacing = 2;    // anything greater than this is ignored
      sourceweight = .1;     // pull towards middle
      

      
   }


  
  /*-----------------------------------------------------------------------------
   *  Initialize Frame Posiitons
   *-----------------------------------------------------------------------------*/
  void initFrames(){
     Rand::Seed();
     frame.clear();
     frame = vector<Member>(numAgents);

     for (auto& f : frame ){
        Vec v( Rand::Num(), Rand::Num(), Rand::Num());
        f.pos() = Round::null(v * initialspacing );
        f.rot() = Gen::rot( Biv(  Rand::Num(), Rand::Num(), Rand::Num() ) );
        f.scale() = .5;
     }
  }

  /*-----------------------------------------------------------------------------
   *  Draw
   *-----------------------------------------------------------------------------*/
    virtual void onDraw(){
      mouse = calcMouse3D();        ///<-- get mouse position

      for (auto& f : frame){        ///<-- draw all frames
        Draw((Frame)f);

        for (auto& n : f.nearest){
          glColor3f(1,1,0);
          if (bDrawConnections) gfx::Glyph::Line( f.pos(), n.pos() );   
        }

        for (auto& n : f.toonear){
          glColor3f(0,1,1);
          if (bDrawAversions) gfx::Glyph::Line( f.pos(), n.pos() );   
        }
        
      }

    }


    /*-----------------------------------------------------------------------------
     *  Simulation
     *-----------------------------------------------------------------------------*/
    virtual void onAnimate(){ 

      /// COPY POSES INTO STATE 
      for(int i=0;i<NUMPOSE;++i){
        state->pose[i].set( frame[i].pos(), frame[i].quat() );
        state->speed[i]= frame[i].toonear.size() * .2;
      }
      maker.set(*state);

      if (bReset) initFrames();


        
      ///frame physics timestep
      for (auto & f : frame ){
        f.clear();
        f.move();
        f.spin();
      }
     
     // float period = 1;
     // float pitch = 1; 
     // DualLine dll = Twist::Along( (mouse ^ Vec::z ^ Inf(1)).dual(), period , pitch );


      int numNeighbors = 3;                             ///<-- how many neighbors to look for
      
      //For Each Frame,
      //find nearest neighbors within view
      //sum up their influence
     for (auto& fa : frame){                           

       Biv db; // Amount to rotate
       Vec dx; // Amount to translate
       float tacc=0;
       float racc;

       float dist = Round::sqd(fa.pos(), mouse);          ///<-- distance from frame to mouse
       float famt = 1.0/(.01 + (dist*dist) );          ///<-- weighting distance

       float thresh = nearestspacing;                  ///<-- distance threshold                      
       float min = toonearspacing;                     ///<-- minimum distance to maintain

       for (auto& fb : frame){
         float halfplane = (fb.pos() <= fa.dxy())[0];          ///<-- on which side of fa's half-plane is fb?
         if ( halfplane > 0 ){                                 ///<-- if it is in front of fa, fa can "see it"
           float dist = Round::sqd( fa.bound(), fb.bound() );     ///<-- distance between frames
           if (dist < min) fa.toonear.push_back( Member::Neighbor(fb,dist) );           ///<-- if it is less than min threshhold
           else if (dist < thresh) fa.nearest.push_back( Member::Neighbor(fb,dist) );        ///<-- if it is within distance threshhold
           if (fa.nearest.size() > numNeighbors || fa.toonear.size() > numNeighbors ) break;       //<-- stop after # nearest neighbors
        }
       }
       
     
       //orient towards neighbors
       if (!fa.nearest.empty()){
        for (auto& n : fa.nearest){
          if (n.dist>FPERROR) {
            db += fa.relOrientBiv( n.pos() ) * followweight;//fa.nearest.size(); 
            tacc += aversionspeed;
          }
        } 
       }
               
        //orient away from neighbors that are too close 
        if (!fa.toonear.empty()){
        for (auto& n : fa.toonear){
          if (n.dist>FPERROR) db -= fa.relOrientBiv( n.pos() ) * aversionweight;//fa.toonear.size(); //fa.xz() * (b?1:-1);
         } 
        }

        db += fa.relOrientBiv( calcMouse3D() ) * sourceweight;
        dx += fa.z();
        
        fa.db() = db * rotAcc; 
        fa.dx() = dx * (acc+tacc);


      }
    
  }
  
  
};



int main(){
                             
  MyApp app;
  app.maker.start();
  app.start();
  
  return 0;

}

