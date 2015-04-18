 /*
 * =====================================================================================
 *
 *       Filename:  xIntro.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/16/2014 17:11:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      amt = 1;
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

      static DualSphere ts = Round::dls(1,0,0,0); //karl and charlie want to change
      Touch(interface, ts);

      DualSphere s = ts.dilate( amt );

      DualPlane p(0,1,0, .5);
      Draw(p,.2,.6,.4);

      auto c = (s ^ p);
     // Draw(c, .8,.2,.3);

      auto cdual = c.dual();
      Draw(cdual,1,0,.3);

       Draw(s,0,1,0,.3);

      float sqr = Round::size( cdual, false );

      if ( sqr > 0 ){
        DualSphere reflection = s.reflect( p );
        Draw(reflection, 1,0,.2);
      }

      cout << sqr << endl; 


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
