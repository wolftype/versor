#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

#include "vsr_set.h"

using namespace vsr; 
   

// template<class T>
// class TSet {
// 	vector<T> mData;  
// };
      
template<class T>
class DrawObj : public Set<T> {
   
//	Set< Vec4f > mColors;
   public:
	 
   void draw(){
	
		for (auto i : this -> mData) Draw(i);
	
	}   
};
   

enum Mode{
	Normal,
	Insert,
	Select
};

struct MyApp : App { 
	
	int mMode;   
   
	Pnt mouse;
	Lin ray;  
	
	DrawObj<Cir> vcir;
	

	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
	}
  
  	void getMouse(){
		auto tv = interface.vd().ray; 
		Vec z (tv[0], tv[1], tv[2] );
		ray = Point( interface.mouse.projectMid ) ^ z ^ Inf(1); 
    	mouse = Point( ray,  Ori(1) );  
  }

  	virtual void onDraw(){ 
       
	  	Draw(CXY(1));
	  	vcir.draw();
		
	}
     

	 virtual bool onEvent(Event::t e, GLV& glv){

	     interface.glv().getKeyboardData(&glv);
	     interface.glv().getMouseData(&glv); 
	
		switch(e){
	         case Event::MouseMove:
	             interface.onMouseMove();
	             break;
	         case Event::MouseDown:
	             interface.onMouseDown();
	             break;
	         case Event::MouseDrag:
	             interface.onMouseDrag();
	             break;
	         case Event::MouseUp:
	             interface.onMouseUp();
	             break;
	         case Event::KeyDown:
	             onKeyDown(glv);
	             interface.onKeyDown();
	             break;
	         case Event::KeyUp:
	             interface.onKeyUp();
	             break;

	     }
         
		 if( mMode == Mode::Normal ) {   		 
			 App::onEvent(e,glv);
	     } else if (mMode & Mode::Insert){
			
		} 
	        



	     return true;
	 }

	virtual bool onKeyDown(GLV& glv){                          
		
		 printf("keyPressed:\t%d\n", glv.keyboard().key()); 	                   
		 switch( glv.keyboard().key() ){   
			
			    case 27:
					mMode = Mode::Normal;
					break;
				
				case 'i':
					mMode |= Mode::Insert;
					break;
			
			 }  
					
			printf("mode: \t%d\n", mMode);      
			return true;
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
