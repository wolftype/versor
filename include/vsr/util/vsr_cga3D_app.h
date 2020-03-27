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


#ifndef vsr_cga3D_app_INC
#define vsr_cga3D_app_INC

#include <vsr/space/vsr_cga3D_op.h>  //<-- conformal 3D types and basic operations

#include <vsr/draw/vsr_cga3D_draw.h>    //<-- fixed pipeline draw routines
#include <vsr/draw/vsr_cga3D_render.h>  //<-- programmable pipeline draw routines

#include <vsr/util/vsr_cga3D_control.h>  //<-- interface controls (mouse and keyboard)
#include <gfx/util/gfx_glv_app.h>        //<-- an app class with built-in gui
#include <gfx/util/glut_window.hpp>  //<-- an app class with built-in gui

#include <gfx/gfx_postscript.h>


///@todo namespace this
struct App : public gfx::GFXAppGui<gfx::GlutContext>
{

  using GuiApp = gfx::GFXAppGui<gfx::GlutContext>;

  vsr::cga::Point mMouse2D;
  vsr::cga::Point mMouse3D;
  vsr::cga::Line mMouseRay;

  gfx::PostScript ps;

  bool bSetMouse;

  vsr::cga::Point calcMouse3D (float z = .99)
  {

    auto &vd = GuiApp::mContext.interface.io.viewdata;
    auto tv = vd.ray;

    auto p = scene.unproject (io ().pos (z));  //vd.projectMid;

    vsr::cga::Vec tz (tv[0], tv[1], tv[2]);

    mMouse3D = vsr::cga::Construct::point (p[0], p[1], p[2]);
    mMouseRay = mMouse3D ^ tz ^ vsr::cga::Infinity (1);

    //intersection of ray with plane
//    mMouse3D = vsr::cga::Construct::meet (mMouseRay, vsr::cga::DualPlane (tz));
    mMouse2D =
      vsr::cga::Construct::meet (mMouseRay, vsr::cga::DualPlane (0, 0, 1));

    return mMouse3D;
  }

  ///@todo move to gfx
  void monoPrint ()
  {
    printf ("output ps mono\n");
    gfx::GL::enablePreset ();
    this->scene.camera.stereo (false);
    this->scene.push (true);
    ps.print (*this);
    this->scene.pop (true);
    gfx::GL::disablePreset ();
  }

  ///@todo move to gfx
  void stereoPrint ()
  { bool b = ps.bTightBounds;
    printf ("output ps stereo\n");
    ps.bTightBounds = false;
    gfx::GL::enablePreset ();
    this->scene.camera.stereo (true);
    this->scene.camera.left (true);
    this->scene.push (true);
    ps.print (*this);
    this->scene.pop (true);

    this->scene.camera.left (false);
    this->scene.push (true);
    ps.print (*this);
    this->scene.pop (true);

    gfx::GL::disablePreset ();
    ps.bTightBounds = b;
  }
  /// Called when a keyboard key is pressed
  virtual void onKeyDown (const gfx::Keyboard &k)
  {

    // Frame f;
    switch (k.code)
      {
        case 'i':
          monoPrint ();
          break;
        case 'o':
          stereoPrint ();
          break;
        case 'x':
          printf ("save mouse position\n");
          bSetMouse = !bSetMouse;
          break;
        case 'm':
          printf ("mono / stereo toggle\n");
          mRenderGraph.mStereoMode =
            mRenderGraph.mStereoMode == gfx::GFXRenderGraph::MONO
              ? gfx::GFXRenderGraph::ANAGLYPH
              : gfx::GFXRenderGraph::MONO;
          break;
        default:
          break;
      }
  }
};

#endif /* ----- #ifndef vsr_cga3D_app_INC  ----- */



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
