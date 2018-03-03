/*
 * =====================================================================================
 *
 *       Filename:  xHypar.cpp
 *
 *    Description:  nested creases
 *
 *        Version:  1.0
 *        Created:  12/16/2013 12:57:37
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

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;
  float iter;
  float scale;
  float rotate;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(iter,"iter",1,100);
      gui(scale,"scale",-5,5);
      gui(rotate,"rotate",-10,10);
      scale = 1;
      iter= 2;
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

      //VECTOR OF CONSTRAINTS
      vector<Rigid3> rigid;


      //INITIAL SQUARE
      Field<Pnt> f(2,2,1);



      //SUBSEQUENT SQUARES
      Pnt tp[4];
      Pnt np[4];
      std::copy( f.dataPtr(), f.dataPtr() + 4, tp );

     /* for (int j = 0; j < 4; ++j){ */
     /*    tp[j].vprint(); */
    /* } */

      //SETUP CONSTRAINTS
      int it = iter;  cout << it << endl; 
      bool bSwitch = true;
      for (int i = 0; i < it; ++i ){
        double t = (double)i/it; //NEED HIGH PRECISION
        
        for (int j = 0; j < 4; ++j){
          np[j] = tp[j].trs( Vec( tp[j] - PAO ).unit() * scale ).rot( Biv::xy * PI * t * rotate);//Ro::loc( tp[j].dil(PAO, scale ) );
        }

        if (bSwitch){
         Rigid3 r0(np[0], tp[1], tp[0], tp[2], bSwitch);
         Rigid3 r3(np[3], tp[2], tp[3], tp[1], bSwitch);
         Rigid3 r1(np[1], np[3], tp[1], np[0], bSwitch);
         Rigid3 r2(np[2], np[0], tp[2], np[3], bSwitch);

         rigid.push_back( r0 ); 
         rigid.push_back( r1 );
         rigid.push_back( r2 );
         rigid.push_back( r3 );


       } else {
          Rigid3 r1(np[1], tp[3], tp[1], tp[0], bSwitch);
          Rigid3 r2(np[2], tp[0], tp[2], tp[3], bSwitch);
          Rigid3 r0(np[0], np[1], tp[0], np[2], bSwitch);
          Rigid3 r3(np[3], np[2], tp[3], np[1], bSwitch);

          rigid.push_back(r0);
          rigid.push_back(r1);
          rigid.push_back(r2);
          rigid.push_back(r3);
        }

        bSwitch = !bSwitch;
        std::copy( np, np + 4, tp );

      }

      //REALIZE CONSTRAINTS
      //
      
      Rigid2 ra(f[1], f[0], f[3]);
      Rigid2 rb(f[2], f[3], f[0]);

      f[1] = ra(amt);
      f[2] = rb(amt);

      Draw(ra.orbit());
      Draw(f);

      if (it > 0){
        rigid[0].update( f[1], f[0], f[2] );
        rigid[3].update( f[2], f[3], f[1] );
        Pnt tn0 = rigid[0]();
        Pnt tn3 = rigid[3]();
        rigid[1].update( tn3, f[1], tn0);
        rigid[2].update( tn0, f[2], tn3);
        Pnt tn1 = rigid[1]();
        Pnt tn2 = rigid[2]();
        Draw(tn0); Draw(tn1); Draw(tn2); Draw(tn3);
        //Draw(rigid[1].possible());
        tn1.vprint();
      }

      bSwitch = false;
      for (int i = 1; i < it; ++i){

        int b0 = (i-1)*4; int b1 = b0+1; int b2 = b0+2; int b3 = b0+3;  //<-- PREV
        int r0 = i*4; int r1 = r0+1; int r2 = r0+2; int r3 = r0+3;      //<-- CURRENT

          Pnt p0 = rigid[b0]();
          Pnt p1 = rigid[b1]();
          Pnt p2 = rigid[b2]();
          Pnt p3 = rigid[b3]();

          Pnt n0, n1, n2,  n3;

        if (bSwitch){
          
          rigid[r0].update( p1, p0, p2 );
          rigid[r3].update( p2, p3, p1 );
          n0 = rigid[r0]();
          n3 = rigid[r3]();
          rigid[r1].update( n3, p1, n0 );
          rigid[r2].update( n0, p2, n3 ); 
          n1 = rigid[r1]();
          n2 = rigid[r2]();

        }else{

          rigid[r1].update( p3, p1, p0 );
          rigid[r2].update( p0, p2, p3);
          n1 = rigid[r1]();
          n2 = rigid[r2]();
          rigid[r0].update( n1, p0, n2 );
          rigid[r3].update( n2, p3, n1 );
          n0 = rigid[r0]();
          n3 = rigid[r3]();

        }

        Draw(n0); Draw(n1); Draw(n2); Draw(n3);

        if (bSwitch){        
           glBegin(GL_TRIANGLES);
             TriNormal( n0, p1, p0, true  );
             TriNormal( n1, p1, n0, true );
             TriNormal( n1, n3, p1, true );
             TriNormal( n3, p3, p1, true );
             TriNormal( n3, p2, p3, true );
             TriNormal( n2, p2, n3, true );
             TriNormal( n2, p0, p2, true );
             TriNormal( n0, p0, n2, true );
          glEnd();
        } else{

        }

        
        bSwitch = !bSwitch;


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
