//Get mouse position as an "unprojected" point in 3D space

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

#include "vsr_field.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {    
 
  float time;
   
	Pnt mouse;
	Lin ray;

  Field<Point> field;

	MyApp(Window * win ) : App(win){
		time = 0;
    scene.camera.pos( 0,0,10 );
    
    field.resize(10,10,1); 
	}
  
  	void getMouse(){
  		auto tv = interface.vd().ray; 
	  	Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
		  ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
    	mouse = Round::point( ray,  Ori(1) );  
  }

  	virtual void onDraw(){ 
      
      time += .01;

      float t = sin(time);
        
	  	getMouse();

      Draw(field);

      //DualPlane dlp = Dlp(1,0,0,2).rot( Bivector::xy * t );

      static DualPlane dlp(1,0,0,2);
      Touch(interface, dlp);

      Draw (dlp,1,0,0);

      DualLine dll = dlp ^ DualPlane(0, 0, 1);

      Motor mot = Gen::mot( dll * t  );

      for (int i = 0; i < field.num(); ++i){
        auto r = field.grid(i) <= dlp;
        if ( r[0] > 0 ) {
          field[i] = field.grid(i).spin( mot /*what thing???*/); 
        }
      }
		
	}
   

	
};


MyApp * app;


int main(){
                             
	GLV glv(0,0);	

	Window * win = new Window(500,500,"Versor",&glv);    
	app = new MyApp( win ); 
  
	
	glv << *app;

	Application::run();

	return 0;

}
