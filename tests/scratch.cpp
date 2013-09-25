#include "vsr_cga3D.h"
#include "vsr_GLVimpl.h" 
#include "vsr_draw.h"
#include "vsr_twist.h"

using namespace vsr;
using namespace glv;  


void GLVApp :: onDraw(){
	
	auto a = Point(1,0,0);
	auto b = Point(-1,0,0);
	auto c = Point(0,1,0);

	static float time = 0; time +=.01;
	
	a = Point(1,0,0).motor( Twist::Along( DLN(0,1,0), cos(time), sin(time) ) );

	auto d = Circle(a, b, c);  
	
	   
	Draw(a,1,0,0); Draw(b); Draw(c);
	Draw(d);
	
	Draw( DLN(0,1,0), 0,1,0 );
	
	
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