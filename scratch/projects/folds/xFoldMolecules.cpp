/*
 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 *
 *  homepage: versor.mat.ucsb.edu
 *
 * */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_fold.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  Point a, b, c;
  float time;
  float amt;


  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 

    time = 0;
    amt = 0;

    a = Ro::null(-1,0,0);
    b = Ro::null(0,3,0);
    c = Ro::null(3,0,0);
  }

  void initGui(){
    gui(amt,"amt",-PI,PI);
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
     // cout << tm << endl; 
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );
     // mouse.vprint();  
  }

    virtual void onDraw(){ 
        
      getMouse();

       Draw(a); Draw(b); Draw(c);
       Touch(interface, a); Touch(interface,b); Touch(interface,c);

       auto re = RabbitEar::Fold(a,b,c,amt);

       //Draw
       glBegin(GL_TRIANGLES);
          TriNormal(re.mA, re.mBary, re.mB);
          TriNormal(re.mB, re.mBary, re.mD);
          TriNormal(re.mD, re.mBary, re.mC);
          TriNormal(re.mC, re.mBary, re.mA);
       glEnd();


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
