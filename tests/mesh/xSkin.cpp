/*
 * =====================================================================================
 *
 *       Filename:  xSkin.cpp
 *
 *    Description:  skin a transformation
 *
 *        Version:  1.0
 *        Created:  03/02/2015 18:02:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D_app.h"   
#include "vsr_map.h"
#include "vsr_graph.h"
#include "vsr_graph_draw.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;

  float wt, size,dist;

  //Two Spheres
  DualSphere sa,sb;

  //A GRAPH
  HEGraph<Point> tmp;
  vector<Point> pnts;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    gui(size,"size",-100,100);
    gui(dist,"dist",-100,100);

    amt=10; size=0;dist=0;
    
    sa = sphere(-3,0,0);
    sb = sphere(3,0,0);

    objectController.attach(&sa);
    objectController.attach(&sb);

    pnts = vector<Point>(20*10);
    tmp.UV(10,20,pnts,false,true);

  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    Pair par = sa ^ sb;

    auto dst = 1.0/(sa<=sb)[0]*dist;

    vector<Circle> tcir;

    for (int i=0;i<10;++i){
      float t = (float)i/10;
      auto ts = sa.bst(par.runit() * t * (amt+dst)/2.0 );
     // Draw(ts,0,1,0,.2);
      tcir.push_back( meet(ts, sphere(0,0,0) ) );//Dlp(1,0,0)) );
      Draw(tcir.back(),t,0,1-t,.5);
    }
    
    //skin 100 pnts per circle
    pnts = functor::eval<Point>(tcir, pointOnCircle, 20, TWOPI);

    generic::Draw(tmp);
    
    for (auto& i : pnts) Draw(i,1,0,0);

    Draw(sa,0,0,1,.5);
    Draw(sb,1,0,0,.5);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
