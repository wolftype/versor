#include "vsr_GLVimpl.h"  
#include "gfx/gfx_glyphs.h"


using namespace glv;  


void GLVApp :: onDraw(){
	
	gfx::Glyph::Circle(1);
	
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