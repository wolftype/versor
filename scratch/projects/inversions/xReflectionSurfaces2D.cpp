/*
 * =====================================================================================
 *
 *       Filename:  xReflectionSurfaces2D.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/04/2014 12:53:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_cga2D.h" 
#include "vsr_cga2D_interface.h"  
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga2D;

struct MyApp : App {    
   
   Pnt mouse;
   float amt;

	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
	}
  void initGui(){  
    gui(amt,"amt",-100,100);	
  }
   
  void getMouse(){
      
      auto tm = interface.mouse.projectMid;  
      
      mouse = Ro::point( tm[0], tm[1] );
  }
    
    virtual void onDraw(){ 

      getMouse();
      Draw(mouse);
    
     static auto c = Ro::dls(1.0,0,0);

     static auto pair = Ro::point(-2,0) ^ Ro::point(2,0) ^ Inf(1);

      Draw(pair, 0,0,1);
     //cout << Ro::size(c, true) << endl; 

    Touch(interface, c);

     Draw(c);
     

     for (int i = 0; i < 100; ++i){

       auto p = Ro::round( Ro::surround( c ), Vec::x.rot( Biv::xy * PI * i/100) );//Ro::pnt_cir( c.dual(), PI * i/100);
       
       auto ps =  Ro::dls( Ro::split( p, true ), amt);
       
      // Draw(ps, 1,0,0);

       auto rpair = Cir(pair).reflect( ps );

       Draw( Ro::loc( rpair ),0,1,0 );
        
        

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
