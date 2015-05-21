/*
 * =====================================================================================
 *
 *       Filename:  vsr_conicCam.h
 *
 *    Description:  cam design with conics
 *
 *        Version:  1.0
 *        Created:  04/03/2014 14:53:57
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
#include "vsr_conic.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,phi,adj;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(phi,"phi",-100,100);
      gui(adj,"adj",-100,100);
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


      static Cir cir = CXY(1);
      Touch(interface,cir);
      Draw(cir);

      Dlp dlp(1,0,0,adj);
      //Touch(interface,dlp);

      //Euclidean vector of circle at theta
      Vec v = Ro::vec(cir,phi);
      Draw( v );

      for(int i=0;i<100;++i){
        Pnt p = Ro::pnt_cir(cir,(float)i/100 * TWOPI);
        auto np =  Conic::Transform(p, v, amt );
        Draw(np,0,1,0); 
      }

      auto tnp = Conic::Transform( Ro::pnt_cir(cir,adj*TWOPI), v, amt ); //tangent
      Draw( Ro::dls(tnp,.2),1,0,0);
     // auto itnp = Conic::I

      Dlp idlp = Conic::ITransform(dlp, v, amt);
      Par par = (idlp ^ cir.dual() ).dual();

     // Pnt ipnt = Pnt(idlp) + Ori(1);
      
      auto tpnt = Ta::at(cir, Pnt(idlp));
      Draw( tpnt, 1,1,0);

      cout << Ro::size(par,false) << endl;

      Draw(dlp,0,0,1);
      Draw(idlp);
      /* Dlp idlp2 = idlp.rot(Biv::xy * adj); */
      /* Draw( idlp2,1,0,0 ); */
      /* Draw( Conic::Transform(idlp2,v,amt) ); */

      for(auto i : Ro::split(par)){
        Draw( Conic::Transform(i,v,amt),1,0,0 );
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
