/*
 * =====================================================================================
 *
 *       Filename:  xMolecule.cpp
 *
 *    Description:  simple molecules of movement
 *
 *        Version:  1.0
 *        Created:  05/26/2015 12:41:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_rigid.h"
#include "form/vsr_knot.h"
#include "form/vsr_twist.h"
#include "form/vsr_fold.h"

using namespace vsr;
using namespace vsr::cga;

//radial constraint structure
struct Radial {

  
  //initialize 
  void init(int numPetals){
    
  }
};


struct RabbitEar{

  struct Data{
     Point a, b, c, d, e;
     Point& operator[](int idx){ return ((&a)[idx]); }
  } V;

  Rigid2_ ra;
  Rigid3 rb;

  RabbitEar(const Point& a, const Point& b, const Point& c)
  : V{a,b,c}
  {
    build();
  }
  
  void build(){
    auto edges = Fold::Lines( V.a, V.b, V.c);
    auto bisectors = Fold::Bisect( edges );
    
    V.e = meet( bisectors[0], bisectors[1] );    
    V.d = Fold::Perpendicular( V.e, edges[1] );

    ra.set( V.d, V.e, V.c );
    rb.set( V.b, V.d, V.e, V.a, true);
  }

  RabbitEar::Data eval( double amt ){
    Pnt td = ra(amt);
    rb.updateA( td );
    Pnt tb = rb();
    return { V.a, tb, V.c, td, V.e };
  }


};

//four-degree 
struct QuadFold{

  Rigid node; //center
  Rigid rigid[4];

  MBO mbo;

  //feed in a central node and four surrounding points
  void set(const Point& n, const Point& a, const Point& b, const Point& c, const Point& d){
    rigid[0].result = a;
    rigid[1].result = b;
    rigid[2].result = c;
    rigid[3].result = d;
    
    node.result = n;
    
    rigid[0].set( &rigid[1], &node, &rigid[3] );
    rigid[1].set( &rigid[2], &node, &rigid[0] );
    rigid[2].set( &rigid[3], &node, &rigid[1] );
    rigid[3].set( &rigid[0], &node, &rigid[2] );
  }

  void eval(float amt){
    rigid[2].orbitB(amt);
    rigid[3].update();
  }

  void mesh(){
    Mesh mesh;
    for(int i =0;i<4;++i){
      int nxt = (i<3) ? i+1 : 0; 
      mesh.add(Vertex( rigid[i].result ) );
      mesh.add(i).add(nxt).add(4); //last (4) is center node
    }
    mesh.add(Vertex(node) );
    mesh.store();
    mesh.mode(GL::T);
    mbo = mesh;
  }

  void update(){
    for (int i =0;i<4;++i){
      mbo.mesh[i].Pos = rigid[i].result;
    }
    mbo.mesh[4].Pos = node.result;
  }

  void draw(GFXSceneNode* re){
    render::pipe(mbo,re);
  }

  void draw(){
    render::draw(mbo);
  }

};


struct Flower : Frame {

  TorusKnot tk = TorusKnot(3,2);
  MBO mbo;

  vector<Point> pnt;
  
  void init(){
    for (int i = 0; i < 4; ++i){
      auto p = point( this->cxy().dilate(this->pos(), .5), TWOPI*(float)i/4 );
      pnt.push_back(p);
    }
  }

  void mesh(){
    Mesh mesh;
    for (int i = 0; i< pnt.size(); ++i){
      mesh.add(Vertex(pnt[i]));
      mesh.add(i);
    }
    mesh.store();
    mesh.mode(GL::LL);
    mbo = mesh;
  }
  
  void open(){

  }

  void close(){

      
  }

  void draw(GFXSceneNode *re){
    render::pipe(mbo, re);
  }

  void draw(){
    Draw(mbo);
  }
  
};

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;

 // Flower f;
  QuadFold f;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");

    f.init();
   // mSceneRenderer.immediate(false);
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  //  f.draw(this);
    f.draw();
   // Draw(mbo);
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
