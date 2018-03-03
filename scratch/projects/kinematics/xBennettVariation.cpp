/*
 * =====================================================================================
 *
 *       Filename:  xBennettVariation.cpp
 *
 *    Description:  a Bennett linkage with adjustable offset R
 *
 *        Version:  1.0
 *        Created:  04/21/2015 16:21:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

 
#include "vsr_app.h"   
#include "form/vsr_chain.h"
#include "draw/vsr_chain_draw.h"
#include "form/vsr_linkages.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float r_offsetA, r_offsetB, skew = 0;

  float theta;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(bReset,"bReset");
    gui(theta,"theta",-100,100)(r_offsetA,"r_offsetA",-100,100)(r_offsetB,"r_offsetB",-100,100)(skew,"skew",-100,100);


    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
      Bennett b(skew,3,3); // bennett skewed by amt skew with links length 1

      b(theta);
      //Draw( (Chain)bennett);
      Draw((Chain)b,true,false,0,1,0);
      DrawR((Chain)b,1,0,1);

     
     Chain c(4);

     for (int i =0;i<b.num();++i){
        c[i].pos() = b[i].pos().trs(b[i].z() * ( (i&1)?r_offsetA:r_offsetB ));
        c[i].rot() = b[i].rot();
     }

      c.base().pos() = c[0].pos();

      //Raw Calc from current positions and orientations
      c.calcLinks();
      c.calcJoints();
      
      c.joint(0) = b.joint(0);
      if(bReset) c.fk();     

     DrawR(c,0,1,0);

  }
  
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}


