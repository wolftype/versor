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

#include "space/vsr_cga3D_op.h"               //<-- conformal 3D types and basic

#include "draw/vsr_cga3D_draw.h"              //<-- fixed pipeline draw routines
#include "draw/vsr_cga3D_render.h"            //<-- programmable pipeline draw routines

#include "util/vsr_cga3D_control.h"           //<-- interface controls (mouse and keyboard)
#include "gfx/util/gfx_glv_app.h"             //<-- an app class with built-in gui

//GL2PS
#include "gl2ps/gl2ps.h"


struct App : public gfx::GFXAppGui {  

  vsr::cga::Point mMouse2D;
  vsr::cga::Point mMouse3D;
  vsr::cga::Line mMouseRay;

  bool bShadedOutput = true;

  vsr::cga::Point calcMouse3D(float z=.99){

    auto& vd = gfx::GFXAppGui::mContext.interface.io.viewdata;
    auto tv = vd.ray;
    
    auto p = scene.unproject( io().pos(z) ); //vd.projectMid;

    Vec tz (tv[0], tv[1], tv[2] );

    mMouse2D =  vsr::cga::Construct::point(p[0],p[1],0);
    mMouse3D =  vsr::cga::Construct::point(p[0],p[1],p[2]);
    mMouseRay = mMouse3D ^ tz ^ vsr::cga::Infinity(1); 

    //intersection of ray with plane
    mMouse3D = vsr::cga::Construct::meet( mMouseRay, vsr::cga::DualPlane(tz) );

    return mMouse3D;
 
  } 
  
  /// Called when a keyboard key is pressed
  virtual void onKeyDown(const gfx::Keyboard& k){
    switch(k.code){
      case 'v':
        printf("v\n"); 
        GL::enablePreset();
        scene.push(true);
        gl2ps();
        scene.pop(true);
        GL::disablePreset();
    }
  } 


   void gl2ps(){
      static int id = 0;
      stringstream os; os << "output_" << id << (bShadedOutput ? ".eps" : ".pdf");
      id++;
      
      FILE *fp;
      int state = GL2PS_OVERFLOW, buffsize = 0;
      
      string name = os.str();
      fp = fopen(name.c_str(), "wb");
      
      printf("writing %s to %s\n", os.str().c_str(), name.c_str() );
      GLint tv[4];
      glGetIntegerv(GL_VIEWPORT, tv);
      
      
      while(state == GL2PS_OVERFLOW){

         buffsize += 1024*1024;

         if (bShadedOutput){
         gl2psBeginPage("test", "gl2psTestSimple", tv , GL2PS_EPS, GL2PS_SIMPLE_SORT, //NO_SORT
                         GL2PS_BEST_ROOT | GL2PS_TIGHT_BOUNDING_BOX | GL2PS_SIMPLE_LINE_OFFSET | GL2PS_OCCLUSION_CULL, //
                         GL_RGBA, 0, NULL, 0, 0, 0, buffsize, fp, "out.eps");
         } else {
         gl2psBeginPage("test", "gl2psTestSimple", tv , GL2PS_PDF, GL2PS_NO_SORT,
                         GL2PS_NO_PS3_SHADING | GL2PS_BEST_ROOT | GL2PS_TIGHT_BOUNDING_BOX | GL2PS_OCCLUSION_CULL, //
                         GL_RGBA, 0, NULL, 0, 0, 0, buffsize, fp, "out.eps");
         }

         gl2psEnable(GL2PS_BLEND);
         gl2psBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         gl2psPointSize(10);
         gl2psLineWidth(1);


          
          //DRAW 
          onDraw();
          
          state = gl2psEndPage();
      }
      
      fclose(fp);
      printf("Done!\n");
  }
  

};

#endif   /* ----- #ifndef vsr_cga3D_app_INC  ----- */
