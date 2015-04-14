/*
 * =====================================================================================
 *
 *       Filename:  xReflectionSurfaces.cpp
 *
 *    Description:  reflections into surfaces
 *
 *        Version:  1.0
 *        Created:  12/03/2013 17:25:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_cga3D_app.h"   
#include "gfx/gfx_mesh.h"
#include "vsr_shapes.h"

using namespace vsr;
using namespace vsr::cga3D;
using namespace gfx;


struct MyApp : App {    
   
  Point mouse;
   
  float time;
  float amtA, amtB;

  float vmode;

  Mesh m;
  Mesh torus;

  bool bDrawSpheres, bDrawRef, bAvg, bUseDist;

  Frame frame,frame2;

  void setup(){

      gui(vmode,"mode",0,5);
      gui(amtA,"amtA",-100,100);
      gui(amtB,"amtB",-100,100);
      gui(bDrawSpheres, "Sphs")(bDrawRef,"refs")(bAvg,"Avg")(bUseDist,"dist");

      m = Mesh::Sphere();
      torus = Shape::Torus();

      frame2.pos(3,0,0);

      objectController.attach(&frame);
      objectController.attach(&frame2);
  }
  

  void onDraw(){ 
        

      mouse = calcMouse3D();

      auto pntcontrol = frame.pos();
      auto parcontrol = frame.py();
      auto ccontrol = frame.cxz();
      auto lcontrol = frame.ly();
      auto planecontrol = frame.dxy();
      
      int mode = vmode;
      switch (mode){
        case 0:Draw(pntcontrol,1,1,0); break;
        case 1:Draw(parcontrol, 1,1,0); break;
        case 2:Draw(ccontrol,1,1,0); break;
        case 3:Draw(lcontrol,1,1,0); break;
        case 4:Draw(planecontrol,1,1,0); break;
      }
      

      auto control2 = frame2.pz();//Ro::sur( cxz ), Vec::y);//mouse;
      Draw(control2, 1,0,1);


     m.drawElements();

     // auto nc = (control);
      int ptr = 0;//m.vertices();
      for (auto i : m.original() ){

        //distance to the line
       auto pnt = Ro::point( i.Pos[0], i.Pos[1], i.Pos[2] );
       VT dist;
      
       switch (mode){
        case 0: 
          dist = 1.0 / (1+ Ro::sqd(pnt, pntcontrol) ) ; break;
        case 1: 
          dist =  1.0  / ( fabs( ( pnt <= ( parcontrol ^ Inf(1) ).dual() ).wt() ) + 1.0 ) ;
          break;
        case 2: 
          dist = 1.0 / ( 1.0 + ( fabs( ( pnt <= ( ccontrol.dual() ) ).wt() ) ) ); break;
        case 3: 
          dist = 1.0 / ( fabs( ( pnt <= ( lcontrol.dual() ) ).wt() ) + 1.0 ); break;
        case 4:
          dist = 1.0 / ( 1.0 + fabs(( pnt <= planecontrol ).wt() ) ); break;
      }


        auto dlsA = Ro::dls( amtA * ( bUseDist ? dist : 1 ), i.Pos[0], i.Pos[1], i.Pos[2] );
        auto dlsB = Ro::dls( amtB, i.Pos[0], i.Pos[1], i.Pos[2] );

        Pnt r1;
        switch (mode){
          case 0: 
            r1 = Ro::loc( pntcontrol.reflect( dlsA) ) ;  
           // r2 = Ro::loc( pntcontrol2.reflect( dlsB) ) ;  
            break;
          case 1: 
            r1 = Ro::loc( parcontrol.reflect( dlsA) );  ;
            break;
          case 2: 
             r1 = Ro::loc( ccontrol.reflect( dlsA) );  break;
          case 3: 
            r1 = Ro::loc( Cir(lcontrol).reflect( dlsA) );  break;
          case 4: 
            r1 = Ro::loc( Dls(planecontrol).reflect( dlsA) );  break;

      }
       
        auto r2 = Ro::loc( control2.reflect( dlsB ) );
        
        if (bDrawRef) { Draw(r1); if (bAvg) Draw(r2); }
        if (bDrawSpheres) Draw(dlsA,0,1,0,.2);
        
        auto t1 = ( r1 );
        auto t2 = ( r2 );
        auto t = bAvg ? (t1 + t2)/2.0 : t1;

        //inverse?
        auto inv = Ro::dls( pntcontrolt, -amtA );
        Draw( Ro::loc ( r1.reflect( inv ) ),0,1,0 );

        m[ptr].Pos = Vec3f( t );        
        ptr++;

       }
      
  }
   

};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
