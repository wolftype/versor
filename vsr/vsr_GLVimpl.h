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

#include "vsr_std_types.h"


	using namespace std;
	using namespace glv;
	using namespace gfx;

	using namespace gfx::GLSL; 
	
	enum {
		SLIDER,
		SLIDERS,
		SLIDER2D,
		BUTTON,
		BUTTONS,
		DIALER,
		COLORSLIDER
	};



	//typedef Widget Widget;
	typedef map<string, Widget*> WidgetMap;
	typedef map<string, Widget*>::iterator WidgetPtr; 

		class Gui : public Table {

				/* Map of Widgets (index by name) */
				WidgetMap mWidget;

				/* Placement Defaults */
				Placer * mPlacer;

				bool bDesc;		    //show descriptor?
				bool bVisible;		//draw to screen?

	            void _init(){

	                enable ( Controllable |  DrawBorder | DrawBack | FocusHighlight );

	                int spacing		= 10;
	                mPlacer = new Placer(*this, Direction::S, Place::TL, spacing, spacing);

	                Label :: Spec spec(Place::CL, width() + 20, 0);
	                Label * tmp = new Label ( name(), spec);

	                *mPlacer << *tmp;

	                addHandler(Event::MouseDrag, Behavior::mouseMove);
	            }

				public:

	                Gui() : Table("<"), bVisible(true) {
	                    _init();
	                }

	                Gui(int _w , int _h ) : Table("<"), bVisible(true) {
	                    this -> w = _w;
	                    this -> h = _h;
	                    _init();
	                }


	                Gui(string _name): Table("<"), bVisible(true) {
	                    name(_name);
	                    _init();
	                }

					void add ( Widget* v) {
						mWidget[ v->name() ] = v;

						Label :: Spec spec(Place::CL, v -> width() + 20, 0);
						Label * tmp = new Label (v->name(), spec);

						//cout << "adding and placing pointer " << (v->name()) << "to gui." << endl;
						*mPlacer << ( ( *mWidget[ v->name() ] ) << *tmp );

					}

	                /*!  add widget and automatically determine type of widget */
	                template<class T>
	                Gui& operator () (T& val, string nm = "", float min = 0, float max = 1){

	                    using namespace vsr;
	                    static int it = 0;
	                    stringstream name;

	                    if (nm == "" ) 
	                        name << typeid(T).name() << "_" << it; 
	                    else name << nm;


	                    switch ( vsr::Types[ typeid(T).name() ] ){
	                        case vsr::BOOLEAN:
	                            add(BUTTON, name.str(), val);
	                            break;
	                        case FLOAT:
	                        case DOUBLE:
	                        case INT:
	                            add(DIALER, name.str(), val, min, max);
	                            break;
	                        default:
	                            break;
	                    }

	                    it++;
	                    return *this;
	                }
	                /*!  add widget and automatically determine type of widget */
	                template<class T>
	                Gui& operator () (T& val, float max){
	                    return (*this)(val, "", 0, max);
	                }
					/*! add widget: enum type, name, min, max, attach, num */
					template <class V>
					void add (int, string, V& val, float min = 0., float max = 1. );

	                void add ( Widget* v, const std::string& _name){

	                    mWidget[ _name ] = v;	
	                    Label * tmp = new Label (_name );

	                    Box * box = new Box();
	                    *this << ( *box << *mWidget[ _name]  << *tmp );	

	                    arrange();		
	                }

					Widget& widget(string name) { return *mWidget[name]; }			///< Get Widget
					WidgetMap& widget() { return mWidget; }							///< Get Widget Map

					void updateValues();

					/* Render on Screen */
					virtual void onDraw(){
						glTranslated(5,h-10,0);
						glColor3f(1,1,1);
						draw::text( name().c_str() );
					}


		};


		template <class V>
		void Gui :: add (int _type, string _name, V& val, float min , float max ){

			Widget * s;
			switch (_type){
				case SLIDER:
				{
					Slider * ns = new Slider(glv::Rect(100,20));
					ns -> attachVariable(val, 0 );
					s = (Widget*)ns;
					break;
				}
				case SLIDER2D:
				{
					Slider2D * ns = new Slider2D();
					s = (Widget*)ns;
					break;
				}
				case BUTTON:
				{
	                cout << " adding Button to gui " << endl; 
					Button *ns = new Button(glv::Rect(20,20));
					ns -> attachVariable(val, 0);
					s = (Widget*)ns;
					break;
				}
				case DIALER:
				{
					NumberDialer * ns = new NumberDialer(4,3,max,min);
					ns -> attachVariable(val, 0);
					s = (Widget*)ns;
					break;
				}
			}

			s->colors().text.set(0,1,0); 
			s->colors().fore.set(0,1,0);  
			//s->colors().set(0,1,0);
			s->name( _name );
			add((Widget*)s, _name);
		}
                          

struct GLVImpl : public Interface::Impl {    

	
	Window * win; 
	
	GLVImpl( Interface * i ) : Interface::Impl(i) {}
    
	virtual void fullScreenToggle(){
		win -> fullScreenToggle(); 
	}

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
    
  App(Window * win = NULL ) : View3D(), 
    renderMode(0)
 {   

	init(win);
  }           

	virtual void init(Window * win){
		stretch(1,1);
		colors().back.set(.1,.1,.1);
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
	    string Vert = AVertex + VaryingN + UMatrix  + NTransform + VLighting + VCalc + MVertN; 
	   // string Vert = SimpleVertex;//SimpleVertex;//AVertex + VaryingN + MVertN; 
		string Frag = MFrag;//TFragMix; //mFrag   //SimpleFragment;//
         
                               
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
             App::onKeyDown(glv);
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
	  
	}
	  
	return true;    
	
 }                                                                                             
	
};

App::~App(){}  





#endif
