 
  
#include "vsr_field.h" 
#include "vsr_set.h" 
#include "vsr_constraint.h" 
 

#include "vsr_cga2D_types.h"  
#include "vsr_cga2D_draw.h" 
#include "vsr_GLVimpl.h"

using namespace vsr;
                                   

struct MyApp : App {
                   
	float time;   
	
	//DoubleData< Vec > points;
	
    Verlet < Vec > system;
	
	void initVariables(){
		Field<Vec> f(10,10,1);
		 
		system.resize( f.num() );
		
		for (int i = 0; i < system.num(); ++i ){
			system.data(i) = f[i]; 
	    } 
	
		system.copy(); //copy to prev
		
		for (int i = 0; i < f.w(); ++i){  
			double u = 1.0 * i/f.w();
			for (int j = 0; j < f.h(); ++j){
				double v = 1.0 * j/f.h(); 
				Patch p = f.surfIdx(u,v);
				 
				Vec& pa = system.data( p.a );
				Vec& pb = system.data( p.b ); 
				Vec& pc = system.data( p.c ); 
				Vec& pd = system.data( p.d ); 	
					
				system.constraint.add( Distance<Vec>( pa, pb, f.spacing() ) );
				
				stringstream s; s << &(f[ p.a ]) << " " ; 
				s << &pa << " " ; s << system.constraint[ system.constraint.size() - 1].a;
				cout << s.str() << endl;  
				
				system.constraint.add( Distance<Vec>( pb, pc, f.spacing() ) ); 
				system.constraint.add( Distance<Vec>( pc, pd, f.spacing() ) ); 
				system.constraint.add( Distance<Vec>( pd, pa, f.spacing() ) );  
			
				
				// cout << s.str() << endl;   
			}
	    }
	}
	
	void onDraw(){       
	 //   points.swap();
                   
	   // system.step();
		time += .01;
		 
		// stringstream s; s << &(system.data(0));
		// s << " " <<  system.constraint[0].a; 
		// s << " " <<  system.constraint[0].b; 
		// s << " " <<  system.constraint[1].a; 
		// s << " " <<  system.constraint[1].b; 
		// cout << s.str() << endl; 
		
	   // system.data(0) = Vec(sin(time),cos(time));
		*(system.constraint[0].a) = Vec(sin(time),cos(time));      
	    
		//system.constraint[0].a -> vprint();
		system.step();
		
		for (int i =0; i < system.num(); ++i ){
		
			Draw( Ro::null( system.data(i) ) );
		}
	   // Draw( points.data() );

	}  

};
                        
MyApp * app;

int main(){

	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"CGA2D",&glv);    
	app = new MyApp;   
	
	app -> init(win);
	app -> initVariables(); 
	       
	glv << *app;
	Application::run();

	return 0;
}
