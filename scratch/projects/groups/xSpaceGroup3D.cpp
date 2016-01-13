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
  float screwAB, screwBC, screwAC;
  float screwAB_trs,screwBC_trs;

  float transA, transB, transC;

  Point point = Round::null(0,0,0);

  float xratio,yratio,zratio;
  int numX, numY, numZ;

  float bravaisType;
  float latticeType;

  float angleB, angleC;

  //drawing
  bool bDrawRoots,bDrawVectors,bDrawScrews, bDrawGlides, bDrawCell, bDrawMotif, bDrawDiamond, bLights;
  float alpha,radius,width,screwIterStart, screwIter, cellXoffset;
};


void drawState(State * state){


     State& s = *state;

     if (s.bLights) GL::lightsOn();
     else GL::lightsOff();


     Vec ratioVec(s.xratio,s.yratio,s.zratio); 
        
     SpaceGroup3D<Vec>::Lattice lattice = {(int)s.bravaisType,(int)s.latticeType};
     SpaceGroup3D<Vec>::Glide glide = { {(int)s.glideA, s.bGlideA},{(int)s.glideB,s.bGlideB},{(int)s.glideC,s.bGlideC} };
     SpaceGroup3D<Vec>::Screw screw = { (int)s.screwAB, (int)s.screwBC, (int)s.screwAC, (int)s.screwAB_trs, (int)s.screwBC_trs };

     SpaceGroup3D<Vec> sg(s.p,s.q,s.pbar,s.qbar,s.pqbar, lattice, ratioVec, glide, screw);
   
     //Angles for Triclinic and Monoclinic lattices 
     sg.angleB(s.angleB);
     sg.angleC(s.angleC);
     
     //sg.print();    

     Vec v(0,0,0);

     if (s.bDrawRoots){
      DrawAt( sg.a, v, 1,0,0);
      DrawAt( sg.b, v, 0,1,0);
      DrawAt( sg.c, v, 0,0,1);
     }
    
    if (s.bDrawScrews){
      
      for (auto& i : sg.scrops){
         auto dll = Gen::log(i);
         Draw( dll, 0,0,0 );

        for (auto& input : sg.apply(s.point) ){
         glBegin(GL_LINE_STRIP);
         for (int j=0; j<50;++j){
          float t = s.screwIterStart + s.screwIter * (float)j/50;
          auto p = input.spin( Gen::mot(dll*t) );
          GL::vertex(p.begin());
         }
         glEnd();
        }
      }

    }
      //cell info
    if (s.bDrawVectors){
     //cout << sg.mB_dir << endl;
     glColor3f(0,0,0);
     
     glPushMatrix();
     glTranslatef(s.cellXoffset,0,0);

     if(s.bDrawCell){
       Glyph::Line( Vec(0,0,0), sg.mB_dir);
       Glyph::Line( sg.mA_dir, sg.mA_dir + sg.mB_dir);
       Glyph::Line( sg.mC_dir, sg.mC_dir+sg.mB_dir);
       Glyph::Line( sg.mC_dir + sg.mA_dir, sg.mC_dir + sg.mA_dir +sg.mB_dir);

       Glyph::Line( Vec(0,0,0),sg.mC_dir);
       Glyph::Line( sg.mA_dir, sg.mA_dir + sg.mC_dir);
       Glyph::Line( sg.mB_dir, sg.mC_dir+sg.mB_dir);
       Glyph::Line( sg.mB_dir + sg.mA_dir, sg.mC_dir + sg.mA_dir +sg.mB_dir);

       Glyph::Line( Vec(0,0,0), sg.mA_dir);
       Glyph::Line( sg.mB_dir, sg.mA_dir + sg.mB_dir);
       Glyph::Line( sg.mC_dir, sg.mC_dir+sg.mA_dir);
       Glyph::Line( sg.mB_dir + sg.mC_dir, sg.mC_dir + sg.mA_dir +sg.mB_dir);
     
     }

     Draw( sg.mB_length,0,1,0);
     Draw( sg.mC_length,0,0,1);
     Draw( sg.mA_length, 1,0,0);

     if (s.bDrawGlides){
       auto glideA = sg.glideA(); 
       auto glideB = sg.glideB(); 
       auto glideC = sg.glideC(); 
       if (glideA.wt() > 0) Draw(glideA,.5,0,.5);
       if (glideB.wt() > 0) Draw(glideB,.5,.5,0);
       if (glideC.wt() > 0) Draw(glideC,0,.5,.5);
    }

     glPopMatrix();

    }
    
     auto dls = Round::sphere(s.point, s.radius);
     vector<Point> pnt;
 
    if (s.bDrawDiamond){
   for (int i=0;i<4;++i){
      float t = (float)i/4;
      Rotor r = Gen::rot( Biv::xz * PI * t);
      pnt.push_back( Construct::point(dls, Vec::x.spin( r ) ) );
   }
   pnt.push_back( s.point.translate(-.1,.25,0) );
   pnt.push_back( s.point.translate(.1,-.25,0) );

   auto res = sg.apply( pnt );
   auto latticeRes = sg.hang( res , s.numX,s.numY,s.numZ);

   float t= s.alpha;

  if (s.bDrawMotif){
    for (int i=0;i<latticeRes.size(); i+=pnt.size() ) {

       auto a = latticeRes[i]; 
       auto b = latticeRes[i+1]; 
       auto c = latticeRes[i+2]; 
       auto d = latticeRes[i+3];
       auto e = latticeRes[i+4];  
       auto f = latticeRes[i+5];
       glColor4f(.6,.2,.2,t);
      // GL::color( ca.begin() );
       Glyph::Triangle(a,b,e);
       glColor4f(.2,.6,.6,t);
      // GL::color( cb.begin() );
       Glyph::Triangle(b,c,e);
       glColor4f(.6,.2,.6,t);
       Glyph::Triangle(c,d,e);
       glColor4f(.3,.3,.6,t);
       Glyph::Triangle(d,a,e);
       glColor4f(.4,1,.4,t);
       Glyph::Triangle(a,b,f);
       glColor4f(.4,1,.3,t);
       Glyph::Triangle(b,c,f);
       glColor4f(.3,1,.1,t);
       Glyph::Triangle(c,d,f);
       glColor4f(.1,1,.4,t);
       Glyph::Triangle(d,a,f);

    }
  }

      } else {
      float phi = PIOVERTWO * s.width;
      auto angle = Gen::rot( Biv::xy * s.angleB);
      for (int i=0;i<4;++i){
        float t = (float)TWOPI * i/4.0;
        auto rota = Gen::rot(t,-phi);
        auto rotb = Gen::rot(t,phi);
        pnt.push_back( Construct::point(dls, Vec::x.spin( angle * rota ) ) );
        pnt.push_back( Construct::point(dls, Vec::x.spin( angle * rotb ) ) );

      }

       auto res = sg.apply( pnt );
       auto latticeRes = sg.hang( res , s.numX,s.numY,s.numZ);

     if (s.bDrawMotif){
      for (int i=0;i<latticeRes.size(); i+=pnt.size() ) {
         auto a = latticeRes[i]; 
         auto b = latticeRes[i+1]; 
         auto c = latticeRes[i+2]; 
         auto d = latticeRes[i+3];
         auto e = latticeRes[i+4];  
         auto f = latticeRes[i+5];
         auto g = latticeRes[i+6];
         auto h = latticeRes[i+7];
        
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
  bool bDrawOrtho;

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

    gui(s.pqbar,"pqbar");                  // abc
    gui(s.p,"p",0,10);                     
    gui(s.pbar,"pbar");                    // rotational?
    gui(s.q,"q",0,10);
    gui(s.qbar,"qbar");

    gui(s.angleB,"b incline", -PI,PI);     //clinics
    gui(s.angleC,"c incline",-PI,PI);
    gui(s.xratio, "xratio",1,100);         //scaling
    gui(s.yratio, "yratio",1,100);
    gui(s.zratio, "zratio",1,100);
    gui(s.latticeType, "latticeType",1,5); //Primitive, Body, etc
    gui(s.bravaisType, "bravaisType",1,7); //Triclinic, Monoclinic, etc
   // gui(s.transA,"trs A",-3,3);             //polynomial trs
  //  gui(s.transB,"trs B",-3,3);
  //  gui(s.transC,"trs C",-3,3);
    gui(s.glideA, "glide a",0,5);
    gui(s.bGlideA, "glide a alt");
    gui(s.glideB, "glide b",0,5);
    gui(s.bGlideB, "glide b alt");
    gui(s.glideC, "glide c",0,5);
    gui(s.bGlideC, "glide c alt");
    gui(s.screwAB, "screw ab",0,5);
    gui(s.screwBC, "screw bc",0,5);
    gui(s.screwAC, "screw ac",0,5);
    gui(s.screwAB_trs, "screw ab trs",0,5);
    gui(s.screwBC_trs, "screw bc trs",0,5);
    gui(s.numX, "num x",1,100);
    gui(s.numY, "num y",1,100);
    gui(s.numZ, "num z",1,100);
    gui(s.bDrawRoots,"bDrawRoots");
    gui(s.bDrawVectors,"bDrawVectors");
    gui(s.bDrawGlides,"bDrawGlides");
    gui(s.bDrawScrews,"bDrawScrews");

    gui(s.bDrawCell,"bDrawCell");
    gui(s.bDrawMotif,"bDrawMotif");
    gui(s.bDrawDiamond,"bDrawDiamond");
    gui(bDrawOrtho,"bDrawOrtho");
    gui(s.bLights,"bLights");
    gui(s.alpha, "alpha");
    gui(s.width,"width");
    gui(s.radius,"radius");
    gui(s.screwIterStart,"screwIterStart",0,-10);
    gui(s.screwIter,"screwIter",0,10);
    gui(s.cellXoffset,"cellXoffset",-10,10);

    s.xratio = s.yratio = s.zratio =1;
    s.p=3;s.q=3;
    s.pbar=s.qbar=s.pqbar=0;
    s.glideA = s.glideB = s.glideC = 0;
    s.latticeType = 1;
    s.bravaisType = 1;

    s.numX = 1;
    s.numY = 1;
    s.numZ = 1;

    s.alpha =1;

    s.bDrawRoots = false;
    s.bDrawVectors= true;
    s.bDrawCell= true;
    s.bDrawMotif = true;
    s.bDrawGlides= true;

    s.bDrawMotif = true;
    bDrawOrtho=false;

    mColor.set(1,1,1);
  }

  void onKeyDown(const gfx::Keyboard& k){
   // cout << k.code << endl;
    if (k.code=='s') bSetMouse = !bSetMouse;

    App::onKeyDown(k);
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
    scene.camera.lens.bOrtho= bDrawOrtho;

    drawState(state);      
  }  
};



int main (int argc, char *argv[]){

    MyApp app;
    app.start();
    return 0;
}




