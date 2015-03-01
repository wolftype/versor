/*
 * =====================================================================================
 *
 *       Filename:  xDifferential.cpp
 *
 *    Description:  differential geometry
 *
 *        Version:  1.0
 *        Created:  02/20/2015 12:55:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vsr_cga3D.h"   
#include "vsr_twist.h"
#include "vsr_cga3D_frame.h"
#include "vsr_differential.h"
#include "vsr_graph.h"
#include "vsr_group.h"
#include "vsr_cga3D_app.h"

using namespace vsr;
using namespace vsr::cga3D;


//Per Vertex Geometric Data 
struct MyData {
  
  Point pnt;
  MyData(const Point& p=point(0,0,0) ) : pnt(p) { }

  void reset(){
    normal = Vec(0,0,0);
    curl = Biv(0,0,0);
  }
  Vec normal = Vec(0,0,0);       //normal (gradient of position)
  Biv curl = Biv(0,0,0);         //gradient of gradient
  float kg = 0;
  float mean =0;   //gaussian and mean;
  
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
  bool bNormalExterior;
  bool bDivideByArea;
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
    gui(bNormalExterior,"bExterior");            //<-- use vector derivatives then exterior
    gui(bDivideByArea,"bDivideByArea");    //<-- use vector derivatives then exterior
    gui(bDrawNormals,"bDrawNormals");      //<-- draw normals
    gui(bDrawReciprocals,"bDrawReciprocals");     //<-- draw normals

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
      double dist = 1.0/(.1+fabs( Ro::sqd(frame.pos(), s ) ) );
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
      for (int j=0;j<e.size();++j){
        auto& b = e[j]->a().pnt;
        auto& c = e[j]->next->a().pnt;
        auto simplex = Simplicial2(a,b,c);
        v.simplex[j] = simplex;
      }
      float area =0;
      Vec ct(0,0,0:
      for (int j=0;j<e.size();++j){
        int next = (j<e.size()-1) ? j+1 : 0; 
        auto& b = e[j]->a().pnt;
        auto& c = e[j]->next->a().pnt;
        auto& nb = e[next]->a().pnt;
        auto& nc = e[next]->next->a().pnt;
        
        auto vd = v.simplex[j].derivative(Vec(a),Vec(b),Vec(c));
        
        auto vda = v.simplex[j].derivative0(Vec(a),Vec(b),Vec(c));
        auto vdb = v.simplex[next].derivative0(Vec(a),Vec(nb),Vec(nc));

        ct += v.simplex[j].cotan( v.simplex[next] );
        
      //  v.normal +=  (vda^vdb).duale(); //normal as exterior derivative of position;
        v.normal += bNormalExterior ? (vda^vdb).duale() : vd.duale();
        area+=v.simplex[j].area; 
      }


      /*-----------------------------------------------------------------------------
       *  
       *-----------------------------------------------------------------------------*/
      v.mean = v.normal.norm()/area; //divide by area?
      v.normal = v.normal.unit();

      if (bUseCotan){
        v.mean = (ct/area).norm();
        v.normal = ct.unit();
      }

    }

    //now calculate second derivative (i.e. riemann curvature)
    for (auto& i : graph.node() ){
      auto& v = i->data();
      auto& na = v.normal;
      auto& simplex = i->data().simplex; //simplex is already built
      auto e = i->valence();
      float area = 0;
      float deficit = TWOPI;
      for (int j=0;j<e.size();++j){
         int next = (j<e.size()-1) ? j+1 : 0;
         auto& nb = e[j]->a().normal;
         auto& nc = e[j]->next->a().normal;
         auto& nnb = e[next]->a().normal;
         auto& nnc = e[next]->next->a().normal;
                  
         /*-----------------------------------------------------------------------------
          *  Use "Exterior Derivative"
          *-----------------------------------------------------------------------------*/
         auto vv = simplex[j].derivative(na,nb,nc); 
         
         auto va = simplex[j].derivative0(na,nb,nc); 
         auto vb = simplex[next].derivative0(na,nnb,nnc); 
       
         //v.curl += va^vb;
         v.curl += bExterior ? va^vb : vv;
         area += simplex[j].area;
         deficit -= simplex[j].deficit();
      }
      v.curl *= wt; 
      deficit *= wt;  
      if (bDivideByArea){ v.curl /= (2*area); deficit /=area; }
      v.kg= bUseCotan ? deficit : -(v.curl <= !(v.normal.duale()))[0];
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
        DrawAt(v.normal, v.pnt,0,v.kg>0,1);
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
        GL::color( fabs(a.kg), a.kg > 0 ,1-fabs(a.kg));
        GL::normal( a.normal.begin());
        GL::vertex( a.pnt.begin() );
        GL::color( fabs(b.kg),  b.kg > 0 ,1-fabs(b.kg));
        GL::normal( b.normal.begin());
        GL::vertex( b.pnt.begin() );
        GL::color( fabs(c.kg),  c.kg > 0,1-fabs(c.kg));
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
