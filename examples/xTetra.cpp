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
	   EGAMV< DIM, typename EGA<DIM>::Biv > b( 0,0,0,e14,e24,e34);// .01, .01 );   
		// 
		
		scene.camera.lens.bOrtho = bOrtho;
		
		
		vector<Vec> proj;  
		vector<VT> col;   	  
		// 
		VT dist = scene.camera.pos().len();
		
		//printf("dist: %f\n", dist);  
		// 		 
		auto& res = tetra.roots; 
		// 
		for (auto& i : res){   
		
			i = i.sp( nrot( b ) ); 
		
			auto tmp = Proj<DIM>::Call(dist,i);
			auto tort = Proj<DIM>::Ortho<3>(i); 
			 
			auto val = Proj<DIM>::Val(dist,i );
			proj.push_back( bOrtho ? tort : tmp);
			col.push_back( val );
		} 
		// 
		int it = 0;    	
		for (auto i : proj ) { 
			float t = ( 1.0 * it) / proj.size();
			it++;
		   //   glColor4f(t,1,1-t, col[it]);
			//Draw(i, t, 1, 1-t, col[it] ); 
			//gfx::Glyph::Dir( i ); 
		 } 
		// 
		glColor3f(1,0,0);                             
		// 
		for (auto i : tetra.edges ) gfx::Glyph::Line( proj[i.a], proj[i.b] );     

	 
		//text("Alt + Mouse Drag to Rotate View");
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
