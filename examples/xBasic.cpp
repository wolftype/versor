#include "vsr_cga3D.h" 
#include "vsr_GLVimpl.h" 


using namespace vsr;
using namespace glv;  


struct MyApp : public App {  
	
	

	void onDraw(){
	                                                                          
		//Circle through 3 Points
		static Cir circle = Point(1,0,0) ^ Point(0,1,0) ^ Point(-1,0,0); ///< i.e. CXY(1)
         
		//Dual Plane with Normal in Y direction
		static Dlp dualPlane = Plane(0,1,0);
         
		//Calculate their Intersection (which is the dual of the outer product of duals . . . )
		auto pointPair = Meet(circle,dualPlane);  //<---- i.e. (circle.dual() ^ dualPlane).dual();


		//Draw 'em with colors
		Draw(circle,0,1,0);   	//<-- Green
		Draw(dualPlane,0,0,1);  //<-- Blue
		Draw(pointPair,1,0,0);  //<-- Red

		//Enable Mouse Control with 'G' 'R' and 'S' keys
		Touch( interface, circle );
			     
		text("use G, R and S keys and drag with mouse to Grab, Rotate and Scale the Circle");
	} 

};
                        
MyApp * myApp;

int main(){


	
	GLV glv(0,0);	
    		        
	Window * win = new Window(800,500,"Versor",&glv);    
	
	myApp = new MyApp;
	myApp -> init(win);
	    
	glv << *myApp;

	Application::run();
	
	return 0;
	
}