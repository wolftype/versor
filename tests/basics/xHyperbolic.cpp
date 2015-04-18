//Get mouse position as an "unprojected" point in 3D space

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

using namespace vsr;
// using namespace cga3D;

struct Hyperbolic{
	static auto Infinity() RETURNS ( Ori(1) + Inf(-.5) )
}; 
struct Spherical{
	static auto Infinity() RETURNS ( Ori(1) + Inf(.5) )  
}; 
struct Euclidean{
	static Inf Infinity() {return Inf(1); } 
};
 
//hyperbolic, spherical or euclidean Versor                                     
template<class M>
auto Plane( VT x, VT y, VT z, VT dist = 0.0) RETURNS (
	Vec(x,y,z) + ( M::Infinity() * dist )
) 

//hyperbolic, spherical or euclidean Line                                     
template<class M>
auto Line( Pnt a, Pnt b ) RETURNS (
	a ^ b ^ ( M::Infinity() )
)
   
template<class V>
Pnt Point(const V& v){
   return Ro::null(v[0], v[1], v[2]);
}	


struct MyApp : App {    
   
	Pnt mouse;
	Lin ray;
              
  
	bool bHyperbolic, bSpherical;
	float dist;

	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
	}   
	
	void initGui(){
		gui(bHyperbolic, "hyperbolic");
		gui(bSpherical, "bSpherical"); 
		gui(dist, "dist",-100,100);
	}
  
  	void getMouse(){
		auto tv = interface.vd().ray; 
		Vec z (tv[0], tv[1], tv[2] );
		ray = Point( interface.mouse.projectMid ) ^ z ^ Inf(1); 
    	mouse = Ro::null( Fl::loc( ray, Ori(1), false) );  
  }

  	virtual void onDraw(){ 
        
		getMouse();
				       
	    //circle on xz plane with radius 1 		
		Cir c = CXZ(1); 
		                                  
		// if (bHyperbolic) {
		// 	auto a = Plane< Hyperbolic > (1,0,0,dist);  
		// 	Draw( c.reflect( a ) );  
		// 	Draw( mouse.reflect(a) );  
		// 	Draw(a,1,0,0,.2);
		// } else if (bSpherical){
		// 	auto a = Plane< Spherical > (1,0,0,dist); 
		// 	a.bprint(); a.vprint();
		// 	Draw( c.reflect( a ) ); 
		// 	Draw( mouse.reflect(a) ); 
		// 	Draw(a,1,0,0,.2);
		// } else {
		//     auto a = Plane< Euclidean > (1,0,0,dist);
		// 	
		// 	Draw( c.reflect( a ) );
		// 	Draw( mouse.reflect(a) );
		// 	Draw(a,1,0,0,.2); 
		// }   
		       
		
	   // void * tmp;
		                                           
		
		if (bHyperbolic){
			auto a = Line<Hyperbolic>( mouse, Ro::null(1,0,0) );
		    Draw( Cir(a) ); 
			
			for (int i = 0; i < 100; ++i){ 
				double t = 1.0 * i/100;   
				
				auto b = Gen::bst(  a * Hyperbolic::Infinity() * -t );   
				
				Draw( Ro::loc( mouse.spin(b) ), 1,0,0 );
			}  
		} else if (bSpherical){
			auto a = Line<Spherical>( mouse, Ro::null(1,0,0) );  
		    Draw(Cir(a));
		  // a.bprint();   
		}
		           
		Trv(1,2,0,0).vprint();
		
		(~(Trv(2))).vprint();
		
		( (Trv(2.5,2,0,1)) * ~(Trv(2.5,2,0,1))).vprint();

		//Draw(*tmp);  
		
		
		
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
