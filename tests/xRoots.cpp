#include "vsr_root.h"  
#include "vsr_GLVimpl.h" 
#include "vsr_ega3D_draw.h"

using namespace vsr;  

 

typedef EGAMV< 4, Blade1<4>::VEC > V4;

//idositetrachoron 24 cell                                                                         

auto D4 = rootSystem<4>( V4(1,0,0,0), V4(0,1,0,0), V4(0,0,1,0), ( V4(-1,-1,-1, 1) * .5 ) );

auto F4 = rootSystem<4>( V4(0,1,-1,0), V4(1,-1,0,0), V4(0,0,1,0), ( V4(-1,-1,-1, 1) * .5 ) );  

struct MyApp : App { 
	
	void onDraw(){
	      
	   for (auto &i : D4 ){
			i = i.sp( Gen::rot( EGAMV<4, MV<10,12> > (.01,.01) ) );
			auto r = Proj<4>::Call(5.0 , i);
			Draw( r );

		}    
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

	
	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"4D ROOTS",&glv);    
    myApp = new MyApp;
	myApp -> init( win );
  
	glv << *myApp;

	Application::run();
	
	return 0;
	
}
