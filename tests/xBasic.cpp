#include "vsr_cga3D.h" 
#include "vsr_cga3D_draw.h"  
#include "vsr_cga3D_interface.h" 
#include "vsr_GLVimpl.h" 
#include "vsr_twist.h"

using namespace vsr;
using namespace glv;  


void GLVApp :: onDraw(){
	
	static MFrame f;        

	Draw(f); 
	Touch(interface, f); 
	
	static Cir cir = CXY(1).trs(1,0,0);   

	static auto dlp = Plane(0,1,0);

	Draw(cir,0,1,0);  
	Draw(dlp,0,0,1);

	Touch( interface, cir );
	Touch( interface, dlp );

	auto p = Meet(cir,dlp);

	Draw(p,1,0,0);
	
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