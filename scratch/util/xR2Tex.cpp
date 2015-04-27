// Render to Texture and save FBO
//

#include <fstream>

#include "vsr_cga3D.h"  
#include "vsr_GLVimpl.h" 
#include "vsr_render.h" 

#include "gfx/gfx_texture.h"
#include "gfx/gfx_fbo.h"  

using namespace vsr;
using namespace glv;  

 

struct MyApp :  App {
    
	//ALL THIS IS THE GL PIPELINE STUFF.
	MBO * rect;   
	Pipe texpipe;  
	   
	FBO * fboA;
	Texture * textureA;    
	Texture * textureB; 
	
	unsigned char * pixels; 
	                
	MyApp(Window * w) : App(w) {
		
		printf("MyApp Construct\n");
	}   
   

	void swapTextures(){
		Texture * tmp = textureA; textureA = textureB; textureB = tmp;
	}
	
	void setFrameBuf(){
		texpipe.program -> uniform("frameBufSize", scene.camera.lens.width(), scene.camera.lens.height() );
	}  

	virtual void initGL(){
		
		printf("INITGL\n");
	
		string Vert = AVertex + VaryingN + UMatrix  + NTransform + VLighting + VCalc + MVertN; 
		string Frag = MFrag;
	
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
		
		pixels = new unsigned char[ int(tw * th * 4) ];
	}  
	
	virtual void onDraw3D(GLV& glv){   

		interface.glv().getViewData(&glv);    
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

		//test read back
		//save();
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

		auto a = Point(2,0,0);
		auto b = Point(0,2,0);
		auto c = Point(0,-2,0);
		// 
		auto cir = Circle(a,b,c);
		// 
		// 	   // Draw(cir,1,0,0); 
	    DRAWCOLOR(cir,1,0,0);   
		//                                
	   // pipe.line(*mbo); 
	}   
	
	void save(){ 
		static int fnum = -1; fnum++;
		stringstream os; os << "test_" << fnum << ".ppm";   
		string filename = os.str();
		int tw = textureA->width();
		int th = textureA->height();
		glReadPixels(0,0,tw,th ,GL_RGBA, GL::UBYTE, pixels); 

		ofstream frame(filename.c_str(),ios::binary);
		frame << "P6\n" <<  tw << " " << th << "\n255\n";
		for (int j=th-1; j>=0; j--) {
			for (int i=0; i < tw; i++) {
				//gfx::Color c( pixels[idx],pixels[idx+1],pixels[idx+1] )
				frame.write((char*)(pixels + tw*4*j+4*i), 3);
				//frame << pixels[idx] << pixels[idx+1] << pixesl[idx+2];
			}
	    }
		frame.close(); 
		                       
		stringstream cmd; cmd << "convert " << os.str() << " test_" << fnum << ".png";
		FILE * convert = popen(cmd.str().c_str(), "r");	
		
	}
};  


                        
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
