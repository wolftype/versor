/*
 * =====================================================================================
 *
 *       Filename:  xTruss.cpp
 *
 *    Description:  truss
 *
 *        Version:  1.0
 *        Created:  05/11/2014 18:18:04
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

#include "vsr_field.h"
#include "vsr_fold.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt, amt2;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(amt2,"amt2",-100,100);
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

      Field<Pnt> f(3,2,1);
      f[2] = (Vec(f[2]) + Vec(amt,0,0) ).null();
      Draw(f);

      Rigid2 rigid(f[4], f[2], f[3], true);

      /* Rigid3 rigidM(f[3], f[4], f[2], f[0], true); */
      /* Rigid3 rigidL(f[1], f[3], f[2], f[0], true); */
      /* Rigid3 rigidR(f[5], f[4], f[2], f[3], true); */

      Truss t( f[1], f[3], f[5],
               f[0], f[2], f[4], false);

      auto np = rigid.orbit(amt2);
      t.update( f[0], f[2], np );

      Draw(np,0,1,0);
      Draw(t.p.tl,1,0,0);
      Draw(t.p.tm,1,0,0);
      Draw(t.p.tr,1,0,0);



      
      /* rigidM.updateA(np); */
      /* auto nm = rigidM(); */

      /* rigidR.updateA(np); */
      /* auto nr = rigidR(); */

      /* Draw(np,0,1,0); */ 
      /* Draw(nm,1); */
      /* Draw(nr,1); */
    
      //rigid.updateB( (f[2] + Vec(0,0,amt2)).null() );
      //Draw( rigid.possible() );

      /* gfx::Glyph::Point( t.p.tl ); */
      /* gfx::Glyph::Point( t.p.tm ); */
      /* gfx::Glyph::Point( t.p.tr ); */

      
    
     //  Draw( rigid.a(), 1,0,0,.2);
    //   Draw( rigid.b(), 0,1,0,.2);
     //  Draw( rigid.c(), 0,0,1,.2);

    
    
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
