/*
 * =====================================================================================
 *
 *       Filename:  xSwam.cpp
 *
 *    Description:  Simple swarm behavior using vsr::Frame, 
 *    considers what is in "view" (in front of) and changes orientation 
 *    based on following neighbors and diverting from neighbors that are too close.
 *
 *    Note: Naive implementation.
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

#include <vsr/vsr_app.h>
#include <vsr/util/vsr_stat.h>
#include <vsr/form/vsr_twist.h>

using namespace vsr;
using namespace vsr::cga;


struct Member : Frame {

  struct Neighbor : Frame {
    Neighbor(const Frame& f, float d) : Frame(f), dist(d) {}
    float dist=0;
  };

  vector<Neighbor> nearest;
  vector<Neighbor> toonear;

  void clear() {
    nearest.clear();
    toonear.clear();
  }

};


struct MyApp : App {

  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  bool bMove = true;
  bool bSpin = true;
  bool bReset = false;
  bool bDrawConnections = false;
  bool bDrawAversions = false;

  float numAgents = 50;      //number of agents
  int numNeighbors = 3;      ///how many neighbors to look for
  float initialspacing = 3.0;//initial spacing of agents
  float max_distance = 2.0;  //beyond this, we don't consider influence
  float min_distance = .75;  //below this, we avert away
  float followweight = .1;   //the pull of neighbors within view
  float aversionweight = .1; //the push from neighbors
  float sourceweight = .1;   //the pull of the middle
  float vel = .02;           //velocity
  float rotVel= .02;        //rotational velocity

  vector<Member> frame;

  void onDrawGui(){
      gui(numAgents,"num",1,1000);
      gui(bReset,"reset");
      gui(bDrawConnections,"draw connections");
      gui(bDrawAversions,"draw aversions");
      gui(initialspacing, "intitialspacing",0,2);
      gui(max_distance, "max_distance",0,2);
      gui(min_distance, "min_distance",0,2);
      gui(sourceweight, "sourceweight",0,1);
      gui(followweight, "followweight",0,1);
      gui(aversionweight, "aversionweight,",0,1);

      gui(vel, "velocity",0,1);
      gui(rotVel, "rotational_velocity",0,1);
  }

  virtual void setup(){

      initFrames();
      mColor.set(0,0,0);
   }



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

    virtual void onDraw(){

      gfx::GL::lightsOff();

      mouse = calcMouse3D();

      for (auto& f : frame){
        draw((Frame)f);

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


    virtual void onAnimate(){

      if (bReset) initFrames();

      for (auto & f : frame ){
        f.clear();
        f.move();
        f.spin();
      }

      //find nearest neighbors
      //sum up their influence
      //note: this is naive approach, optimizations to this are a very good idea
     for (auto& fa : frame){

       Biv db; // Amount to rotate
       Vec dx; // Amount to translate

       //Squared Distance to Origin
       float dist = Round::sqd(fa.pos(), PAO);
       ///Weighting distance
       float famt = 1.0/(.01 + (dist*dist) );

       for (auto& fb : frame){
          ///on which side of fa's half-plane is fb? e.g. can it "see" it?
         float halfplane = (fb.pos() <= fa.dxy())[0];

         if ( halfplane > 0 ){
           float dist = Round::sqd( fa.bound(), fb.bound() );

           if (dist < min_distance)
             fa.toonear.push_back( Member::Neighbor(fb,dist) );
           else if (dist < max_distance)
             fa.nearest.push_back( Member::Neighbor(fb,dist) );        
                                                                                                  
           if ((fa.nearest.size() + fa.toonear.size()) >= numNeighbors ) 
             break;
         }
       }

       //orient towards neighbors
       if (!fa.nearest.empty()){
        for (auto& n : fa.nearest){
          if (n.dist>FPERROR) {
            db += fa.relOrientBiv( n.pos() ) * followweight;
          }
        }
       }

        //orient away from neighbors that are too close
        if (!fa.toonear.empty()){
        for (auto& n : fa.toonear){
          if (n.dist>FPERROR)
            db -= fa.relOrientBiv( n.pos() ) * aversionweight;
         }
        }

        db += fa.relOrientBiv( Vec(0,0,0) ) * sourceweight;
        dx += fa.z();

        fa.dx() = dx * vel;//(acc+tacc);
        fa.db() = db * rotVel;


      }

  }


};



int main(){

  MyApp app;
  app.start();

  return 0;

}
