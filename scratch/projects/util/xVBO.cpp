// #define __CON__ //must define global metric !
// #define GLOBAL_METRIC RMetric<4,1>::Type()     

#include "vsr_cga3D.h"
#include "vsr_GLVimpl.h" 
#include "vsr_render.h" 
// #include "vsr_draw.h"  

using namespace vsr;
using namespace glv;  

struct MyApp : App{
	
	void  onDraw(){
	
		//cout <<   scene.xf.projMatrixf()  << endl; 
	
		auto a = Point(1,0,0);
		auto b = Point(0,1,0);
		auto c = Point(0,-1,0);
	
		auto cir = Circle(a,b,c);
	
	   // Draw(cir,1,0,0); 
		DRAWCOLOR(cir,1,0,0);   

	} 

};
                        
MyApp * myApp;

int main(){
    
	
	GLV glv(0,0);	
	//glv.colors().back.set(.3,.3,.3);
    		        
	Window * win = new Window(500,500,"Versor",&glv);    
    myApp = new MyApp;

	myApp -> init( win ); 
	myApp -> initGL(); 
	myApp -> renderMode = 1;
	
	glv << *myApp;

	Application::run();
	
	return 0;
	
}         
