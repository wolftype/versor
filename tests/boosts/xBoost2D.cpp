#include "vsr_cga2D_types.h"  
#include "vsr_cga2D_draw.h" 
#include "vsr_GLVimpl.h" 

using namespace vsr;
     

struct MyApp : App {
                   
	float time;   
	
	Pnt * field;
	int width;
	int height;  
	
	float amt;
	
	void initVariables(){  
		
		gui(amt, "amt", -10,10);
		width = 10; height = 10;
		field = new Pnt[width * height];
		
		for (int i = 0; i < width; ++i){   
			float u = 1.0 * i/width;
		  	for (int j = 0; j < height; ++j){ 
				float v = 1.0 * j/height; 
				
				int idx = i * width + j;
				field[idx] = Ro::null( Vec(-2.0 + u*4, -2.0 + v*4) );
			}
		}
	}
	
	void onDraw(){
	                                                        
		time +=.1;
		Dls cir = Ro::dls( Vec(0,0),  1.0).trs( Drv(sin(time),0) );  

		Draw(cir);  

		
		Pnt a = Ro::point(1,0);
		Pnt b = Ro::point(-1,0);   
		
		Lin lina = a ^ Vec(0,.5) ^ Inf(1);//Line(a,b);
		Lin linb = (a ^ b ^ Inf(1)).rot( Biv(time) );//Line(a,b);   
		
		auto m = linb/lina;
		
	//	m.bprint();
		Dll().bprint();
		
		 Draw(linb);  
		
		auto pp =  (linb.dual() ^ cir ).dual();//.bprint(); 
	   

		Draw( pp,1,0,0 );  
		
		
		
		
		Bst bst = Gen::bst( Par( Tnv(0,1) ) * amt );//.trs(  Drv(sin(time),0) ) * amt );
		bst.vprint();
		for (int i = 0; i < width; ++i){   
			float u = 1.0 * i/width;
		  	for (int j = 0; j < height; ++j){ 
				float v = 1.0 * j/height;
				int idx = i * width + j;
				Pnt np = Ro::loc( field[idx].sp(bst) );//Ro::loc(  );
				//field[idx] = np;
				np.vprint();
				Draw(np);
		  }
			
		}
		 
	
	}  

};
                        
MyApp * app;

int main(){

	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"CGA2D",&glv);    
	app = new MyApp;   
	
	app -> init(win);
	app -> initVariables(); 
	       
	glv << *app;
	Application::run();

	return 0;
}
