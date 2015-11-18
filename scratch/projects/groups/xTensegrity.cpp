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

  float numStepA, numStepB;
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
    gui(bDrawTension,"bDrawTension");

    gui(numStepA,"numStepA",1,10);
    gui(numStepB,"numStepB",1,10);
    
    p=3;
    q=3;
    
    numStepA=1;
    numStepB=1;

    pa = Construct::point(-1.5,.5,1);
    pb = Construct::point(1.5,.5,1);
    objectController.attach(&pa);
    objectController.attach(&pb);
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

    SpaceGroup3D<Vec> sg(p,q,pbar,qbar,pqbar);
    
    auto res = sg.apply(pair);

    glLineWidth(2);
    gl2psLineWidth(2);
    if (bDrawTension){
    if (res.size() > 1 ){
      auto ta = (int)numStepA % res.size();
      auto tb = (int)numStepB % res.size();
      auto motifA = Construct::pointA(res[0]) ^ Construct::pointA(res[ta]);
      auto motifB = Construct::pointA(res[0]) ^ Construct::pointB(res[tb]);

      auto resA = sg.apply(motifA);
      auto resB = sg.apply(motifB);

      for (auto& i : resA) Draw(i,.3,0,.2);
      for (auto& i : resB) Draw(i,.0,.3,.3);
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
