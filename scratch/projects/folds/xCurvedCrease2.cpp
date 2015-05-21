/*
 * =====================================================================================
 *
 *       Filename:  xCurvedCrease2.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/13/2013 17:38:39
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
  float amtA, amtB,viter;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amtA,"amtA",-100,100);
      gui(amtB,"amtB",-100,100);
      gui(viter,"iter",2,100);

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
  
     static Dls dls = Ro::dls(5.0, 0,-5,0);
     Touch(interface,dls);


      //some lines
      int iter = viter;
      float spacing = 10;
      vector<Lin> lines(iter);

      int it = 0;
      for ( auto &i : lines ){
        it++;
        float t = -spacing/2.0 + spacing * (float) it/iter;
        i = LN(0,1,0).trs(t,0,0);

        Draw(i);

        auto par = ( i.dual() ^ dls ).dual();
        VT size = Ro::size( par, false );
        
        if ( size > 0 ) {
          
          Pnt tpnt = Ro::split( par, true );
          Pln pln = Ta::at( dls.undual(), tpnt ) ^ Inf(1);

          //ref line
          auto rline = ( pln.dual() ^ Dlp(0,0,1) ).runit();

          auto ri = i.reflect( pln.dual() );

          Draw( ri,  0,1,0 );
          Draw( tpnt,1,0,0 );
          Draw( rline, 0,0,1);

          
          for (int j = 0; j < 100; ++j){
              Mot mot = Gen::mot( rline * PIOVERTWO * (float)j/100);
              Draw( ri.spin(mot) );
          }

        }

    }

       Draw(dls,0,1,1,.2); 

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
