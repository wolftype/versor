#include "vsr_GLVimpl2.h"  
#include "gfx/gfx_glyphs.h"


using namespace glv;  


void GLVApp :: onDraw(){
	
	gfx::Glyph::Circle(1);
	
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