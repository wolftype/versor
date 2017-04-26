/*
 * =====================================================================================
 *
 *       Filename:  xPoints3D.cpp
 *
 *    Description:  3d Point groups
 *
 *    Note: experiment with compare function in vsr_root.h . . .
 *
 *        Version:  1.0
 *        Created:  03/11/2014 15:32:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *
 * =====================================================================================
 */

#include "vsr_app.h"
#include "form/vsr_root.h"
#include "form/vsr_group.h"
#include "form/vsr_hull.h"

#include "draw/vsr_generic_draw.h"
#include "draw/vsr_graph_draw.h"

using namespace vsr;

struct MyApp : App {    

  float time;
  float p,q;

  float amt;

  bool bDrawHull;

  // 3d convex hull
  ConvexHull<3> hull;

  void setup(){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
      
    bindGLV();

    gui(amt,"amt",0,100);
    gui(p,"p",0,10)(q,"q",0,10);
    gui(bDrawHull,"drawHull");
    p =4; q = 3;
  }
  
  virtual void onDraw(){ 

      auto a = NEVec<3>::y;//.rot(NEBiv<3>::xy * PIOVERFOUR/2.0) ;
      auto c = NEVec<3>::x;//.rot(NEBiv<3>::xy * PIOVERFOUR/2.0) ;
      
      double tb = PIOVERTWO;
      double ta = PI/(int)p;
      double tc = PI/(int)q;

      //the good old spherical trig cosine rule
      double ca = cos(ta);
      double sa = sin(ta);
      double cb = cos(tb);
      double sb = sin(tb);
      double cc = cos(tc);
      double sc = sin(tc);

      double tA = acos( (ca-(cb*cc))/(sb*sc) );
      double tB = acos( (cb-(ca*cc))/(sa*sc) );
      double tC = acos( (cc-(ca*cb))/(sa*sb) );

      auto bivA = (a ^ c).rot( a.dual() * tA / 2.0 );
      auto bivC = (a ^ c).rot( c.dual() * -tC / 2.0 );

      //Here we find b via coincidence of planes rotated by spherical trig amts
      auto b = (bivA.dual() ^ bivC.dual()).dual().unit();

      //Generate Root System (reflection groups)
      auto rs = Root::System(a, b, c);
     // auto gs = SimpleGroup<NEVec<3>>(ts);
     // auto rs = gs( NEVec<3>::x );
      
      //DRAW
     // Draw(a,1,0,0); Draw(b,1,0,0); Draw(c,1,0,0);      
      for (int i=0;i<rs.size();++i){
        //rs[i].print();
        generic::Draw( rs[i],0,1,0 );
      }

      hull.initialFace(rs);
      hull.altConvexPass(rs);
      hull.closeHoles(rs,amt);
      if (bDrawHull) generic::Draw(hull.graph);

  }
   
};


int main(){
                             
  MyApp app;
  app.start();
  return 0;

}
