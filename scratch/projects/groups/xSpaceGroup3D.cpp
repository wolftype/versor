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
  
  bool bGlideA, bGlideB, bGlideC;
  float glideA, glideB, glideC;

  Point point = Round::null(0,0,0);

  float xratio,yratio,zratio;
  int numX, numY, numZ;

  float bravaisType;
  float latticeType;

};


void drawState(State * state){

     State& s = *state;
    
     SpaceGroup3D<Vec>::Lattice lattice = {(int)s.bravaisType,(int)s.latticeType};
     Vec ratioVec(s.xratio,s.yratio,s.zratio);
     SpaceGroup3D<Vec>::Glide glide = { {(int)s.glideA, s.bGlideA},{(int)s.glideB,s.bGlideB},{(int)s.glideC,s.bGlideC} };
     
     SpaceGroup3D<Vec> sg(s.p,s.q,s.pbar,s.qbar,s.pqbar, lattice, ratioVec, glide);

     Draw( sg.a,1,0,0);
     Draw( sg.b,1,1,0);
     Draw( sg.c,1,0,1);
     Draw( sg.mB_dir );
     Draw( sg.mB_length,1,0,1);
     Draw( sg.mC_dir, 1,.3,.1 );
     Draw( sg.mC_length,0,1,1);

     auto dls = Round::sphere(s.point, .3);
     vector<Point> pnt;
     for (int i=0;i<4;++i){
        float t = (float)i/4;
        Rotor r = Gen::rot( Biv::xz * PI * t);
        pnt.push_back( Construct::point(dls, Vec::x.spin( r ) ) );
     }
     pnt.push_back( s.point.translate(0,.15,0) );

     auto res = sg.apply( pnt );
     auto latticeRes = sg.hang( res , s.numX,s.numY,s.numZ);

   
    for (int i=0;i<latticeRes.size(); i+=pnt.size() ) {

       auto a = latticeRes[i]; 
       auto b = latticeRes[i+1]; 
       auto c = latticeRes[i+2]; 
       auto d = latticeRes[i+3];
       auto e = latticeRes[i+4];  
       glColor3f(1,1,0);
       Glyph::Triangle(a,b,e);
       glColor3f(0,1,1);
       Glyph::Triangle(b,c,e);
       glColor3f(1,0,1);
       Glyph::Triangle(c,d,e);
       glColor3f(1,.5,.5);
       Glyph::Triangle(d,a,e);
       glColor3f(.2,1,.5);
       Glyph::Triangle(a,b,c);
       Glyph::Triangle(c,d,a);

    }

   }

/*-----------------------------------------------------------------------------
 *  SIMULATION
 *-----------------------------------------------------------------------------*/

struct MyApp : App {

  State * state;
   
  //Some Variables
  bool bReset = false;
  bool bSetMouse=false;

  Point mouse = Round::null(1,1,1);;

  float latticetype;

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
    gui(s.xratio, "xratio",1,100);
    gui(s.yratio, "yratio",1,100);
    gui(s.zratio, "zratio",1,100);
    gui(s.latticeType, "latticeType",1,5);
    gui(s.bravaisType, "bravaisType",1,7);
    gui(s.glideA, "glide a",0,5);
    gui(s.bGlideA, "glide a alt");
    gui(s.glideB, "glide b",0,5);
    gui(s.bGlideB, "glide b alt");
    gui(s.glideC, "glide c",0,5);
    gui(s.bGlideC, "glide c alt");
    gui(s.numX, "num x",1,100);
    gui(s.numY, "num y",1,100);
    gui(s.numZ, "num z",1,100);

    s.xratio = s.yratio = s.zratio =1;
    s.p=4;s.q=2;
    s.pbar=s.qbar=s.pqbar=0;
    s.glideA = s.glideB = s.glideC = 0;
    s.latticeType = 1;
    s.bravaisType = 1;

    s.numX = 1;
    s.numY = 1;
    s.numZ = 1;
  }

  void onKeyDown(const gfx::Keyboard& k){
    if (k.code=='s') bSetMouse = !bSetMouse;
  }


  void onAnimate(){
    state -> point = mouse;
    state -> pose = (Pose)scene.camera;
    state -> model = scene.model;
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




