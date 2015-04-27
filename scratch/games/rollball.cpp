/*
 * =====================================================================================
 *
 *       Filename:  rollball.cpp
 *
 *    Description:  unity mock up -- w/o lights for now
 *                  make things, "add playerController"
 *
 *        Version:  1.0
 *        Created:  11/04/2014 19:20:53
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
#include "vsr_cga3D_frame.h"

using namespace vsr;
using namespace vsr::cga3D;


namespace vsr { namespace Gen{

 template<TT DIM> 
 auto ratio_( 
  const CGAMV<DIM, typename CGA<DIM>::Vec >& a, 
  const CGAMV<DIM, typename CGA<DIM>::Vec >& b ) -> decltype( (a*b) ) {
   
  using TVEC = CGAMV<DIM, typename CGA<DIM>::Vec >;
  using TBIV = CGAMV<DIM, typename CGA<DIM>::Biv >;
  using TROT = decltype( (a^b) + 1);
  
  VT s = ( a <= b )[0];  
  cout << s << endl;            

  //180 degree check
  if ( FERROR( (a <= TVEC(b.conjugation()))[0] ) ) {
    
    if ( a == TVEC::y || a == -TVEC::y ) {
      printf("conj A\n");

      return rot( TBIV::xy * PIOVERTWO );
    }        
    printf("conj\n");
    return rot( a ^ TVEC::y * PIOVERTWO); //mind the ordering of blades
  }         
  
     // printf("not 180\n");  
      VT ss = 2 * (s+1);
      VT n = ( ss >= 0 ? sqrt ( ss ) : -sqrt(-ss) );

      if (ss < 0) printf("ss<=0\n");
      if (FERROR(n)) printf("error n\n");

      TROT r = b * a  ; //cout << r << endl;

      r[0] += 1;  
      if (!FERROR(n)) r /= n;
      if ( r == TROT() ) return TROT(1);//else cout << r << endl; //printf("0 in gen::ratio\n");
      return r;    
  } 
}}

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  bool bReset;

  //Sphere player;
  Frame player;
  Frame fcamera;
  DualPlane ground;
  int numthings;
  bool bHit;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
    init();
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(bReset,"reset");
      amt = 4.72;
  }

  void init(){
    ground = Dlp(0,1,0);
    player.pos(0,1,0);
  }

  void update(){

  }

  void fixedUpdate(){
    //test for intersection
    auto pp = (player.bound() ^ ground).dual();
    if (Ro::size(pp,true) > 0) bHit = true;

    if (bHit){
      player.move(0,-Ro::rad(pp),0 );
    }

    player.move();
  }

  void lateUpdate(){
    //fcamera = player;
   // fcamera.move(0,5,0);
    fcamera.orient ( player.pos() );
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

      //note would like to be able to "attach a behavior" to player
      //so frames could have a list of behaviors . . . and an addForce method
      //Unity has "update" and "fixed update" and "lateUpdate"
      //2. want to be able to make a frame a child of another frame (follow it)
      //want to be able to add a behavior to a camera that makes it follow a frame . . .
      time+=.01;
      fcamera.pos(cos(amt)*10,5,sin(amt)*5);
      fixedUpdate();
      lateUpdate();

    //  cout << fcamera.y().norm() << endl;
     
    //  auto nv = Vec(Op::pj( Vec::y, fcamera.xy())).unit();
    //  DrawAt( nv,  fcamera.negZ()); 
    //  DrawAt( fcamera.y().spin( Gen::ratio( fcamera.y(), nv) ), fcamera.negZ()*1.1,0,1,1);
      //auto q = fcamera.quat();       
      //scene.camera.quat() = Quat(q[0],q[1],q[2],q[3]);
      //scene.camera.pos() = fcamera.vec();

      Draw(fcamera);
      Touch(interface,player);

      Draw(player.bound(), 0,0,1);
      Draw(ground);
      
  }

  virtual bool onKeyDown(GLV& glv){ 
    
    //App::onKeyDown(glv);

    Vec v(0,0,0);
    if (interface.keyboard.code == 'i'){
      cout <<"up" <<endl;
      v += Vec(0,0,-.1);
    }
    if (interface.keyboard.code == 'k'){
      //cout <<"up" <<endl;
      v += Vec(0,0,.1);
    }
    if (interface.keyboard.code == 'l'){
      //cout <<"up" <<endl;
      v += Vec(.1,0,0);
    }
    if (interface.keyboard.code == 'j'){
      //cout <<"up" <<endl;
      v += Vec(-.1,0,0);
    }

    player.dx() += v * amt;
    
   // cout << "false" << endl;
    return true;
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
