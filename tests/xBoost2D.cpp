#include "vsr_cga2D_types.h"  
#include "vsr_cga2D_draw.h" 
#include "vsr_GLVimpl.h" 

using namespace vsr;
     

struct MyApp : App {

	void onDraw(){
	      
		Dls a = Ro::dls( Vec(1,0),  1.0);
	
		Draw(a);  
	
		for (int i = 0; i < 10; ++i){          
			float t = 1.0 * i/10; 
			auto tmp = a.sp( Gen::trv( Tnv(1,0) * t ) ); 
		   
			Draw(  tmp );		
		}           
	
	}  

};
                        
MyApp * app;

int main(){

	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"CGA2D",&glv);    
	app = new MyApp;   
	
	app -> init(win); 
	       
	glv << *app;
	Application::run();
	
	return 0;
	
}

