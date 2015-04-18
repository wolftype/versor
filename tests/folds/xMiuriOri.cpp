/*
 * =====================================================================================
 *
 *       Filename:  xMiuriOri.cpp
 *
 *    Description:  miuri ori folds
 *
 *        Version:  1.0
 *        Created:  11/15/2013 16:53:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  
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
  float dx,dy,decay;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 

  }
  
  void initGui(){

    gui(dx,"dx")(dy,"dy")(decay,"decay")(amt);

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
      Field<Pnt> f3 (3,3,1);

      for (int i = 0; i< f3.w(); ++i){
        f3.at(i,1) = f3.gridAt(i,1).trs(-dx,dy,0); 
      }

      Truss ta ( f3.at(0,1), f3.at(1,1), f3.at(2,1),
                  f3.at(0,0), f3.at(1,0), f3.at(2,0), false );

      Truss tb ( f3.at(0,2), f3.at(1,2), f3.at(2,2), 
                  f3.at(0,1), f3.at(1,1), f3.at(2,1), false );


      Truss ua ( f3.at(1,2), f3.at(1,1), f3.at(1,0), 
                  f3.at(0,2), f3.at(0,1), f3.at(0,0), false );

      Truss ub ( f3.at(2,2), f3.at(2,1), f3.at(2,0),
                  f3.at(1,2), f3.at(1,1), f3.at(1,0), true );

        
     //MOVEMENT
     Line l = f3.at(1,0) ^ f3.at(1,1) ^ Inf(1); l = l.unit();
     Pnt np = f3.gridAt( 2, 0).mot( l.dual() * PI * amt ) ;

     //FIRST UPDATE
     ta.update(  f3.at(0,0), f3.at(1,0), np );
     tb.update(  ta.p.tl, ta.p.tm, ta.p.tr );

  //   ua.update(  tb.p.tr, ta.p.tr, ta.base().tr );
  //   ub.update(  ua.p.tl, ua.p.tm, ua.p.tr );


    int width = 4; 
    int height = 5;
    
     for (int i = 0; i < height; ++i){

        Draw(ta); 
        Draw(tb);

        /* float t = (float)i/height; */
        /* for (int k = 0; k < f3.w(); ++k){ */
        /*    f3.at(k,1) = f3.gridAt(k,1).trs( - dx - (decay * t),dy,0); */ 
        /* } */

        Truss nta ( f3.at(0,1), f3.at(1,1), f3.at(2,1),
                  f3.at(0,0), f3.at(1,0), f3.at(2,0), false );

        Truss ntb ( f3.at(0,2), f3.at(1,2), f3.at(2,2), 
                  f3.at(0,1), f3.at(1,1), f3.at(2,1), false );
        
        ta.update( tb );
        tb.update( ta );  

        ua.update(  tb.p.tr, ta.p.tr, ta.base().tr );
        ub.update( ua );

        Draw(ua);
        Draw(ub);

       /* for (int j = 0; j < width; ++j){ */ 
       
       /*    float t = 1.0 *j/width;  //cout << t << endl; */
       /*    float tmp = 1 + (decay * t); */

       /*    for (int k = 0; k < f3.w(); ++k){ */
       /*      f3.at(k,1) = f3.gridAt(k,1).trs( - dx - (decay * t),dy,0); */ 
       /*    } */

       /*    Truss nua ( f3.at(1,2), f3.at(1,1), f3.at(1,0), */ 
       /*                f3.at(0,2), f3.at(0,1), f3.at(0,0), false ); */

       /*    Truss nub ( f3.at(2,2), f3.at(2,1), f3.at(2,0), */
       /*                f3.at(1,2), f3.at(1,1), f3.at(1,0), true ); */

       /*     nua.update( ub ); */
       /*     nub.update( nua  ); */


       /*     ua = nua; ub = nub; */
       /*     Draw(ua); Draw(ub); */
       /* } */

        

       // ta.update( tb );
       // tb.update( ta );

        //ta.rigidA.moveB(t*decay);

        //ta.rigidB.calibrate( ta.rigidA() );
        
        //ta.update( tb.p.tl, tb.p.tm, tb.p.tr );
        
        //tb.update( ta.p.tl, ta.p.tm, ta.p.tr );

        
      
       
     }

      /*   ua.update(  tb.p.tr, ta.p.tr, ta.base().tr ); */
      /*   ub.update(  ua.p.tl, ua.p.tm, ua.p.tr ); */
      /*   Draw(ua); Draw(ub); */
 
      /*  for (int j = 0; j < width; ++j){ */ 
       
      /*   float t = 1.0 *j/width;  cout << t << endl; */
      /*   float tmp = 1 + (decay * t); */

      /*   ua.update(ub); */
      /*   Draw(ua); */
      /*   ub.update(ua); */
      /*   Draw(ub); */
      /*  } */



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


         /* for (int i = 0; i < n; i += 1 ) { */
         /*  float t = -5 + 10 *(float)i/n; */
         /*  dll[i] = DLN(1,0,0).trs(0,t,0); */
         /*  //Draw(dll[i]); */   
     /* } */
     
     
     /* for ( int i = 0; i < n; i += 1 ) { */
        /* float t = -5 + 10 * (float)i/n; */
        /* Dlp dlp(1,0,0,t); //Draw(dlp); */
        /* Dlp dlp2(1,0,0,t-.33); */

        /* for (int j = 0 ; j < n/2; ++j ){ */
         /*  f.at(i,j*2) = meet(dll[j*2], dlp); //(dll[j] ^ dlp ).dual().null(); */
         /*  f.at(i,j*2+1) = meet( dll[j*2+1], dlp2 ); */
        /* } */

     /* } */

           /* for (int i = 0 ;i < f.w()-1; ++i){ */
        /* //float u = (float)i/f.w(); */
        /* int w2 = i * 2; */
        /* for (int j = 0; j < f.h()-1; ++j){ */
           /*  int j2 = j * 2; */
          /* //  float v = (float)j/f.h(); */
           /*  Point a = f.at(i,j); */
           /*  Point b = f.at(i,j+1); */
           /*  Point c = f.at(i+1,j+1); */
           /*  Point d = f.at(i+1,j); */
           /*  Point ab = ((a+b)/2.0).null(); */
           /*  Point bc = ((b+c)/2.0).null(); */
           /*  Point cd = ((c+d)/2.0).null(); */
           /*  Point da = ((d+a)/2.0).null(); */
           /*  Point e = ((ab+cd)/2.0).null();//( ab * theta + cd * (1-theta) ).null(); */
            
           /*  int ia = f3.idx(w2,j2,0); */
           /*  int ib = f3.idx(w2,j2+2,0); */
           /*  int ic = f3.idx(w2+2,j2+2,0); */
           /*  int id = f3.idx(w2+2,j2,0); */
           /*  int iab = f3.idx(w2,j2+1,0); */
           /*  int ibc = f3.idx(w2+1,j2+2,0); */
           /*  int icd = f3.idx(w2+2,j2+1,0); */
           /*  int ida = f3.idx(w2+1,j2,0); */
           /*  int ie = f3.idx(w2+1,j2+1,0); */


           /*  f3.grid(ia) = a; */
           /*  f3.grid(ib) = b; */
           /*  f3.grid(ic) = c; */
           /*  f3.grid(id) = d; */

           /*  if (w2 == 0 ) f3.grid(iab)= ab; */
           /*  f3.grid(ibc) = bc; */
           /*  if (w2 + 2 < f3.w()-1) f3.grid(icd) = (cd * (1-theta) + e * theta ).null(); */
           /*  else f3.grid(icd) = cd; */
           /*  f3.grid(ida) = da; */
           /*  f3.grid(ie) = (e * (1-theta) + ab * theta ).null(); */


        /* } */
      /* } */

      /* for (int i = 0; i < f3.num(); ++i) f3[i] = f3.grid(i); */

      /* Draw(f3); */


