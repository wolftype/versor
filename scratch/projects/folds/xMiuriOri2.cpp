/*
 * =====================================================================================
 *
 *       Filename:  xMiuriOri2.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/02/2013 15:37:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */







#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_fold.h"
#include "vsr_set.h"

using namespace vsr;
using namespace vsr::cga3D;

void Draw(const Truss& ta){
  
  Truss::Points b = ta.base();

 glColor3f(.3,.3,.3);
  glBegin(GL_TRIANGLES);
    TriNormal( ta.p.tl, ta.p.tm, b.tm );
    TriNormal( ta.p.tm, ta.p.tr, b.tm );
    TriNormal( ta.p.tr, b.tr, b.tm );
    TriNormal( b.tm, b.tl, ta.p.tl );
  glEnd();

  glColor3f(1,0,0);
   glBegin(GL_LINE_STRIP);
     gfx::GL::Tri( ta.p.tl, ta.p.tm, b.tm );
     gfx::GL::Tri( ta.p.tm, ta.p.tr, b.tm );
     gfx::GL::Tri( ta.p.tr, b.tr, b.tm );
     gfx::GL::Tri( b.tm, b.tl, ta.p.tl );
   glEnd();
}

void Draw(const Truss& ta, const Truss& tb){

 glColor3f(.3,.3,.3);
  glBegin(GL_TRIANGLES);
    TriNormal( ta.p.tl, ta.p.tm, tb.p.tm );
    TriNormal( ta.p.tm, ta.p.tr, tb.p.tm );
    TriNormal( ta.p.tr, tb.p.tr, tb.p.tm );
    TriNormal( tb.p.tm, tb.p.tl, ta.p.tl );
  glEnd();

 glColor3f(1,0,0);
  glBegin(GL_LINE_STRIP);
    gfx::GL::Tri( ta.p.tl, ta.p.tm, tb.p.tm );
    gfx::GL::Tri( ta.p.tm, ta.p.tr, tb.p.tm );
    gfx::GL::Tri( ta.p.tr, tb.p.tr, tb.p.tm );
    gfx::GL::Tri( tb.p.tm, tb.p.tl, ta.p.tl );
  glEnd();
}


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float amt;
  double dx,dy,decay,num;

  bool bDrawRigid;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 

  }
  
  void initGui(){

    gui(dx,"dx")(dy,"dy")(decay,"decay")(amt)(num,"num",1,100);

    gui(bDrawRigid);
    dx = .2; dy = 0; amt = .1;

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
      
      gfx::GL::twoSidedLighting();
      scene.light = Vec3f(5,1,.3);

      //BASE
      Field<Pnt> f (2,3,1);

      for (int i = 0; i< f.w(); ++i){
        f.at(i,1) = f.gridAt(i,1).trs(-dx,dy,0); 
      }

      Truss truss ( f.at(1,2), f.at(1,1), f.at(1,0), 
                  f.at(0,2), f.at(0,1), f.at(0,0), true );

      truss.bUseSplit = true;  
     //MOVEMENT
     /* Line l = f.at(1,0) ^ f.at(1,1) ^ Inf(1); l = l.unit(); */
     /* Pnt np = f.gridAt( 2, 0).mot( l.dual() * PI * amt ) ; */
     Line l = f.at(0,1) ^ f.at(1,1) ^ Inf(1); l = l.unit();
     Pnt np = f.gridAt( 1, 2).mot( l.dual() * PI * amt ) ;


     //FIRST UPDATE
     Draw(truss); 
     truss.reverse(); 
     truss.update( np, f.at(1,1), f.at(1,0));//, np);
    
    
  
      
    int width = num; 
    int height = 5;

    bool switcher = true;
    
     for (int i = 0; i < width; ++i){

         switcher = !switcher;
         Draw(truss); 
        // truss.reverse(); 
        // truss.update(); 

        Draw( ( truss.rigidA.a() ^ truss.rigidA.b() ).dual(), 0,1,0);

       

         f.at(1,1) = f.at(1,1).trs(-decay,0,0);
        
        Truss tt ( f.at(1,2), f.at(1,1), f.at(1,0), 
                  f.at(0,2), f.at(0,1), f.at(0,0), switcher );

        tt.update( truss );
        truss = tt;

        f.at(0,1) = f.at(0,1).trs(-decay,0,0);

         if(bDrawRigid){
        Draw( truss.rigidA.a(),1,0,0,.1);
        Draw( truss.rigidA.b(),1,0,0,.1);
        Draw( truss.rigidA.c(),1,0,0,.1);
        }


        
    }


    /* static Cir cir = CXY(1); */
    /* Touch(interface,cir); Draw(cir); */
    /* static Dls dls = Ro::dls(1.0, 2,0,0); */
    /* Touch(interface,dls); Draw(dls); */

    /* //Draw( Ta::at( cir, dls ) ); */
    /* Draw( (cir.dual() ^ dls).dual(), 0,1,0); */

  }
   

  
};


MyApp * app;


int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(800,800,"Miuri-ori",&glv);    
  app = new MyApp( win ); 
  app -> initGui();
  
  glv << *app;

  Application::run();

  return 0;

}


