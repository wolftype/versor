//Get mouse position as an "unprojected" point in 3D space

#include "vsr_cga3D.h" 
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray; 
  
  float time;
  float radius;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto v= interface.mouse.projectMid; 
      ray = Ro::point(v[0],v[1],v[2] ) ^ z ^ Inf(1); 
      mouse = Ro::point( ray,  Ori(1) );  
   }   

  void initGui(){
    gui(radius,"radius",0,10); 
    radius = 1.0;
  }

    virtual void onDraw(){ 
        
    getMouse();
    
    time += .01;
     
    //Pnt a, b;
    
    Point a = Ro::point(-5,0,0);
    Point b = Ro::point(5,0,0); 
    
    Draw(a); Draw(b);
    
    auto sa = Ro::sphere( a, -radius );
    auto sb = Ro::sphere( b, radius );   

    //same as spin( Gen::rot . . .)
    auto p = Ro::round( sa, Vec::x.rot( Biv::xy * time ) );
    auto splitp = Ro::split( p, true); 
    
    Draw(splitp,1,0,0);
    
    Draw( LN(1,0,0), 0,1,0 ); 
    
    
    auto sc = Ro::sphere( splitp, radius );

    auto c = ( sb ^ sc ).dual();
    
    Draw(c,1,1,.2);
    
    if ( Ro::size(c, false) > 0 ){
        auto cir = ( sb ^ Dlp(0,0,1) ).dual();
        Draw(cir,1,0,0);
      
        auto par = ( c.dual() ^ Dlp(0,0,1) ).dual();
        Draw(par,0,0,1);
    } 
    
    Draw(sc,0,1,0,.4);  
    Draw(sa); 
    Draw(sb,0,1,0,.4);    
    
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
