#include "vsr_cga2D.h"   
#include "gfx/util/gfx_glv_app.h"

using namespace vsr;


struct MyApp : GFXAppGui {    
   
   void setup(){ 
     bindGLV();
      
    }
     
   void onDraw(){ 
        
	}
   
	
};



int main(){
                             
  MyApp app;
  app.start();

	return 0;

}
