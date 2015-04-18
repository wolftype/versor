/*
 * =====================================================================================
 *
 *       Filename:  xLegs.cpp
 *
 *    Description:  some leg mechanics
 *
 *        Version:  1.0
 *        Created:  04/02/2014 12:08:38
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
#include "vsr_chain.h"
#include "vsr_chain_draw.h"


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
      time +=.05;


      Chain chain(4);  int it = 0;
      for(auto& i : chain.links() ) {
        i.pos() = Ro::null(0,-1,0);
        chain[it].scale() = .4 * (1-(float)it/chain.num());
        it++;
      }      
      chain.link(2).pos() = Ro::null(-.5,-.2,0);
      chain.fk();

      Dls bound = chain[3].bound();
      Pnt pnt = Ro::pnt( bound, -Vec::y).null();
      static auto floor = Ta::at( bound.undual(), pnt ) ^ Inf(1);      
      Draw(floor,0,1,0);

      //Kinematics
      Pnt pm = meet( ray, Dlp(0,0,1) );
      Draw(pm);
      bound = Ro::dls( pm, Ro::rad(bound) );

      auto floor_meet =  Ro::size( bound ^ floor.dual(), true);
      cout << floor_meet << endl; 
      if (floor_meet <= 0 ){
        chain.fabrik( pm, 3, 0 );
        chain.fk();
      }



      Draw(chain);
    
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
