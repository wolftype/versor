/*
 * =====================================================================================
 *
 *       Filename:  xElectret.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/06/2013 12:34:11
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
      
      Field<Vec> field(20,20,20);
      Field<Flp> electric(5,5,5,4);

      bool bSource = true;
      for (int j = 0; j < electric.num(); ++j){
        bSource = !bSource;
        float val = amt * ( bSource ? -1 : 1 ) ;
        electric[j] = electric.grid(j) ^ Inf( val );
      }

      
      auto gen = mouse ^ Inf(-.1); ///<-- A "Flat Point"

      auto source = Gen::bst( gen ); /// Exponential of Flat Point

      for (int i = 0; i < field.num(); ++i){
        Flp flp;
        for (int j = 0; j < electric.num(); ++j){
          float dist = Ro::sqd( field.grid(i), electric.grid(j) );
          flp += electric[j] * ( 1.0 / (dist + .1) );
        }
        auto s = Gen::bst( flp );
        Point np = field.grid(i).spin( s );
        np = np / np[3];
        field[i] = np - field.grid(i);
      }

        Draw(field);




    
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
