/*
 * =====================================================================================
 *
 *       Filename:  xWaterbomb.cpp
 *
 *    Description:  waterbomb fold
 *
 *        Version:  1.0
 *        Created:  11/22/2013 17:55:29
    
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_fold.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;
  float iter;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(iter,"iter",1,100);
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

      Field<Pnt> f(2,2,1, 2);

     f[3] = f[3].trs(-.5,-.5, 0);
      
      Draw(f);

      vector<Point> src;
      src.push_back(f[0]);
      src.push_back(f[1]);
      src.push_back(f[3]);
      src.push_back(f[2]);

      auto edges = Fold::Lines(src); //fold lines a | b _ c | d _

      auto valleys = Fold::Bisect(edges); //bisectors

      Point center = Fold::Meet( valleys );    //center

      auto nodes = Fold::Perpendiculars( center, edges ); //new nodes


      Pnt tmp;
       
      Rigid2 r0( nodes[2], center, f[2] );     
      Rigid2 r1( nodes[0], center, f[0] );
      
      Rigid2 ra( f[3], center, nodes[2] );
      Rigid2 rb( f[1], center, f[3] );

      Rigid3 rc( nodes[0], f[1], center, f[0], false );

      Pnt ncd = r0.orbit(  amt * PIOVERTWO );
      ra.updateB( ncd );

      Pnt pa = ra( f[1], 1-amt );
      rb.updateB( pa );

      Pnt pb = rb( nodes[0], 1-amt );
      rc.updateA(  pb ); 

      Pnt pc = rc();

      Draw( ncd, 0,1,1);
      Draw( pa, 0,1,0);
      Draw( pb, 0,1,0);
      Draw( pc );

      Draw( ra.orbit() ); 
      Draw( rb.orbit() );
     // Draw( rc.orbit() );

      glBegin(GL_TRIANGLES);
      
        TriNormal( f[2], ncd, center );
        TriNormal( ncd, pa, center );
        TriNormal( pa, pb, center );
        TriNormal( pb, pc, center );
        TriNormal( f[0], center, pc );
        TriNormal( f[0], f[2], center);

      glEnd();


    
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
