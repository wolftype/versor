/*
 * =====================================================================================
 *
 *       Filename:  xGradient.cpp
 *
 *    Description:  Geometric Calculus . . . a gradient 
 *
 *        Version:  1.0
 *        Created:  11/22/2013 16:41:36
 *       Revision:  none
 *       Compiler:  gcc4.7 or clang3.2 or greater
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  
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

  //A Field
  Field<Vec> field; 


  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;

    
    field.resize(10,10,1);

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

      Vec deriv(.1,.1,.1);
            
      int n = field.num();
      
      for ( int i = 0; i < field.w(); i += 1 ) {
        
        float tu = (float)i/field.w();
        float tdu = tu + .001;

        for (int j = 0; j < field.h(); j += 1 ){
      
          float tv = (float)j/field.h();
          float tdv = tv + .001;

          Sca tmp = Vec::x.rot( Biv::xy * PI * tu ) <= Vec::y.rot( Biv::yz * TWOPI * tv );
          Sca tmp2 = Vec::x.rot( Biv::xy * PI * tdu ) <= Vec::y.rot( Biv::yz * TWOPI * tdv );
          
          field[i] = Vec(tmp[0], tmp2[1], 0);      
        
        }
      
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
