           // EUCLIDEAN REFLECTIONS

#include "vsr_ega3D_draw.h" 
#include "vsr_GLVimpl.h" 
 

using namespace vsr;
using namespace glv;  

struct MyApp : App {
	
	float theta, phi;
	float time;
	bool bRef;
	
	virtual void initGui(){
		gui(theta,"theta",-TWOPI,TWOPI); 
		gui(phi,"phi",-PIOVERTWO,PIOVERTWO);
		gui(bRef,"reflections");  
	}

	void onDraw(){
 
		Biv ba(1,.5,0); 
		
		Biv bb(.2,3,.5);
		
		Draw(ba,1,0,0);
		Draw(bb,0,0,1);
		
		Draw( ( ba.dual() ^ bb.dual() ).dual() );

	}

};
                        
MyApp * myApp;



int main(){
 
	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"Versor",&glv);    
                          
	myApp = new MyApp;
	myApp -> init(win);
	myApp -> initGui();
	
	glv << *myApp;

	Application::run();
	
	return 0;
	
}