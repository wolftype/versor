/*
 * =====================================================================================
 *
 *       Filename:  xSphericalTrig.cpp
 *
 *    Description:  for making reflection groups
 *
 *        Version:  1.0
 *        Created:  03/17/2015 13:08:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  bool bShaded = false;
  float amt = 0;
  float p,q;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset")(bShaded,"bShaded");
    gui(p,"p",0,10)(q,"q",0,10);

    mColor.set(1,1,1);
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

      bShadedOutput = bShaded;

      //0. a and c are at 90 degrees, must find b...
      auto a = Vec::x;
      auto c = Vec::y;
      
      auto xy = Biv::xy;

      // employ the spherical trig cosine rule ...
      // cos a = cos b cos c + sin b sin c cos A
      // A = acos( ( cos a - cos b cos c ) / sin b sin c
      //
      // so if ta, tb, and tc represent the angles between vectors on a sphere
      // find amt to rotate planes and then intersect them ...
      double tb = PIOVERTWO;
      double ta = PI/(int)p;
      double tc = -PI/(int)q;

      double ca = cos(ta);
      double sa = sin(ta);
      double cb = cos(tb);
      double sb = sin(tb);
      double cc = cos(tc);
      double sc = sin(tc);


      //rotate yz and xz planes by polar angles
      double tA = acos( (ca-(cb*cc))/(sb*sc) );
      double tC = acos( (cc-(ca*cb))/(sa*sb) );

      //2. ... to rotate the yx plane ...
      auto bivA = xy.rot( a.duale() * tC / 2.0 );
      auto bivC = xy.rot( c.duale() * tA / 2.0 );

      //3. ... and find b via coincidence of planes ...
      auto b = -(bivA.duale() ^ bivC.duale()).duale().unit();  
      

      if (bReset){
       DrawB(bivA,1,1,0,1);//.5);
       Draw(b,0,1,0);  
       DrawB(bivC,0,1,1,1);//.5);
       Draw(c,0,0,1);  
       DrawB(Biv::xy,1,0,1,1);//.5);
       Draw(a,1,0,0);  

      } else{

       Draw(Biv::xy,.2,.2,.2);
       Draw(bivA,1,1,0);
       Draw(bivC,0,1,1);
      }
      
      Draw(a,1,0,0);  
      Draw(b,0,1,0);  
      Draw(c,0,0,1);  

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
