//Get mouse position as an "unprojected" point in 3D space

#include "vsr_cga3D.h"

#include "vsr_cga3D_funcs.h"   

#include "vsr_GLVimpl.h"

#include "vsr_field.h"

#include "vsr_set.h"

using namespace vsr;

/* template <class T> */
/* struct FoldAndCut { */
   
/*    Set<Dlp> mRef; */
/*    Set<T*> mRes; */
/*    T * mData; */
   
/*    void data(T * ptr ) { mData = ptr; } */
/*    T& data() { return *mData; } */
   
/*    void operator () (const T& vertices, int num){ */
/*         if */ 
/*    } */   
/* }; */


struct MyApp : App {    
   
	Pnt mouse;
	Lin ray;

  Field<Pnt> field;

  bool bDrawFold;
  float num;

	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
	
    field.resize(20,20,20);
  }

  void initGui(){
    gui(bDrawFold)(num,"num",1,100);
  }
  
  	virtual void onDraw(){ 
        		
		//fold along
    vector<Dlp> vd;
    //cut plane
    static Dlp cut(0,1,0,3);
    Draw(cut,1,0,0);
    Touch(interface,cut);

    int mNum = num;

    for (int i = 0; i < mNum; ++i){ 
       float t = 1.0 * i/mNum;
       Dlp dlp = Dlp(1,0,0,5*t).rotate( Biv::xy * PI*(t)*(1-t) ); 
       vd.push_back( dlp );
       Draw( dlp,0,1,0 );
    }


      for (int i =0; i < field.num(); ++i){
         Pnt tmp = field.grid(i) ;

        for (int j = 0; j < mNum; ++j){ 
          if ( (tmp <= vd[j])[0] > 0 ){
            tmp = tmp.reflect( vd[j] ); 
          }
        }
         if ( (tmp <= cut)[0] > 0 ){
            Draw( field[i],0,1,0 ); //green cut
         }

         if(bDrawFold) Draw(tmp,1,0,0);
      }
		
	}
   

	
};


MyApp * app;


int main(){
                             
	GLV glv(0,0);	

	Window * win = new Window(500,500,"Versor",&glv);    
	app = new MyApp( win ); 
  app -> initGui();
	
	glv << *app;

	Application::run();

	return 0;

}
