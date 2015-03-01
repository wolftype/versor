/*
 * =====================================================================================
 *
 *       Filename:  xConic2.cpp
 *
 *    Description:  conic transforms
 *
 *        Version:  1.0
 *        Created:  01/09/2014 13:06:20
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

#include "vsr_conic.h"
#include "gfx/gfx_mesh.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,beta,gamma;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(beta,"beta",-1,1);
      gui(gamma,"gamma",-1,1);
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
  
        Vec mv = -Vec(mouse[0], mouse[1] ).unit();

        Dls dls = Ro::dls(1,0,0,0);

       Draw( Vec::x.sp(  Gen::rot( PI * beta, PIOVERFOUR * gamma )), 0,1,0 ); 

        
        vector<Point> vp;
        int num = 20;
        for (int i = 0; i <= num; ++i){
          for (int j = 0; j < num; ++j){

            float tu= PI * i/num;//-1 + 2.0 * i/num;
            float tv = -PIOVERFOUR  + PIOVERFOUR * j/num;
            Point p1 = Ro::split( Ro::round( dls, Vec::x.sp(  Gen::rot(tu, tv) ) ), true).null();//Ro::pnt_cir( cir, tu );
          
            Point tp = p1.bst( -Par(mv.copy<Tnv>()).trs(mv) * amt );// Draw( p1, 1,1,0);
            Draw(tp,1,0,0);

            Point ap = Conic::Transform( p1, mv, amt );
            vp.push_back(ap);

          
          }
        }

          Mesh mesh = Mesh::UV( vp.data(), num+1, num );
          mesh.drawElements();

          Dlp td = mouse <= Drv(1,0,0);
          Draw(td,0,0,1);

          Dlp itd = Conic::ITransform( td, mv, amt);//(ipa ^ ipb ^ ipc ^ Inf(1)).dual();

         // Draw(pb); Draw(pc);

          Draw(itd);


          //LINE INTERSECTION
         // auto itl = Conic::ITransform ( tl, mv, amt );
        //  auto par = Ro::split( itl ^ dls );
        //  for (auto i : par ) Draw( Ro::dls( Conic::Transform(i, mv, amt), .2 ), 1,1,0 );
          

          auto meet = (itd ^ dls).dual();

        //  Draw(meet);

          for (int i = 0; i < num; ++i){
            Point mp = Ro::pnt_cir( meet, PI * i/num );
            mp = Conic::Transform( mp, mv, amt);
            Draw(mp,0,1,0);
          }


          Draw( Conic::ITransform( Fl::loc( td, mouse, true).null(), mv, amt ), 0,1,1);
         // Draw( Conic::ITransform( Vec::y.null(), mv, amt ), 0,1,1);
        /* Line la = LN(0,1,0).trs( sin(time) * 1, 0,0); */
        /* Draw(la); */

        /* Line ila = Conic::ITransform( la, mv, amt ); */

        /* auto pp = ( ila.dual() ^ dls).dual();//Ro::sur(cir) ).dual(); */
        /* auto p2 = Ro::split( pp ); */
        /* Pnt pa = Conic::Transform (p2[0], mv, amt); */
        /* Pnt pb = Conic::Transform(p2[1], mv, amt); */
         
        /* Draw( pa ); Draw(pb); */ 
    
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
