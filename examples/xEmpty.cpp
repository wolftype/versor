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
    auto tm = interface.mouse.projectMid;
		ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
    		mouse = Round::point( ray,  Ori(1) );  
  }

  	virtual void onDraw(){ 
        
		getMouse();
		
		Draw(mouse,0,1,0);
		         
		Cir cir = Circle( mouse, Point(0,0,0), Point(1,0,0));
		Draw( cir );
		
		Draw( Point(1,0,0), 1,0,0 );
		
		static auto lin = Line( Point(0,1,0), Point(-1,0,0) );
		Touch(interface,lin);
		
		Draw(lin,0,1,0);
		                                 
		for (int i = 0; i < 100; ++ i ){
			float t = PI * i/100;
			Mot mot = Gen::mot( lin.dual() * t );
			Draw( cir.spin( mot ) );
		}
		 
		
		
		
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
