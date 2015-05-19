/*
 * =====================================================================================
 *
 *       Filename:  xSpaceGroup3D.cpp
 *
 *    Description:  from allo
 *
 *        Version:  1.0
 *        Created:  04/30/2015 15:30:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_group.h"


//#include "Cuttlebone/Cuttlebone.hpp"
//#include "alloutil/al_OmniStereoGraphicsRenderer.hpp"
//#include "horo_vsr.hpp"

using namespace vsr;
using namespace vsr::cga;

struct State{

  gfx::Pose pose;
  gfx::Pose model;

  float amt=0;

  float p=2;
  float q=2;
  bool pbar=false;
  bool qbar=false; 
  bool pqbar=false;
  
  Point point = round::null(0,0,0);
 

};


void drawState(State * state){



     State& s = *state;
     if (s.p < 7 && s.q < 5){
     PointGroup3D<Vec> pg(s.p,s.q,s.pbar,s.qbar,s.pqbar);

   // if (pg.ops.size()>2){
      for (int i=0;i<pg.ops.size();++i){
        Draw(pg.ops[i],(float)i/pg.ops.size(),0,1-(float)i/pg.ops.size());
      }
   // }
    for (auto& i : pg.sops){
     // Draw(Biv(i));
    }
    Draw(pg.a,1,0,0); Draw(pg.b,0,1,0); Draw(pg.c,0,0,1);
    
    auto res = pg( s.point ^ s.point.trs(1,0,2) );
    auto res2 = pg( s.point ^ s.point.trs(0,3.5,.2) );
    auto res3 = pg( s.point ^ s.point.trs(0,0,3) );

    //auto res3 = pg( round::round( round::dls(mouse,.2), Biv(.1,.2,.3).runit() ) ); // mouse.trs(0,.5,.2) );
    


   for (int i=0;i<res.size();++i) {
     Draw(res[i],(float)i/res.size(),0,1-(float)i/res.size());
     Draw(res2[i],(float)i/res.size(),1,1-(float)i/res.size());
    // Draw(res3[i],(float)i/res.size(),1,1-(float)i/res.size());
      auto a =  round::split(res[i], true);
      auto b =  round::split(res[i], false);
      auto c =  round::split(res2[i], true);
      auto d =  round::split(res3[i], true);

      glColor3f(1,1,0);
      Glyph::Triangle(a,b,c);
      glColor3f(0,1,1);
      Glyph::Triangle(a,b,d);
      glColor3f(1,0,1);
      Glyph::Triangle(b,c,d);
      glColor3f(1,.5,.5);
      Glyph::Triangle(b,a,d);

    }
    }
   // cout << pg.ops.size() << " " << pg.sops.size() << endl;

}

/*-----------------------------------------------------------------------------
 *  SIMULATION
 *-----------------------------------------------------------------------------*/

struct MyApp : App {

 // cuttlebone::Maker<State> maker;
  State * state;
   
  //Some Variables
  bool bReset = false;
  bool bSetMouse=false;

  Point mouse = round::null(1,1,1);;

 // MyApp() : App(), maker("192.168.10.255"){}

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){

    state = new State;    
    memset(state, 0, sizeof(State));

    ///Bind Gui
    bindGLV();
    
    State& s = *state;
    ///Add Variables to GUI
    gui(s.amt,"amt",-100,100)(bReset,"bReset");
    gui(s.p,"p",0,10);
    gui(s.pbar,"pbar");
    gui(s.q,"q",0,10);
    gui(s.qbar,"qbar");
    gui(s.pqbar,"pqbar");
    
    s.p=3;s.q=2;
    s.pbar=s.qbar=s.pqbar=0;
  }

  void onKeyDown(const gfx::Keyboard& k){
    if (k.code=='s') bSetMouse = !bSetMouse;
  }


  void onAnimate(){
    state -> point = mouse;
    state -> pose = (Pose)scene.camera;
    state -> model = scene.model;
    //maker.set(*state);
  }
  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    if (bSetMouse) mouse = calcMouse3D();

    drawState(state);      
  }  
};



int main (int argc, char *argv[]){

    MyApp app;
    app.start();
    return 0;
}




