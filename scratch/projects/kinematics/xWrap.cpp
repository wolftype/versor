/*
 * =====================================================================================
 *
 *       Filename:  xWrap.cpp
 *
 *    Description:  wrapp around a central cylinder
 *
 *        Version:  1.0
 *        Created:  02/12/2014 11:51:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_set.h"
#include "vsr_graph.h"
#include "vsr_fold.h"

using namespace vsr;
using namespace vsr::cga3D;

//spiral fold (Guest, 1992)

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,rtheta;

  int numCir, poly;

  Set< Point > set;
  HEGraph< Point > graph;
  Set< Rigid2 > initial;
  Set< Rigid3 > constraint;
  
  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
    initMesh(5,6);
  }

  void initMesh(int nc, int p){

    graph.clear();
    set.clear();
    initial.clear();
    constraint.clear();

    numCir = nc;
    poly = p;

    Circle cir = CXY(1);
    for (int i = 0; i < numCir; ++i){
      VT radius = 1 + 2.0 * (float)i/numCir;
      Circle tcir = cir.dilate( log( radius ) );
      for (int j = 0; j < poly; ++j ){
          VT theta = TWOPI * j / poly;
          theta += (rtheta * i * PI/poly);
          Point p = Round::pnt_cir(tcir, theta );
          set.add( p );
      }
    }

    bool bSwitch = false;
    for (int j = 0; j < poly; ++j){
      int a = j;
      int b = ( j < poly - 1 ) ? j + 1 : 0;
      int c = j + poly;
      //cout << a << " " << b << " " << c << endl;
      initial.add( Rigid2( set[c], set[a], set[b], bSwitch ) );
      bSwitch = !bSwitch;
    }

     bSwitch = false;

    for (int i = 1; i < numCir-1; ++i){
      for (int j = 0; j < poly; ++j ){
        int a = i * poly + j;
        int b = a + poly;
        int c = j < poly -1 ? a + 1 : i * poly;
        int d = j < poly -1 ? a - poly + 1 : (i-1)*poly;
        constraint.add( Rigid3( set[b], set[a], set[d], set[c], bSwitch ) );
        bSwitch = !bSwitch;

      }
    }

  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(rtheta,"rtheta",-100,100);

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

      for (auto& i : set.data() ){
          Draw(i);
      }

      initMesh(3,6);


      vector <Point> pnt;

      for (int i = 0; i < poly; ++i ){
        float col = (float)i/poly;
        Draw( initial[i].circle(), col,0,1-col );
       // Draw( initial[i].distA(),0,1,0,.2 );
       // Draw( Ro::dls(initial[i].distB.p,.2),1,1,0,.2 );

        auto p = initial[i].orbit( amt );
        Draw( p, 1,0,0);
        pnt.push_back(p);       
      }
      
      for (int i = 0; i < poly; ++i){
        int next = i < poly - 1 ? i + 1 : 0;
        constraint[i].updateA( pnt[i] );
        constraint[i].updateC( pnt[ next]  );
        Draw( constraint[i](),1,0,0 );
      }

      /*  for (int i = 1; i < numCir-1; ++i){ */
      /*    for (int j = 0; j < poly; ++j){ */
      /*      int idx = i * poly + j; */
      /*      int prev = idx - poly; */ 
      /*      int nprev = prev + 1; */
      /*      constraint[idx].update( constraint[prev](), constraint[prev].c.p, constraint[nprev]() ); */ 

      /*      if (j == 0 ) Draw( constraint[idx](),1,0,0 ); */
      /*    } */
      /* } */
    
      

    
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
