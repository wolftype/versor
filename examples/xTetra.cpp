#include "vsr_products.h"
                                
//nd cubes
#include "vsr_ncube.h"  

//it all gets projected down to 3d eventually so
#include "vsr_ega3D_draw.h"

//handle window events and stuff
#include "vsr_GLVimpl.h"  

using namespace vsr;
using namespace std;    

template<  class B >
auto nrot( const B& b) ->  decltype( B() + 1 ) { 	
	VT  c = sqrt(- ( b.wt() ) );
    VT sc = -sin(c);
    if (c != 0) sc /= c;
	return  (b*sc) + cos(c);
} 


struct MyApp : App {
	         
	bool bOrtho;
	static const int DIM = 4; 
	NCube<DIM> tetra;
  
  float e14,e24,e34;
   	
	MyApp(Window * w ) : App(w) {
		initGui();
	}
	
	virtual void initGui(){  
		printf("init gui\n");
		gui(bOrtho, "ortho"); 
    gui(e14,"e14",-1,1);
    gui(e24,"e24",-1,1);
    gui(e34,"e34",-1,1);


	}   
	
	virtual void onDraw(){

    typedef EGAMV< DIM, MV< blade(1,4), blade(2,4), blade(3,4) > BIV;
	
    //EGAMV< DIM, typename EGA<DIM>::Biv > b(0,0,0,e14,e24,e34);// .01, .01 );   
		
    BIV(e14,e24,e34);

		scene.camera.lens.bOrtho = bOrtho;
		
		VT dist = scene.camera.pos().len();

    auto proj = tetra.project(dist);

    auto val = tetra.val(dist);
		
		glColor3f(1,0,0);                             
		 
		for (auto i : tetra.edges ) gfx::Glyph::Line( proj[i.a], proj[i.b] );     
	 
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
