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

  Point point = round::null(0,0,0);

  float xratio,yratio,zratio;
  int numX, numY, numZ;

  SpaceGroup3D<Vec>::LatticeType latticeType;
 

};


void drawState(State * state){

     State& s = *state;
//     if (s.p < 7 && s.q < 5){
//     PointGroup3D<Vec> pg(s.p,s.q,s.pbar,s.qbar,s.pqbar);
//    
//     Draw(pg.a,1,0,0); Draw(pg.b,0,1,0); Draw(pg.c,0,0,1);
//    
//     auto res =  pg( s.point ^ s.point.trs(1,0,2) );
//     auto res2 = pg( s.point ^ s.point.trs(0,3.5,.2) );
//     auto res3 = pg( s.point ^ s.point.trs(0,0,3) );
//
//     for (int i=0;i<res.size();++i) {
//       Draw(res[i],(float)i/res.size(),0,1-(float)i/res.size());
//       Draw(res2[i],(float)i/res.size(),1,1-(float)i/res.size());
//      // Draw(res3[i],(float)i/res.size(),1,1-(float)i/res.size());
//        auto a =  round::split(res[i], true);
//        auto b =  round::split(res[i], false);
//        auto c =  round::split(res2[i], true);
//        auto d =  round::split(res3[i], true);
//  
//        glColor3f(1,1,0);
//        Glyph::Triangle(a,b,c);
//        glColor3f(0,1,1);
//        Glyph::Triangle(a,b,d);
//        glColor3f(1,0,1);
//        Glyph::Triangle(b,c,d);
//        glColor3f(1,.5,.5);
//        Glyph::Triangle(b,a,d);
//
//      }
    

     Vec ratioVec(s.xratio,s.yratio,s.zratio);
     SpaceGroup3D<Vec> sg(s.p,s.q,s.pbar,s.qbar,s.pqbar, s.latticeType, ratioVec);


     auto res =  sg( s.point ^ s.point.trs(1,0,2) );
     auto res2 = sg( s.point ^ s.point.trs(0,3.5,.2) );
     auto res3 = sg( s.point ^ s.point.trs(0,0,3) );

     cout << "num ref: " <<  res.size() << endl;

     auto latticeRes = sg.hang( res , s.numX,s.numY,s.numZ);
     auto latticeRes2 = sg.hang( res2 ,s.numX,s.numY,s.numZ);
     auto latticeRes3 = sg.hang( res3 ,s.numX,s.numY,s.numZ);
    
     for (int i=0;i<latticeRes.size();++i) {

        auto a =  round::split(latticeRes[i], true);
        auto b =  round::split(latticeRes[i], false);
        auto c =  round::split(latticeRes2[i], true);
        auto d =  round::split(latticeRes3[i], true);
  
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

  float latticetype;

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
    gui(s.xratio, "xratio",1,100);
    gui(s.yratio, "yratio",1,100);
    gui(s.zratio, "zratio",1,100);
    gui(latticetype, "latticetype",0,6);
    gui(s.bGlideA, "glide a");
    gui(s.bGlideB, "glide b");
    gui(s.bGlideC, "glide c");
    gui(s.numX, "num x");
    gui(s.numY, "num y");
    gui(s.numZ, "num z");


    s.xratio = s.yratio = s.zratio =1;
    s.p=3;s.q=2;
    s.pbar=s.qbar=s.pqbar=0;
    s.bGlideA = s.bGlideB = s.bGlideC = 0;

    s.numX = 5;
    s.numY = 5;
    s.numZ = 5;
  }

  void onKeyDown(const gfx::Keyboard& k){
    if (k.code=='s') bSetMouse = !bSetMouse;
  }


  void onAnimate(){
    state -> point = mouse;
    state -> pose = (Pose)scene.camera;
    state -> model = scene.model;
    //maker.set(*state);
    state -> latticeType = (SpaceGroup3D<Vec>::LatticeType)latticetype;
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




