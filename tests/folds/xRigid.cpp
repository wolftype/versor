
/*
 * =====================================================================================
 *
 *       Filename:  xRigid.cpp
 *
 *    Description:  rigid simplex constraints 
 *
 *        Version:  1.0
 *        Created:  11/18/2014 13:58:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_group.h"
#include "vsr_rigid.h"
#include "vsr_graph.h"

using namespace vsr;
using namespace vsr::cga3D;

//construct a point with some extra structure (a rigid)
struct RigidPoint : public Point {
  RigidPoint(const Point& p=Ro::null(0,0,0) ) : Point(p){ }
  Rigid_ rigid; //not Rigid, but Rigid_ ... NOTE
};


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,nudge,starter;

  bool bReset, bDo, bDrawBalls, bDrawOriginal;

  vector<RigidPoint> pnt;
  HEGraph<RigidPoint> graph;

  //vector<Rigid> rigid;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);          //amt to nudge
      gui(nudge,"nudge",-100,100);      //which node to nudge
      gui(starter,"starter",-100,100);  //where to start calculating in op graph
      gui(bReset,"reset");
      gui(bDo,"do");
      amt=1;

      //Tessellate, Hang, Edge Graph
      int w=3; int h=3;
      SpaceGroup2D<Vec> sg(3,1,true);
      pnt = sg.hang( RigidPoint(PAO),w,h);     
      graph.UV(w,h,pnt);

      cout <<"NUM NODES: " << graph.node().size() << " NUM RIGID: " << pnt.size() << endl;
      
      //each node is controlled by three others
      //which ones: alternating around valenced edges
      
      //1. first we set position of rigid result
      for (auto& i : graph.node() ){
        i->data().rigid.set(i->data());
      }

      bool bMtn = false;
      //2. then we build dependency graph 
      for (auto& i : graph.node() ){
        auto tmp = i->neighbors();
        if (tmp.size()==6){
          i->data().rigid.set( &(tmp[0]->data().rigid), &(tmp[2]->data().rigid), &(tmp[4]->data().rigid), bMtn );
        }// else if (tmp.size()>=3){
        //  i->data().rigid.set( &(tmp[0]->data().rigid), &(tmp[1]->data().rigid), &(tmp[2]->data().rigid), bMtn );
       // } else if (tmp.size()==2){
       //   i->data().rigid.set( &(tmp[0]->data().rigid), &(tmp[1]->data().rigid), bMtn);
      //  }
        //bMtn=!bMtn;
      }
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      getMouse();

      //enable calculations
      for (auto& i : graph.node()){
        i->data().rigid.set( i->data() );
        i->data().rigid.reset();
      }

      //select node to nudge
      auto& n = graph.node(CLAMP( (int)nudge,0,graph.node().size()-1) );
      auto nb = n.neighbors();
      auto& pa = n.data().rigid.result;

      //get two spheres of influence and wedge them
      auto ra = Ro::at( nb[0]->data().rigid.result, pa);
      auto rb = Ro::at( nb[1]->data().rigid.result, pa);
      auto tcir = (ra^rb).dual();
      Draw(tcir);
    
      //nudge along circle orbit
      pa = Ro::pnt_cir( tcir, amt);;

      //select node in graph and start computing
      auto& s = graph.node( CLAMP( (int)starter,0,graph.node().size()-1));
      if (bDo) s.data().rigid();


      //draw stuff
      for (auto& i : graph.edge()){
        glColor4f(1,0,0,.2);
        Glyph::Line(i->a().rigid.result, i->b().rigid.result);
      }

      Draw( *(Point*)(&n.data()),0,1,0);
      auto v = n.valence();
      auto neigh = n.neighbors();
      int iter=0;

      for (auto& i : v){
        double t= (float)iter/v.size(); iter++;
        glColor3f(t,0,1-t);
        Glyph::Line(i->a(), i->b());
      }

      for(auto& i : neigh){
        Draw(*(Point*)(&i->data()),0,1,0);
      }
      
      //draw original mesh
      if(bDrawOriginal){     
        for (auto& i : graph.edge()){
         glColor4f(1,1,1,.2);
         Glyph::Line(i->a(), i->b());
        }
      }

      //draw rigid balls
      if(bDrawBalls){
        if (n.data().rigid.ra!=NULL){
          Draw( n.data().rigid.da(),1,0,0,.2);
          Draw( n.data().rigid.db(),1,.4,0,.2);
          Draw( n.data().rigid.dc(),1,.8,0,.2);
        }
      }

      for (auto& i : graph.node() ){
        Rigid& r = i->data().rigid;
        auto meet = r.meet();
        if ( Ro::size(meet,false) < -.001 ) {
          Draw(meet,0,0,1);
          Draw(r.da(),1,0,0,.2);
          Draw(r.db(),1,.4,0,.2);
          Draw(r.dc(),1,1,0,.2);
        }
    }


  }
   

  
};


MyApp * app;


int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(500,500,"Versor",&glv);    
  app = new MyApp( win ); 
  app -> initGui();
  
  
  glv << *app;

  Application::run();

  return 0;

}
