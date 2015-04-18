/*
 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 *
 *  homepage: versor.mat.ucsb.edu
 *
 *
 * Petal Fold
 * make a p fold from four points and an amt, returns 6 new points
 * */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_fold.h"

using namespace vsr;
using namespace vsr::cga3D;

void Draw(const Petal& p){

    glBegin(GL_TRIANGLES);
      
      TriNormal(p.base, p.flapA, p.nodeA);
      TriNormal(p.nodeA, p.flapA, p.tip);
      TriNormal(p.tip, p.flapB, p.nodeB);
      TriNormal(p.nodeB, p.flapB,  p.base);

      TriNormal(p.base, p.nodeB, p.nodeA);
      TriNormal(p.nodeA, p.nodeB, p.tip);
      TriNormal(p.nodeA, p.nodeB, p.stamen);
  
   glEnd();
}



struct MyApp : App {    

  Point a, b, c, d, e;
  float time;
  float amt;
  float ratA, ratB; //Ratios along edges


  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 

    time = 0;
    amt = 0;

    a = Ro::null(-1,-1,0);
    b = Ro::null(-1,1,0);
    c = Ro::null(1,1,0);
    d = Ro::null(1,-1,0);

    scene.camera.pos(0,0,2);
  }

  void initGui(){
    gui(amt,"amt",-PI,PI)(ratA,"ratA")(ratB,"ratB");
  }
  
    virtual void onDraw(){ 

      gfx::GL::twoSidedLighting();
        
    static Point light = Ro::null(3,3,3); //Draw( Ro::dls_pnt( light ) );
    scene.light = scene.camera.pos();//Vec( light[0], light[1], light[2] );
    Touch(interface,light);
    
    Draw(a); Draw(b); Draw(c); Draw(d);

    Petal p = Petal::Fold( a,b,c,d,amt);

    Draw(p.nodeA); Draw (p.nodeB); Draw(p.flapA); Draw(p.flapB);
    Draw(p.tip);
    Draw(p.base);

    Draw(p);
    
    Petal p2 = Petal::Fold( c,b,a,d,amt);
    Draw(p2);



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
