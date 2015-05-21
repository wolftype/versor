/*
 * =====================================================================================
 *
 *       Filename:  xRotation.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/22/2014 18:18:02
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
  float amt,amt2,amt3;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(amt2,"amt2",-100,100);
      gui(amt3,"amt3",-100,100);
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

      Vec v = Vec::y.rot( Biv::xy * PIOVERTWO * amt);
      Draw(v);

      Vec v2 = Vec::y.rot( Biv::xy * PIOVERTWO * amt2);
      Draw(v2);

      VT dot = (v<=v2)[0];
      VT ctarg = cos( PI * amt3 )/dot;
      VT ang = acos( ctarg );
      //VT pang = PI - ang;
      //VT diff = pang -ang;
      //VT interp = ((amt3) - ang)/diff;

     // cout << ang << " " << pang << " " << diff << " " << interp << endl;
      //cout << amt << " " << acos(dot)/PI << endl; 
      Biv b = Biv::yz.rot( Biv::xy * PIOVERTWO * amt );

      Vec v3 = v.rot ( b * ang/2.0);
      Draw(v3);

      VT nang = acos( (v3<=v2)[0] );

      cout << nang/PI << endl; 





    
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
