// EUCLIDEAN REFLECTIONS

#include "draw/vsr_ega3D_draw.h" 

#include "gfx/util/gfx_glv_app.h" 
 

using namespace vsr;

struct MyApp : gfx::GFXAppGui {
	
	float theta, phi;
	float time;
	bool bRef;
	
	void setup(){
    bindGLV();
		gui(theta,"theta",-TWOPI,TWOPI); 
		gui(phi,"phi",-PIOVERTWO,PIOVERTWO);
		gui(bRef,"reflections");  
	}

	void onDraw(){
		time += .01;               
		//VECTORS
		Vec x(1,0,0);   
		Vec y(0,1,0);    
    Vec z(0,0,1);    

		Draw(x,1,0,0); 		
		Draw(y,0,1,0);  
		Draw(z,0,0,1);
		  	    
    //BIVECTORS
		Biv xy = y^x;    
		Biv xz = x^z;    
	  Biv yz = y^z;    

		Draw(xy,1,1,0); 
		Draw(xz,1,0,1);  
		Draw(yz,0,1,1);   
		
		//ROTATIONS 
		Rot tr =  Gen::rot( xz * theta / 2.0 ); 
		Rot pr = Gen::rot( xy.sp(tr) * phi /2.0); 
		Vec nv = x.sp( pr * tr ); 
		
		Draw(xy.sp(tr));
		Draw( nv ); 
		
		//REFLECTIONS
		if (bRef) {
			Draw( nv.re( x ),1,.5,.5 );  
			Draw( nv.re( y ),.5,1,.5 ); 
			Draw( nv.re( z ),.5,.5,1 ); 
		} 
		
	}

};
                        

int main(){
 
  MyApp app;
  app.start();	
	return 0;
	
}
