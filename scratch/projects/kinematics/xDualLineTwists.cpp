/*
 * =====================================================================================
 *
 *       Filename:  xDualLineTwists.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/24/2015 16:25:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */



#include "vsr_app.h"  
#include <vsr/form/vsr_twist.h> 

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  bool bMoment = false;
  bool bPeriod = false;
  float xdist,period, pitch = 0;

  Point point;
  bool bSetMouse,bGeneralLog;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(xdist,"xdist",-10,10);
    gui(period,"period",-TWOPI,TWOPI)(pitch,"pitch",-10,10)(bReset,"bReset")(bGeneralLog,"bGeneralLog");
    gui(bMoment,"bMoment");
    gui(bPeriod,"bPeriod");
    
    xdist = -2;
    period= PIOVERTWO;
    pitch = 1;
  }


  void onKeyDown(const gfx::Keyboard& k){
    
    switch (k.code){
      case 's':
        bSetMouse = !bSetMouse;
    }

    App::onKeyDown(k);
   }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    
    if (bSetMouse) point = calcMouse3D();

    Draw( Construct::sphere(point,.05), 0,0,1);

    auto dllA = DLN(1,1,0).runit().translate(xdist,0,0);

    Draw( dllA, .6, 0, 0);
    
    auto p = Flat::location(dllA, PAO);

    DrawBAt( Biv(dllA), p, 0, .6, 0 );
    DrawAt( Biv(dllA), p, 0, .6, 0 );
    DrawAt( Drv(dllA), p, 0, .6, 0 );

    DrawAt( Biv(dllA).duale(), p, .6,0,0);

    auto db = Ori(-1) <= Drv(dllA);
  //  db.print();
 //   Drv(dllA).print();
    Draw( Drv(dllA).copy<Vec>() <= Biv(dllA), 0, .6, .6);


   // Draw( Construct::sphere(0,0,0,.1),0,0,0);

  
    auto dll = Twist::Along(dllA, period,pitch);//, 1 );
 
  
    auto  inf2 = Inf(1) * Inf(1);    
 
    if (bReset){

     DrawAt( Drv(dll) - Drv(dllA*period), Drv(dllA*period).copy<Vec>() + p, .6, 0, .6); 

     if (bPeriod){
     DrawAt( Drv(dllA*period),p,0,.2,.6);
     DrawBAt( Biv(dll), p, .2, 0, .6 );
     DrawAt( Biv(dll), p, .2, 0, .6 );
     DrawAt( Biv(dll).duale(), p, .6, 0, .6 );
     }
    
     DrawAt( Drv(dll), p, 0, 0, .6 );


     glBegin(GL_LINE_STRIP);
     glColor3f(0,.2,.6);
     for (int i =0;i<100;i++){
      float t = 4 * (float)i/100;
      auto mot1 = Gen::mot(-dll*t);
      auto mot2 = Gen::con( Gen::split(Pair(-dll)),t);
      if (bGeneralLog)  GL::vertex( Round::loc( point.spin( mot2 ) ).begin() );
      else GL::vertex( point.spin(mot1).begin() );//,.2,.6,.6 ); 
     }
     glEnd();
    } else{


     glBegin(GL_LINE_STRIP);
     glColor3f(0,.2,.6);
     for (int i =0;i<100;i++){
      float t = 4 * (float)i/100;
      auto mot = Gen::mot(-dllA*t);
      GL::vertex( point.spin(mot).begin() );
     }
     glEnd();



    }
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
