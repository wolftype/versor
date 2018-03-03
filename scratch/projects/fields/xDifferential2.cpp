/*
 * =====================================================================================
 *
 *       Filename:  xDifferential2.cpp
 *
 *    Description:  go around edges (uses boundaries)
 *
 *        Version:  1.0
 *        Created:  02/23/2015 16:36:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_twist.h"
#include "form/vsr_differential.h"
#include "form/vsr_graph.h"
#include "form/vsr_group.h"

using namespace vsr;
using namespace vsr::cga;


//Per Vertex Geometric Data 
struct MyData {
  
  Point pnt;
  MyData(const Point& p=point(0,0,0) ) : pnt(p) { }

  void reset(){
    normal = Vec(0,0,0);
    curl = Biv(0,0,0);
    dn = Vec(0,0,0);
  }
  Vec normal = Vec(0,0,0);       //normal (gradient of position)
  Vec dn = Vec(0,0,0);           //derivative of normal

  Biv curl = Biv(0,0,0);         //gradient of gradient
  float kg = 0;
  float mean =0;   //gaussian and mean;
  float deficit;
  
  vector<Simplicial2> simplex;  //simplicial domain

};


struct MyApp : App {

  //mouse position
  Point mouse;
  //data
  MeshData<MyData> mesh;
  //half-edge pointers to data
  HEGraph<MyData> graph;

  float amt;
  float wt;
  bool bUseCotan;
  bool bExterior;
  bool bDivideByArea;
  bool bDrawMean;
  bool bDrawNormals;
  bool bDrawReciprocals;

  //Control
  bool bTrackMouse;


  void setup(){
    bindGLV();
    
    gui(amt,"amt",-100,100);               //<-- amt of transform
    gui(wt,"wt",-100,100);                 //<-- wt of diff
    gui(bUseCotan,"bUseCotan");            //<-- use TWOPI deficit formula
    gui(bExterior,"bExterior");            //<-- use vector derivatives then exterior
    gui(bDivideByArea,"bDivideByArea");    //<-- use vector derivatives then exterior
    gui(bDrawMean,"bDrawMean");            //<-- draw mean color (vs gaussian) 
    gui(bDrawNormals,"bDrawNormals");      //<-- draw normals
    gui(bDrawReciprocals,"bDrawReciprocals");     //<-- draw normals

    //0. set starting parameters
    wt = 1.0;
    amt = 10.0;
    bDrawNormals = true;
    scene.camera.pos(0,0,10);

    //1. Generate SpaceGroup Tessellate, Hang on lattice, Graph
    int w=20; int h=w;
    SpaceGroup2D<Vec> sg(3,1,true);
    auto tmp = sg.hang( point(0,0,0), w, h);
    for(auto& i : tmp){
      mesh.add( MyData(i));
    }
    mesh.store();
    graph.UV(w,h,mesh);
    //per node, add valence number of simplices
    for(auto& i : graph.node() ){
      int num = i->valence().size();
      i->data().simplex = vector<Simplicial2>(num);
    }
  }
  
  void onKeyDown(const gfx::Keyboard& k){
    if (k.code=='s') bTrackMouse = !bTrackMouse;
  }

  void onAnimate(){

    if (bTrackMouse) mouse = calcMouse3D();

    Frame frame(mouse[0], mouse[1], .5);
    //Generator
    auto dll = Twist::Along( frame.dly(), amt,0);
    //transform data
    for(int i=0;i<mesh.num();++i){
      auto& v = mesh[i].pnt; 
      auto& s = mesh.store(i).pnt;
      double dist = 1.0/(.1+fabs( Round::sqd(frame.pos(), s ) ) );
      auto motor = Gen::mot(dll*dist);
      v = s.spin(motor);
    }

    //per node
    //1. per node, calculate reciprocal simplices in neighborhood
    //2. (use edge node as root?)
    for(auto& i : graph.node() ){
     
      auto& v = i->data();
      v.reset();
      auto& a = v.pnt;
      auto e = i->valence();        //per edge
      float area=0;
      float deficit = TWOPI;
      
      for (int j=0;j<e.size();++j){
       
        auto& b = e[j]->a().pnt;
        auto& c = e[j]->next->a().pnt;
        //auto simplex = Simplicial2(Vec(b),Vec(c),Vec(a));
        auto simplex = Simplicial2(Vec(b),Vec(c),Vec(a));
        
        v.normal += simplex.derivative0(Vec(b),Vec(c),Vec(a));

        area += simplex.area;       //sum area
        
        v.simplex[j] = simplex;
       
        //for comparison: the classic way of defining curvature as deficit from TWOPI 
        deficit -= acos( ( ( Vec(c-a).unit() )<=( (Vec(b-a).unit()) ) )[0] );

      }
      v.deficit = deficit*wt;
      v.mean = v.normal.norm()/area; //divide by area?
      v.normal = v.normal.unit();
      Vec orient = v.simplex[0].tpss.duale();
      // if ( (v.normal <= orient)[0]  < 0 ) v.normal = -v.normal;
    }


    //now calculate second derivative (i.e. riemann curvature)
    for (auto& i : graph.node() ){
      auto& v = i->data();
      auto& na = v.normal;
      auto& simplex = i->data().simplex; //simplex is already built
      auto e = i->valence();
      float area = 0;
      float mean = 0;
      for (int j=0;j<e.size();++j){

         auto& nb = e[j]->a().normal;
         auto& nc = e[j]->next->a().normal;
         v.curl += simplex[j].derivative(na,nb,nc);
        
         //nope, unless summing around area
         v.dn += simplex[j].derivative0(nb,nc,na);

         area += simplex[j].area;

         //for comparison, use of cotan formula 
        
      }
      
      v.dn *= wt; 
      v.curl *= wt;
      if (bDivideByArea){ v.dn /= area; v.curl /= area; v.deficit /=area; }
      auto tmp0 = -(v.dn.duale() <= !(v.normal.duale())) [0];
      auto tmp = -(v.curl <= !(v.normal.duale()))[0];
      v.kg= bUseCotan ? v.deficit : tmp0;

     // v.kg= bUseCotan ? v.deficit : tmp; 
    }
    
  }

  void onDraw(){

    if(bDrawReciprocals){
      for (auto& i : graph.node() ){
        auto& v = i->data();
        for (auto& j : v.simplex){
          DrawAt(j.ra, v.pnt,0,1,0);
          DrawAt(j.rb, v.pnt,0,1,0);
        }
      }
    }
    if(bDrawNormals){
      for (auto& i : graph.node() ){
        auto& v = i->data();
        DrawAt(v.normal, v.pnt,0,v.kg < 0,1);
      }
    }

    for (auto& i : graph.edge() ){
      GL::color(1,0,0);
      Glyph::Line( i->a().pnt,i->b().pnt);
    }

    glBegin( GL_TRIANGLES );
      for(auto& i : graph.face() ){
        auto& a = i->a();
        auto& b = i->b();
        auto& c = i->c();
        float ta = bDrawMean ? fabs(a.mean) : fabs(a.kg);
        float tb = bDrawMean ? fabs(b.mean) : fabs(b.kg);
        float tc = bDrawMean ? fabs(c.mean) : fabs(c.kg);
        bool ba = bDrawMean ? a.mean < 0 : a.kg < 0;
        bool bb = bDrawMean ? b.mean < 0 : b.kg < 0;
        bool bc = bDrawMean ? c.mean < 0 : c.kg < 0;
        GL::color( ta, ba, 1-ta,.5);
        GL::normal( a.normal.begin());
        GL::vertex( a.pnt.begin() );
        GL::color( tb, bb, 1-tb,.5);
        GL::normal( b.normal.begin());
        GL::vertex( b.pnt.begin() );
        GL::color( tc, bc, 1-tc,.5);
        GL::normal( c.normal.begin());
        GL::vertex( c.pnt.begin() );
      }
    glEnd();
  }


};

int main(){
  MyApp app;
  app.start();
  return 0;
}
