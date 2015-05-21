/*
 * =====================================================================================
 *
 *       Filename:  xRoots.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/20/2015 14:10:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_root.h"
#include "form/vsr_group.h"


using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  Point mouse;
  //Some Variables
  bool bReset = false;
  float amt = 0;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

      mouse = calcMouse3D();

//      auto a = Vec::x;
//      auto c = Vec::y;
//
//      int p = 5; int q =3;
//      
//      //1. employ the good old spherical trig cosine rule ...
//      double tb = PIOVERTWO;
//      double ta = PI/(int)p;
//      double tc = PI/(int)q;
//
//      double ca = cos(ta);
//      double sa = sin(ta);
//      double cb = cos(tb);
//      double sb = sin(tb);
//      double cc = cos(tc);
//      double sc = sin(tc);
//
//      double tA = acos( (ca-(cb*cc))/(sb*sc) );
//      double tC = acos( (cc-(ca*cb))/(sa*sb) );
//
//      //2. ... to rotate the yx plane ...
//      auto bivA = (a ^ c).rot( a.duale() * tC / 2.0 );
//      auto bivC = (a ^ c).rot( c.duale() * tA / 2.0 );
//
//      //3. ... and find b via coincidence of planes ...
//      auto b = (bivA.duale() ^ bivC.duale()).duale().unit();//    auto h = Root::System(
//
//      Draw(a,1,0,0);
//      Draw(b,1,1,0);
//      Draw(c,1,0,1);
//
//      auto system = Root::System(a,b,c);
//
//      Group<Vec> g; g.ops = system;
//
//      for (auto & i : system){
//          Draw(i);
//      }

//      auto res = g( mouse );
      PointGroup3D<Vec> pg(5,3);

      for (auto& i : pg.ops){ Draw(i); }

      vector<Point> res; res.push_back(mouse);
      
      int tn = res.size();
      
      for (auto& i : pg.ops){
       for (int j=0;j<tn;++j){
         auto ts = res[j];
         ts = ts.reflect( i.unit() );
         res.push_back(ts);
       }
      }

      for (auto & i : res){
          Draw(i);
      }

  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
