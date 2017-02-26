
#include "vsr_app.h"
   
#include "form/vsr_twist.h" 

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {    
   
  float time;

  void setup(){
    bindGLV();
  }
  
  void onDraw(){ 
       
    auto mouse = calcMouse3D();
 
    time += .01;
	
    Frame f1(-5,0,0); 
    draw(f1);   
    Frame f2( mouse, Gen::rot( Biv::xy * time ) ); 
    draw(f2); 
	
    for (int i = 0; i <= 100; ++i){
	    float t = (float)i/100;
	    Motor mot = Gen::ratio( f1.mot(), f2.mot(), t);
	    draw( Frame( mot * f1.mot() ) ); 
    }
    
  }
   
};


int main(){
  MyApp app;
  app.start();
  return 0;
}
