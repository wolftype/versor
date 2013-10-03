#include "vsr_cga3D.h"
#include "vsr_GLVimpl.h" 
#include "vsr_draw.h" 

#include "vsr_knot.h"


using namespace vsr;
using namespace glv;  


void GLVApp :: onDraw(){

	static auto a = Point(2,0,0);  
	auto b = a;

	TorusKnot tk(3,2); 
	
	for (int i = 0; i < tk.iter(); ++i){
	    b = Ro::loc( b.sp( tk.bst() ) ); 
		Draw(b);
	}                                        
	
	
	// HopfFiber hf;
	// 
	// static float time = 0; time +=.01;
	// 
	// hf.vec() = Vec::x.rot( Biv::xy * time );
	// 
	// Cir fa = hf.fiberA();
	// Cir fb = hf.fiberB();
	// 
	// Draw(fa , 1, 0, 0); 
	// Draw(fb, 0, 1, 0);   
	
   // for (int i = 0; i < )
	
}
                        
GLVApp * myApp;

int main(){

	myApp = new GLVApp();
	myApp -> stretch(1,1);
	
	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"Versor",&glv);    
    
	glv << *myApp;

	Application::run();
	
	return 0;
	
}