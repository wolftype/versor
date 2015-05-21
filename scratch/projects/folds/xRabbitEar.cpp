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
      gfx::GL::twoSidedLighting(); 
      getMouse();

     //  Draw(a); Draw(b); Draw(c);
       Touch(interface, a); Touch(interface,b); Touch(interface,c);

       RabbitEar re(a,b,c);

       //Draw( re.mE,1,1,0);
       
       auto p = re.eval(amt);

       for (int i = 0; i < 5; ++i )  Draw( p[i],0,0,0);


       glColor3f(1,0,0);
       glBegin(GL_LINE_STRIP);
          GL::Tri( p.a, p.e, p.b);//re.mA, re.mE, p[1]);
          GL::Tri( p.b, p.e, p.c );//p[1], re.mE, p[0]);
          GL::Tri( p.c, p.e, p.a);//re.mC, re.mE, re.mA);
          GL::vertex( p.a.begin());
          GL::vertex( p.e.begin());
          glColor3f(0,0,1);
          GL::vertex( p.e.begin());
          GL::vertex( p.d.begin());
       glEnd();

       glColor3f(.5,.5,.5);
       glBegin(GL_TRIANGLES);
                        
          TriNormal(p.c, p.e, p.a);//re.mC, re.mE, re.mA);
          TriNormal(p.d, p.e, p.c);//p[0], re.mE, re.mC);
          TriNormal(p.b, p.e, p.d );//p[1], re.mE, p[0]);
          TriNormal(p.a, p.e, p.b);//re.mA, re.mE, p[1]);

       glEnd();


       //Draw
       /* glColor3f(.5,.5,.5); */
       /* glBegin(GL_TRIANGLES); */
       /*    TriNormal(re.mA, re.mE, p[1]); */
       /*    TriNormal(p[1], re.mE, p[0]); */
       /*    TriNormal(p[0], re.mE, re.mC); */
       /*    TriNormal(re.mC, re.mE, re.mA); */
       /* glEnd(); */

       /* glColor3f(1,0,0); */
       /* glBegin(GL_LINES); */
       /*    TriNormal(re.mA, re.mE, p[1]); */
       /*    TriNormal(p[1], re.mE, p[0]); */
       /*    TriNormal(p[0], re.mE, re.mC); */
       /*    TriNormal(re.mC, re.mE, re.mA); */
       /* glEnd(); */
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
