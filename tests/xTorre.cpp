//#include "vsr_cga3D.h"  
#include "vsr_cga3D_draw.h" 
#include "vsr_cga3D_interface.h"  

#include "vsr_GLVimpl.h"

#include "vsr_field.h"


struct MyApp : App {   
	
    Field<Dll> f(4,1,1); 
	float time;

   	MyApp (Window * win) : App(win){
	
		for (int i = 0;i< f.num(); ++i){
			float t= -1.0 + 2.0 * i/f.num() );
			f[i] = DLN(0,1,0).trs(t,0,0);
		}
	}
	
	void points(){
		// static Field<Pnt> f(10,50,10);
		// 
		// static Lin lin = Line( Point(0,0,0), Vec(0,1,0) );        
		// 
		// for (int i = 0; i < f.w(); ++i){
		//    	for (int j = 0; j < f.w(); ++j){ 
		// 		Pnt p = f.at(i,j,0);
		// 		VT dist = ( p <= lin )[0] * -2.0;
		// 		
		// 		Par par = Par( Tnv(0,0,1) ).trs( p );
		// 		
		// 		//float dist = Distance( )
		// 	} 
		// }    
		
		//Draw(f);
		
	}  
	
	
	virtual void onDraw(){       
	     
		Draw(f); 
		time += .01; 
		
		int numCir = 10;
		
		for (int i = 0; i < f.num();++i){
			
			for (int j = 0; j < numCir; ++j ){
				Cir c = CXY(.5).mot( f[i] * sin(time) );  
			}
		   
			
		}
		
		

	}
	
};

MyApp * app;

int main(){
                             
	GLV glv(0,0);	

	Window * win = new Window(500,500,"Versor",&glv);    
	app = new MyApp(win); 
   // app -> init( win ); 
	
	glv << *app;

	Application::run();

	return 0;

}