/*
 * =====================================================================================
 *
 *       Filename:  xCrimp.cpp
 *
 *    Description:  crumple it
 *
 *        Version:  1.0
 *        Created:  05/26/2015 17:06:19
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

using namespace vsr;
using namespace vsr::cga;




struct Crumpling : Frame{
  
  vector<Point> pnt;
  vector<Rigid2> rigid;

  MBO mbo;

  void init(){
    
    mNum = 10;
    
    for (int i=0; i<mNum; ++i){
      auto p = point( this->cxy(), TWOPI*(float)i/10 );
      pnt.push_back(p); 
    }
    pnt.push_back( this->pos() );
    mesh();

    //constraints
    for (int i=0;i<mNum; ++i){
      rigid.push_back( Rigid2(pnt[i]) );
    }
    for (int i=0;i<mNum; ++i){
      int prv = i>0? i-1 : rigid.size()-2;
      int nxt = (i<rigid.size()-2)? i+1 : 0;
      rigid[i].add( &rigid.back(), &rigid[prv], true); 
      rigid[i].add( &rigid[nxt], &rigid.back(), true); 
    }
    
  }

  void satisfy(){
    for (auto& i : rigid ){
      i.result = Constrain::Tangency(i.result, i.parents[0].da(), i.parents[0].db());
    }
  }
  
  void mesh(){
    Mesh mesh;
    for (int i = 0; i< pnt.size()-1; ++i){
      int nxt = (i<pnt.size()-2) ? i+1 : 0;
      float t = (float)i/pnt.size();
      mesh.add( Vertex(pnt[i], Vec3f(0,0,1), Vec4f(0,1-t,t*.5,1)) );
      mesh.add(i).add(nxt).add(pnt.size());
    }
    mesh.add(pnt.back() );
    mesh.store();
    mesh.mode(GL::T);
    mbo = mesh;
  }

  void update(){
    for (int i=0;i<pnt.size();++i){
      mbo.mesh[i].Pos = pnt[i];
    }
    mbo.update();
  }

  void draw(GFXSceneNode* re){
    render::pipe(mbo, re);
  }

};



struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;

  Crumpling crimp;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");

    mSceneRenderer.immediate(false);

    crimp.init();
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
    crimp.draw(this);
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
