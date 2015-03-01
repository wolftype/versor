/*
 * =====================================================================================
 *
 *       Filename:  vsr_examples.h
 *
 *    Description:  utility for demos which binds with gfx and glv
 *
 *        Version:  1.0
 *        Created:  01/26/2015 17:29:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  vsr_cga3D_app_INC
#define  vsr_cga3D_app_INC

#include "vsr_cga3D.h"
#include "gfx/util/gfx_glv_app.h"

using namespace vsr;
using namespace vsr::cga3D;

struct App : public gfx::GFXAppGui {  
 
  App(int w =800, int h=400, string name="cga3D") : gfx::GFXAppGui(w,h,name) {}
  
  Point mMouse2D;
  Point mMouse3D;
  Line mMouseRay;

  Point calcMouse3D(float z=.99){

    auto& vd = gfx::GFXAppGui::mContext.interface.io.viewdata;
    auto tv = vd.ray;
    
    auto p = scene.unproject( io().pos(z) ); //vd.projectMid;

    Vec tz (tv[0], tv[1], tv[2] );

    mMouse2D =  Round::point(p[0],p[1],0);
    mMouse3D =  Round::point(p[0],p[1],p[2]);
    mMouseRay = mMouse3D ^ tz ^ Inf(1); 

    //intersection of mouse with plane through origin
    mMouse3D = meet( mMouseRay, Dlp(tz) );

    return mMouse3D;
 
  }    

};

#endif   /* ----- #ifndef vsr_cga3D_app_INC  ----- */
