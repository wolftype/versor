/*
 * =====================================================================================
 *
 *       Filename:  xParabolicDish.cpp
 *
 *    Description:  folding of a parabolic dish
 *
 *        Version:  1.0
 *        Created:  04/08/2014 17:48:57
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

#include "vsr_conic.h"
#include "gfx/gfx_mesh.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,beta,gamma;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(beta,"beta",-1,1);
      gui(gamma,"gamma",-1,1);
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
  
      vector<Point> vp;
      int num = 20;

      //Transformed Points on Bottom half of a Sphere
      Dls dls = Ro::dls(1,0,0,0);
      for (int i = 0; i <= num; ++i){
          for (int j = 0; j < num; ++j){
            float tu= PI * i/num;
            float tv = -PIOVERFOUR  + PIOVERFOUR * j/num;
            Point p1 = Ro::pnt(dls, Vec::x.sp(Gen::rot(tu, tv)));
            Point ap = Conic::Transform( p1, Vec(0,-1,0), amt );
            vp.push_back(ap);    
          }
        }

        Mesh mesh = Mesh::UV( vp.data(), num+1, num );
        mesh.drawElements();

          for (int i = 0; i < num; ++i){
            Point mp = Ro::pnt_cir( meet, PI * i/num );
            mp = Conic::Transform( mp, mv, amt);
            Draw(mp,0,1,0);
          }


          Draw( Conic::ITransform( Fl::loc( td, mouse, true).null(), mv, amt ), 0,1,1);

    
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
