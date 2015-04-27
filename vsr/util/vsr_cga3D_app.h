/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_app.h
 *
 *    Description:  utility for demos which binds with gfx and glv
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

#include "space/vsr_cga3D_op.h"               //<-- conformal 3D types and basic
#include "space/vsr_cga3D_funcs.h"               //<-- conformal 3D types and helper functions

#include "draw/vsr_cga3D_draw.h"              //<-- fixed pipeline draw routines
#include "draw/vsr_cga3D_render.h"            //<-- programmable pipeline draw routines

#include "util/vsr_cga3D_control.h"      //<-- interface controls (mouse and keyboard)
#include "gfx/util/gfx_glv_app.h"             //<-- an app class with built-in gui


struct App : public gfx::GFXAppGui {  
 
  App(int w =1920, int h=1080, string name="cga3D") : gfx::GFXAppGui(w,h,name) {}
  
  vsr::cga::Point mMouse2D;
  vsr::cga::Point mMouse3D;
  vsr::cga::Line mMouseRay;

  vsr::cga::Point calcMouse3D(float z=.99){

    auto& vd = gfx::GFXAppGui::mContext.interface.io.viewdata;
    auto tv = vd.ray;
    
    auto p = scene.unproject( io().pos(z) ); //vd.projectMid;

    Vec tz (tv[0], tv[1], tv[2] );

    mMouse2D =  vsr::cga::point(p[0],p[1],0);
    mMouse3D =  vsr::cga::point(p[0],p[1],p[2]);
    mMouseRay = mMouse3D ^ tz ^ vsr::cga::Infinity(1); 

    //intersection of ray with plane
    mMouse3D = vsr::cga::meet( mMouseRay, vsr::cga::DualPlane(tz) );

    return mMouse3D;
 
  }    

};

#endif   /* ----- #ifndef vsr_cga3D_app_INC  ----- */
