
#include "vsr_cga2D.h"   
#include "vsr_GLVimpl.h"

using namespace vsr;


struct MyApp : App {    
   
	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
	}
  
  	virtual void onDraw(){ 
        
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
