/*
 * =====================================================================================
 *
 *       Filename:  xSimplex.cpp
 *
 *    Description:  n-simplex (build with devel)
 *
 *        Version:  1.0
 *        Created:  05/06/2014 12:54:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_GLVimpl.h"
#include "vsr_simplex.h"
#include "vsr_ncube.h"
#include "vsr_generic_draw.h"
#include "vsr_cga3D_frame.h"
#include "vsr_stat.h"

#include "vsr_cga3D_op.h"


using namespace vsr;

template<int N>
void Drw( const Simplex<N>& s, float dist, bool bOrtho=false){

  using Vec = NEVec<3>;

  auto proj = s.project(dist);

  glNormal3f(0,0,1);
  glColor3f(1,0,0);
  for (auto& i : s.edges){
    gfx::Glyph::Line( proj[i.a], proj[i.b] );
  }

  glColor4f(0,0,1,.2);
  for (auto& i : s.faces){
    gfx::Glyph::Triangle( proj[i.a], proj[i.b], proj[i.c] );
  }

}



template<int N>
struct SimplexFrame : public Frame {

  Simplex<N> simplex;

  void draw(float dist){
     gfx::GL::push();

     gfx::GL::translate ( pos().begin() );
     gfx::GL::rotate( Gen::aa( rot() ).begin() ); 
     gfx::GL::scale( scale() );   
    
     Drw( simplex, dist);

     gfx::GL::pop();

  }
  
};


struct MyApp : App {    
   
  float time;
  float dist,whichBiv,amt;  
  bool bOrtho;

  static const int num = 10;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(dist,"dist",-100,100);
      gui(whichBiv,"which_biv",0,100);
      gui(amt,"amt",-100,100);
      amt = 0;
      gui(bOrtho,"ortho");
      bOrtho = false;
      dist = .5;
  }
  
  virtual void onDraw(){ 
       
    static const int N = 4;

    time += .01;
    
    static SimplexFrame<N> sim[num*num];
   
    for (int i = 0;i<num;++i){
      for (int j = 0;j<num;++j){
        float u = (float)i/num * 5;
        float v = (float)j/num * 5;
        int idx = i*num+j;
        sim[idx].pos() = Ro::null(u,v,0);
        sim[idx].simplex.spin( Rand::Int(N), amt);
        sim[idx].draw(dist);
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
