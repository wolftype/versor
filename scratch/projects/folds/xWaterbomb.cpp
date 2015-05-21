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
       gfx::GL::twoSidedLighting();  
      getMouse();

      Field<Pnt> f(2,2,1, 2);

     //f[3] = f[3].trs(-.5,-.5, 0);
      
      Waterbomb wb( f[0], f[1], f[3], f[2] );

      auto p = wb.eval(amt);

//      Draw( wb.rc.orbit() );
//      Draw( wb.V.b,1,0,0);

      for (int i = 0; i < 7; ++i) Draw( p[i],0,0,0);

      glColor3f(.5,.5,.5);
      glBegin(GL_TRIANGLES);
      
        TriNormal( p.d, p.g, p.e );//f[2], ncd, center );
        TriNormal( p.g, p.c, p.e );//ncd, pa, center );
        TriNormal( p.c, p.b, p.e);//center );
        TriNormal( p.b, p.f, p.e );
        TriNormal( p.f, p.a, p.e);
        TriNormal( p.a, p.d, p.e);//center);

      glEnd();

      glColor3f(1,0,0);
      glBegin(GL_LINE_STRIP);
        GL::Tri( p.d, p.e, p.c );//f[2], ncd, center );
        GL::Tri( p.c, p.e, p.b );//ncd, pa, center );
        GL::Tri( p.b, p.e, p.a);//center );
        GL::vertex( p.e.begin() );
        glColor3f(0,0,1);
        GL::vertex( p.e.begin() );
        GL::Tri( p.f, p.e, p.g );
        //GL::Tri( p.f, p.a, p.e);
        //GL::Tri( p.a, p.d, p.e);//center);
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
