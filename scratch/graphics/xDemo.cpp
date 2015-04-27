/*
 * =====================================================================================
 *
 *       Filename:  xDemo.cpp
 *
 *    Description:  DEMO FOR LYNDA AUTHOR AUDITION
 *
 *        Created:  01/28/2014 15:51:52
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  LYNDA.COM *
 * =====================================================================================
 */

#include "gfx/gfx_mesh.h"
#include "vsr_GLVimpl.h"



struct MyApp : App {    
   
  float time;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

    virtual void onDraw(){ 

      Mesh m = Mesh::Cylinder(.5,3);

      //glPolygonMode( GL_FRONT, GL_LINE );
      m.drawElements();
     // m.draw();
    
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

