/*
 * =====================================================================================
 *
 *       Filename:  xPairInterpolation.cpp
 *
 *    Description:  interpolate generators
 *
 *        Version:  1.0
 *        Created:  03/23/2015 17:35:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_cga3D_app.h"   
#include "vsr_cga3D_frame.h"
#include "vsr_field.h"

//#include "vsr_cga3D_log.h" //will be replaced

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;


  //frames
  Frame fa,fb;
  Field<Point> field;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    

    fa.pos(-2,0,0);
    fb.pos(2,0,0);
    
    field.resize(20,20,1,.5);
    field.basicInit();

    for (int i = 0;i<field.num();++i){
      //field.grid(i).print();
    }

  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    Draw(fa.tz(), 0,1,0);
    Draw(fb.tz(),0,1,0);

    Draw(field);

    auto log = Gen::log( Gen::ratio( fa.tz(), fb.tz() ) );

    log[0].print(); 
    cout << " b " << endl;
    log[1].print();

    for (int i=0;i<10;++i){
      float t = (float)i/10;
      
      auto xf = Gen::bst( log[0] * t );//Gen::con( log, t );
      Draw( fa.cxz().spin(xf),0,1,0);
      //fa.tz() * t + fb.tz * (1-t);

    }

    
//    for (int j=0;j<field.num();++j){
//      float distA = 1.0 / (.01 + Ro::sqdist(field[j], fa.pos() ) ); 
//      float distB = 1.0 / (.01 + Ro::sqdist(field[j], fb.pos() ) ); 
//      auto gen = (fa.tz()*distA + fb.tz()*distB) * t; 
//      auto splt = split(gen);
//      auto bst = Gen::bst(splt[0]) * Gen::bst(splt[1]) ;
//
//      Draw( Ro::loc( field[j].spin(bst) ), t,0,1-t);
//    }


  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
