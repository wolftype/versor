
/*
 * =====================================================================================
 *
 *       Filename:  xHexagrid2cpp
 *
 *    Description:  a hexagonal grid using pointgroups is folded evolutionarily
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
#include "vsr_differential.h"

using namespace vsr;
using namespace vsr::cga3D;

//construct a point with some extra structure (a rigid)
struct RigidPoint : public Point {
  RigidPoint(const Point& p=Ro::null(0,0,0) ) : Point(p){ }

  //two options
  Rigid rigid;
  Rigid2 rigid2;
  //a normal
  Vec normal;
  //simplicial variables (valences)
  vector<Simplicial2> simplex;
  double gaussian;
};


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,nudge,starter,numiter;

  bool bReset, bDo,bDown, bUp, bSwitch;

  vector<RigidPoint> pnt;
  HEGraph<RigidPoint> graph;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);          //amt to nudge
      gui(nudge,"nudge",-100,100);      //which node to nudge
      gui(numiter,"numiter",0,1000);    //which node to nudge
      gui(starter,"starter",-100,100);  //where to start calculating in op graph
      gui(bReset,"reset");
      gui(bDo,"do")(bDown,"down")(bUp,"up")(bSwitch,"switch");
      amt=1;

      //Tessellate, Hang, Graph
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
        i->data().rigid2.set(i->data());
      }

      bool bMtn = false;
      //2. then we build dependency graph 
      //rigid3 version (per node)
      for (auto& i : graph.node() ){ 
        auto tmp = i->neighbors();
        
        if (tmp.size()==6){
          i->data().rigid.set( &(tmp[0]->data().rigid), &(tmp[2]->data().rigid), &(tmp[4]->data().rigid), bMtn );
        } else if (tmp.size()>=3){
          i->data().rigid.set( &(tmp[0]->data().rigid), &(tmp[1]->data().rigid), &(tmp[2]->data().rigid), bMtn );
        } else if (tmp.size()==2){
          i->data().rigid.set( &(tmp[0]->data().rigid), &(tmp[1]->data().rigid), bMtn);
        }

      }

      //rigid 2 version (loops per node, adds all relevant parents)
      for (auto& i : graph.node() ){
        auto v = i->valence();
        auto& a = i->data().rigid2;
        for(int k=0; k<v.size(); ++k){
         // if (!v[k]->node->visited()){
            auto& b = v[k]->next->a().rigid2;
            v[k]->a().rigid2.add(&a,&b,false);
            v[k]->node->visited(true);
         // }
        }
      }


  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

       /*-----------------------------------------------------------------------------
        *  CURVATURE
        *-----------------------------------------------------------------------------*/
  virtual void shape(){

       //calculate normals at each node
       for (auto& i: graph.node()){
          //references
          vector<Simplicial2>& simplex = i->data().simplex;
          Vec& normal = i->data().normal;
          
          //reset
          normal = Vec(0,0,0);
          simplex.clear();

          auto v = i->valence();
          for(auto& k : v ){
            simplex.push_back( Simplicial2(i->data().rigid.result, k->a().rigid.result, k->next->a().rigid.result) );
          }

          //normals
          for (auto& j : simplex){
            normal += ( j.ra ^ j.rb).duale();  
          //  DrawAt(j.ra, i->data().rigid.result);                        
          //  DrawAt(j.rb, i->data().rigid.result); 
          }
          
          normal = normal.unit();

         // DrawAt(normal, i->data().rigid.result,0,1,0);
       }

       //calculate curvature
       for (auto& i : graph.node()){
          //references
          vector<Simplicial2>& simplex = i->data().simplex;
          Vec& normal = i->data().normal;

          Biv tbiv;
          int iter=0;
          auto v = i->valence();
          for (auto& k : v){
            auto& kna = k->a().normal;
            auto& knb = k->next->a().normal;
            auto dv = simplex[iter].derivative( normal, kna, knb );
            tbiv += dv[0]^dv[1]/simplex[iter].area;
            iter++; 
          }
          i->data().gaussian = (tbiv<=(!normal.duale()))[0];

       }

  }

    virtual void onDraw(){ 
        
      getMouse();

      auto dls = Ro::dls(amt,0,0,0);

      (dls <= dls).print();
      //reset calculations
      for (auto& i : graph.node()){
        i->data().rigid.set( i->data() );
        i->data().rigid2.set( i->data() );

        i->data().rigid.reset();
        i->data().rigid2.reset();
      }

      //select node to nudge and grab list of neighbors
      auto& n = graph.node(CLAMP( (int)nudge,0,graph.node().size()-1) );

      auto& pa = n.data().rigid2;


      pa.result = pa.orbit(amt);
      pa.bReCalc=false;
      if (bDown) bSwitch ? pa() : pa.cascade();

      for(auto& i : graph.node()){
        i->data().rigid2.reset();
      }

      //select node and start computing
      if(bUp) bSwitch ? pa.cascade() : pa();

       
      //Draw( *(Point*)(&n.data()),0,1,0);
      for (auto& i : graph.node() ){
        Rigid& r = i->data().rigid;
        Rigid2& r2 = i->data().rigid2;
        r.result = r2.result;
      }

      for (int j=0;j<numiter;++j){

        for (auto& i : graph.node() ){
          i->data().rigid2.reset();
        }

        for (auto& i : graph.node() ){
          Rigid& r = i->data().rigid;
          Rigid2& r2 = i->data().rigid2;
          auto meet = r.meet();
          if ( Ro::size(meet,false) < -.001 ) {
            r.update();
            r2.cascade();
          }
          r.result=r2.result;

          if (j==numiter-1) {
            for (int m=0;m<r2.
            Draw(r2.circle(m),0,1,0);
          }
        }
      }

      //draw stuff

      int iter=0;
      for(auto& i : graph.node() ){
        Rigid2& r = i->data().rigid2;
        float t = 1;//i->data().gaussian;
        glColor3f(t,0,1-t);
        Glyph::Line(r.result, *r.parent[0].da.src);
        Glyph::Line(r.result, *r.parent[0].db.src);
        Glyph::Line(*r.parent[0].da.src, *r.parent[0].db.src);
        iter++;
      }

      shape();

      for (auto& i : graph.node() ){
        Rigid& r = i->data().rigid;
        auto meet = r.meet();
        auto size = Ro::size(meet,false);
        if ( size < -.001 ) {
          Draw( Ro::loc( Ro::split(meet,true) ) );
          Draw( Ro::loc( Ro::split(meet,false) ) );
          DrawAt(i->data().normal, r.result,0,1,0);
         // Draw(r.da(),1,0,0,.2);
         // Draw(r.db(),1,.4,0,.2);
         // Draw(r.dc(),1,1,0,.2);
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
