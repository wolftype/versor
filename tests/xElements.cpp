// #define __CON__ //must define global metric !
// #define GLOBAL_METRIC RMetric<4,1>::Type()     

#include "vsr_cga3D.h"
#include "vsr_GLVimpl.h" 
#include "vsr_render.h" 
// #include "vsr_draw.h"  

using namespace vsr;
using namespace glv;  


void GLVApp :: onDraw(){
	
	//cout <<   scene.xf.projMatrixf()  << endl; 
	
	auto a = Point(1,0,0);
	auto b = Point(0,1,0);
	auto c = Point(0,-1,0);
	
	auto cir = Circle(a,b,c);
	
   // Draw(cir,1,0,0); 
	DRAWCOLOR(cir,1,0,0);   

}
                        
GLVApp * myApp;

int main(){
    
	myApp = new GLVApp();
	myApp -> stretch(1,1); 
	myApp -> renderMode = 1;
	
	GLV glv(0,0);	
	//glv.colors().back.set(.3,.3,.3);
    		        
	Window * win = new Window(500,500,"Versor",&glv);    
    
	
	glv << *myApp;

	myApp -> initGL();
	Application::run();
	
	return 0;
	
}         
