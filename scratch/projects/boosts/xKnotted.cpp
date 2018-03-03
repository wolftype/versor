#include "vsr_app.h"
 
using namespace vsr;
using namespace glv;  
   

static const int DIM = 6;

typedef NVec<DIM> Vec;

typedef NInf<DIM> Inf; 
typedef NPnt<DIM> Pnt;
typedef NTnv<DIM> Tnv;
typedef NPar<DIM> Par; 
typedef NDll<DIM> Dll; 
typedef NLin<DIM> Lin; 
typedef NBst<DIM> Bst; 
typedef NDlp<DIM> Dlp; 

struct MyApp : App {
	
	float amt, P, Q, dilamt, iter, ecc1, ecc2;
	
	virtual void initGui(){
		gui(amt,"amt",0,10)(P,"P",0,10)(Q,"Q",0,10)(dilamt,"dilamt",-100,100)(iter,"iter",100,10000);
		gui(ecc1, "ecc1",-1000,1000)(ecc2, "ecc2",-1000,1000);
		
		amt = .003; 
	}

	void onDraw(){
     
                                                          
		static Pnt a = Ro::null( Vec(2,0,0,ecc1) );  
		
		//Touch(interface, a);
		
		Pnt np = a;	
		
		//Par().bprint();
		auto para = Ro::round( Ro::dls(Vec(),1), Vec(0,1,0,ecc2) ); 
		Draw(para);
		auto tmp = a ^ para;
		Draw( tmp );
		auto tmeet = ( Dlp(1,0,0,0,1) ^ tmp.dual() ).dual();
		//tmeet.bprint();
		Draw( tmeet, 0, 1, 0 );                                                                            
		// auto tmp =  Ro::round( Ro::dls(Vec(),1), Vec(1,0,0,ecc2) ).undual().trs(0,0,1,0).trv( 0, 0, .5, 0 );
		// 
		// Draw( Lin(tmp) );    
		// 
		// auto parb = tmp.dual();
		// 
		// 
		// // Dll().bprint();
		//         auto npar = para * PI/P + parb * PI/Q; 
		// 
		// 
		// // 		 
		// Draw(para,1,0,0);
		// 
		// for (int i = 0; i < iter; ++i){ 
		// 	
		// 	float wt = 1.0 / ( Ro::sqd( np, Ro::loc(para) ) + .001 );
		// 	auto bst = Gen::bst( npar * amt * wt );  
		// 			
		// 	np = Ro::loc( np.spin( bst ) );   
		// 	
		// 	Draw(np);
		// }   
			
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
