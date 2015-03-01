/*
 * =====================================================================================
 *
 *       Filename:  xLinkages.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/13/2014 12:50:12
 *       Revision:  none
 *       Compiler:  clang++3.2 or higher or gcc4.7 or higher
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

#include "vsr_chain.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt, pinRatio, decayRate;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(pinRatio, "pinRatio",0,100)(decayRate,"decayRate",-100,100);

      pinRatio = 1;
      decayRate = 0;
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

      static int num = 20;    
      static Chain chainA(num);
      static Chain chainB(num);
    
      static Point pnt = PT(.5,-2,0);  

      Touch(interface, pnt);
      
      chainA[0].pos() = pnt;
    
      for (int i = 0; i < num; ++i){
        Draw( chainA[i] ); Draw(chainB[i]);
        
      }

      double tpinRatio = pinRatio;
      bool flip = false;
    
      for (int i = 0; i < num; i+=2){
        
        //Next chain element connects A and B (pivot)
        Dls dlsA = chainA.nextDls(i);        
        Dls dlsB = chainB.nextDls(i);
        
        //Alternate dilation of A and B
        Dls tdlsa = flip ? dlsA : dlsA.dil( dlsA.null(), log(tpinRatio) );
        Dls tdlsb = !flip ? dlsB : dlsB.dil( dlsB.null(), log(tpinRatio) );
                         
        //Intersection               
        Par p = ( tdlsa ^ tdlsb ^ Dlp(0,0,1,0) ).dual(); 
        Pnt pnt = Ro::split(p,flip);
        
        //Set Chain Position of Next Element
        chainA[i+1].pos() = pnt;
        chainB[i+1].pos() = pnt;
        
        //Alternate next position ratio 
        double a = ( 1 /tpinRatio);
        double b = tpinRatio;
        
        //chain + direction * ratio
        chainA[i+2].pos() =  Ro::null( chainA[i].vec() + ( ( chainA[i+1].vec() - chainA[i].vec() ) * (1 + ( (flip) ? b : a ) ) ) );
        chainB[i+2].pos() =  Ro::null( chainB[i].vec() + ( ( chainB[i+1].vec() - chainB[i].vec() ) * (1 + ( (flip) ? a : b ) ) ) );
        
        flip = !flip;
        
        tpinRatio *= (1-decayRate);

        Draw ( pnt ); 

    }

    chainA.joints();
    chainB.joints();   
          
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

