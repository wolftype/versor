#include "vsr_cga3D.h"
#include "vsr_GLVimpl.h" 
#include "vsr_knot.h"


using namespace vsr;
using namespace glv;  

struct MyApp : App {
	
	float amt;
	float P, Q;
	
	virtual void initGui(){
		gui(P,"Q",0,10)(Q,"Q",0,10)(amt,"amt",0,10);
		
		amt = .003; 
		P = 3; 
		Q = 2;
	}

	void onDraw(){
     

		TorusKnot tk(P,Q,amt);  
		
		cout << tk.iter() << endl;
	
		static auto a = Ro::null(2,0,0);     

		Touch(interface, a);   
		Draw( Ro::dls(a,.2), 1, 1, 0 );

		tk.calc( a );
	
		for (int i = 0; i < tk.iter(); ++i){
			Draw( tk.cir[i],0,1,0 );
		}                        
	
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