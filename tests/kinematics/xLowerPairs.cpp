/*
 * =====================================================================================
 *
 *       Filename:  xLowerPairs.cpp
 *
 *    Description:  P (prismatic) R (revolute) C (cylindrical) H (helical screw) and Pl(planar) and S (spherical)
 *
 *        Version:  1.0
 *        Created:  01/21/2014 18:36:15
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

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt, amt2, amt3, amt4, amt5, amt6, d1, d2;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
    gui( d1, "d1", 0,5) (d2, "d2", 0,5);
      gui(amt,"amt",-100,100);
       gui(amt2,"amt2",-100,100);
      gui( amt3, "amt3", -100,100);
      gui( amt4, "amt4",-100,100);
       gui( amt5, "amt4",-100,100);

        gui( amt6, "amt4",-100,100);

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

      VT namt = PIOVERTWO * amt;
      VT phi = PI - PI * amt2;

      VT pamt = PI*amt2;

      VT tha = PIOVERTWO;
      Biv b1 = Biv::yz * tha/2.0;
      VT ctha = ( 1.0/cos(tha) ) / tan( pamt / 2.0 );
      VT tphi = atan( ctha ) * 2.0;

      //( Biv::yz + Biv::xy ) * namt; 
      //Biv b2
      /* VT sd1 = sin( namt ) / d1; */
      /* VT tamt = asin( d2 * sd1 ); */
      /* Biv b2 =  Biv::yz * tamt; */

     // b1.vprint();
     // b2.vprint();

    //  VT as = sin( ( namt + tamt ) / 2.0);
    //  VT bs = sin( ( namt - tamt ) / 2.0);
     // VT phi = 2.0 * atan( ( as / bs ) / tan( PI * amt2/2.0 ) );

      Chain chain(3);

      chain.link(0).pos() = PT(0,d1,0);
      chain.link(1).pos() = PT(0,d2,0);

      chain.link(0).rot( b1 );
      chain.link(1).rot( b1 );

      chain.joint(0).rot( Biv::xy * pamt/2.0 );

       chain.fk();

     // chain.link(1).rot() = b1;//Gen::ratio( chain[0].z(), chain[1].z() );
      //chain.link(1).rot() = chain.link(1).rot() * chain.link(1).rot();
      //some spherical trig

      VT dot  = ( chain[1].y() <= chain[0].y() )[0] ;
      VT ctarg = (dot == 0 ) ? 0 : cos( phi )/dot;
      VT bphi = acos( ctarg );

      chain.joint(1).rot( Biv::xy * bphi/2.0 );
      
     // chain.joint(2).rot( Biv::xy * PIOVERTWO * amt2 );//theta3 / 2.0 );

      chain.fk();

      dot  = ( chain[2].y() <= chain[1].y() )[0] ;
      ctarg =  (dot == 0 ) ? 0 : cos( pamt )/dot;
      VT bphi2 = acos( ctarg );

      chain.joint(2).rot( Biv::xy * bphi2/2.0 );
      
      chain.fk();
      //cout << acos( ( chain[1].y() <= chain[0].y() )[0] ) << " " << phi << endl;

      for (int i = 0; i < chain.num()-1; ++i){
        DrawAt( chain[i].z(), chain[i].pos() );
        DrawAt( chain[i].y() * chain[i].scale(), chain[i].pos(),0,1,0 );
        Draw( chain[i].cxy() );
        Glyph::Line( chain[i].pos(), chain[i+1].pos() );
      }
      
      Chain rchain(3);

      rchain.joint(0) = chain[2];

      rchain.link(0).pos() = PT(0,d1,0);
      rchain.link(1).pos() = PT(0,d2,0);

      rchain.link(0).rot() = chain.link(0).rot();
      rchain.link(1).rot() = chain.link(1).rot();

      rchain.fk();

      dot  = ( rchain[1].y() <= rchain[0].y() )[0] ;
      ctarg =  (dot == 0 ) ? 0 : cos( phi )/dot;
      bphi = acos( ctarg );

      rchain.joint(1).rot( Biv::xy * (bphi)/2.0  ); //!chain.joint(1).rot();// Biv::xy * tphi/2.0 );
     
     //rchain.joint(2).rot(Biv::xy * (pamt)/2.0 ); //!chain.joint(2).rot();//.rot( Biv::xy * PIOVERTWO * amt2  );//ttheta3/2.0 ) ;

      rchain.fk();

      for (int i = 0; i < rchain.num(); ++i){
        DrawAt( rchain[i].z(), rchain[i].pos() );
        DrawAt( rchain[i].y(), rchain[i].pos(),0,1,0 );
        Draw( rchain[i].cxy() );// rchain[i].pos() );
      }
      

      /* Frame f = chain[0]; */
      /* chain!chain.joint(0).rot() * f.rot(); */

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

