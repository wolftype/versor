/*
 * =====================================================================================
 *
 *       Filename:  xFlow.cpp
 *
 *    Description:  designing a flow using conformal rotors
 *
 *        Version:  1.0
 *        Created:  04/03/2015 16:15:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_field.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt, amtB = 0;
  bool bRealA, bRealB, bDrawTrace;


  Field<Point> field;

  Frame fa, fb;
  
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(amtB,"amtB",-100,100)(bReset,"bReset");
    gui(bRealA, "bRealA")(bRealB,"bRealB");
    gui(bDrawTrace,"bDrawTrace"); 
    field.resize(5,5,5);   

    fa.pos(-.5,0,0);
    fb.pos(.5,0,0);

    objectController.attach(&fa);
    objectController.attach(&fb);
  
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    Draw(field);

    auto ca = bRealA ? fa.cxy() : fa.icxy();
    auto cb = bRealB ? fb.cxz() : fb.icxz();

    Draw(ca,1,0,0);
    Draw(cb,1,1,0);

    Draw( ca.re(cb),0,0,1);
    Draw( cb.re(ca),0,0,1);

    auto ratio = Gen::ratio( ca, cb );
    auto log = Gen::log( ratio );

    cout << log[0].wt() << " " << log[1].wt() << endl;
    
    auto rot = Gen::con( log, amt );
  
    
    for (int i=0; i<field.num(); ++i){
      field[i] = Round::loc( field[i].spin(rot) ); 
    }

    
    if (bReset) field.reset();
    if (bDrawTrace){
     for (int i=0; i<field.num(); ++i){
       for (int j=0;j<20;++j){
        float t = (float)j/20.0;
        Draw( Round::loc( field[i].spin(Gen::con(log,t*amtB)) ),t,0,1-t );
       }
      }
    }
    
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
