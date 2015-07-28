/*
 * =====================================================================================
 *
 *       Filename:  xConic2.cpp
 *
 *    Description:  conic transforms
 *
 *        Version:  1.0
 *        Created:  01/09/2014 13:06:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */



#include "vsr_app.h"   
#include "form/vsr_cga3D_conic.h"
#include "util/vsr_map.h"

#include "gfx/gfx_mesh.h"

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {    
   
  Pnt mouse;
  DualPlane mousePlane;
  DualPlane imousePlane; /// inverse transform of plane
  Circle circleMeet;     /// meet of inverse transform and sphere

  float time;
  float amt,beta,gamma;

  vector<Point> vp;
  vector<Point> cp;
  vector<Point> np;

  virtual void setup(){
      bindGLV();

      gui(amt,"amt",-100,100);
      gui(beta,"beta",-1,1);
      gui(gamma,"gamma",-1,1);
  }
 
  virtual void onAnimate(){
    
    Vec dir = Vec(1,0,0);
     
    vp.clear();

    //a bunch of points on the surface of a sphere
    vp = pointsOnSphere( sphere(0,0,0), 20, 20);
    //conically transformed points
    cp = functor::fmap<Point>( conic::transform::point, vp, dir, amt);

    //dualPlane through mouse with normal in x direction
    mousePlane = mouse <= Drv(1,0,0);
    //inverse transform of plane
    imousePlane = Conic::ITransform( mousePlane, dir, amt );

    //meet of inversely transformed plane and preimage of paraboloid
    circleMeet = (imousePlane ^ sphere(0,0,0) ).dual();
    //extract points on the circle from the meet
    np = pointsOnCircle(circleMeet, 100);
    np = functor::fmap<Point>( conic::transform::point,  np, dir,  amt );
    

  }

  virtual void onDraw(){ 
        
     mouse = calcMouse3D();

    // Draw( Vec::x.sp(  Gen::rot( PI * beta, PIOVERFOUR * gamma )), 0,1,0 ); 

    auto p = Flat::loc( mousePlane, Ori(1), true );
    p.print();
    // Draw( ,1,0,0); 
    
     Mesh mesh = Mesh::UV( cp.data(), 20, 20 );
     mesh.drawElements();

     Draw(mousePlane,0,0,1);
     
    // Draw(imousePlane);
     Draw(circleMeet,0,1,0);

     for (auto& i : np) Draw(i,1,0,0);



        //  Draw( Conic::ITransform( Fl::loc( td, mouse, true).null(), mv, amt ), 0,1,1);
         // Draw( Conic::ITransform( Vec::y.null(), mv, amt ), 0,1,1);
        /* Line la = LN(0,1,0).trs( sin(time) * 1, 0,0); */
        /* Draw(la); */

        /* Line ila = Conic::ITransform( la, mv, amt ); */

        /* auto pp = ( ila.dual() ^ dls).dual();//Ro::sur(cir) ).dual(); */
        /* auto p2 = Ro::split( pp ); */
        /* Pnt pa = Conic::Transform (p2[0], mv, amt); */
        /* Pnt pb = Conic::Transform(p2[1], mv, amt); */
         
        /* Draw( pa ); Draw(pb); */ 
    
  }
   

  
};




int main(){
                             
    MyApp app;
    app.start();
  return 0;

}
