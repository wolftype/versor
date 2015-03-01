/*
 * =====================================================================================
 *
 *       Filename:  xCga2D.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/23/2014 15:29:48
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

using namespace vsr;


struct MyApp : App {    
   
	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
	}
  
  	virtual void onDraw(){ 
        
        auto cA = Ro::dls( 1.0, 0.0, 0.0 );
        auto cB = Ro::dls( 2.0, 1.5, 0.0 );

        auto intersect = ( cA ^ cB).dual();
        Draw(cA); Draw(cB); Draw(intersect,1,0,0);
	}
   
	
};


MyApp * app;


int main(){
                             
	GLV glv(0,0);	

	Window * win = new Window(500,500,"Versor",&glv);    
	app = new MyApp( win ); 
  
	
	glv << *app;

	Application::run();

	return 0;

}
