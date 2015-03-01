
//Get mouse position as an "unprojected" point in 3D space

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_frame.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      getMouse();

      static Frame pa (-5,0,0); 
      static Frame pb (5,0,0);

      Touch(interface,pa); Touch(interface,pb);
      Draw(pa); Draw(pb);

      auto a = pa.ty();
      auto b = pb.ty();

      auto to = Par(Tnv(0,1,0));

     // (to/a).bprint();
     // (to/b).vprint();

      auto da = pa.dll();
      auto db = pb.dll();

      //interpolate
      for (int i = 0; i < 100; ++i){
          float t = (float)i/100;
          auto c = a * (1-t) + b * t;
          Draw(Ro::loc(c) );
          DrawAt(Ro::dir(c),Ro::loc(c) );

          auto lin = pa.ly() * (1-t) + pb.ly() * t;
          Draw(lin,0,1,0);

          auto dc = da * (1-t) + db* t;

          Point p  = (pa.pos() * (1-t) + pb.pos()*t).null();// Frame( Gen::mot(dc) ).pos();

          Draw( Ro::loc(p.bst(c)),1,0,0 );

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
