#include "vsr_cga3D.h"
#include "vsr_GLVimpl.h" 
#include "vsr_knot.h"
  
using namespace vsr;
using namespace glv;  
   
struct MyApp : App {
	
	float amt, P, Q, dilamt, iter, ecc1, ecc2;  
	 
	TorusKnot tkA;
	TorusKnot tkB;
	
	virtual void init(Window * win){
		App::init(win);
		
		tkA = TorusKnot(3,2); 
		tkB = TorusKnot(3,2);
		
		tkA.HF.cir() = CXY(1).translate(0,1,0);		
		tkB.HF.cir() = CXZ(1).translate(0,-1,0);
	}
	
	virtual void initGui(){
		gui(amt,"amt",0,10)(P,"P",0,10)(Q,"Q",0,10)(dilamt,"dilamt",-100,100)(iter,"iter",100,10000);
		gui(ecc1, "ecc1",-1000,1000)(ecc2, "ecc2",-1000,1000);
		// gui(P, "p",0,10)(Q,"q",0,10);   
		amt = .003; 
	}

	void onDraw(){
     
                                                          
		static Pnt a = Ro::null( Vec(2,0,0) ); 
		 
		float twtA = 1.0 / ( Ro::sqd( a, Ro::loc(tkA.HF.cir()) ) + .001 );
		float twtB = 1.0 / ( Ro::sqd( a, Ro::loc(tkB.HF.cir()) ) + .001 ); 
		auto tbst = Gen::bst( ( tkA.par() * twtA + tkB.par() * twtB ) * amt );
		a = Ro::loc( a.spin( tbst ) );
		Draw(a,1,0,0);
		 
		Pnt np = a;
		Touch(interface, a);

		Touch(interface, tkA.HF.cir());
		Touch(interface, tkB.HF.cir()); 
		
		tkA.P = tkB.P = P;
		tkA.Q = tkB.Q = Q;

		Draw(tkA.HF.cir() );                
		Draw(tkB.HF.cir() );
		
		for (int i = 0; i < iter; ++i){ 
			
			float wtA = 1.0 / ( Ro::sqd( np, Ro::loc(tkA.HF.cir()) ) + .001 );
			float wtB = 1.0 / ( Ro::sqd( np, Ro::loc(tkB.HF.cir()) ) + .001 ); 
			 
			auto bst = Gen::bst( ( tkA.par() * wtA + tkB.par() * wtB ) * amt );  
					
			np = Ro::loc( np.spin( bst ) );   
			
			Draw(np);
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