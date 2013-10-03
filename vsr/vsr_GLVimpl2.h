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
#include "gfx/gfx_pipe.h" 
#include "gfx/gfx_interface.h"  


using namespace std;
using namespace glv;
using namespace gfx;

using namespace gfx::GLSL; 
                          

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
           
           interface -> mouse.pos          = Vec3f( glv.mouse().x() / glv.width(), 1 - glv.mouse().y() / glv.height(), 0 ) ;
           interface -> mouse.move         = Vec3f( glv.mouse().dx()/ glv.width(), - glv.mouse().dy()/glv.height(), 0 ) ;
           interface -> mouse.accel        = Vec3f( glv.mouse().ddx(), -glv.mouse().ddy(),0);
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
  
	  int renderMode;

	  GLVInterface interface;
              
	  gfx::Scene scene;
	  Pipe pipe; 

	  Mat4f mvm;
    
  GLVApp() : View3D(), 
    renderMode(0)
 {
	
	stretch(1,1);
	colors().back.set(.1,.3,.3); 
	
	interface.scene = &scene;
	
	scene.camera.bUseFrust = false; 
	initView(); 
	
	printf("GLVAPP done\n");
  }  

	virtual ~GLVApp();

	virtual void initGL(){
	    string Vert = AVertex + VaryingN + UMatrix  + NTransform + VLighting + VCalc + MVertN; 
	   // string Vert = SimpleVertex;//SimpleVertex;//AVertex + VaryingN + MVertN; 
		string Frag = MFragN;//TFragMix; //mFrag   //SimpleFragment;//
         
                               
		printf("%s\n", "initGL");
		
		//cout << Vert << Frag << endl; 

		pipe.program = new ShaderProgram(Vert,Frag,0);
		pipe.bind();
		pipe.bindAll();
		pipe.unbind();
		
		//pipe.bindAll(); 
	} 
	
		void initView(){
            

			cout << width() << " by  " << height() << endl; 
			float tw =  width();
			float th = height();     

		    float aspect = 1.0 * tw / th;   
          
			scene.fit(tw,th);

			int numscreens = 4;

			Pose p(-tw/2.0,-th/2.0, 0); //bottom left of screen

			scene.camera.view = gfx::View( scene.camera.pos(), p, aspect, th ); 
			
		    //cout << scene.xf.projMatrixf() << endl;  
    }

  virtual void onDraw();
  virtual void update(){}

  virtual void onDraw3D(GLV& glv){   
	    
	   interface().getViewData(&glv);
	   
	
	   //cout << interface.vd().w << " " << interface.vd().h << endl; 
		scene.resize( interface.vd().w, interface.vd().h );
 
		//scene.camera.set( cam ); 
		//scene.model.set( model ); 
		switch( renderMode ){
			case 0: renderA(); break;
			case 1: renderB(); break;
			case 2: renderC(); break;
		}
 } 
     //immediate mode
	 void renderA(){
	        
			scene.camera.step();
			scene.model.step();
		
		 	scene.push();    	
			scene.getMatrices();  
		    interface.viewCalc(); 
			
			onDraw();
     
			scene.pop();
	}  
	
	//vbo render (opengl es 2.0)
	void renderB(){     
		            	  
			 // glViewport(0,0,surface.width,surface.height); 
			 // 	         glClearColor(background[0],background[1],background[2],background[3]);
			 // 	         glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     
             
             scene.onFrame();    //update matrices
		   // scene.camera.print(); 
             update();

		   	 mvm = scene.xf.modelViewMatrixf(); 

	         pipe.bind( scene.xf );

	         	onDraw();
			 
		     pipe.unbind();
			 //swapBuffers();

	}     
	
	//simple render (point)
	void renderC(){
		scene.onFrame(); 
		update();
		pipe.bind();
		onDraw();   
		pipe.unbind();
	}
   
 	//render to texture
	void renderD(){
		
	}

 virtual bool onEvent(Event::t e, GLV& glv){

     interface().getKeyboardData(&glv);
     interface().getMouseData(&glv); 
     
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
             //onKeyDown(glv);
             interface.onKeyDown();
             break;
         case Event::KeyUp:
             interface.onKeyUp();
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

GLVApp::~GLVApp(){}  

#endif