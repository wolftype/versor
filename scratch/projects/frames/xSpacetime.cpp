//ooph spacetime algebra
#include "space/vsr_sta.h"    
 
using namespace vsr;   


struct MyApp : App {
	        
	double time;
	
	MyApp(Window * w ) : App(w) {
		initGui(); 
		
		using namespace sta;
		
		Sca::bprint();
		Y0::bprint();
		Y1::bprint();
		Y2::bprint();
		Y3::bprint(); 

		I::bprint(); 
		printf("spacelike vector planes\n");
		P3::bprint(); 

		(O1() * O2() * O3() ) .bprint();  

		( I(1) * O3(1) ).bprint();

		//auto a = O3(1);   

		(Y0(1) * ~(O2(1)*O3(1)) * Y0(1) ).vprint();  

		( Vec(1,1,1,1) * Y0(1) ).bprint();   //scalar + spacetime bivector . . .   

		( Vec(1,1,1,1) * ( Vec(1,1,1,1) ) ).bprint();
		
	}
	
	virtual void initGui(){  

	}   
	
	virtual void onDraw(){
		using namespace sta;
		               
		time += .01;
		
		sta::Event p1( time, sin(time), 0, 0 );
		sta::Event p2( time, 0, sin(time), 0 );        
	                               
		//p1 relative to p2
		sta::Split t = Y0( 1 ) * p1; 
		
		p1.vprint();
		p2.vprint();
		
		printf("\nrel\n");
		t.vprint();
		
	}
};
  

MyApp * app; 

int main(){
                             
	GLV glv(0,0);	

	Window * win = new Window(500,500,"Versor",&glv);    
	app = new MyApp(win); 
	
	glv << *app;

	Application::run();

	return 0;

}
