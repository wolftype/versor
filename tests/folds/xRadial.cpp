/*
 * =====================================================================================
 *
 *       Filename:  xRadial.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/19/2013 17:22:56
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


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt, spacing, niter,numPoly,dz,dr,expand;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(spacing,"spacing",-100,100);
      gui(niter,"iter",1,100);
      gui(numPoly, "numPoly",3,1000);
      gui(expand,"expand",0,100);
      gui(dz, "dz",-10,10)(dr,"dr",-10,10);
      niter = 1; spacing = .5; numPoly = 4;
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 

      glLineWidth(5);
        
      getMouse();

      int poly = numPoly;
      Set<Pnt> s;
      Set<Rigid2> rigid2;
      Set<Rigid3> rigid3;
      s.add(PAO);

      //BUILD PATTERN
      bool bSwitch = false;
      int iter = niter;
      for (int j = 0; j < iter; ++j){
        for (int i = 0; i < poly; ++i){
          VT t = PI * i/poly;
          Vec v = Vec::x.rotate( Biv::xy * t);// + Vec::z * (float)j/iter * dz;
          
          VT tdr =  dr * (float)j/iter; //radius offset
          VT tdz = (float)j/iter * dz; //rotation

          Pnt p = Ro::point ( v ).trs( v.unit() * spacing * j * ( bSwitch ? 1 : tdr ) ).rotate( Biv::xy * tdz);
          s.add(p);
          bSwitch = !bSwitch;
        }
      }

      //DRAW
      for (auto i : s ) Draw(i,0,1,0);

      //SET CONSTRAINTS AT CENTER
       for (int i = 0; i < poly; ++i){
            int idx = i + 1;
            int prev = (idx != 1 ) ? i : poly;
            int next = (idx != poly ) ? idx + 1 : 1; 
            bool bTheta = i < poly/2.0 - 1;
            Rigid2 rig( s[idx], s[0], s[prev] );
            Rigid2 rodd( s[idx], s[next], s[0] );
            rigid2.add( rig ); //bTheta ? rig : rodd );
        }

      //SET CONSTRAINTS FOR REST
      bSwitch = false;
      for (int j = 1; j < iter; ++j){
       
        int start = j * poly + 1;
        int pstart = (j-1) * poly + 1;

        for (int i = bSwitch ? 0 : 1; i < poly; i+=2){
          int idx = start + i;
          int pidx = pstart + i;
          int pprev = ( i > 0 ) ? pidx - 1 : pstart + poly -1;
          int pnext = (i < poly - 1 ) ? pidx + 1 : pstart;
          Rigid3 rig( s[idx], s[pprev], s[pidx], s[pnext], bSwitch );
          rigid3.add(rig); 
        }

        for (int i = bSwitch ? 1 : 0; i < poly; i+=2){
          int idx = start + i;
          int pidx = pstart + i;
          int prev = (i > 0 ) ? idx - 1 : start + (poly-1) ;
          int next = (i < poly-1) ? idx + 1 : start;
          Rigid3 rig( s[idx], s[prev], s[pidx], s[next], bSwitch );
          rigid3.add(rig);                
        }

        bSwitch = !bSwitch;
     }

      
      //MOVE CENTER
      vector<Pnt> tp;
      for (int i = 0; i < poly ; ++i){
        tp.push_back(s[i+1]);
      } 
      for (int i = 0; i < poly; i+=2 ){
        tp[i] = rigid2[i](PIOVERTWO*amt * .1);     
      }

     //MOVE REST
     bSwitch = false;
     for (int j = 0; j < iter-1; ++j){
        int start = j * poly;

        int it = 0;
        for (int i =  (bSwitch ? 0 : 1); i < poly; i+=2){
          int idx = start + it; 
          int prev = (i > 0 ) ? i - 1 : poly-1;
          int next = (i < poly-1) ? i + 1 : 0;
          rigid3[idx].update( tp[prev], tp[i], tp[next] );  
          it++;      
        }
   
        it = 0;
        for (int i = ( bSwitch ? 0 : 1); i < poly; i+=2){
           int idx = start + it;
           tp[i]  = rigid3[idx]();
          // Draw( rigid3[idx].possible() );
         //  Draw( tp[i],0,1,0 );
           it++;
        }

        it = poly/2.0;
        for (int i =  ( bSwitch ? 1 : 0); i < poly; i+=2){
          int idx = start + it;
          int prev = (i > 0 ) ? i - 1 : poly-1;
          int next = (i < poly-1) ? i + 1 : 0;
          rigid3[idx].update(tp[prev], tp[i], tp[next] );//add(rig); 
          it++;               
        }

        it = poly/2.0;
       for (int i =  (bSwitch ? 1 : 0); i < poly; i+=2){
           int idx = start + it;
           tp[i]  = rigid3[idx]();
          // Draw( rigid3[idx].possible() );
          // Draw( tp[i], 0,1,1);
           it ++;
        }


        bSwitch = !bSwitch;
     }

   glColor3f(0,.5,.5);
    glBegin(GL_TRIANGLES);
     for (auto i : rigid3.data() ){
        auto pnt = i();
        TriNormal( pnt, i.b.p, i.a.p);
        TriNormal( pnt, i.c.p, i.b.p );
     }
     glEnd();

    glColor3f(1,0,0);
    glNormal3f(0,0,1);
     for (auto i : rigid3.data() ){
           glBegin(GL_LINE_STRIP);

        auto pnt = i();
        GL::Tri( pnt, i.b.p, i.a.p);
        GL::Tri( pnt, i.c.p, i.b.p );
             glEnd();

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
