/*
 * =====================================================================================
 *
 *       Filename:  xSpinningWheel.cpp
 *
 *    Description:  self explanatory?  towards vehicles . . .
 *
 *        Version:  1.0
 *        Created:  11/03/2014 12:04:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  pretty awesome 
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_cga3D_frame.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;
  float amt2;

  bool bReset;

  Frame f;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
    init();
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(amt2,"amt2",-100,100);
      gui(bReset,"reset");
  }

  void init(){
    f.pos( 0,3,0);
    f.dx() = Vec(0,-.05,0);
    f.ax() = 1.1;
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      time++;
      getMouse();

     // static Frame f(0,3,0);
      Draw(f);
      Touch(interface,f);

      //f.rot() = ( Gen::rot( Biv(amt2,0,0) ) );

      //wheel
      auto wheel = f.cxy();

      //floor
      Dlp floor(0,1,0,-.5);

      //intersection
      auto meet = (floor ^ wheel.dual()).dual();
      //is it real? if so, print "hit" and draw intersection points
      bool bHit = Ro::size(meet,false) > 0;
      if (bHit) {
        cout << "HIT" << endl;
        Draw( meet, 0,0,1);
       // f.ax() *= .9; //reverse and slow down acc;
         f.dx() *= -.5;
      } else {     

        //gravity acc
      //  f.ax() = 1.1;
        f.dx() += Vec(0,-.01,0);
        if (f.dx()[1] > 0) f.ax() = .9; else f.ax() = 1.1;//Vec(0,.2,0);
      }
       f.move();

      f.dx().print();

      Draw(wheel,1,0,0);
      Draw(floor,0,1,0);


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

