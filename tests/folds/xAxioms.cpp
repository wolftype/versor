//huzita's axioms (from Geometric Folding Algorithms)

#include "vsr_cga3D.h"
#include "vsr_cga3D_funcs.h"
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga3D;
                      
struct MyApp : App {
                   
  float time;   

  
  void initVariables(){       
    time = 0;

  }
  
  void onDraw(){
    
    //vsr::Circle cir = CXY(1);
    //given two points, one can fold a crease line through them

    Point a = Ro::point(-1,0,0);
    Point b = Ro::point(1,0,0);
    
    //given two points, one can fold one onto the other
    
    //dual plane bisector
    DualPlane dlp = Dlp(a - b);
    //line meet of bisector and plane of paper
    DualLine crease1 = ( dlp.unit() ^ Dlp(0,0,1) );
   
    Draw(a,1,0,0); Draw(b,0,0,1); 
    Draw(crease1,0,1,0); 

    time +=.01;
    float t = (sin(time)+1)/2.0;
    
    //fold a to b
    Draw( a.spin( Gen::mot( crease1 * -PIOVERTWO * t ) ), 1-t, 0, t);


    //given two lines, one can fold one onto the other

    DualLine da = DLN(1,1,0).trs(1,0,0);
    DualLine db = DLN(0,1,0).trs(-1,0,0);

    Draw(da,1,0,0); Draw(db,0,0,1);

    //bisector?
    Dlp dlpa( (Vec(0,0,1)<=da).unit() );
    Dlp dlpb( (Vec(0,0,1)<=db).unit() );
    
    Vec tv = Vec(dlpa+dlpb)/2.0;
    Draw( (dlpa ^ dlpb).copy<Vec>() );
    Draw( tv, 1,0,0 );

    auto f = (da ^ dlpb).dual();
    Dlp dlp2 = Ro::null(f) <= tv.copy<Drv>(); 

    //fold da to db
    DualLine crease2 = ( dlp2.unit() ^ Dlp(0,0,1) );//Gen::log( da, db );
    Draw( crease2 );
    Draw( da.mot( crease2 * -PIOVERTWO * t  ), 1-t, 0, t);
    
    
    //Given two points and a line, one can fold point b through point a
    
     
  }  

};
                        
MyApp * app;

int main(){

  GLV glv(0,0);  
                
  Window * win = new Window(500,500,"CGA2D",&glv);    
  app = new MyApp;   
  
  app -> init(win);
  app -> initVariables(); 
         
  glv << *app;
  Application::run();

  return 0;
}
