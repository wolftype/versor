#include "vsr_cga3D.h"
#include "vsr_GLVimpl.h" 
#include "vsr_knot.h"
#include "vsr_twist.h"
  
using namespace vsr;
using namespace glv;  
   
struct MyApp : App {
	
	float amt, P, Q, dilamt, iter, ecc1, ecc2, spread;  
	 
	TorusKnot tkA;
	TorusKnot tkB;
	
	virtual void init(Window * win){
		App::init(win);
		
		tkA = TorusKnot(3,2); 
		//tkB = TorusKnot(3,2);
		
	}
	
	virtual void initGui(){
		gui(amt,"amt",0,10)(P,"P",0,10)(Q,"Q",0,10)(dilamt,"dilamt",-100,100)(iter,"iter",100,10000);
		gui(ecc1, "ecc1",-1000,1000)(ecc2, "ecc2",-1000,1000)(spread,"spread",0,100);
		// gui(P, "p",0,10)(Q,"q",0,10);   
		amt = .003; 
	}

	void onDraw(){
     
                                                          
		static Pnt a = Ro::null( Vec(2,0,0) ); 
		static Cir cir = CXZ(1);
		
		Touch(interface, cir); 
		 
		Pnt np = a;
		Touch(interface, a);

		Touch(interface, tkA.HF.cir());

		        
		tkA.P =  P;
		tkA.Q =  Q;
                

		
		for (int i = 0; i < iter; ++i){ 
			                
			float t = 1.0 * i/iter;
			tkA.HF.cir() = cir.trs(1,0,0).mot( Twist::Along( DLN(0, 0,1), PI, 1.0 ) * t * PI );
			if ( int(i/10.0) & 1 )  Draw(tkA.HF.cir() );                
			 
			auto bst = tkA.bst(amt);  
					
			np = Ro::loc( np.spin( bst ) );   
			
			Draw(np,0,1,0);
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