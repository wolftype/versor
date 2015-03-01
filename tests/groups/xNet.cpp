/*
 * =====================================================================================
 *
 *       Filename:  xNet.cpp
 *
 *    Description:  2d nets
 *
 *        Version:  1.0
 *        Created:  01/08/2014 13:46:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_cga2D.h"   
#include "vsr_GLVimpl.h"

#include "vsr_group.h"


using namespace vsr;
using namespace vsr::cga2D;


struct MyApp : App {    


  float time;
  float amt;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
    amt = 1;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
  }
  
    virtual void onDraw(){ 
     
      PointGroup2D<Vec> pg((int)amt);

      cout << pg.ops.size() << endl;

      for (auto i : pg.ops ){
        Draw(i);
      }
      
      time += .01;
      auto m = Vec::x.rot( Biv::xy * time) ;

      auto res = pg ( m ); 
      
      for (auto i : res ) DrawAt( gfx::Glyph::Point, i ); 

    
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
