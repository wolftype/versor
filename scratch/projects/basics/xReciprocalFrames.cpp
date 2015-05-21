/*
 * =====================================================================================
 *
 *       Filename:  xReciprocalFrames.cpp
 *
 *    Description:  some calculus tests
 *
 *        Version:  1.0
 *        Created:  02/05/2014 12:39:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PC 
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

     // Circle ca = CXZ(1).trs(0,-2,0);
     // Circle cb = CXZ(1).trs(0,2,0);

      auto ca = Ro::dls(0.0,0,-3,0);
      auto cb = Ro::dls(0.0,0,3,0);

      Drv drv =  Vec(ca-cb).unit().copy<Drv>();
      Dlp da = ca <= drv;
      Dlp db = cb <= -drv;

      auto intersect = ( ray.dual() ^ Dlp(0,0,1) ).dual().unit().null();


      VT sqd = Ro::sqd( ca, cb );

      ( intersect <= da / sqd ).print();
      ( intersect <= db / sqd ).print();
       
     auto pss = ca ^ cb ^ Inf(1);
    
      VT norm = pss.rnorm();

    auto rca = ( (cb ^ Inf(1) )<= pss ) / (norm*norm);
    auto rcb = ( (ca ^ Inf(1) )<= pss ) / (norm*norm);

    
    //  cout << "r" << endl;

    //  rca.print();


      intersect.print();

      auto sa = intersect <= rca;
      auto sb = intersect <= rcb;

      Draw (ca); Draw(cb);

     // cout << "and ... " << endl; 
     // sa.print(); sb.print();

     // cout << "or ... " << endl;

      auto ss = intersect <= (ca ^ cb);

     // ss.print();
     // cout << ss.wt() << endl; 
      
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
