/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_simple_app.h
 *
 *    Description:  app without gl2ps
 *
 *        Version:  1.0
 *        Created:  05/18/2015 17:30:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
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

//GL2PS
//#include "gl2ps/gl2ps.h"


struct App : public gfx::GFXAppGui {  
 
  App(int w =1400, int h=700, string name="cga3D") : gfx::GFXAppGui(w,h,name) {}
  
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
  
  /// Called when a keyboard key is pressed
  virtual void onKeyDown(const gfx::Keyboard& k){
  } 


//   void gl2ps(){
//      static int id = 0;
//      stringstream os; os << "output_" << id << ".pdf";
//      id++;
//      
//      FILE *fp;
//      int state = GL2PS_OVERFLOW, buffsize = 0;
//      
//      string name = os.str();
//      fp = fopen(name.c_str(), "wb");
//      
//      printf("writing %s to %s\n", os.str().c_str(), name.c_str() );
//      GLint tv[4];
//      glGetIntegerv(GL_VIEWPORT, tv);
//      
//      
//      
//      //gl2psEnable( GL2PS_POLYGON_OFFSET_FILL );
//      while(state == GL2PS_OVERFLOW){
//       //   gl2psEnable(GL2PS_BLEND);
//       //   gl2psEnable(GL2PS_LINE_STIPPLE);
//       //   gl2psBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        
//          buffsize += 1024*1024;
//          gl2psBeginPage("test", "gl2psTestSimple", tv , GL2PS_PDF, GL2PS_NO_SORT,//SIMPLE_SORT,
//                         GL2PS_BEST_ROOT | GL2PS_TIGHT_BOUNDING_BOX | GL2PS_SIMPLE_LINE_OFFSET | GL2PS_OCCLUSION_CULL, // GL2PS_NO_PS3_SHADING || GL2PS_TIGHT_BOUNDING_BOX  
//                         GL_RGBA, 0, NULL, 0, 0, 0, buffsize, fp, "out.eps");
//
//          gl2psPointSize(5);
//          gl2psLineWidth(1);
//          
//    //      gl2psEnable(GL2PS_LINE_STIPPLE);
//          gl2psEnable(GL2PS_BLEND);
//          gl2psBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//
//          
//          //DRAW 
//          onDraw();
//          
//          state = gl2psEndPage();
//      }
//      
//      fclose(fp);
//      printf("Done!\n");
//  }
  

};

#endif   /* ----- #ifndef vsr_cga3D_app_INC  ----- */
