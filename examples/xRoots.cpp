#include "vsr_root.h"  
#include "vsr_GLVimpl.h" 
#include "vsr_ega3D_draw.h"

using namespace vsr;  

 

typedef EGAMV< 4, Blade1<4>::VEC > V4;

//idositetrachoron 24 cell                                                                         

auto D4 = rootSystem<4>( V4(1,0,0,0), V4(0,1,0,0), V4(0,0,1,0), ( V4(-1,-1,-1, 1) * .5 ) );

auto F4 = rootSystem<4>( V4(0,1,-1,0), V4(1,-1,0,0), V4(0,0,1,0), ( V4(-1,-1,-1, 1) * .5 ) );  

auto H4 = rootSystem<4>( V4(0,-1,0,0), V4(0,1.618,1,-.618) * .5, V4(0,0,-1,0), ( V4(-.618,0,1, 1.618) * .5 ) ); 

struct MyApp : App { 
	
	bool d4, f4, h4, bSpin, bOrtho;
	 
	virtual void initGui(){
		gui(d4,"d4")(f4,"f4")(h4,"h4");
		gui(bSpin, "spin")(bOrtho, "ortho");  
		d4 = true;
	}
	
	void onDraw(){
	   
	  scene.camera.lens.bOrtho = bOrtho;  
	  
	  if (d4){
	   for (auto &i : D4 ){
			if (bSpin) i = i.sp( Gen::rot( EGAMV<4, MV<10,12> > (.01,.01) ) );
			Vec tp = Proj<4>::Call(5.0 , i);
			Vec to = Proj<4>::Ortho<3>(i);
			Draw( bOrtho ? to : tp );

		} 
	  }  
	
	  if (f4){
	   for (auto &i : F4 ){
			if (bSpin) i = i.sp( Gen::rot( EGAMV<4, MV<10,12> > (.01,.01) ) );
			Vec tp = Proj<4>::Call(5.0 , i);
			Vec to = Proj<4>::Ortho<3>(i);
			Draw( bOrtho ? to : tp );

		} 
	  } 
	 
	if (h4){
		  for (auto &i : H4 ){
			if (bSpin) i = i.sp( Gen::rot( EGAMV<4, MV<12> > (.01) ) );
			Vec tp = Proj<4>::Call(5.0 , i);
			Vec to = Proj<4>::Ortho<3>(i);
			Draw( bOrtho ? to : tp );
		
		} 
			  }  
	
	text("Alt + Mouse to Rotate the Group\nShift + Mouse to Zoom in");
	} 
	       
};
                        
MyApp * myApp;

int main(){

    cout << "D4: NumRoots =" << D4.size() << endl;
   for (auto i : D4 ){

	 i.vprint();

	}

	cout << "\nF4: NumRoots = " << F4.size() << endl;
   for (auto i : F4 ){

	 i.vprint();

	} 

	cout << "\nH4: NumRoots = " << H4.size() << endl;
	   for (auto i : H4 ){
	
	 i.vprint();
	
	}  
	
	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"4D ROOTS",&glv);    
    myApp = new MyApp;
	myApp -> init( win ); 
	myApp -> initGui();
  
	glv << *myApp;

	Application::run();
	
	return 0;
	
}
