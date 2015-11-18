/*
 * =====================================================================================
 *
 *       Filename:  xTwistField.cpp
 *
 *    Description:  twist crystallographic field
 *
 *        Version:  1.0
 *        Created:  09/30/2015 13:46:32
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
#include "form/vsr_twist.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  float amt1 = 0; float amt2 = 0;
  bool bToggle, bSet;


  float p=2;
  float q=2;
  bool pbar=false;
  bool qbar=false; 
  bool pqbar=false;

  float bravaisType, latticeType;
  
  bool bGlideA, bGlideB, bGlideC;
  float glideA, glideB, glideC;
  float screwAB, screwBC, screwAC;
  float screwAB_trs,screwBC_trs;


  TwistField twf;

  Point mouse;
   
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(bToggle,"bToggle")(bSet,"bSet");

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
    

    for (auto& i : twf.frame()){
      objectController.attach(&i);    
    }   

    amt1 = .5;
    amt2 = .5;

    bToggle =true;
   }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

      bShadedOutput = false;

      if (bSet) mouse = calcMouse3D();

     //motif
     auto dls = Round::sphere(mouse, amt1);
     vector<Pnt> pnt;
     float phi = PIOVERTWO * amt2;
     for (int i=0;i<4;++i){
       float t = (float)TWOPI * i/4.0;
       auto rota = Gen::rot(t,-phi);
       auto rotb = Gen::rot(t,phi);
       pnt.push_back( Construct::point(dls, Vec::x.spin( rota ) ) );
       pnt.push_back( Construct::point(dls, Vec::x.spin( rotb ) ) );
    }


    //space group
    SpaceGroup3D<Vec>::Lattice lattice = {(int)bravaisType,1};
    SpaceGroup3D<Vec>::Glide glide = { {(int)glideA, bGlideA},{(int)glideB,bGlideB},{(int)glideC,bGlideC} };
    SpaceGroup3D<Vec>::Screw screw = { (int)screwAB, (int)screwBC, (int)screwAC, (int)screwAB_trs, (int)screwBC_trs };
    SpaceGroup3D<Vec> sg(p,q,pbar,qbar,pqbar,lattice,Vec(1,1,1),glide,screw);
    //symmetry
    auto group = sg.apply( pnt );
    auto sgroup = sg.hang( group, 3,3,3);

    if (bToggle) twf.set(sgroup);

    for (auto& i : twf.frame()) DrawB(i);

    auto warp = twf.apply( sgroup );

      for (int i=0;i<warp.size(); i+=pnt.size() ) {
         auto a = warp[i]; 
         auto b = warp[i+1]; 
         auto c = warp[i+2]; 
         auto d = warp[i+3];
         auto e = warp[i+4];  
         auto f = warp[i+5];
         auto g = warp[i+6];
         auto h = warp[i+7];
        
         glBegin(GL_QUADS);
        
         glColor3f(.6,.2,.2);
         GL::normal( ((c-a)^(b-a)).duale().unit().begin() );
         GL::Quad(a,c,d,b);         
         glColor3f(.2,.6,.6);
         GL::normal( ((e-c)^(d-c)).duale().unit().begin() );
         GL::Quad(c,e,f,d);         
         glColor3f(.6,.2,.6);
         GL::normal( ((g-e)^(f-e)).duale().unit().begin() );
         GL::Quad(e,g,h,f);         
         glColor3f(.1,1,.4);
         GL::normal( ((a-g)^(h-g)).duale().unit().begin() );
         GL::Quad(g,a,b,h);  
         glColor3f(.3,.3,.6);
         GL::normal( ((c-a)^(g-a)).duale().unit().begin() );
         GL::Quad(a,c,e,g);         
         glColor3f(.8,1,.2);
         GL::normal( ((d-b)^(h-b)).duale().unit().begin() );
         GL::Quad(b,d,f,h);         

         glEnd();

         glBegin(GL_LINES);
         glColor3f(.6,0,0);
         GL::Line(a,b);
         GL::Line(c,d);
         GL::Line(e,f);
         GL::Line(g,h);
         GL::Line(a,c);
         GL::Line(c,e);
         GL::Line(g,a);
         GL::Line(b,d);
         GL::Line(d,f);
         GL::Line(f,h);
         GL::Line(h,b);
         glEnd();

      }



  }

  void onKeyDown(const gfx::Keyboard& k){
   // cout << k.code << endl;
    if (k.code=='s') bSet = !bSet;

    App::onKeyDown(k);
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
