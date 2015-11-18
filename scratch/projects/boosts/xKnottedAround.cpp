#include "vsr_app.h"
#include "form/vsr_knot.h"
  
using namespace vsr;
using namespace vsr::cga;
   
struct MyApp : App {
	
	float amt, P, Q, amt2,P2,Q2,iter;  
	 
	TorusKnot tkA;
	TorusKnot tkB;

  Pnt a = Round::null( Vec(2,0,0) );  

  bool bUseBst, bUseDist;
	
	virtual void setup(){

    bindGLV();
		gui(amt,"amt",0,10)(P,"P",0,10)(Q,"Q",0,10);
		gui(amt2,"amt2",0,10)(P2,"P",0,10)(Q2,"Q",0,10);
    gui(iter,"iter",100,10000);
    gui(bUseBst,"bUseBst");
    gui(bUseDist,"bUseDist");
    
    amt = .003; 

		tkA = TorusKnot(3,2); 
		tkB = TorusKnot(3,2);

    tkA.HF.cir() = CXZ(1).translate(-1,0,0);		
		tkB.HF.cir() = CXZ(1).translate(1,0,0);

    objectController.attach(&a);
    objectController.attach(&tkA.HF.cir());
    objectController.attach(&tkB.HF.cir());

    bUseBst = true;

	}

	void onDraw(){
     
    
    if (bSetMouse) a = calcMouse3D();
		 
		float twtA =  bUseDist ?  1.0 / ( Round::sqd( a, Round::loc(tkA.HF.cir()) ) + .001 ) : 1;
		float twtB =  bUseDist ?  1.0 / ( Round::sqd( a, Round::loc(tkB.HF.cir()) ) + .001 ) : 1; 
		
    auto tbst = Gen::bst( ( tkA.par() * twtA * amt + tkB.par() * twtB * amt2) );

    auto tcon = Gen::bst( ( tkA.par() * amt * twtA) ) *  Gen::bst( tkB.par() * amt2 *  twtB );
 
		a = bUseBst ? Round::loc( a.spin( tbst ) ) : Round::loc( a.spin( tcon ) );
		Draw(a,1,0,0);
		 
		Pnt np = a;
		
		tkA.P = P; tkB.P = P2;
		tkA.Q = Q; tkB.Q = Q2;

		Draw(tkA.HF.cir() );                
		Draw(tkB.HF.cir() );
		
		for (int i = 0; i < iter; ++i){ 

      float t = (float)i/iter;
			
			float wtA = bUseDist ?  1.0 / ( Round::sqd( np, Round::loc(tkA.HF.cir()) ) + .001 ) : 1;
			float wtB =  bUseDist ?  1.0 / ( Round::sqd( np, Round::loc(tkB.HF.cir()) ) + .001 ) : 1; 
			 
			auto bst = Gen::bst( tkA.par() * wtA * amt + tkB.par() * wtB  * amt2 );  

      auto con = Gen::bst( ( tkA.par() * amt* wtA*t) ) *  Gen::bst( tkB.par() * amt2*  wtB *t);
					
			np = bUseBst ? Round::loc( np.spin( bst ) ) : Round::loc( a.spin( con ) );   
			
			Draw(np);
		}   		
	}

};
                        

int main(){
                          
  MyApp app;
  
  app.start();	
	return 0;
	
}
