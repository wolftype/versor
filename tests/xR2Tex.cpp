 // #define __CON__ //must define global metric !
// #define GLOBAL_METRIC RMetric<4,1>::Type()     

#include "vsr_cga3D.h"  
#include "vsr_GLVimpl.h" 
#include "vsr_render.h" 

#include "gfx/gfx_texture.h"
#include "gfx/gfx_fbo.h"  

using namespace vsr;
using namespace glv;  

 
void GLVApp::onDraw(){}

struct MyApp : public GLVApp {
    
	//ALL THIS IS THE GL PIPELINE STUFF.
	MBO * rect;   
	Pipe texpipe;  
	   
	FBO * fboA;
	Texture * textureA;    
	Texture * textureB;   
	                
	MyApp(Window * w) : GLVApp(w) {
		
		printf("MyApp Construct\n");
	}   
	
    virtual ~MyApp(); 

	void swapTextures(){
		Texture * tmp = textureA; textureA = textureB; textureB = tmp;
	}
	
	void setFrameBuf(){
		texpipe.program -> uniform("frameBufSize", scene.camera.lens.width(), scene.camera.lens.height() );
	}  

	virtual void initGL(){
		
		printf("INITGL\n");
	
		string Vert = AVertex + VaryingN + UMatrix  + NTransform + VLighting + VCalc + MVertN; 
		string Frag = MFragN;
	
		pipe.program = new ShaderProgram(Vert,Frag,0);
		pipe.bind();
		pipe.bindAll();
		pipe.unbind(); 
		
		float tw = width();//scene.camera.lens.width(); 
		float th = height();//scene.camera.lens.height();
		
		printf("%f, %f\n", tw, th);                
		
		fboA = new FBO();
		textureA = new Texture( tw, th );
		textureB = new Texture( tw, th );
		texpipe.program = new ShaderProgram( ClipSpaceVert, FOGL+FXAA, 0 );//Frag, 0);//	
		texpipe.bindAll();
	 	fboA -> attach(*textureA, GL::COLOR);  
	
		rect = new MBO( Mesh::Rect( 2.0, 2.0 ).color(0,0,0,1.0) );  
	}  
	
	virtual void onDraw3D(GLV& glv){   

		interface().getViewData(&glv);    
		// 
		// //cout << interface.vd().w << " " << interface.vd().h << endl; 
		scene.resize( interface.vd().w, interface.vd().h );  
		//        
		//Write into textureA using textureB 
		fboA -> attach( *textureA, GL::COLOR );  
		fboA -> bind();               
		// 
		    glViewport(0,0,textureA->width(),textureA->height() ); 
			glClearColor(0,0,0,1);
		    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 
		 	render(); 
		// 
		fboA -> unbind();  
		// 
		// //Switch to full Viewport and Bind Texture to Rect 
 		glViewport(0,0,scene.camera.lens.width(),scene.camera.lens.height());     
		// 
		texpipe.bind();  
		// 
		   setFrameBuf();
		// 
		   textureA -> bind();   
		// 	
				texpipe.line( *rect );  
		// 	
		   textureA -> unbind();     
		// 
		 texpipe.unbind();   
		// 
		swapTextures();   
 
 	}    

	void render(){
		scene.onFrame(); 
		mvm = scene.xf.modelViewMatrixf();   
		pipe.bind(scene.xf);
		    this->onDraw();   
		pipe.unbind();
	}

    virtual void onDraw(){
		 
	   // cout <<   scene.xf.projMatrixf()  << endl; 

		auto a = Point(1,0,0);
		auto b = Point(0,1,0);
		auto c = Point(0,-1,0);
		// 
		auto cir = Circle(a,b,c);
		// 
		// 	   // Draw(cir,1,0,0); 
	    DRAWCOLOR(cir,1,0,0);   
		//                                
	   // pipe.line(*mbo); 
	}
};  

 
MyApp::~MyApp(){}

                        
MyApp * myApp;

int main(){
	
	printf("myapp"); 
	

	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"Versor",&glv);  
    myApp = new MyApp(win);
	
	printf("myapp made\n"); 
	myApp -> stretch(1,1);
	                
	glv << *myApp;  

	myApp -> initGL(); 
	Application::run();
	
	return 0;
	
}         
