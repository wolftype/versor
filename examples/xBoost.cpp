#include "vsr_cga3D.h"  
#include "vsr_cga3D_constructors.h"
#include "vsr_GLVimpl.h"

#include "vsr_field.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {
      
  float time;
  
  virtual void onDraw(){       
    
    time += .01;
    
    int num = 100;
    
    for (int i = 0; i < num; ++i ){
      
      float t = 1.0 * i/num;
      
      Par par = Par( Tnv( 1,0,0) * t).trs( cos(time), 0, 0); 

      auto c = Circle(0,0,0).bst( par ); 

      Draw ( c, 1, t, 1-t );
      }
  
  }
  
};

MyApp * app;

int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(500,500,"Versor",&glv);    
  app = new MyApp(); 
  app -> init( win ); 
  
  glv << *app;

  Application::run();

  return 0;

}  
