#ifndef GLV_APP_INCLUDED
#define GLV_APP_INCLUDED     


/*

	IMPLEMENTATION FILE  - This implements GLV library 
	1 - copies state of window, keyboard, and mouse to generic interface data
	2 - calls interface and mouse on events

*/

#include "GLV/glv.h" 
#include "GLV/glv_binding.h"  

#include "gfx/gfx_gl.h"  
#include "gfx/gfx_scene.h"  

#include "frame.h"
#include "vsr_interface.h"

using namespace vsr;
using namespace glv;    

struct GLVImpl : public Interface::Impl {    
	
	Window * win;
	
	GLVImpl( Interface * i ) : Interface::Impl(i) {}

	virtual void getViewData(void * udata) {
        GLV& glv = *(GLV*)(udata);
        
        //COPY VIEW DATA
        interface -> vd().w		 = glv.width();
        interface -> vd().h		 = glv.height(); 

	   // cout << "ViewData Copy: " << endl;

    }
	
    virtual void getKeyboardData(void * udata){
           GLV& glv = *(GLV*)(udata);
           
           //COPY KEYBOARD DATA
           interface -> keyboard.alt       = glv.keyboard().alt();
           interface -> keyboard.shift     = glv.keyboard().shift();
           interface -> keyboard.ctrl      = glv.keyboard().ctrl();
           interface -> keyboard.caps      = glv.keyboard().caps();
           interface -> keyboard.meta      = glv.keyboard().meta();	
           interface -> keyboard.code      = glv.keyboard().key();
    }                                

    virtual void getMouseData(void * udata){
           GLV& glv = *(GLV*)(udata);       
           //COPY MOUSE DATA
           interface -> mouse.x            = glv.mouse().x();
           interface -> mouse.y            = glv.mouse().y();

           interface -> mouse.dx           = glv.mouse().dx() / glv.width();
           interface -> mouse.dy           =  - glv.mouse().dy() / glv.height();
           
           interface -> mouse.ddx          = glv.mouse().ddx();
           interface -> mouse.ddy          = glv.mouse().ddy();
           
           interface -> mouse.xrel         =  glv.mouse().xRel() / glv.width();
           interface -> mouse.yrel         = 1 - glv.mouse().yRel() / glv.height();
           
           interface -> mouse.pos          = Vec( glv.mouse().x() / glv.width(), 1 - glv.mouse().y() / glv.height(), 0 ) ;
           interface -> mouse.move         = Vec( glv.mouse().dx()/ glv.width(), - glv.mouse().dy()/glv.height(), 0 ) ;
           interface -> mouse.accel        = Vec( glv.mouse().ddx(), -glv.mouse().ddy(),0);
       }
}; 


struct GLVInterface : public Interface {
	GLVInterface() : Interface() { init(); }
	GLVImpl& operator()(){ return *(GLVImpl*)impl; }
    virtual void init(){
		impl = new GLVImpl(this);
	}  
};

struct GLVApp : public View3D{  

  GLVInterface interface;
              
  gfx::Scene scene; 

  Frame frame; 		// Camera
  Rot modelView;    // ModelView
  
  
  GLVApp() : View3D(), 
    frame(0,0,5),
	modelView(1,0,0,0) {
	
	stretch(1,1);
	colors().back.set(.1,.3,.3);
	
  }   


  virtual void onDraw();

  virtual void onDraw3D(GLV& glv){   
	    
	   interface().getViewData(&glv);
	
		scene.fit( interface.vd().w, interface.vd().h );
 
		scene.cam.set( frame );
		scene.push();
	    
		Rot t = Gen::aa( modelView );
		gfx :: GL :: rotate( t.begin() );

		onDraw();
        
		scene.pop();
 }  

 virtual bool onEvent(Event::t e, GLV& glv){

     interface().getKeyboardData(&glv);
     interface().getMouseData(&glv); 
     
     switch(e){
         case Event::MouseMove:
            // interface.onMouseMove();
             break;
         case Event::MouseDown:
             //interface.onMouseDown();
             break;
         case Event::MouseDrag:
             //interface.onMouseDrag();
             break;
         case Event::MouseUp:
             //interface.onMouseUp();
             break;
         case Event::KeyDown:
             //onKeyDown(glv);
             //interface.onKeyDown();
             break;
         case Event::KeyUp:
             //interface.onKeyUp();
             break;
             
     }
     
     return false;
 }   

  virtual bool onKeyDown(GLV& glv){ 
	
	cout << "key down" << endl;
	switch( glv.keyboard().key() ){
		case glv::Key::Up : {
			cout << "up" << endl;
		}
	 

	}
	  
	return true;    
	
 }                                                                                             
	
	
	
};   

#endif