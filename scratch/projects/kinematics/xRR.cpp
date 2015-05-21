

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_chain_draw.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float theta, phi, lengthA, lengthB;

  bool bDraw, bSwitch;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(theta,"theta",-100,100);
      gui(phi,"phi",-100,100);
      gui(bDraw)(bSwitch);
      gui(lengthA,"lengthA", 1,10)(lengthB,"lengthB",1,10);

      lengthA = lengthB = 1.0;
      bDraw = bSwitch = false;
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

      Chain chain("RRRR");
      //Chain ichain("RR");

      /* for (int i = 0; i < chain.num();++i){ */
      /*   Touch(interface, chain.joint(i), chain[i] ) ; */
      /* } */

      VT ts = sin( (phi) );

      cout << ts << endl; 
      if ( ts < 0 ) bSwitch = true;
      else bSwitch = false; 

      Rot ja = Gen::rot( Biv::xy * phi /2.0  );
     // Rot jb = Gen::rot( Biv::xy * (PI-phi)/2.0);
      
      Rot la = Gen::rot( Biv::xz * theta / 2.0 );

      VT nt = asin( sin(theta) * ( lengthB / lengthA ) ) ;
      Rot lb = Gen::rot( Biv::xz * nt /2.0 );
      
      chain.link(0).pos() = Ro::point(0,lengthA,0);
      chain.link(2).pos() = Ro::point(0,lengthA,0);
      chain.link(1).pos() = Ro::point(0,lengthB,0);
      chain.link(3).pos() = Ro::point(0,lengthB,0);


      chain.link(0).rot() = la;//Gen::rot( Biv::yz * theta );
      chain.link(1).rot() = !lb;
      chain.link(2).rot() = la;
      chain.link(3).rot() = !lb;
     
      chain.joint(0).rot() = ja;
      chain.fk();
     
      
      Frame tf = Frame( !chain.link(3).mot() );
      tf.scale( .5 );
      if (bDraw) Draw(tf);

      Frame tf0 = Frame( !chain.link(2).rot() * tf.mot() );
      
      Draw( Ro::point( tf.pos() ), 0,0, 1);

      DualSphere da = Ro::dls_pnt(chain[1].pos(), lengthB);
      DualSphere db = Ro::dls_pnt(tf.pos(), lengthA);

      //INCIDENCE
      Circle meet = (da^db).dual();
      Draw(meet,1,0,0);
      Pair pair = ( chain[1].dxy() ^ meet.dual() ).dual();
      Draw( pair, 0,1,0);

      /* Pair pair2 = ( tf0.dxy() ^ meet.dual() ).dual(); */
      /* Draw( pair2, 0,0,1); */
      /* Draw( tf0.dxy() ); */

      Pnt target = Ro::loc( Ro::split(pair,bSwitch) );
      Vec dv = (target - chain[1].pos()).unit(); //Draw(dv);
      chain.joint(1).rot() = Gen::rot( Biv::xy * acos( ( dv<=chain[1].y() )[0] )/2.0  * (bSwitch ?  -1 : 1 ) );

      chain.fk();

      target = tf.pos();
      dv = (target - chain[2].pos()).unit(); //Draw(dv);
      chain.joint(2).rot() = Gen::rot( Biv::xy * acos( ( dv<=chain[2].y() )[0] )/2.0 * (bSwitch ?  -1 : 1 ) );

      chain.fk();

      target = chain[0].pos();
      dv = (target - chain[3].pos()).unit(); //Draw(dv);
      chain.joint(3).rot() = Gen::rot( Biv::xy * acos( ( dv<=chain[3].y() )[0] )/2.0 * (bSwitch ?  -1 : 1 ) );

      chain.fk();

      Draw(chain);

     // Frame finalF = Frame ( chain[3].mot() * chain.link(3).mot() );
     // Draw(finalF);
    //  Draw(chain.nextDls(3) );


      //Draw(chain);
//      Draw(ichain);
      

      /* VT dot  = ( chain[1].y() <= chain[0].y() )[0] ; */
      /* VT ctarg = (dot == 0 ) ? 0 : cos( (PI-phi) )/dot; */
      /* VT bphi = acos( ctarg ); */

      //chain.joint(1).rot() = jb;

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
