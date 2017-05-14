/*
 * =====================================================================================
 *
 *       Filename:  xSimplexCoordinate.cpp
 *
 *    Description:  n-simplex coordinates and reciprocal frames
 *
 *        Version:  1.0
 *        Created:  08/05/2014 16:30:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D_op.h"   
#include "vsr_cga3D_funcs.h"   
#include "vsr_GLVimpl.h"
#include "vsr_simplex.h"
#include "vsr_simplex_draw.h"

using namespace vsr;
using namespace vsr::cga3D;

template<class A, class B>
auto wedge(const A& a, const B& b) -> decltype(A()^B()) {
  return a ^ b;
}

template<class A, class B, class ... XS>
auto wedge(const A& a, const B& b, const XS& ... xs) RETURNS(
  wedge( wedge(a,b), xs ... )
);

/* template<class X> */
/* auto wedge(const vector<X>& v) { */
/*   for (auto& i : v){ */
/*     wedge(i, */ 
/*   } */
/* ); */

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

      static const int N = 4;
      typedef NEVec<N+1> HVec;

      Simplex<N> simplex;

      vector<HVec> p;  
      for (auto& i: simplex.verts){
         p.push_back(Euc::hom(i));
       //  i.print();
      }
      auto pss =(p[0])^(p[1])^(p[2])^(p[3])^p[4];
      auto norm = pss.norm();
      auto sqnorm = norm*norm;
     // pss /= sqnorm;
     // pss.print();

      Draw(simplex);

      vector<HVec> rcp;
      for (auto& i: simplex.verts){
        vector<HVec> v; 
        for (auto& j: simplex.verts){
          if ((i^j).rwt() > FPERROR){
            v.push_back( Euc::hom(j) );
          }
        }
        auto recip = ( (v[0]^v[1]^v[2]^v[3] ) <= pss)/sqnorm;
        rcp.push_back(recip);
        //recip.print();
      }


      DrawAt( rcp[(int)amt], simplex.verts[(int)amt], 1,0,0);
     
    


      
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
