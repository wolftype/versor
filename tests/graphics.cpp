#define __EUC__ //must define global metric !

#include "GLV/glv.h" 
#include "GLV/glv_binding.h"  

#include "gfx/gfx_gl.h"  
#include "gfx/gfx_scene.h"

#include "root.h" 
#include "tetra.h"

using namespace vsr;
using namespace glv;  


typedef Blade1<4>::VEC Vec4;
typedef EGA<3>::Rot Rot; 
typedef EGA<3>::Biv Biv;  

//idositetrachoron 24 cell
auto D4 = rootSystem<4>( Vec4(1,0,0,0), Vec4(0,1,0,0), Vec4(0,0,1,0), ( Vec4(-1,-1,-1, 1) * .5 ) );     




struct App : public View3D{  
  
  Scene scene;
  Rot modelView;
  
  App() : View3D(), modelView(1,0,0,0) {
	
	stretch(1,1);
	colors().back.set(.1,.3,.3);
	
  }   

  virtual void onDraw3D(GLV& glv){   
	    
	    static VT time = 0;
		modelView = Gen::rot( Biv(0,1,0) * time );
		time+=.01;
     
		scene.push();
	    
		Rot t = Gen::aa( modelView );
		gfx :: GL :: rotate( t.begin() );

		glBegin(GL_LINES);
    	for (auto i : D4 ){
			gfx::GL::vertex(0,0,0);
			gfx::GL::vertex( Proj<4>::Call(2.0, i) ); 
      	} 
		glEnd();
        
		scene.pop();
 }     

  virtual void onKeyDown(){
	
	
 }                                                                                             
	
	
	
};
                        
App * myApp;

int main(){

	myApp = new App();
	myApp -> stretch(1,1);
	
	GLV glv(0,0);	
	//glv.colors().back.set(.3,.3,.3);
    		        
	Window * win = new Window(500,500,"Versor",&glv);    
    
	glv << *myApp;

	
	Application::run();
	
	return 0;
	
}