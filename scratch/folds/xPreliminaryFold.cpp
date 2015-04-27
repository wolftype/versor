/*
 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 *
 *  homepage: versor.mat.ucsb.edu
 *
 *
 * Preliminary Fold
 *
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

void Draw( const Preliminary& p){
    glBegin(GL_TRIANGLES);
      TriNormal( p.mA, p.mAB, p.mE);
      TriNormal( p.mAB, p.mB, p.mE);
      TriNormal( p.mB, p.mBC, p.mE);
      TriNormal( p.mBC, p.mC, p.mE);
      TriNormal( p.mC, p.mCD, p.mE);
      TriNormal( p.mCD, p.mD, p.mE);
      TriNormal( p.mD, p.mDA, p.mE); 
      TriNormal( p.mDA, p.mA, p.mE); 
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
    e = Ro::null(0,0,0);

    scene.camera.pos(0,0,2);
  }

  void initGui(){
    gui(amt,"amt",-PI,PI)(ratA,"ratA")(ratB,"ratB");
  }
  
    virtual void onDraw(){ 
        
    /* //Preliminary Fold */
    Draw(a); Draw(b); Draw(c); Draw(d); Draw(e);

    Preliminary p = Preliminary::Fold( a, b, c, d, amt);

    Petal petal = Petal::Fold( p.mAB, e, p.mBC, p.mB, amt);

    Petal petal2 = Petal::Fold( p.mCD, e, p.mDA, p.mD, amt);


    Draw(p); 
    Draw(petal);
    Draw(petal2);



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
