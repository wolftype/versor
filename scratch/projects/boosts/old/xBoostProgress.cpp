#include "vsr_app.h"  
#include "form/vsr_field.h"

struct MyApp : App {


  virtual void setup(){

  }
	
	virtual void onDraw(){       
		
		float spread = 5.0; 
		
		static Field<Pnt> f(20,20,1);

		static float time = 0.0; time +=.01;

		for (int i = 0; i < f.h(); ++i){   
			
			float t = -spread/2.0 + spread * i/f.h();
			
			auto a = Line( Point(0,t,0), Vec(1,0,0) );		
			
			auto tnv = Tnv(0,0,.15) * (1+t);//.rot( Biv::xz * PIOVERTWO * t );
			auto par = Par( tnv * sin(time) );//.trs(0,t,0);  
			
			auto b = par == Par() ? Cir(a) : Cir(a).bst( par ) ;//bst( par ); 

		  // Draw(b);  

			for (int j = 0; j < f.w(); ++j){ 
				float t2 =-spread/2.0 + spread * i/f.w();   
				auto dlp = Plane(1,0,0, t2);
				auto m = Meet( b, dlp );
				f.at(j,i,0) = PointB(m); 

			}    
		}
	    
		// glBegin(GL_QUADS);
		// 	    for (int i = 0; i < f.w()-1; ++i) {
		//     for (int j = 0; j < f.h()-1; ++j) {       
		// 	    
		// 		int a = f.idx(i,j,0);
		// 	    int b = f.idx(i,j+1,0); 
		// 		int c = f.idx(i+1,j+1,0); 
		// 		int d = f.idx(i+1,j,0); 
		// 		gfx::GL::Quad( f[a], f[b], f[c], f[d] ); 
		// 	}
		// }  
		// glEnd(); 
	   
	  Draw(f);
	}
	
};




/*-----------------------------------------------------------------------------
 *  start app
 *-----------------------------------------------------------------------------*/
int main(){
                             
  MyApp app;
  app.start();

	return 0;

}
