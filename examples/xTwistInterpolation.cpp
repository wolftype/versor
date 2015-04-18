/*
 * =====================================================================================
 *
 *       Filename:  xTwistInterpolation.cpp
 *
 *    Description:  example of interpolating a bunch of frames
 *
 *        Version:  1.0
 *        Created:  02/27/2014 13:02:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

#include "vsr_cga3D_frame.h"
#include "vsr_interp.h"
#include "vsr_stat.h"   //< Random Number Generator

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  bool bDrawInterp;

  int mNumFrames, mRes;

  Frame * frame;      ///< Frames
  DualLine * dll;     ///< Dual Line Representations

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
    mNumFrames = 10;
    mRes = 100;
    frame = new Frame[mNumFrames];
    dll = new Dll[mNumFrames];

    //Set Frames up along a circle
    for (int i = 0; i < mNumFrames; ++i){
       float t = TWOPI * (float)i/mNumFrames;
       frame[i].pos() = Ro::point( 3*cos(t), 3*sin(t), 0);
       frame[i].rot() = Gen::rot( Biv::xy * t/2.0 );
    }

  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(bDrawInterp);
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

      for (int i = 0; i < mNumFrames; ++i){
    
        Draw(frame[i]);
        
        // ADD RANDOM ROTATION        
        Biv biv;
        
        double rand = Rand::Num(-amt,amt);
        int axis = Rand::Int(2);

        switch(axis){
          case 0: biv = Biv::xy * rand; break;
          case 1: biv = Biv::xz * rand; break;
          case 2: biv = Biv::yz * rand; break;
        }

        frame[i].db() = biv;
        frame[i].step();

        dll[i] = frame[i].dll();

      }


      for (int i = 0; i < mRes; ++i){
          float t = (float)i/mRes;

/*           for (int j = 0; j < mNumFrames: ++j){ */
              
/*           } */
          //Dll tdll = Interp::linear(dll[8],dll[9], t);// Interp::quadric(&dll[7], 3, t, false );

          //Make and Draw Frame
          /* Draw( Frame(tdll) ); */

          /* Dll tdll2 = Interp::linear(dll[7],dll[8], t); */
          /* Draw( Frame(tdll2) ); */


          /* Dll tdll3 = Interp::linear(dll[9],dll[0], t); */
          /* Draw( Frame(tdll3) ); */

          Dll tdll = Gen::log( frame[(int)(fabs(amt))].mot() / frame[9].mot() );

          Draw( Frame( Gen::mot(tdll*t) * frame[9].mot() ) );


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
