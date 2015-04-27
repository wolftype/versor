/*
 * =====================================================================================
 *
 *       Filename:  xCurvedCrease.cpp
 *
 *    Description:  attempt to model curved creasing
 *
 *        Version:  1.0
 *        Created:  12/11/2013 13:17:56
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
  float amtA, amtB;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amtA,"amtA",-100,100);
      gui(amtB,"amtB",-100,100);

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

     static Dlp dlp = Vec(0,1,1).unit() + Inf(0);
     Touch(interface,dlp);
     Draw(dlp,0,1,0);

      //some lines
      int iter = 50;
      float spacing = 10;
      vector<Lin> lines(iter);

      int it = 0;
      for ( auto &i : lines ){
        it += 1;
        float t = -spacing/2.0 + spacing * (float) it / iter;


        //Top curvature
        Par tp = Par( Tnv(0,0,amtA) ).trs(0,3,0);       
        auto bstA = Gen::bst( tp );
        
        Pnt tpnt = Ro::point( t, 3, 0 );

        Dll dll = (tpnt ^ Vec::y ^ Inf(1) ).dual();

        Pnt pa = Ro::loc( tpnt.spin( bstA ) );


        //Bottom Curvature
        Par bp = Par( Tnv(0,0,amtB) ).trs(0,-3,0);       
        auto bstB = Gen::bst( bp );
       
        Pnt bpnt = Ro::point( t, -3, 0 );
        Pnt pb = Ro::loc( bpnt.spin( bstB ) );

        i = pa ^ pb ^ Inf(1); i = i.runit();

        Draw(i);

        Pnt rp = ( i.dual() ^ dlp ).dual().unit().null();
        Draw(rp,1,0,1);
      
        Draw(pa,1,0,0); Draw(tpnt,0,1,0); Draw(pb,1,0,0);


 //       Par itp = Par( Tnv(0,0,amtA) ).trs ( Fl::loc( LN(0,1,0), rp, false ) );
 //       auto ibst = Gen::bst(itp);
 //
        VT dist = Ro::dist( pa, rp );

        Pnt rpp = Ro::loc( Ro::split( ( Ro::dls( tpnt, dist ) ^ dll ).dual(), false )) ;

        VT dist2 = Ro::dist( tpnt, rpp );

 //       cout << dist << " " << dist2 << endl;
//        Pnt rpp = Ro::loc( rp.spin( !ibst ) ); //inverse operation
        Draw( rpp, 1,1, 0);
        auto ri = i.reflect(dlp);

        Draw(ri,0,0,1);

    }


        // Draw(dls,0,1,1,.2); 

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
