//Get mouse position as an "unprojected" point in 3D space
// Attempt to understand the horoball (horocycle, horosphere, etc)

#include "vsr_cga2D_types.h"  
#include "vsr_cga2D_draw.h" 
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace cga2D;

struct MyApp : App {    
   
  float time;
  float dil;
  float amt;
  float rad;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
  }
  
  void initGui(){
    gui(dil,"dil",-10,10); //horoball radius
     gui(amt,"amt",-10,10); //horoball amt
     gui(rad,"rad",-10,10);// spherical or hyperbolic . . .

  }

    virtual void onDraw(){ 
        
        using namespace Ro;
        
        time +=.01;

        Point a = point(0,3);
        Point ta = a;

        a[2] = dil ;
        a[3] = 0;
        a += Ro::dls( Vec(), rad) * amt;

        a.vprint();

        Draw(a,0,0,1);
        
        //parabolic projection onto a
        
        for (int i = 0; i < 100; ++i){
            Vec tmp(-5 + 10*i/100.0, -1);
            Line tline = Fl::line(1,0).trs(0,-1);

            //Draw( tmp.null() );
             Draw(tline);
            
            auto r = tmp.null().reflect( a );//Ori(1) + tmp + a * ( tmp.wt() / 2.0 );

            Draw ( Dll(ta - r) );
           
            auto rl = Circle(tline).reflect( a );
            
            Draw(rl);
            
            Draw( r, 1,0,0); 
            Draw( Ro::loc(r),0,1,0);
            Draw( Ro::cen(r),0,1,1);

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
