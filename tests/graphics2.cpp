// #define __CON__ //must define global metric !
// #define GLOBAL_METRIC RMetric<4,1>::Type()     

#include "operations.h"
#include "vsr_GLVimpl.h" 
#include "vsr_draw.h"

using namespace vsr;
using namespace glv;  


void GLVApp :: onDraw(){
	
	static Cir cir = CXY(1);
	                                         
	static VT time = 0.0; time += .01;
	Mot M = Gen::mot( DLN(0,1,0) * time); 
	Rot R = Gen::rot( Biv::xz * .01 );
	        
	Vec vec = Vec::x.sp( Gen::rot(Biv::xy * time ) );
	Draw( vec,1,0,0 );                                     
	Dlp dlp = Dlp(vec).sp( Gen::trs(1,0,0) );
	Draw( dlp,0,1,0 );
	cir = cir.sp( R ); 
	Draw( cir,0,0,1 );
	
	Par par = ( dlp ^ cir.dual() ).dual();
	Draw(par);

}
                        
GLVApp * myApp;

int main(){

	myApp = new GLVApp();
	myApp -> stretch(1,1);
	
	GLV glv(0,0);	
	//glv.colors().back.set(.3,.3,.3);
    		        
	Window * win = new Window(500,500,"Versor",&glv);    
    
	glv << *myApp;

	
	Application::run();
	
	return 0;
	
}