//2D Voronoi


#include "vsr_cga2D_types.h"   
#include "vsr_cga2D_draw.h"  
#include "vsr_set.h"
#include "vsr_GLVimpl.h"

using namespace vsr;


struct MyApp : App {    
   
	Pnt mouse;
	
	Set<Pnt> vp;

	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
		scene.camera.lens.bOrtho = true;  
		
		vp.add( Point(1,0) ).add( Point(0,1)).add( Point(-1,-.5) ).add( Point(.2,-2) );
		
	}
  
  	void getMouse(){                                
		mouse = Point( interface.mouse.projectMid ); 
  }

  	virtual void onDraw(){ 

		getMouse();
		Draw(mouse); 
		
		for (auto i : vp.data() ) Draw(i);
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
