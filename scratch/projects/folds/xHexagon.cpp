/*
 * =====================================================================================
 *
 *       Filename:  xCylinder.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/02/2013 18:05:45
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
  float amt;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
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

      int poly = 6;

      Set<Point> s;
      Set<Rigid2> rig;

      //make the tessellation pattern
      int height = 1;
      int width = 10;
      for (int h = 0; h < height; ++h){
        float th = -(height) + 2*h;//(float)h/height;
        s.add( Ro::point( 0, th, 0 ) ); 
              
        for (int i = 0; i < poly; ++i){
          float t= PI * i/poly; //theta  
          s.add( Ro::point( Vec::x.rot(Biv::xy * t ) ).trs(0, th, 0 ) ) ;        
       }
      }

    
      Rigid2 tr ( s[0], s[poly-2], s[poly-1], false);
      rig.add(tr);

      //creases
      for (int i = 1; i <= poly; ++i){
       int prev = ( i > 1 ) ? i - 1 : poly ;
       Rigid2 r (s[i], s[0], s[prev], false);
       rig.add( r );
     }

      Rigid3 r4 ( s[4], s[3], s[0], s[5], false ); 

      Pnt s1 = rig[1]( amt * PIOVERTWO); 
      Draw(s1);

      //SEQUENCE
      Pnt tp[7]; int it = 0;
      for (auto i : s.data() ) {
       Draw(i);
       tp[it] = s[it]; it++;
      }

    tp[0] = s[0];
    tp[1] = s1;
    for (int i = 2; i < poly-2; ++i){
        int next = ( i < poly  ) ? i + 1 : 1;
        int prev =  i - 1; 
        rig[i].updateB( tp[prev] );
        tp[i] = ( i > 2 ) ? rig[i]( s[next], 1 ) :  rig[i]( s[next], s[prev], amt );//rig[i]( s[next], 1 );// 
        float t = (float)i/poly;
        Draw(tp[i], t, 0, 1-t);
       // Draw( rig[i].orbit(), 0, t, 1-t);
    }
    
    r4.updateA( tp[3] );
    tp[4] = r4();

    Draw( tp[4] );
    //Draw( r4.possible() );
    /* for (int h = 1; h < height; ++h){ */
    /*   int idx = h * 3 + 2; */
    /*   int below = idx - 4; */
    /*   int above = idx - 1; */
    /*   int across = idx - 2; */
    /*   Rigid3 rig( set[idx], set[below], set[above], set[across], false); */ 
    /* } */


      
     // Draw( Ro::dls( s[5], .2) );
     //
     glBegin(GL_TRIANGLES);
        
        for (int i = 1; i < poly; ++i){
          TriNormal( tp[0], tp[i], tp[i+1]  );
        }
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
