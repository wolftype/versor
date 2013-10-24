//Get mouse position as an "unprojected" point in 3D space

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

using namespace vsr;


struct MyApp : App {    
   
	Pnt mouse;
	Lin ray;

	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
	}
  
  	void getMouse(){
		auto tv = interface.vd().ray; 
		Vec z (tv[0], tv[1], tv[2] );
		ray = Point( interface.mouse.projectMid ) ^ z ^ Inf(1); 
    	mouse = Point( ray,  Ori(1) );  
  }

  	virtual void onDraw(){ 
        
		getMouse();
		
		Draw(mouse,0,1,0);
		
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
