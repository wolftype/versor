/*
 * =====================================================================================
 *
 *       Filename:  xCircleNet5.cpp
 *
 *    Description:  circle nets from curvx and curvy
 *
 *        Version:  1.0
 *        Created:  11/16/2015 15:46:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_cyclide.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  
  float amtX = 0;
  float amtX2 = 0;
  float amtY = 0;


  float spanX = 0;
  float spanY = 0;

  Point mouse;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amtX,"amtX",-100,100)(amtY,"amtY",-100,100)(amtX2,"amtX2",-100,100);
    gui(spanX,"spanX",-100,100)(spanY,"spanY",-100,100);
    
    
    gui(bToggle,"bToggle")(bSet,"bSet");
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    if (bMouseSet) mouse = calcMouse3D();

    auto sa = Construct::sphere(0,1,0); //Dlp(0,0,1,0);//
    auto ptmp = Construct::point(sa,Vec::z);//Construct::point(-1,3,0);
    Contact tmp (ptmp, sa);
    
    auto sbx = sa.spin( Gen::bst(tmp.tnv*amtX) );
    auto sby = sa.spin( Gen::bst(tmp.tnv*amtY) );
    
    auto pa = Construct::point(sbx,Vec::z);    
    auto pb = Construct::point(sbx,Vec::z.rotate(-Biv::xz*spanX/2));   
    auto pd = Construct::point(sby,Vec::z.rotate(Biv::yz*spanY/2));
    
    Contact ca (pa,sbx);
    Contact cb (pb,sbx);
    Contact cd (pd,sby);

    auto cir = pa ^ pb ^ pd;
    Draw(cir,0,1,1);
   // auto pc = (cir.dual() ^ 
    
   auto sbx2 = Sphere( Round::carrier( cd.tnv.undual() ) ).spin( Gen::bst(cd.tnv*amtX2) ).dual();
   auto pc = Construct::pointB( (cir.dual() ^ sbx2).dual() );

   Contact cc (pc, sbx2);
   
   CyclideQuad cyclide(pa,pb,pc,pd);

   

    Draw(pa,1,0,0);
    Draw(pb,1,0,0);
    Draw(pc,1,0,0);
    Draw(pd,1,0,0);
    
    Draw( ca.tnv,0,0,1);
    Draw( cb.tnv,0,0,1);
    Draw( cc.tnv,0,0,1);
    Draw( cd.tnv,0,0,1);

    Draw(sbx,.3,0,1,.2);
    Draw(sby,0,.3,1,.2);
    Draw(sbx2,.3,.3,1,.2);
  //  auto pb = Construct::point(-1,3,0);
  

  }


//  /*-----------------------------------------------------------------------------
//   *  Keyboard control (hit 's' to toggle mouse tracking)
//   *-----------------------------------------------------------------------------*/
//  void onKeyDown(const gfx::Keyboard& k){
//    switch (k.code){
//      case 's': bMouseSet = !bMouseSet;
//    }
//
//    /// Default
//    App:onKeyDown(k);
//  }

  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
