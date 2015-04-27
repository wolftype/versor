/*
 * =====================================================================================
 *
 *       Filename:  xFieldSphere.cpp
 *
 *    Description:  field of spheres, trilinearly interpolated pair generator
 *
 *        Version:  1.0
 *        Created:  04/03/2015 18:25:12
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

#include "gfx_map.h"

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {
 
  //Some Variables
  bool bReset,bSimple = false;
  float amt = 0;

  Field<Pair> field;
  Field<Point> fpoint;
  Field<Point> mesh;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    gui(bSimple,"bSimple");
    
    field.resize(2,2,2,2,2,2);
    fpoint.resize(2,2,2,2,2,2);

    mesh.resize(10,10,10,.2,.2,.2);
  
    for (int i=0;i<fpoint.num();++i){
      fpoint[i] = Ro::dls( fpoint.grid(i), .3);//Ro::dls( point[i], .3);
      objectController.attach(&fpoint[i]);
    }
  }

  void onAnimate(){
    for (int i =0;i<fpoint.num();++i){
      field[i] = Gen::log( Gen::ratio( Ro::dls(1,0,0,0), fpoint[i] ) );
    }
    
    auto vals = data::map<Pair>(10,10,10,[&](float x,float y,float z){
      return field.vol(x,y,z);
    });

    for (int i =0;i<mesh.num();++i){
      auto bst = Gen::bst(vals[i]*-amt);
      auto con = Gen::con( Gen::split(vals[i]), amt );
      auto pntA = Ro::loc( mesh.grid(i).spin( bst ) );
      auto pntB = Ro::loc( mesh.grid(i).spin( con ) );
      mesh[i] = bSimple ? pntA : pntB;
    }

  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    Draw(fpoint,1,0,0);
    //for (int i=0;i<point.num();++i){ Draw( point[i], 1,0,0); }

    Draw(mesh);


  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
