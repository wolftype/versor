/*
 * =====================================================================================
 *
 *       Filename:  xTensegrity.cpp
 *
 *    Description:  use of symmetry groups to construct tension-integrity structures
 *
 *        Version:  1.0
 *        Created:  10/09/2015 11:24:10
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

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  bool bUseX, bUseXY;

  Point mouse;
  
  float amt1 = 0;
  float amt2 = 0;

  float p;
  float q;
  bool pbar,qbar,pqbar;

  float bravaisType, latticeType, glideA, glideB, glideC, screwAB, screwBC, screwAC, screwAB_trs, screwBC_trs;
  bool bGlideA, bGlideB, bGlideC;
   
  float numX, numY, numZ;
  float xratio, yratio, zratio;
  float numStepA, numStepB, numStepC, numStepD, numStepE, numStepF;
  bool bFlipA,bFlipB,bFlipC,bFlipD,bFlipE,bFlipF;
  bool bDrawTension;
  Point pa, pb;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(bToggle,"bToggle")(bSet,"bSet")(bUseX,"bUseX");

    gui(pqbar,"pqbar");                  // abc
    gui(p,"p",0,10);                     
    gui(pbar,"pbar");                    // rotational?
    gui(q,"q",0,10);
    gui(qbar,"qbar");
    gui(bravaisType,"bravais",1,7);
    gui(latticeType,"lattice",1,5);
    gui(glideA, "glide a",0,5);
    gui(bGlideA, "glide a alt");
    gui(glideB, "glide b",0,5);
    gui(bGlideB, "glide b alt");
    gui(glideC, "glide c",0,5);
    gui(bGlideC, "glide c alt");
    gui(screwAB, "screw ab",0,5);
    gui(screwBC, "screw bc",0,5);
    gui(screwAC, "screw ac",0,5);
    gui(screwAB_trs, "screw ab trs",0,5);
    gui(screwBC_trs, "screw bc trs",0,5);
    gui(numX, "num x",1,100);
    gui(numY, "num y",1,100);    
    gui(numZ, "num z",1,100);    
    
    gui(bDrawTension,"bDrawTension");

    gui(numStepA,"numStepA",1,100)(bFlipA);
    gui(numStepB,"numStepB",1,100)(bFlipB);
    gui(numStepC,"numStepC",1,100)(bFlipC);
    gui(numStepD,"numStepD",1,100)(bFlipD);
    gui(numStepE,"numStepE",1,100)(bFlipE);
    gui(numStepF,"numStepF",1,100)(bFlipF);

    gui(xratio,"xratio",1,10);
    gui(yratio,"yratio",1,10);
    gui(zratio,"zratio",1,10);
    p=3;
    q=3;
    
    numStepA=1;
    numStepB=1;

    latticeType = 1;
    bravaisType = 7;
    pa = Construct::point(-1.5,.5,1);
    pb = Construct::point(1.5,.5,1);
    objectController.attach(&pa);
    objectController.attach(&pb);

    mColor.set(1,1,1);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    Draw(pa); Draw(pb);

    if (bMouseSet) mouse = calcMouse3D();
    if (bUseX) mouse = Construct::point(1,0,0);
    else if (bUseXY) mouse = Construct::point(1,1,0);

    Pair pair = pa ^ pb;// PAO ^ mouse;

     Vec ratioVec(xratio, yratio, zratio); 
        
     SpaceGroup3D<Vec>::Lattice lattice = {(int)bravaisType,(int)latticeType};
     SpaceGroup3D<Vec>::Glide glide = { {(int)glideA, bGlideA},{(int)glideB,bGlideB},{(int)glideC,bGlideC} };
     SpaceGroup3D<Vec>::Screw screw = { (int)screwAB, (int)screwBC, (int)screwAC, (int)screwAB_trs, (int)screwBC_trs };

     SpaceGroup3D<Vec> sg(p,q,pbar,qbar,pqbar, lattice, ratioVec, glide, screw);
    //SpaceGroup3D<Vec> sg(p,q,pbar,qbar,pqbar);

    auto resA = sg.apply(pair);
        
    auto res = sg.hang(resA, numX, numY, numZ);

    glLineWidth(2);
    gl2psLineWidth(2);
    if (bDrawTension){
    if (res.size() > 1 ){
      auto ta = (int)numStepA % res.size();
      auto tb = (int)numStepB % res.size();
      auto tc = (int)numStepC % res.size();
      auto td = (int)numStepD % res.size();
      auto te = (int)numStepE % res.size();
      auto tf = (int)numStepF % res.size();

      auto motifA = Construct::pointA(res[0]) ^ (bFlipA ? Construct::pointB(res[ta]) : Construct::pointA(res[ta]));
      auto motifB = Construct::pointA(res[0]) ^ (bFlipB ? Construct::pointB(res[tb]) : Construct::pointA(res[tb]));
      auto motifC = Construct::pointA(res[0]) ^ (bFlipC ? Construct::pointB(res[tc]) : Construct::pointA(res[tc]));
      auto motifD = Construct::pointB(res[0]) ^ (bFlipD ? Construct::pointB(res[td]) : Construct::pointA(res[td]));
      auto motifE = Construct::pointB(res[0]) ^ (bFlipE ? Construct::pointB(res[te]) : Construct::pointA(res[te]));
      auto motifF = Construct::pointB(res[0]) ^ (bFlipF ? Construct::pointB(res[tf]) : Construct::pointA(res[tf]));

      auto resA = sg.hang(sg.apply(motifA),numX,numY,numZ);
      auto resB = sg.hang(sg.apply(motifB),numX,numY,numZ);
      auto resC = sg.hang(sg.apply(motifC),numX,numY,numZ);
      auto resD = sg.hang(sg.apply(motifD),numX,numY,numZ);
      auto resE = sg.hang(sg.apply(motifE),numX,numY,numZ);
      auto resF = sg.hang(sg.apply(motifF),numX,numY,numZ);

      for (auto& i : resA) Draw(i,.3,0,.2);
      for (auto& i : resB) Draw(i,.0,.3,.3);
      for (auto& i : resC) Draw(i,.3,.5,.2);
      for (auto& i : resD) Draw(i,.1,.5,.4);
      for (auto& i : resE) Draw(i,.2,.2,.4);
      for (auto& i : resF) Draw(i,.0,.2,.5);
    }
    }

    
    glLineWidth(10);
    gl2psLineWidth(10);
    for (auto& i : res) Draw(i,.2,.2,.2);


  }


  /*-----------------------------------------------------------------------------
   *  Keyboard control
   *-----------------------------------------------------------------------------*/

  void onKeyDown(const gfx::Keyboard& k){
   // cout << k.code << endl;
    if (k.code=='s') bMouseSet = !bMouseSet;

    App::onKeyDown(k);
  }

  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
