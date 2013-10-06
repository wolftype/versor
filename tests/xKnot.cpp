#include "vsr_cga3D.h"
#include "vsr_GLVimpl.h" 
#include "vsr_cga3D_draw.h" 
#include "vsr_cga3D_interface.h"  

#include "vsr_knot.h"


using namespace vsr;
using namespace glv;  

struct MyApp : App {
	

	void onDraw(){
     

		TorusKnot tk(3,2); 
	
		static auto a = Point(2,0,0);  
	
		Touch(interface, a);
	
		Draw( Sphere(a,.5), 1, 0, 0 );

		tk.calc( a );
	
		for (int i = 0; i < tk.iter(); ++i){
			Draw( tk.cir[i],0,1,0 );
		}                        
	
	}
};
                        
MyApp * myApp;

int main(){
                          
	
	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"Versor",&glv);    
                          
	myApp = new MyApp;
	myApp -> init(win);
	
	glv << *myApp;

	Application::run();
	
	return 0;
	
}