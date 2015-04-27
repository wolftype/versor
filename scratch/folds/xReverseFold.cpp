/*
 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 *
 *  homepage: versor.mat.ucsb.edu
 *
 * */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga3D;

template<class T>
void TriNormal( const T& a, const T& b, const T& c){
 gfx::GL::normal( Ro::dir( a^b^c ) );
 gfx::GL::Tri( a, b, c );
}

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  Point a, b, c;
  float time;
  float amt;
  float ratA, ratB; //Ratios along edges


  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 

    time = 0;
    amt = 0;

    a = Ro::null(-1,0,0);
    b = Ro::null(0,3,0);
    c = Ro::null(3,0,0);
  }

  void initGui(){
    gui(amt,"amt",-PI,PI)(ratA,"ratA")(ratB,"ratB");
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
     // cout << tm << endl; 
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );
     // mouse.vprint();  
  }

    virtual void onDraw(){ 
        
      getMouse();

      //Reverse Fold
      //
    Draw(a); Draw(b); Draw(c);
    Touch(interface,a); Touch(interface,b); Touch(interface,c);

    Line ab = a ^ b ^ Inf(1); ab = ab.runit(); Draw(ab);
    Line ca = c ^ a ^ Inf(1); ca = ca.runit(); Draw(ca);
    Line bc = b ^ c ^ Inf(1); bc = bc.runit(); Draw(bc); 

    //Bisector Line sb
    Line sb = (ab - bc); sb = sb.runit(); Draw(sb,0,1,0);
    
    //center
    Point d = meet( sb, ca ) * (1-ratB) + b * ratB; Draw( Ro::loc(d) );

    //Point halfway along ab
    Point e =  (a * (1-ratA) + b * ratA).null(); Draw(e);
    //Reflection of point to bc
    Point f = e.reflect( sb ); f /= f[3]; Draw(f);

    //Circle from point e around bisector line sb
    //Circle cirb = e ^ sb.dual(); Draw(cirb, 0,1,1);
    
    //Circle from point b around hinge center - f
    Circle circ = b ^ ( d ^ f ^ Inf(1) ).runit().dual();  Draw(circ,1,1,0);


    Point ma = a.mot( sb.dual() * amt ); Draw(ma,1,0,0);

    Point me =  ( (ma * (1-ratA) + b * ratA)).null(); Draw(me);


    Dls da = Ro::at( me, b );  //Draw(da,1,0,0,.2);
    //Dls db = Ro::at( d, b );  Draw(db,1,0,0,.2);
    //Dls dc = Ro::at( f, b );  Draw(dc,1,0,0,.2);

    auto par = (da ^ circ.dual()).dual(); //Draw(par);
    Point mb =  Ro::split(par,false);
    Draw( mb );


    glBegin(GL_TRIANGLES);
      TriNormal( ma, me, d);
      TriNormal( d, me, mb);
      TriNormal( d, mb, f);
      TriNormal( d, f, c );
    glEnd();

    



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
