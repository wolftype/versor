/*
 * =====================================================================================
 *
 *       Filename:  xPantograph.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/03/2014 19:01:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


//Get mouse position as an "unprojected" point in 3D space

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_linkages.h"
#include "vsr_chain_draw.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,ratio,decay, ratioSub, decaySub;
        
  Pantograph panto;

  vector <Pantograph> subpanto;


  MyApp(Window * win ) : App(win)
  {
    scene.camera.pos( 0,0,10 ); 
    time = 0;
    int num = 40;
    
    panto.alloc(num);
    subpanto = vector<Pantograph>(num/2);
    for (auto& i : subpanto){
      i.alloc(20);
    }
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(ratio,"ratio", -100,100);
      gui(decay,"decay",-100,100);
      gui(ratioSub,"ratioSub", -100,100);
      gui(decaySub,"decaySub",-100,100);

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

      panto.reset();

      panto.chainA().baseFrame().pos() = PT(-amt,0,0);
      panto.chainB().baseFrame().pos() = PT(amt,0,0);
    
      panto.ratio( ratio );
      panto.decay( decay );
      panto();
      panto.fk();

      Draw( panto.chainA() );
      Draw( panto.chainB() );

      
      bool bFlip = false;
      for (int i = 0; i < subpanto.size(); ++i){
       
        subpanto[i].reset();

        Vec dv = Vec( bFlip ?  panto.chainA()[i*2].pos() - panto.chainB()[i*2].pos() : 
                               panto.chainB()[i*2].pos() - panto.chainA()[i*2].pos() ).unit();
        
        Rot rot = Gen::ratio( Vec::x, dv ) * Gen::rot( Biv::yz * -PIOVERFOUR);


        subpanto[i].chainA().baseFrame().pos() = bFlip ? panto.chainB()[i*2].pos() : panto.chainA()[i*2].pos();
        subpanto[i].chainA().baseFrame().rot() = rot;

        subpanto[i].chainB().baseFrame().pos() = bFlip ? panto.chainA()[i*2].pos() : panto.chainB()[i*2].pos();
        subpanto[i].chainB().baseFrame().rot() = rot;
        
        subpanto[i].ratio( ratioSub ); 
        subpanto[i].decay( decaySub ); 
        
        subpanto[i]();
        subpanto[i].fk();
         
        Draw( subpanto[i].chainA() );
        Draw( subpanto[i].chainB() );
        bFlip = !bFlip;
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
