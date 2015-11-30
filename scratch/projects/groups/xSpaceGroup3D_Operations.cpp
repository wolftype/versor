/*
 * =====================================================================================
 *
 *       Filename:  xSpaceGroup3D_Operations.cpp
 *
 *    Description:  create a field of operations
 *
 *        Version:  1.0
 *        Created:  11/02/2015 13:39:18
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
#include "form/vsr_field.h"
#include "form/vsr_graph.h"
#include "form/vsr_twist.h"

using namespace vsr;
using namespace vsr::cga;


vector<Vertex> SphereMesh(int w=20, int h=20){


  vector<Vertex> v;

  for (int i = 0;i<w;++i){
    for (int j=0;j<h;++j){
      auto tu = (float)i/w * TWOPI;
      auto tv = -PIOVERTWO + (float)j/h * PI;
      auto rot = Gen::rot(tu,tv);    
      v.push_back(  Vec3f( Vec::x.spin(rot) ) );         
    }
  }

  return v;
}


vector<Vertex> CylinderMesh(int w=20, int h=20, float radius = 1.0, float height = 2.0){


  vector<Vertex> v;

  for (int i = 0;i<w;++i){
    for (int j=0;j<h;++j){
      auto tu = (float)i/w * PI;
      auto tv = -height/2.0 + (float)j/h * height;
      auto rot = Gen::rot( Biv::xz * tu);    
      v.push_back(  Vec3f( Vec::x.spin(rot) * radius ) + Vec3f(0,tv,0));         
    }
  }

  return v;
}

   template<> 
   void Renderable<vsr::HEGraph<Vertex>>::DrawImmediate( const vsr::HEGraph<Vertex>& graph){
     glBegin(GL_TRIANGLES);
     int iter =0;
     bool bChecker = false;
     for (auto& i : graph.face()){
          iter++;
          float t = (float)iter/graph.face().size(); 
          auto& a = i->a();
          auto& b = i->b();
          auto& c = i->c(); 
          glColor3f(bChecker,bChecker,bChecker);
          GL::normal( a.Norm[0], a.Norm[1],a.Norm[2]);//.begin() );
          GL::vertex( a.Pos[0], a.Pos[1], a.Pos[2] );
          GL::normal( b.Norm[0], b.Norm[1], b.Norm[2] );
          GL::vertex( b.Pos[0], b.Pos[1], b.Pos[2] );
          GL::normal( c.Norm[0], c.Norm[1], c.Norm[2] );
          GL::vertex( c.Pos[0], c.Pos[1], c.Pos[2] );
          if (!(iter&1)) bChecker = !bChecker;
     }
     glEnd();
 //    glColor3f(0,.7,.7);
 //    glBegin(GL_LINES);
 //    for (auto& i : graph.face()){
 //         auto& a = i->a();
 //         auto& b = i->b();
 //         auto& c = i->c(); 
 //         //glColor4f(.2,1,.2,.7);
 //      //   GL::normal( a.Norm[0], a.Norm[1],a.Norm[2]);//.begin() );
 //         GL::vertex( a.Pos[0], a.Pos[1], a.Pos[2] );
 //      //   GL::normal( b.Norm[0], b.Norm[1], b.Norm[2] );
 //         GL::vertex( b.Pos[0], b.Pos[1], b.Pos[2] );
 //     //    GL::normal( a.Norm[0], a.Norm[1],a.Norm[2]);//.begin() );
 //     //    GL::vertex( a.Pos[0], a.Pos[1], a.Pos[2] );
 //     //    GL::normal( c.Norm[0], c.Norm[1], c.Norm[2] );
 //     //    GL::vertex( c.Pos[0], c.Pos[1], c.Pos[2] );
 //    }
 //    glEnd();
     
  }



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

  bool bSphere = false;

  float xratio,yratio,zratio;
  int numX, numY, numZ;

  float bravaisType;
  float latticeType;

  float angleB, angleC;
  float lightX,lightY;

  float cellsWidth, cellsHeight;
  float totalHeight;

  float period, pitch;
  bool bUseMotor;

  //drawing
  bool bDrawRoots,bDrawVectors,bDrawScrews, bDrawGlides, bDrawCell, bDrawMotif, bDrawDiamond, bLights;
  float alpha,radius,width,screwIterStart, screwIter, cellXoffset;
};


void drawState(State * state){


    
    State& s = *state;
    GL::lightPos(s.lightX, 0, s.lightY );

   //  if (s.bLights) GL::lightsOn();
   //  else GL::lightsOff();


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
    

      //cell info
     if (s.bDrawRoots){
      DrawAt( sg.a, v, 1,0,0);
      DrawAt( sg.b, v, 0,1,0);
      DrawAt( sg.c, v, 0,0,1);
     }

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
    
    auto tpair = Round::produce( Round::sphere(s.point, -s.radius), Vec::y.rot(Biv::xy * s.angleB) );
    vector<Pair> pair;
    pair.push_back( tpair ); 
   
    auto groupA = sg.apply( pair );
    auto groupB = sg.hang( groupA , s.numX,s.numY,s.numZ);

    auto tdll = Twist::Along( (tpair ^ Inf(1)).dual(), s.period, s.pitch);
    auto tmp = sg.apply( tdll );
    auto groupC = sg.hang( tmp, s.numX, s.numY, s.numZ);

    if (s.bDrawMotif){
     for (auto& i : groupB){
        Draw(i,0,1,0);
     } 
    }

    int w = s.cellsWidth;
    int h = s.cellsHeight;
    //Mesh mesh =  Mesh::Sphere();
    auto mesh = s.bSphere ? SphereMesh(w,h) : CylinderMesh(w,h,1,s.totalHeight);
    

    HEGraph<Vertex> graph;

    for (int i=0;i<mesh.size();++i){
       auto pt =  Round::point(mesh[i].Pos[0], mesh[i].Pos[1], mesh[i].Pos[2]);
       Pair pair; 
       DualLine dll;
      for (int j =0;j<groupB.size();++j){
       auto dist = Round::sqd( Round::location(groupB[j]), pt );
       auto wt = 1.0/(dist+.01);
       pair += groupB[j] * wt;
       dll += groupC[j] * wt;
      }

      auto npt = Round::location( pt.spin( Gen::boost(pair * s.amt) ) );
      auto mpt = pt.spin( Gen::motor( dll * s.amt) );
      mesh[i].Pos = s.bUseMotor ? Vec3f(mpt[0],mpt[1],mpt[2]) : Vec3f(npt[0],npt[1],npt[2]);

    }


    graph.UV(w,h,mesh, true);//.vertex());

   for (auto& i : graph.node()){
   
     auto va = i->data().Pos;
     auto vb = i->edge -> a().Pos;
     auto vc = i->edge -> next -> a().Pos;
     Vec3f normal =(vb-va).cross(vc-va).unit() ;//(mesh[i+2].Pos - mesh[i].Pos).cross( mesh[i+1].Pos - mesh[i].Pos).unit();
   
     i->data().Norm = normal;
   }
    

    Draw(graph, .2, .6,.6);

 //   mesh.drawElements();

 //   mesh.mode(GL::L);
    
 //   glColor3f(1,0,0);
 //   mesh.drawElements();

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
  float lineWidth;

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
    gui(s.bSphere,"bSphere");
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

    gui(s.lightX, "lightX",-10,10);
    gui(s.lightY, "lightY",-10,10);

    gui(s.bDrawCell,"bDrawCell");
    gui(s.bDrawMotif,"bDrawMotif");
    gui(s.bDrawDiamond,"bDrawDiamond");
    gui(bDrawOrtho,"bDrawOrtho");
    gui(s.bLights,"bLights");
    gui(s.alpha, "alpha");
    gui(s.width,"width");
    gui(s.radius,"radius",-10,10);
    gui(s.screwIterStart,"screwIterStart",0,-10);
    gui(s.screwIter,"screwIter",0,10);
    gui(s.cellXoffset,"cellXoffset",-10,10);
    gui(lineWidth,"lineWidth",0,10);
    gui(s.cellsWidth,"cellswidth",10,50);
    gui(s.cellsHeight,"cellsheight",10,50);
    gui(s.totalHeight,"totalHeight",1,10);
    gui(s.period, "period",-100,100);
    gui(s.pitch, "pitch",-100,100);
    gui(s.bUseMotor,"usemotor");

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

    glLineWidth(lineWidth);
    gl2psLineWidth(lineWidth);

    if (bSetMouse) mouse = calcMouse3D();
  //  scene.camera.lens.bOrtho= bDrawOrtho;

    drawState(state);      
  }  
};



int main (int argc, char *argv[]){

    MyApp app;
    app.start();
    return 0;
}




