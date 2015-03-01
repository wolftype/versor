#include "vsr_cga3D.h"
#include "vsr_cga3D_funcs.h"   
#include "vsr_GLVimpl.h"

#include "vsr_field.h"

using namespace vsr;
using namespace vsr::cga3D; 

struct MyApp : App {    
   
	Pnt mouse;
	Lin ray;       
	
	Field<Pnt> field; 
	float amtA,amtB; 
	
	Pnt pntA, pntB;

	MyApp(Window * win ) : App(win)//, field(10,10,1)
	
	{
		scene.camera.pos( 0,0,10 ); 
	    field.resize(10,10,1);   
	
		pntA = Point(-2,0,0);
		pntB = Point(2,0,0);
	}               
	
	void initGui(){
		gui(amtA,"amtA",0,PI);
	    gui(amtB,"amtB",0,PI);   
	}
  
  	void getMouse(){
		auto tv = interface.vd().ray; 
		Vec z (tv[0], tv[1], tv[2] );
		ray = Ro::point( tv[0], tv[1], tv[2] ) ^ z ^ Inf(1); 
    	mouse = Ro::point( ray,  Ori(1) );  
  }

  	virtual void onDraw(){ 
        
		getMouse();      
		
		//First Fold
		 
		Lin line = Point(-2,-2,0) ^ Point(-1,1,0) ^ Inf(1);
		Dll dualLine = line.dual().runit();         
		
		auto b = Gen::mot( dualLine * -amtA/2.0 );
		
		auto d = ( line ^ Vec::z ).dual();
		auto c = ( mouse <= d ); 
		
		c.vprint(); c.bprint(); printf("\n%f\n", c.wt());

		for (int i = 0; i < field.num(); ++i){
			if ( (field.grid(i) <= d )[0] > 0 ) {
				field[i] = field.grid(i).spin( b );
			}  
		}            
		 
		Lin line2 = pntA^ pntB ^ Inf(1); 
		Dll dualLine2= line2.dual().runit();       
		auto b2 = Gen::mot( dualLine2 * -amtB/2.0 );
		
		auto d2 = ( line2 ^ Vec::z ).dual();  
		// //Second Fold
		 for (int i = 0; i < field.num(); ++i){
			if ( (field.grid(i).spin( b ) <= d2 )[0] > 0 ) {
				field[i] = field.grid(i).spin( b2 * b );
			}  
		}    
		               
		Draw(dualLine2,0,1,1);
		Draw(d2,1,1,0);    
		
		Draw(field);
		Draw(dualLine,0,1,0); 
	    Draw(d,1,0,0);
		Draw(pntA,0,0,1); Touch(interface,pntA);
		Draw(pntB,0,0,1); Touch(interface,pntB);
		
	}
   

	
};


MyApp * app;


int main(){
                             
	GLV glv(0,0);	

	Window * win = new Window(500,500,"Versor",&glv);    
	app = new MyApp( win ); 
	app -> initGui();
	
	glv << *app;

	Application::run();

	return 0;

}
