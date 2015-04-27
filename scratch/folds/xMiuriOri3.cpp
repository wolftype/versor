/*
 * =====================================================================================
 *
 *       Filename:  xMiuriOri3.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/02/2013 17:53:52
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

void Draw(const Truss& ta, float r = 1.0, float g = 0.0, float cb = 1.0){
 
 
  Truss::Points a = ta.p; 
//  Truss::Points b = ta.base();

  glColor3f(1,1,1);
  glBegin(GL_POINTS);
    GL::vertex( a.tl.begin() );
    GL::vertex( a.tm.begin() );
    GL::vertex( a.tr.begin() );
  glEnd();

 /* glColor3f(r,g,cb); */
 /*  glBegin(GL_TRIANGLES); */
 /*    TriNormal( ta.p.tl, ta.p.tm, b.tm ); */
 /*    TriNormal( ta.p.tm, ta.p.tr, b.tm ); */
 /*    TriNormal( ta.p.tr, b.tr, b.tm ); */
 /*    TriNormal( b.tm, b.tl, ta.p.tl ); */
 /*  glEnd(); */

}

void Draw(const Truss& ta, const Truss& tb){

 glColor3f(.3,.3,.3);
  glBegin(GL_TRIANGLES);
    TriNormal( ta.p.tl, ta.p.tm, tb.p.tm );
    TriNormal( ta.p.tm, ta.p.tr, tb.p.tm );
    TriNormal( ta.p.tr, tb.p.tr, tb.p.tm );
    TriNormal( tb.p.tm, tb.p.tl, ta.p.tl );
  glEnd();

}


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float amt;
  float dx,dy,decay,num,spacing,numK;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 

  }
  
  void initGui(){

    gui(dx,"dx")(dy,"dy")(decay,"decay",-10,10)(amt,"amt")(num,"num",1,100)(spacing,"spacing",1,100);
    gui(numK,"numK",0,10);
    dx = .2; dy = 0; amt = .1; num = 2;

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

      int numSections = num;
      int numPoints = ( num * 2 ) + 1;
      Field<Pnt> f ( numPoints, numPoints, 1);

      vector<Truss> truss;

      //BUILD PATTERN
      double ts = spacing * num * 2;
      bool bSwitch = true;
      for (int i = 0; i < num; ++i){
        double u = -ts/2.0 + ts * (double)i/num;
        double tu =  -1.0 + 2*(double)i/num;
        bSwitch = !bSwitch;
        for (int j = 0; j < num; ++j){
          double v = -ts/2.0 + ts * (double)j/num;
          double tv = -1.0 + 2* (double)j/num;
          double ttu = tu;
          for (int k = 0; k < (int)numK; ++k){
            ttu *= tu;
          }
          double ddx = dx;// * ttu;// * decay;// (bSwitch) ? dx : dx * decay ;

          Pnt a = PT( u, v, 0 );
          Pnt b = PT( u + ddx, v + spacing + dy , 0 );
          Pnt c = PT( u + spacing + ddx, v + spacing + dy, 0);
          Pnt d = PT( u + spacing, v, 0 );
          f.at(i*2, j*2 ) = a;
          f.at(i*2, j*2+1) = b;
          f.at(i*2+1, j*2+1) = c;
          f.at(i*2+1, j*2) = d; 

          if( j == num-1) {
            f.at(i*2, j*2+3) = PT( u, v + 2*spacing,0);
            f.at(i*2+1, j*2+3) = PT( u + spacing, v+ 2*spacing,0);
          }
        }
      }

      //<t_úX>Draw(f);

      //SET CONSTRAINTS
      for (int j = 0; j < num; ++j){
          Pnt& a = f.at(0, j*2 );
          Pnt& ab = f.at(0, j*2+1);
          Pnt& b = f.at(0, j*2+2);
          Pnt& bc = f.at(1, j*2+2);
          Pnt& c = f.at(2, j*2+2);
          Pnt& cd = f.at(2, j*2+1);
          Pnt& d = f.at(2, j*2);
          Pnt& da = f.at(1, j*2);
          Pnt& e = f.at(1, j*2+1);

          Truss ua( ab, e, cd,
                    a, da, d, true);
          Truss ub( b, bc, c,
                    ab, e, cd, false);

          truss.push_back(ua);
          truss.push_back(ub);

      }

      bSwitch = true;
      for (int i = 1; i < num; ++i){
       // bSwitch = !bSwitch;
        for (int j = 0; j < num; ++j){
          Pnt& a = f.at(i*2, j*2 );
          Pnt& ab = f.at(i*2, j*2+1);
          Pnt& b = f.at(i*2, j*2+2);
          Pnt& bc = f.at(i*2+1, j*2+2);
          Pnt& c = f.at(i*2+2, j*2+2);
          Pnt& cd = f.at(i*2+2, j*2+1);
          Pnt& d = f.at(i*2+2, j*2);
          Pnt& da = f.at(i*2+1, j*2);
          Pnt& e = f.at(i*2+1, j*2+1);

          Truss va( bc, e, da,
                   b, ab, a, bSwitch);
          Truss vb( c, cd, d,
                    bc, e, da, !bSwitch);

          truss.push_back(va);
          truss.push_back(vb);

        }
      }

    
    //REALIZE CONSTRAINTS
    //FIRST CELL
    Rigid2 ra( f.at(2,0), f.at(1,0), f.at(1,1), true );
    Pnt tp = ra( PI * amt );

    Draw(tp,0,1,0);

    truss[0].update( tp, f.at(1,0), f.at(0,0) ); 
    /* truss[1].update( truss[0] ); */

    Draw(truss[0]);
   // Draw(truss[1]);

    //FIRST COLUMN
    bSwitch = false;
    for (int j = 1; j < num; ++j){
        bSwitch = !bSwitch;
        int idx = j * 2;

       // truss[idx].update( truss[idx-1] );
       // truss[idx+1].update( truss[idx] );

       // Draw(truss[idx],.6,.3,.3);
       // Draw(truss[idx+1],.6,.3,.3);
    }

    //SECOND COLUMN
    for (int j = 1; j < num; ++j){
       bSwitch = !bSwitch;
        int idx = (num) * 2 + j * 2;
        int pt = j * 2 + 1;
        int pb = pt - 1;

/*         truss[idx].update( truss[pt].p.tr, truss[pb].p.tr, truss[pb].base().tr ); */
/*         truss[idx+1].update( truss[idx] ); */
        
     //   Draw(truss[idx],.6,.3,.3);
     //   Draw(truss[idx+1],.6,.3,.3);

    }

    //THE REST
    for (int i = 2; i < num; ++i){
      for (int j = 1; j < num; ++j){
        bSwitch = !bSwitch;
        int idx = ( i * (num) * 2) + j * 2;
        int prev = 1 + idx - (num)*2;
        
        /* truss[idx].update( truss[prev] ); */
        /* truss[idx+1].update( truss[idx] ); */
       // Draw(truss[idx],.6,.3,.3);
       // Draw(truss[idx+1],.6,.3,.3);
      }
    }

   // Draw(f);

    /* for (auto i : truss ){ */
    /*    Draw( i.p.tl ); Draw(i.p.tm); Draw(i.p.tr); */
    /* } */

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


      /* //BASE */
      /* Field<Pnt> f (2,3,1); */

      /* for (int i = 0; i< f.w(); ++i){ */
      /*   f.at(i,1) = f.gridAt(i,1).trs(-dx,dy,0); */ 
      /* } */

     /* Truss truss ( f.at(1,2), f.at(1,1), f.at(1,0), */ 
      /*              f.at(0,2), f.at(0,1), f.at(0,0), true ); */ 



    /* //Line of rotation */
     /* Line l = f.at(0,1) ^ f.at(1,1) ^ Inf(1); l = l.unit(); */
     /* Pnt np = f.gridAt( 0, 2).mot( l.dual() * PI * amt ) ; */

     /* Rigid2 rigid( f.at(0,1), np, f.at(0,0) ); */

    
      /* Draw(truss); */
      /* Draw( rigid(),0,1,0 ); */
      /* Draw( rigid( f.at(1,1 )), 0,1,0); */

     /* //FIRST UPDATE */
      
    /* int width = num; */ 
    /* int height = 5; */

    /* bool switcher = true; */
    
     /* for (int i = 0; i < width; ++i){ */

      /*   switcher = !switcher; */


        
    /* } */


