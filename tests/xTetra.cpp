
#include "vsr_products.h"
#include "vsr_GLVimpl2.h" 
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

static const int DIM = 4; 

NCube<DIM> tetra;  
  
void GLVApp :: onDraw(){ 
		

		// EGAMV< MV<9,12,34,96> > b( .01,.01,.01,.01 );// .01, .01 );   
		//
		EGAMV< MV<12> > b(.01);
		typedef EGAMV< typename EGA<3>::Vec > Vec; 
		
		vector<Vec> proj;  
		vector<VT> col;   	  
		// 
		VT dist = scene.camera.pos().len();
		
		printf("dist: %f\n", dist);  
		// 		 
		auto& res = tetra.roots; 
		// 
		for (auto& i : res){   
		
			i = i.sp( nrot( b ) ); 
		
			auto tmp = Proj<DIM>::Call(dist,i); 
			auto val = Proj<DIM>::Val(dist,i );
			proj.push_back(tmp);
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

	   

    
	GLVApp * myApp;

	int main(){

		myApp = new GLVApp();
		myApp -> stretch(1,1);

		GLV glv(0,0);	

		Window * win = new Window(500,500,"Versor",&glv);    

		glv << *myApp;
        
	   printf("DIM: %d\n", DIM);


		Application::run();

		return 0;

	}