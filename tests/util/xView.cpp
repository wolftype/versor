 

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

using namespace vsr;


struct MyApp : App {    
   
	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
	}

  virtual void onDraw(){ 
		auto tv = interface.vd().ray; 
		Vec z (tv[0], tv[1], tv[2] );
		Lin lin = Point( interface.mouse.projectMid ) ^ z ^ Inf(1); 
		
		Draw(lin,0,1,0);
	
    	auto a = Point( lin,  Ori(1) );  
		Draw( Point( interface.mouse.projectNear ), 1, 0, 0);
		Draw(z);
	    Draw(a,1,0,0);
		
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
