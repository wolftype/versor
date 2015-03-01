#include "vsr_cga3D.h"  
#include "vsr_cga3D_draw.h" 
#include "vsr_cga3D_interface.h"  
#include "vsr_GLVimpl.h"   

#include "vsr_field.h"


using namespace vsr;


struct MyApp : App {    
   
	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
	}

  virtual void onDraw(){
	
	static Field< Frame > f(2,2,2,5); 
	static Field< Dll > f2(2,2,2,5); 

	int num = 10;

	static Field< Pnt > f3(num+1,num+1,num+1);

	for (int i = 0; i < f.num(); ++i){
		Touch(interface, f[i]); 
		Draw(f[i]);
		f2[i] = f[i].dll();
	}  

	for (int i = 0; i <= num; ++i ){ 
		float tu = 1.0 * i/num;
		for (int j = 0; j <= num; ++j ){
			float tv = 1.0 * j/num;  
	    for (int k = 0; k <= num; ++k ){   
				float tw = 1.0 * k/num;  
				Dll dll = f2.vol(tu,tv,tw); 
				Frame fr(dll);
				Draw( fr );   
				f3.at(i,j,k) = fr.pos();
			}
		}
	}  

   // glBegin(GL_QUADS);  
	for (int i = 0; i < f3.face().size(); ++i ){
	   // Draw( f3[ f3.face(i) ] ); 
		//cout << vxl << endl; 
		//cout << vxl << endl; 
		//Draw( f3[vxl.a] );
		//gfx::GL::Quad( f3[vxl.a],f3[vxl.b], f3[vxl.c], f3[vxl.d]);
	} 

   glBegin(GL_QUADS);
	for (int i = 0; i < f3.faceVxl().size(); ++i ){
		Vxl vxl = f3.faceVxl(i);  
		Biv b = Ro::dir( f3[vxl.a] ^ f3[vxl.b] ^ f3[vxl.c] ).copy<Biv>(); 
		gfx::GL::normal( Op::dle(b).unit().begin() );
		gfx::GL::Quad( f3[vxl.a],f3[vxl.b], f3[vxl.c], f3[vxl.d]);
	} 
   glEnd();
	   
	
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
