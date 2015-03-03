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
    spur = Vec(0,0,0);
    kgGrad = Vec(0,0,0);
    area=kg=mean=0;
  }
  Vec normal = Vec(0,0,0);       //normal (gradient of position)
  Vec kgGrad = Vec(0,0,0);       //gradient of gaussian
  Vec spur = Vec(0,0,0);         //sum of dn
  Biv curl = Biv(0,0,0);         //curl of dn
  float kg = 0;
  float mean =0;   //gaussian and mean;
  float area =0;  // area sum;
  
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
  float wt, wt2;
  bool bUseCotan;
  bool bExterior;
  bool bUseNextForExterior;
  bool bDivideByArea;
  bool bDrawNormals;
  bool bDrawMean;
  bool bDrawReciprocals;
  bool bDrawGaussGrad;
  bool bDerivOfShape;

  //Control
  bool bTrackMouse;


  void setup(){
    bindGLV();
    
    gui(amt,"amt",-100,100);               //<-- amt of transform
    gui(wt,"wt",-100,100);                 //<-- wt of diff
    gui(wt2,"wt2",-100,100);                 //<-- wt of grad of gauss or mean
    gui(bUseCotan,"bUseCotan");            //<-- use TWOPI deficit formula
    gui(bExterior,"bExterior");            //<-- use vector derivatives then exterior
    gui(bUseNextForExterior,"bNormalExterior");            //<-- use vector derivatives then exterior
    gui(bDerivOfShape,"bDerivOfShape");            //<-- use deriv of shape biv to calc mean
    gui(bDivideByArea,"bDivideByArea");    //<-- use vector derivatives then exterior
    gui(bDrawNormals,"bDrawNormals");      //<-- draw normals
    gui(bDrawMean,"bDrawMean");      //<-- draw normals
    gui(bDrawGaussGrad,"bDrawGaussGrad");      //<-- draw normals
    gui(bDrawReciprocals,"bDrawReciprocals");     //<-- draw normals

    //1. Generate hexagonal SpaceGroup, Tessellate, Hang points on lattice, Graph
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
        v.area += simplex.area;
      }

      /*-----------------------------------------------------------------------------
       *  now we have simplices recorded (local metric, area, reciprocal basis)
       *-----------------------------------------------------------------------------*/
      Vec ct(0,0,0);
      for (int j=0;j<e.size();++j){

        auto& b = e[j]->a().pnt;
        auto& c = e[j]->next->a().pnt;

        int next = (j<e.size()-1) ? j+1 : 0; 
        auto& nb = e[next]->a().pnt;
        auto& nc = e[next]->next->a().pnt;
        
        //exterior diff
        auto vd = v.simplex[j].derivative(Vec(a),Vec(b),Vec(c));
        
        //or invididual
        auto vda = v.simplex[j].derivative0(Vec(a),Vec(b),Vec(c));
        auto vdb = v.simplex[next].derivative0(Vec(a),Vec(nb),Vec(nc));

        ct += v.simplex[j].cotan( v.simplex[next] );
        
        v.normal += bUseNextForExterior ? (vda^vdb).duale() : vd.duale();
       // v.mean += vda.norm();
      }

     // v.mean = v.normal.norm() * wt2/v.area; //divide by area?
      v.normal = v.normal.unit();

      if (bUseCotan){
        v.mean = (ct/v.area).norm();
        v.normal = ct.unit();
      }

    }

    //now calculate second derivative (i.e. riemann curvature, gaussian)
    for (auto& i : graph.node() ){
      auto& v = i->data();
      auto& na = v.normal;
      auto& simplex = i->data().simplex; //simplex is already built
      auto e = i->valence();
      float deficit = TWOPI;
      for (int j=0;j<e.size();++j){
         int next = (j<e.size()-1) ? j+1 : 0;
         auto& nb = e[j]->a().normal;
         auto& nc = e[j]->next->a().normal;
         auto& nnb = e[next]->a().normal;
         auto& nnc = e[next]->next->a().normal;
                  
         /*-----------------------------------------------------------------------------
          *  Use internal exterior per simplex or "Next Exterior Derivative"
          *-----------------------------------------------------------------------------*/
         auto vv = simplex[j].derivative(na,nb,nc); 
         
         auto va = simplex[j].derivative0(na,nb,nc); 
         auto vb = simplex[next].derivative0(na,nnb,nnc); 
       
         v.curl += bExterior ? va^vb : vv;
         v.spur += va;
         deficit -= simplex[j].deficit();
      }
      
      v.curl *= wt; 
      deficit *= wt; 
       
      if (bDivideByArea){ 
        v.curl /= (2*v.area); 
        v.spur /= 2*v.area;
        deficit /=v.area; 
     
     }
     
      v.mean = (-v.normal<=v.spur)[0] * wt2;

      v.kg= bUseCotan ? deficit : -(v.curl <= !(v.normal.duale()))[0];
    }

      /*-----------------------------------------------------------------------------
       *  And Average -> mean
       *-----------------------------------------------------------------------------*/
    if (bDerivOfShape){
        for (auto& i : graph.node() ){
          auto e = i->valence();
          auto& v = i->data();
          v.spur = Vec(0,0,0);
          auto a = v.curl.duale();
          auto& simplex = v.simplex;
          for (int j=0;j<e.size();++j){
            auto b = e[j]->a().curl.duale();
            auto c = e[j]->next->a().curl.duale();
            auto sa = simplex[j].derivative(a,b,c);
            v.spur += sa; 
          }
          v.spur /= 2*v.area;
          v.mean = (-v.normal<=v.spur)[0] * wt2;
        }
    }

    /*-----------------------------------------------------------------------------
     *  Gradient of Gaussian
     *-----------------------------------------------------------------------------*/
      for (auto& i : graph.node() ){
          auto e = i->valence();
          auto& v = i->data();
          auto a = v.kg;
          auto& simplex = v.simplex;
          float area = 0;
          for (int j=0;j<e.size();++j){
            auto& b = e[j]->a().kg;
            auto& c = e[j]->next->a().kg;
            auto grad = simplex[j].derivative0(a,b,c);
            v.kgGrad += grad;//.duale();
          }
          v.kgGrad *= (wt2/v.area);
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
    if(bDrawGaussGrad){
      for (auto& i : graph.node() ){
        auto& v = i->data();
        DrawAt(v.kgGrad, v.pnt,0,1,1);
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
        GL::color( ta, ba, 1-ta,.8);
        GL::normal( a.normal.begin());
        GL::vertex( a.pnt.begin() );
        GL::color( tb, bb, 1-tb,.8);
        GL::normal( b.normal.begin());
        GL::vertex( b.pnt.begin() );
        GL::color( tc, bc, 1-tc,.8);
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
