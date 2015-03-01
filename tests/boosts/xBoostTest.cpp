//Get mouse position as an "unprojected" point in 3D space

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_field.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;
  float amt2;
  float amt3;

  bool bReset;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-10,10);
      gui(amt2,"amt2",-10,10);
      gui(amt3, "amt3",-10,10);
      gui(bReset, "reset");
      amt = .05;
      amt2 = .05;
      amt3 = 0;
      bReset = 0;
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

      Tnv tnv(0,1,0); // TangentVector

      Point pa = Round::point(0,1,0);
      Point pb = Round::point(0,-1,0);

      gfx::Glyph::Line( pa, pb );
      
      Draw(pa,0,1,0); Draw(pb,0,1,0);

      Pair pair = pa ^ pb;

      DualSphere dls = Ro::dls( Ro::point(0,0,0), 1);
      static Pair ipair = Round::round( dls, Vec(0,1,0) );

      DualSphere dls2 = Ro::dls( Ro::point(0,0,0), .5);
      static Pair ipair2 = Round::round( dls2, Vec(0,1,0) );

      Touch(interface, ipair);
      Touch(interface, ipair2);

      auto line = ipair ^ Inf(1);
      //Draw(line);

      Draw( ipair.dual(), 1,1,0);
      Draw( ipair2.dual(),0,1,1);


      ipair = ipair.boost( ipair2 * amt3 );
      ipair2 = ipair2.boost( ipair * amt3);

      static Field<Pnt> field(10,10,10);


      if (bReset) field.reset();

      for (int i = 0; i < field.num(); ++i){

          float da = (1.0 / ( Ro::sqd( field[i], Ro::loc(ipair) ) + .01 ));
          float db = (1.0 / ( Ro::sqd( field[i], Ro::loc(ipair2) ) + .01));

          field[i] = field[i].boost( ipair * amt * da + ipair2 * amt2 * db );
          field[i] = Ro::loc( field[i] );
      }

      Draw(field);



      //Draw(tnv);
    
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
