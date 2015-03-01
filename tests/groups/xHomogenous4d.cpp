/*
 * =====================================================================================
 *
 *       Filename:  xHomogenous4d.cpp
 *
 *    Description:  test of planes in 4d ega ( without resorting to cga . . . )
 *
 *        Version:  1.0
 *        Created:  11/26/2013 19:21:49
 *       Revision:  none
 *       Compiler:  gcc4.7 and clang3.2
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_generic_draw.h"
#include "vsr_GLVimpl.h"
#include "vsr_field.h"
#include "vsr_group.h"

using namespace vsr;

typedef NEVec<4> V4;
typedef NEe<4,4> E0; //e0 blade of 4D 

typedef NEVec<3> V3;
typedef NEBiv<3> Biv;


struct MyApp : App {   
  
  float dist, rot, rnum; 
   
	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
	}

  void initGui(){
    gui(rot, "rot",-PI,PI);
    gui(dist,"dist",-10,10);
    gui(rnum, "rnum",0,20);
  }
  
  	virtual void onDraw(){ 
      
      auto group = Group<V3>( Root::System( V3(1,1,0), V3(-1,1,0), V3(0,1,1) ) );
       
      Field< V4 > field (20,20,20,1);

      for (int i = 0; i < field.num(); ++i){
       field[i][3] = 1.0; 
      }

      //fold (reflect) over each pin
      for (int i = 0; i < group.ops.size(); ++i ) {
        if( i < rnum ){
          V4 pin( group.ops[i] ); //make a v4 pin
          for ( int j = 0; j < field.num(); ++j){
            if ( (field[j] <= pin)[0] < 0 ){   // if location is on one side of pin ...     
              field[j] = field[j].reflect(pin);  // ... reflect it
            }
          }
        }
      }

      auto vec = V3(1,0,0).rot(  Biv::xy * rot );     
      auto dlp = V4(vec) + E0(dist);
      E0(1).print();

      for (int i = 0; i < field.num(); ++i){
        auto sca = field[i] <= dlp;
        if (sca[0] < 0) {
          DrawAt( NPnt<4>(), field.grid(i),1,0,0 );
        } else {
           DrawAt( NPnt<4>(), field.grid(i),0,1,0,.1 );
        }
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
