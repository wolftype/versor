#include "vsr_cga3D_app.h"  
#include "vsr_field.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {
      
  float time;

  virtual void setup(){
    bindGLV();
  }
  
  virtual void onDraw(){       
    
    time += .01;
    
    int num = 100;
    for (int i = 0; i < num; ++i ){
      
      float t = 1.0 * i/num;
      Par par = Par( Tnv( 1,0,0) * t).trs( cos(time), 0, 0); 
      auto c = Ro::circle(0,0,0).bst( par ); 
      draw ( c, 1, t, 1-t  );
    
    }
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}  
