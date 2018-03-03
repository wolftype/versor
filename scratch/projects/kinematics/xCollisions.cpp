/*
 * =====================================================================================
 *
 *       Filename:  xCollisions.cpp
 *
 *    Description:  collisions of rods in CGA
 *
 *        Version:  1.0
 *        Created:  10/10/2014 13:06:06
 *       Revision:  none
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



struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amtA, amtB;

  bool bReset;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amtA,"amtA",-100,100);
      gui(amtB,"amtB",-100,100);
      gui(bReset,"reset");
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

      static Frame fa(-1,0,0);  static Frame fb(1,0,0);

      Touch(interface,fa); Touch(interface,fb);

      auto pa = fa.px();
      auto pb = fb.py();

      auto lb = fb.ly();
      Draw(lb);

      Draw(pa,1,0,0); Draw(pb,0,1,0);

      Draw( LN(1,0,0).trs(0,.5,0) );
      Draw( LN(1,0,0).trs(0,1,0) );
      Draw( LN(0,1,0).trs(1,0,.5) );
      Draw( LN(0,1,0).trs(1,0,1) );

      auto sigma = Ro::sur(pa);
      auto carrier = (Ro::car(pa).unit());

      auto x = Vec(mouse[0], mouse[1],0).null();
      auto dx = mouse <= sigma;
      if (dx[0] <= 0) {
        //Draw(  );
        auto ca = x ^ carrier;
       // cout << ca.rnorm() << endl;
      }
  
      //Line
      //Intersects boundary if squared radius >= 0

     // cout << sqrad << endl;

      //Coplanarity: Are the lines within cylinder's radius?
      auto lineDist = lb.dual() ^ carrier.dual();
      if ( fabs(lineDist[0]) <= amtA ){
          //does line go through surround?
          auto sqrad = Ro::size( (lb.dual() ^ sigma), true );                  
          if (sqrad >= 0){
            //are lines NOT parallel (that's a special intersection case if they are)?
            auto parallel = lb <= carrier;
            if (fabs(parallel[0]) != 1){
              //point on line closest to cylinders axis 
              auto pm = meet(carrier,lb);
              Draw(pm,1,0,0);
              //point on cylinder's axis closest to line
              auto pc = Fl::loc(carrier, pm, false); // "false" means carrier is not dual;
              //normalize flat point
              pc /= pc[3]; pc = pc.null();
              Draw(pc,0,1,0);
              //sphere at pc with radius . . .
              auto s = Ro::dls_pnt(pc,amtA);
              //intesection of line with s . . .(doesn't get intersection with cyl . . .)
              auto pp = (lb.dual() ^ s).dual();
              Draw(pp, 0,0,1);
            } else { 
              //some special case for parallel lines...  
            }
        }
      }
      /* //Twist lb through carrier */
      /* auto lbr = lb.reflect(carrier); */
      /* //Bisect reflection and lb (will be orthogonal to carrier) */
      /* auto l2 =(lb-lbr.unit()).unit(); */
      /* //Ok draw that */
      /* Draw(lbr,0,0,1); */
      /* Draw(l2,0,1,0); */
      /* //Find a point on l2 near origin */
      /* Point pori = Fl::loc(l2, Ori(1), false); */
      /* pori = (pori / pori[3]).null(); */
      /* Draw(pori,1,1,0); */
      /* //Reflect that point over carrier */ 
      /* Point tp = pori.re( carrier ); */
      /* Draw(tp,1,1,0); */
      /* //Find Midpoint . . . */ 
      /* auto fp = ( ( (tp / tp[3]) + pori)/2.0 ).null(); */ 



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
