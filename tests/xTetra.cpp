
#include "vsr_products.h"
#include "vsr_GLVimpl.h" 
#include "vsr_ncube.h"

#include "gfx/gfx_glyphs.h"  


using namespace vsr;
using namespace std;    

template< class B >
auto nrot( const B& b) -> EGAMV<decltype( B() + 1 )> { 	
	VT  c = sqrt(- ( b.wt() ) );
    VT sc = -sin(c);
    if (c != 0) sc /= c;
	return  (b*sc) + cos(c);
} 


struct MyApp : App {
	         
	
	bool bOrtho;
	static const int DIM = 6; 

	NCube<DIM> tetra;
   	
	MyApp(Window * w ) : App(w) {
		initGui();
	}
	
	virtual void initGui(){  
		printf("init gui\n");
		gui(bOrtho); 
	}   
	
	virtual void onDraw(){
	   	// EGAMV< MV<9,12,34,96> > b( .01,.01,.01,.01 );// .01, .01 );   
		// 
		
		scene.camera.lens.bOrtho = bOrtho;
		
		EGAMV< MV<12> > b(.01);
		typedef EGAMV< typename EGA<3>::Vec > Vec; 
		
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
		 	 glColor4f(t,1,1-t, col[it]);
		     gfx::Glyph::Dir( i ); 
		 } 
		// 
		glColor3f(0,1,0);                             
		// 
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