#include "vsr_cga3D.h" 
#include "vsr_GLVimpl.h" 


using namespace vsr;
using namespace vsr::cga3D;
using namespace glv;  


struct MyApp : public App {  

  void onDraw(){

                                                                            
    //Circle through 3 Points
    static Circle circle = Ro::point(1,0,0) ^ Ro::point(0,1,0) ^ Ro::point(-1,0,0); ///< i.e. CXY(1)
    
         
    //Dual plane with Normal in Y direction
    static DualPlane dualplane(0,1,0);// = Fl::plane(0,1,0);
         
    //Calculate their Intersection (which is the dual of the outer product of duals . . . )
    auto pair = meet(circle,dualplane);  //<---- i.e. (circle.dual() ^ dualplane).dual();

    //Draw 'em with colors
    Draw(circle,0,1,0);     //<-- Green
    Draw(dualplane,0,0,1);  //<-- Blue
    Draw(pair,1,0,0);  //<-- Red

    //Enable Mouse Control with 'G' 'R' and 'S' keys
    Touch( interface, circle );
           
    text("use G, R and S keys and drag with mouse to Grab, Rotate and Scale the Circle");
  } 

};
                        
MyApp * myApp;

int main(){


  
  GLV glv(0,0);  
                
  Window * win = new Window(800,500,"Versor",&glv);    
  
  myApp = new MyApp;
  myApp -> init(win);
      
  glv << *myApp;

  Application::run();
  
  return 0;
  
}
