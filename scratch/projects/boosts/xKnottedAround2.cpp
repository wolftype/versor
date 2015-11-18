//Not very interesting

#include "vsr_app.h"
#include "form/vsr_knot.h"
#include "form/vsr_twist.h"
  
using namespace vsr;
using namespace vsr::cga;
using namespace glv;  
   
struct MyApp : App {
	
	float amt, P, Q, dilamt, iter, ecc1, ecc2, spread;  
	 
	TorusKnot tkA;
	TorusKnot tkB;

  Pnt a = Round::null( Vec(2,0,0) ); 
	Cir cir = CXZ(1);
	
	void setup(){
		
		tkA = TorusKnot(3,2); 

    bindGLV();
		gui(amt,"amt",0,10)(P,"P",0,10)(Q,"Q",0,10)(dilamt,"dilamt",-100,100)(iter,"iter",100,10000);
		gui(ecc1, "ecc1",-1000,1000)(ecc2, "ecc2",-1000,1000)(spread,"spread",0,100);
		
    amt = .003; 

    objectController.attach(&cir);
    objectController.attach(&a);
    objectController.attach(&tkA.HF.cir());

	}

	void onDraw(){
     
		Pnt np = a;
    	        
		tkA.P =  P;
		tkA.Q =  Q;
		
		for (int i = 0; i < iter; ++i){ 
			                
			float t = 1.0 * i/iter;
			tkA.HF.cir() = cir.trs(1,0,0).mot( Twist::Along( DLN(0, 0,1), PI, 1.0 ) * t * PI );
			if ( int(i/10.0) & 1 )  Draw(tkA.HF.cir() );                
			 
			auto bst = tkA.bst(amt);  
					
			np = Round::loc( np.spin( bst ) );   
			
			Draw(np,0,1,0);
		}   
			
	}
};
                        
int main(){
                          
  MyApp app;
  app.start();	
	return 0;
	
}
