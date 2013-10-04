#include "vsr_cga3D.h"
#include "vsr_GLVimpl.h" 
#include "vsr_cga3D_draw.h" 

#include "vsr_knot.h"


using namespace vsr;
using namespace glv;  


void GLVApp :: onDraw(){

	static auto a = Point(2,0,0);  
	auto b = a;

	TorusKnot tk(3,2);           
	
	a = a.sptest( tk.bst() );
	
	for (int i = 0; i < tk.iter(); ++i){  
		float t = 1.0 * i/tk.iter();
	    b = Ro::loc( b.sptest( tk.bst() ) ); 
		Draw(b, t, .5, 1-(t*.5));
	}                                        
	
}
                        
GLVApp * myApp;

int main(){
                          
	
	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"Versor",&glv);    
                          
	myApp = new GLVApp(win);
	myApp -> stretch(1,1);
	
	glv << *myApp;

	Application::run();
	
	return 0;
	
}