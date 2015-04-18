/*
 * =====================================================================================
 *
 *       Filename:  xSkinning.cpp
 *
 *    Description:  skin a chain . . .
 *
 *        Version:  1.0
 *        Created:  02/05/2014 16:07:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: pablo colapinto
 *   Organization: mat, ucsb 
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
  float amt, falloff;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100)(falloff,"falloff",-100,100);
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

      Draw(mouse);

      Mesh m = Mesh::Cylinder(.3,3,20,20);
      m.mode(GL::P);
     // m.drawElements();


      static const int num = 3;

      static Dll log[num];

      Chain refChain(num);

      Chain chain(num);

      chain.joint(0).pos() = Ro::point(0,-1.5,0);
      refChain.joint(0).pos() = chain.joint(0).pos();

      chain.fabrik( mouse, num-1, 0 );

      chain.fk();
      refChain.fk();


      for (int i = 0; i < chain.num(); ++i){
        Draw( chain[i] );      

        Mot m = chain[i].mot()/refChain[i].mot(); double n = m.rnorm(); 
        if (n!=0) m /= n;

        log[i] = Gen::log( m ) * amt;
        Mot mot = Gen::mot( log[i] );

      }


      typedef decltype( ( Pnt() ^ Inf() ) / ( Pnt() ^ Pnt() ^ Inf() ) ) RCType;
      RCType rca[ num -1 ];
      RCType rcb[ num -1 ];
    
      for (int i = 0; i < refChain.num()-1; ++i){

          Point a = refChain[i].pos();
          Point b = refChain[i+1].pos();
                  
          auto pss = a ^ b ^ Inf(1);
          VT norm = pss.rnorm();
          norm *= norm;
          pss /= norm;

         // pss.vprint();

          rca[i] = ( a ^ Inf(1) )/pss; 
          rcb[i] = ( b ^ Inf(1) )/pss; 

      }



      
      for (int i = 0; i < m.num(); ++i){
        Dll tdll;  
        Pnt p = Ro::point( m[i].Pos[0], m[i].Pos[1], m[i].Pos[2] );
       //<t_úX> p.vprint();
        for (int j = 0; j < chain.num() -1; ++j){

         // Dll lf = refChain[j].dly();
          // auto pf = refChain[j].pos() ^ refChain[j].pos().trs(0,1,0);//efChain[j].pos().trs(0,.5,0), 1.0);
          
          auto wa = fabs ( ( p <= rca[j] )[0] );
          
        //  cout << wa << endl;   
         // auto wb = fabs ( ( p <= rcb[j] )[0] );  

        //  cout << wa << " " << wb << endl;
        //  VT sum = wa + wb;
          VT wta = 1.0/(wa*wa) ;//( (p <= pf).wt() * falloff ); //Ro::sqd( p, refChain[j].pos() );
          //VT wtb = 1.0 / (wb);

          tdll += log[j] * wta;// + log[j+1] * wb;
        }

        // tdll.vprint();

          Draw( p.mot( tdll / chain.num() ),1,0,0 ); 

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
