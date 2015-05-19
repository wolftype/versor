//Get mouse position as an "unprojected" point in 3D space

#include "vsr_cga3D.h" 
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga3D;


struct watt{
  
   float distance; ///< distance between points a and b
   float ra,rb;    ///< length of end rods in terms of distance
   float rc;       ///< length of connecting rod absolute

   float fitness;  ///< fitness metric of linkage to accomplish desired path

};


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray; 
  
  float time;
  float radius, ratio, radius2, theta;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto v= interface.mouse.projectMid; 
      ray = Ro::point(v[0],v[1],v[2] ) ^ z ^ Inf(1); 
      mouse = Ro::point( ray,  Ori(1) );  
   }   

  void initGui(){
    gui(radius,"radius",0,10); 
    gui(ratio,"ratio",0,10); 
    gui(radius2,"radius2",0,10); 
    gui(theta,"theta",0,10); 
    radius = 1.0;
  }

    virtual void onDraw(){ 
        
    getMouse();
    
    time += .01;

    //ideal
    auto ln = LN(0,1,0);
    Draw(ln,0,.2,1);
    
    //starting points 
    Point a = Ro::point(-5,0,0);
    Point b = Ro::point(5,0,0);     
    Draw(a,0,0,0); Draw(b,0,0,0);
        
    auto sa = Ro::sphere( a, radius );
    auto sb = Ro::sphere( b, radius * ratio );   

    double fit =0;
    //MOVEMENT
    for (int i=0;i<100;++i){
      auto ttheta = PI*i/100;
      //Point on sphere at theta
      auto p = Ro::round( Ro::sphere( a, -radius ), Vec::x.rot( Biv::xy * ttheta ) );
      auto splitp = Ro::split( p, true);    
      Draw(splitp,1,0,0);
      
      //sphere of size radius2
      auto sc = Ro::sphere( splitp, radius2 );

      //meet of end and first
      auto c = ( sb ^ sc ).dual();    
      //Draw(c,1,1,.2);
      
      //Is legit?
      if ( Ro::size(c, false) > 0 ){
         // Draw( (sc^Dlp(0,0,1)).dual(),0,0,1);
         // Draw( c,0,0,1);
          auto cir = ( sb ^ Dlp(0,0,1) ).dual();
          auto par = ( c.dual() ^ Dlp(0,0,1) ).dual();
          auto splitq = Ro::loc( Ro::split(par,true));

          auto mid = ( splitp + ( (splitq - splitp ) *.5) ).null();

          auto pln = mid ^ ln;
          fit += pln.rnorm();
         // cout << pln.rnorm() << endl; 
          glVertex4f(0,0,0,0);
          Glyph::Line(splitq, splitp);
          Glyph::Line(sa, splitp);
          Glyph::Line(sb, splitq);
          Draw(mid,0,1,1);

      } 
    }

    cout << fit << endl; 
    //Draw(sc,0,1,0,.4);  
    Draw((sa^Dlp(0,0,1)).dual(),0,0,0); 
    Draw((sb^Dlp(0,0,1)).dual(),0,0,0);    
    
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
