/*
 * =====================================================================================
 *
 *       Filename:  xPointGroup3D.cpp
 *
 *    Description:  point group 3d generators
 *
 *        Version:  1.0
 *        Created:  03/03/2015 13:57:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_group.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;
  float p,q;
  bool pbar,qbar,pqbar;

  bool bSetMouse=false;
  Point mouse;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    gui(p,"p",0,10);
    gui(pbar,"pbar");
    gui(q,"q",0,10);
    gui(qbar,"qbar");
    gui(pqbar,"pqbar");
    
    p=3;q=2;
    pbar=qbar=pqbar=0;
  }

  void onKeyDown(const gfx::Keyboard& k){
    if (k.code=='s') bSetMouse = !bSetMouse;
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
    if (bSetMouse) mouse = calcMouse3D();
  
    PointGroup3D<Vec> pg(p,q,pbar,qbar,pqbar);

   // if (pg.ops.size()>2){
      for (int i=0;i<pg.ops.size();++i){
        Draw(pg.ops[i],(float)i/pg.ops.size(),0,1-(float)i/pg.ops.size());
      }
   // }
    for (auto& i : pg.sops){
     // Draw(Biv(i));
    }
    Draw(pg.a,1,0,0); Draw(pg.b,0,1,0); Draw(pg.c,0,0,1);
    
    auto res = pg( mouse ^ mouse.trs(.2,0,.1) );
    auto res2 = pg( mouse ^ mouse.trs(0,.5,.2) );
    //auto res3 = pg( Ro::round( Ro::dls(mouse,.2), Biv(.1,.2,.3).runit() ) ); // mouse.trs(0,.5,.2) );


   for (int i=0;i<res.size();++i) {
     Draw(res[i],(float)i/res.size(),0,1-(float)i/res.size());
     Draw(res2[i],(float)i/res.size(),1,1-(float)i/res.size());
     //Draw(res3[i],(float)i/res.size(),1,1-(float)i/res.size());
     
     //make a knot?
     //TorusKnot tk(3,2); tk.HF.cir = res3[i];
     //distance to original?

    }

    cout << pg.ops.size() << " " << pg.sops.size() << endl;
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
