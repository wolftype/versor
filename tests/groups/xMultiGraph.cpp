/*
 * =====================================================================================
 *
 *       Filename:  xMultiGraph.cpp
 *
 *    Description:  n-simplex barycentrism
 *
 *        Version:  1.0
 *        Created:  07/24/2014 12:08:27
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
#include "vsr_simplex.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  bool bReset;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(bReset,"reset");
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

      
      Simplex<4> simplex;
      typedef Simplex<4>::Vec V;
      typedef decltype(V()^V()^V()^V()) Recip; 
      Recip recip[5];

      for (auto& i : simplex.edges ){
        gfx::Glyph::Line(simplex.verts[i.a].begin(), simplex.verts[i.b].begin());
      }

      for (int i=0;i<simplex.verts.size();++i){
        recip[i] = Recip(1);
        for (int j=0;j<simplex.verts.size();++j){
          if (i!=j) recp[i] = recip[i] ^ simplex.verts[j];
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
