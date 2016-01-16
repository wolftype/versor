/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_app.h
 *
 *    Description:  utility for cga demos which binds with gfx and glv
 *
 *        Version:  1.0
 *        Created:  01/26/2015 17:29:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  pretty awesome
 *
 * =====================================================================================
 */


#ifndef  vsr_cga3D_app_INC
#define  vsr_cga3D_app_INC

#include "space/vsr_cga3D_op.h"               //<-- conformal 3D types and basic operations

#include "draw/vsr_cga3D_draw.h"              //<-- fixed pipeline draw routines
#include "draw/vsr_cga3D_render.h"            //<-- programmable pipeline draw routines

#include "util/vsr_cga3D_control.h"           //<-- interface controls (mouse and keyboard)
#include "gfx/util/gfx_glv_app.h"             //<-- an app class with built-in gui

#include "gfx/gfx_postscript.h"

struct App : public gfx::GFXAppGui {  

  vsr::cga::Point mMouse2D;
  vsr::cga::Point mMouse3D;
  vsr::cga::Line mMouseRay;

  gfx::PostScript ps;
  
  bool bSetMouse;

  vsr::cga::Point calcMouse3D(float z=.99){

    auto& vd = gfx::GFXAppGui::mContext.interface.io.viewdata;
    auto tv = vd.ray;
    
    auto p = scene.unproject( io().pos(z) ); //vd.projectMid;

    cga::Vec tz (tv[0], tv[1], tv[2] );

    mMouse2D =  vsr::cga::Construct::point(p[0],p[1],0);
    mMouse3D =  vsr::cga::Construct::point(p[0],p[1],p[2]);
    mMouseRay = mMouse3D ^ tz ^ vsr::cga::Infinity(1); 

    //intersection of ray with plane
    mMouse3D = vsr::cga::Construct::meet( mMouseRay, vsr::cga::DualPlane(tz) );

    return mMouse3D;
 
  } 
  
  /// Called when a keyboard key is pressed
  virtual void onKeyDown(const gfx::Keyboard& k){
    
   // Frame f;
    switch(k.code){
      case 'v':
        printf("v\n"); 
        GL::enablePreset();
        scene.push(true);
         ps.print(*this);
        scene.pop(true);
        GL::disablePreset();
        break;

      case 's':
        bSetMouse = !bSetMouse;
        break;


    }

  } 
  

};

#endif   /* ----- #ifndef vsr_cga3D_app_INC  ----- */




 //     case  '0':
 //   
 //      f.pos( PT(0,0,5) );
 //      f.orient( Vec(0,0,0), false );

 //      scene.camera.set( f.pos(), f.quat() );
 //      break;

 //     case  '1':
 //      f.pos( PT(-5,0,0) );
 //      f.orient( Vec(0,0,0), false );

 //      scene.camera.set( f.pos(), f.quat() );
 //      
 //      break;
 //     case  '2':
 //      f.pos( PT(5,0,0) );
 //      f.orient( Vec(0,0,0), false );

 //      scene.camera.set( f.pos(), f.quat() );
 //      break;
 //     case  '3':
 //      f.pos( PT(0,5,0) );
 //      f.orient( Vec(0,0,0), false );

 //      scene.camera.set( f.pos(), f.quat() );
 //      break;
 //     case  '4':
 //      f.pos( PT(0,-5,0) );
 //      f.orient( Vec(0,0,0), false );

 //      scene.camera.set( f.pos(), f.quat() );
 //      break;
 //     case  '5':
 //      f.pos( PT(0,0,-5) );
 //      f.orient( Vec(0,0,0), false);

 //      scene.camera.set( f.pos(), f.quat() );
 //      break;
