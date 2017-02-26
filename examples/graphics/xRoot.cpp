
#include "form/vsr_root.h"  
#include "draw/vsr_ega3D_draw.h"
#include "gfx/util/gfx_glv_app.h"

using namespace vsr;  
using namespace vsr::ega;  
using vsr::nga::Proj;
using vsr::nga::Gen;
 
typedef NEVec<4> V4;
typedef algebra< metric<4>,double> Algebra;
                                                                       

auto D4 = Root::System( V4(1,0,0,0), V4(0,1,0,0), V4(0,0,1,0), ( V4(-1,-1,-1, 1) * .5 ) );
auto F4 = Root::System( V4(0,1,-1,0), V4(1,-1,0,0), V4(0,0,1,0), ( V4(-1,-1,-1, 1) * .5 ) );  
auto H4 = Root::System( V4(0,-1,0,0), V4(0,1.618,1,-.618) * .5, V4(0,0,-1,0), ( V4(-.618,0,1, 1.618) * .5 ) ); 

struct MyApp : gfx::GFXAppGui { 
	
	bool d4, f4, h4, bSpin, bOrtho;
	 
	void setup(){
	  bindGLV();
  	gui(d4,"d4")(f4,"f4")(h4,"h4");
		gui(bSpin, "spin")(bOrtho, "ortho");  
		d4 = true;
	}
	
	void onDraw(){
	   
	  scene.camera.lens.bOrtho = bOrtho;  
	  
	  if (d4){
	   
     for (auto &i : D4 ){
			  if (bSpin) i = i.sp( Gen::rot( Multivector< Algebra, Basis<10,12> > (.01,.01) ) );
			  Vec tp = Proj<4>::Call(5.0 , i);
			  Vec to = Proj<4>::Ortho<3>(i);
			  Draw( bOrtho ? to : tp );
		  } 
	  }  
	
	  if (f4){
	   for (auto &i : F4 ){
			if (bSpin) i = i.sp( Gen::rot( Multivector<Algebra, Basis<10,12> > (.01,.01) ) );
			Vec tp = Proj<4>::Call(5.0 , i);
			Vec to = Proj<4>::Ortho<3>(i);
			Draw( bOrtho ? to : tp );

		  } 
	  } 
	 
	  if (h4){
		  for (auto &i : H4 ){
			  if (bSpin) i = i.sp( Gen::rot( Multivector<Algebra, Basis<12> > (.01) ) );
			  Vec tp = Proj<4>::Call(5.0 , i);
			  Vec to = Proj<4>::Ortho<3>(i);
			  Draw( bOrtho ? to : tp );
		  } 
		}  
	
	} 
	       
};
                        



int main(){

    cout << "D4: NumRoots =" << D4.size() << endl;
   for (auto i : D4 ){

	 i.print();

	}

	cout << "\nF4: NumRoots = " << F4.size() << endl;
   for (auto i : F4 ){

	 i.print();

	} 

	cout << "\nH4: NumRoots = " << H4.size() << endl;
	   for (auto i : H4 ){
	
	 i.print();
	
	}  
	
  MyApp app;
  app.start();
	
	return 0;
	
}
