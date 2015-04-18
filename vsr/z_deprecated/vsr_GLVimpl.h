#ifndef GLV_APP_INCLUDED
#define GLV_APP_INCLUDED     

/*

  IMPLEMENTATION FILE  - This implements GLV library 
  1 - copies state of window, keyboard, and mouse to generic interface data
  2 - calls interface and mouse on events

*/

#include "GLV/glv.h" 
#include "GLV/glv_binding.h"
#include "GLV/glv_color_controls.h"  

#include "gfx/gfx_gl.h" 
#include "gfx/gfx_glsl.h"  
#include "gfx/gfx_scene.h" 
#include "gfx/gfx_pipe.h" 
#include "gfx/gfx_interface.h"  

//GL2PS
#include "gl2ps/gl2ps.h"

//GUI
#include "vsr_gui.h"


using namespace std;
using namespace glv;
using namespace gfx;

using namespace gfx::GLSL; 

struct GLVImpl : public Interface::Impl {    

  
  Window * win; 
  
  GLVImpl( Interface * i ) : Interface::Impl(i) {}
    
  virtual void fullScreenToggle(){
    win -> fullScreenToggle(); 
  }

  virtual void getViewData(void * udata) {
        GLV& glv = *(GLV*)(udata);
        
        //COPY VIEW DATA
        interface -> vd().w     = glv.width();
        interface -> vd().h     = glv.height(); 

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


struct GLVInterface : public gfx::Interface {
  
  GLVInterface() : gfx::Interface() { init(); }
  GLVImpl& glv(){ return *(GLVImpl*)impl; }
    
  virtual void init(){
    impl = new GLVImpl(this);
  } 
};

struct App : public View3D{  
  
    int renderMode;

    GLVInterface interface;
      Gui gui;
        
    gfx::Scene scene;
    Pipe pipe; 

    Mat4f mvm;

    bool bPrintPS;
    
  App(Window * win = NULL ) : View3D(), 
    renderMode(0), bPrintPS(false)
 {   

  init(win);
  }           

  virtual void init(Window * win){
    stretch(1,1);
    colors().back.set(.3,.3,.3);
      *this << gui;  
  
    //gui.colors().back.set(0,1,0);

      set(win); 

    interface.scene = &scene;

    scene.camera.bUseFrust = false; 
    initView();
  }
      
  void set(Window * win) { interface.glv().win = win; }

  virtual void initGui(){}

  virtual ~App();

  virtual void initGL(){
    string Vert = DefaultVert; 
    string Frag = DefaultFrag; 
         
    printf("%s\n", "initGL");
    
    pipe.program = new ShaderProgram(Vert,Frag,0);
    pipe.bind();
    pipe.bindAll();
    pipe.unbind();
    
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

  virtual void onDraw(){}
  virtual void update(){}

  virtual void onDraw3D(GLV& glv){   
      
     interface.glv().getViewData(&glv);
     
  
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

       if (bPrintPS){
          ps();
          bPrintPS = false;
       }
  }  
  
  //vbo render (opengl es 2.0)
  void renderB(){     
                    
       // glViewport(0,0,surface.width,surface.height); 
       // glClearColor(background[0],background[1],background[2],background[3]);
       // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     
             
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
  
  void text(string s, int ow = 50, int oh = 100){
          draw::enter2D(w, h);
            glTranslated(ow,h-oh,0);
            glColor3f(1,1,1);
            draw::text( s.c_str() );
        //draw::pop2D();
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
             interface.mouseNavigate();
             break;
         case Event::MouseUp:
             interface.onMouseUp();
             interface.mouseNavigateStop();
             break;
         case Event::KeyDown:
             onKeyDown(glv);
             interface.onKeyDown();
             interface.keyboardNavigate(); 
             break;
         case Event::KeyUp:
             interface.onKeyUp(); 
             interface.keyboardNavigateStop(); 
             break;
             
     }
     
     return false;
 }   

 

  virtual bool onKeyDown(GLV& glv){ 
  
  switch( glv.keyboard().key() ){
        case 96: //tilde
            //printf("TILDE!\n");
            interface.impl -> fullScreenToggle();
      break; 
    case 'v':
        gui.toggle(Visible);
        break;
    case 'p':
        bPrintPS = true;
        break;
    
  }
    
  return true;    
  
 }                                                                                             
 
        void ps(){
//            interface.view().fit();
            GL::enablePreset();
            scene.push();
                gl2ps();
            scene.pop();
            GL::disablePreset();
        }
        
                
        virtual void gl2ps(){
            static int id = 0;
            stringstream os; os << "output_" << id << ".eps";
            id++;
            
            FILE *fp;
            int state = GL2PS_OVERFLOW, buffsize = 0;
            
            string name = os.str();
            fp = fopen(name.c_str(), "wb");
            
            printf("writing %s to %s\n", os.str().c_str(), name.c_str() );
            GLint tv[4];
            glGetIntegerv(GL_VIEWPORT, tv);
//            glPointSize(15);
            
            //| GL2PS_NO_BLENDING 
            //| GL2PS_SIMPLE_LINE_OFFSET
            //
            
            
            //gl2psEnable( GL2PS_POLYGON_OFFSET_FILL );
            while(state == GL2PS_OVERFLOW){
                buffsize += 1024*1024;
                gl2psBeginPage("test", "gl2psTestSimple", tv , GL2PS_EPS, GL2PS_SIMPLE_SORT,//GL2PS_NO_SORT,//, //GL2PS_SIMPLE_SORT, 
                               GL2PS_NO_PS3_SHADING | GL2PS_BEST_ROOT | GL2PS_SIMPLE_LINE_OFFSET | GL2PS_TIGHT_BOUNDING_BOX ,// | GL2PS_OCCLUSION_CULL,// | GL2PS_TIGHT_BOUNDING_BOX,// | GL2PS_BEST_ROOT, GL2PS_NO_PS3_SHADING | //| 
                               GL_RGBA, 0, NULL, 0, 0, 0, buffsize, fp, "out.eps");
                
                gl2psEnable(GL2PS_BLEND);
                gl2psBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                gl2psPointSize(10);
                gl2psLineWidth(1);
                 onDraw();
                
                state = gl2psEndPage();
            }
            
            fclose(fp);
            printf("Done!\n");
        }
 
  
};

App::~App(){}  





#endif
