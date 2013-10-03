#include "vsr_cga3D.h"  
#include "vsr_cga3D_draw.h" 
#include "vsr_cga3D_interface.h"  

#include "vsr_GLVimpl2.h"   

#include "vsr_field.h" 
#include "vsr_knot.h"  

using namespace vsr;
           


void bend(){
	
	int num = 10;  
	float spread = 5.0;

   // Field< Pnt > (90,450,1); 

	static float time = 0.0; time +=.01;

	for (int i = 0; i < num; ++i){
		float t = 1.0 * i/num;
		auto a = Line( Point(t,0,0), Vec(0,1,0) );		
		//Draw(a);

		auto tnv = Tnv(.2,0,0).rot( Biv::xz * PIOVERTWO * t );
		auto par = Par( tnv * sin(time) ).trs(t,0,0);
		auto b = Cir(a).bst( par ) ;//bst( par ); 

		Draw(b);  

		for (int i = 0; i < num; ++i){ 
			float t2 = -spread/2.0 + spread * i/num;  
			auto dlp = Plane(0,1,0, t2);
			auto m = Meet( b, dlp );  
			Draw(dlp,0,1,0);
			Draw(m,1,0,0); 
		}    
	}
}  

void knot(){
	
	TorusKnot tk(3,2);
	
	int num = 100;
	
	for (int i = 0; i < 100; ++i ){
		
	} 
	  
}   

void twist(){
	
	static Field< Dll > f(2,2,2,5); 
	
	
	
	int num = 10;
	  
	for (int i = 0; i < num; ++i ){ 
		float tu = 1.0 * i/num;
		for (int j = 0; j < num; ++j ){
			float tv = 1.0 * j/num;  
            for (int k = 0; k < num; ++k ){   
				float tw = 1.0 * k/num;  
				Dll dll = f.vol(tu,tv,tw); 
				
				Draw( Frame( dll ) );
			}
		}
	}
	
}

void GLVApp :: onDraw(){
	      
	//bend();
   // twist();
   // knot();  

	
   // interface.touch( cir, Ro::loc(cir), .1 );
}
                        
GLVApp * myApp;

int main(){

	myApp = new GLVApp();
	myApp -> stretch(1,1);
	
	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"CGA2D",&glv);    
    
	glv << *myApp;

	Application::run();
	
	return 0;
	
}

