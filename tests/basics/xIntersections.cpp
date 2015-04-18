//Get mouse position as an "unprojected" point in 3D space

#include "vsr_cga3D.h"   
#include "vsr_cga3D_funcs.h"
#include "vsr_GLVimpl.h"

using namespace vsr;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
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
     
      /*  Given Two Lines, find Intersection 
          can either find if origin intersects with ori ^ line (= 0 if it does)
          or use bisector method (see Covariant Approach to Geometry, lasenby et al)
          
      */

      /* Line da = Ro::point(0,0,0) ^ Ro::point(1,1,0) ^ Inf(1); */
      /* Line db = Ro::point(0,1,0) ^ Ro::point(.2,.3,0) ^ Inf(1); */

      /* Draw(da,1,0,0); Draw(db,0,1,0); */

      /* da = da.runit(); db = db.runit(); */

      /* Line dc = da.reflect(db); */
   
      /* Draw(dc,0,0,1); */
      /* //Find Bisector through subtraction */

      /* Line dd = da - dc.runit(); */
      /* Draw(dd.runit()); */
      /* dd.vprint(); */

      //What is Rotor of two lines?

      //auto da = Fl::line(0,1,1).translate(2,0,3); //Line in Y direction
      //auto db = Fl::line(1,1,0).translate(0,-5,-5); //Line in X Direction

      auto da = Round::circle(0,1,1);//Line in Y direction
      auto db = Round::circle(1,1,0).translate(0,-1,-0); //Line in X Direction


      da = da.runit(); db = db.runit();
      Draw(da,0,1,0); Draw(db,0,0,1);
      
      (da * db ).vprint();
      (da / db ).vprint();
       
      auto dc = da.reflect(db);
      Draw(dc,1,1,0); 
      
      /* auto dd = da - dc.runit(); */
      /* Draw(dd); */
      
      static Pair a = Ro::point(5,0,0) ^ Ro::point(-5,0,0);
      Draw(a,1,0,0);
      Touch(interface,a);
      
      Point nmp =  Ro::null(mouse[0],mouse[1],0) ;

      //if collinear = 0
      auto t = nmp ^ a ^ Inf(1);
      printf("collinearity: %f\n",t.wt() );
      //if inside < 0
      auto p = nmp <= Round::sur( a );
      p.vprint();

      if ( hit(nmp,a) ) Draw( Ro::sphere(nmp,.2),0,1,1 );
  }
   

  
};


MyApp * app;


int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(500,500,"Versor",&glv);    
  app = new MyApp( win ); 
  
  glv << *app;

  Application::run();

  return 0;

}
