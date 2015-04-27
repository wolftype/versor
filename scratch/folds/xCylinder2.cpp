/*
 * =====================================================================================
 *
 *       Filename:  xCylinder2.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/20/2013 12:55:46
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
#include "vsr_fold.h"
#include "vsr_set.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float amt;
  float dx,dy,decay,num,spacing,numK;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 

  }
  
  void initGui(){

    gui(dx,"dx")(dy,"dy")(decay,"decay",-10,10)(amt,"amt")(num,"num",1,100)(spacing,"spacing",1,100);
    gui(numK,"numK",0,10);
    dx = .2; dy = 0; amt = .1; num = 2;
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      gfx::GL::twoSidedLighting();
      scene.light = Vec3f(5,1,.3);

      int numSections = num;
      int numPoints = ( num * 2 );
      Field<Pnt> f ( numPoints, numPoints, 1);

      //vector<Truss> truss;
      Set<Rigid3> rigid;

      //BUILD PATTERN
      double ts = spacing * num * 2;
      bool bSwitch = true;
      for (int i = 0; i < num; ++i){
        double u = -ts/2.0 + ts * (double)i/num;
        double tu =  -1.0 + 2*(double)i/num;
        bSwitch = !bSwitch;
        for (int j = 0; j < num; ++j){
          double v = -ts/2.0 + ts * (double)j/num;
          double tv = -1.0 + 2* (double)j/num;
          double ttu = tu;
          for (int k = 0; k < (int)numK; ++k){
            ttu *= tu;
          }
          double ddx = dx;// (bSwitch) ? dx : dx * decay ;

          Pnt a = PT( u, v, 0 );
          Pnt b = PT( u + ddx, v + spacing + dy , 0 );
          Pnt c = PT( u + spacing + ddx, v + spacing + dy, 0);
          Pnt d = PT( u + spacing, v, 0 );
          f.at(i*2, j*2 ) = a;
          f.at(i*2, j*2+1) = b;
          f.at(i*2+1, j*2+1) = c;
          f.at(i*2+1, j*2) = d; 

          /* if( j == num-1) { */
          /*   f.at(i*2, j*2+3) = PT( u, v + 2*spacing,0); */
          /*   f.at(i*2+1, j*2+3) = PT( u + spacing, v+ 2*spacing,0); */
          /* } */
        }
      }

      Draw(f);

      for (int i = 1; i < num-1; ++i){
        for (int j = 1; j < num; ++j){
          Rigid3 bl( f.at(i*2, j*2), f.at(i*2+1, j*2-1), f.at(i*2, j*2-1), f.at(i*2-1, j*2) ); //Bottom Left
          Rigid3 tl( f.at(i*2, j*2+1), f.at(i*2, j*2), f.at(i*2-1, j*2), f.at(i*2-1, j*2+1) ); //Top Left
          Rigid3 br( f.at(i*2+1, j*2), f.at(i*2+2, j*2-1), f.at(i*2+1, j*2-1), f.at(i*2,j*2) ); //Bottom Right
          Rigid3 tr( f.at(i*2+1,j*2+1), f.at(i*2+1,j*2), f.at(i*2,j*2), f.at(i*2,j*2+1) );

          rigid.add(bl); rigid.add(tl); rigid.add(br); rigid.add(tr);    
        }
      }

      int it = 0;
      for (int i = 1; i < num-1; ++i){
        for (int j = 1; j < num; ++j){
          rigid[it]
          rigid[it+1]
          rigid[it+2]
          rigid[it+3]
          
          it++;
        }
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
