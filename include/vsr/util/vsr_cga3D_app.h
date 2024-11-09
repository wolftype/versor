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

#include <gfx/util/gfx_imgui_app.h> //<-- an app class with built-in gui
#include <gfx/util/gfx_std_types.h>
#include <gfx/gfx_postscript.h>
#include <vsr/draw/vsr_cga3D_draw.h>   //<-- fixed pipeline draw routines
#include <vsr/draw/vsr_cga3D_render.h> //<-- programmable pipeline draw routines
#include <vsr/space/vsr_cga3D_op.h> //<-- conformal 3D types and basic operations
#include <vsr/util/vsr_cga3D_control.h> //<-- interface controls (mouse and keyboard)

#include <gfx/util/glfw_window.hpp> //<-- an app class with built-in gui

///@todo namespace this
struct App : public gfx::GFXAppImGui {
  App(int w = 800, int h = 600, string name = "Versor")
      : gfx::GFXAppImGui(w, h, name) {}

  using GuiApp = gfx::GFXAppImGui;

  vsr::cga::Point mMouse2D;
  vsr::cga::Point mMouse3D;
  vsr::cga::Line mMouseRay;

  gfx::PostScript ps;

  bool bSetMouse;

  // z = 0 is at camera near plane, z=1 is far clipping plane
  vsr::cga::Point calcMouse3D(float z = .99) {
    auto &vd = GuiApp::mContext.interface.io.viewdata;
    auto tv = vd.ray;

    auto p = scene.unproject(io().pos(z)); // vd.projectMid;

    vsr::cga::Vec tz(tv[0], tv[1], tv[2]);

    mMouse3D = vsr::cga::Construct::point(p[0], p[1], p[2]);
    mMouseRay = mMouse3D ^ tz ^ vsr::cga::Infinity(1);

    // intersection of ray with plane
    //    mMouse3D = vsr::cga::Construct::meet (mMouseRay, vsr::cga::DualPlane
    //    (tz));
    mMouse2D =
        vsr::cga::Construct::meet(mMouseRay, vsr::cga::DualPlane(0, 0, 1));

    return mMouse3D;
  }

  ///@todo move to gfx
  void monoPrint() {
    printf("output ps mono\n");
    gfx::GL::enablePreset();
    this->scene.camera.stereo(false);
    this->scene.push(true);
    ps.print(*this);
    this->scene.pop(true);
    gfx::GL::disablePreset();
  }

  ///@todo move to gfx
  void stereoPrint() {
    bool b = ps.bTightBounds;
    printf("output ps stereo\n");
    ps.bTightBounds = false;
    gfx::GL::enablePreset();
    this->scene.camera.stereo(true);
    this->scene.camera.left(true);
    this->scene.push(true);
    ps.print(*this);
    this->scene.pop(true);

    this->scene.camera.left(false);
    this->scene.push(true);
    ps.print(*this);
    this->scene.pop(true);

    gfx::GL::disablePreset();
    ps.bTightBounds = b;
  }
  /// Called when a keyboard key is pressed
  virtual void onKeyDown(const gfx::Keyboard &k) {
    cout << "KEY DOWN:" << k.code << endl; // Frame f;
    switch (k.code) {
    case 67:
    case 'c': {
      printf("vsr_cga3D_app Recenter Camera\n");
      scene.camera.pos(0, 0, 10);
      scene.camera.quat() = gfx::Quat(1, 0, 0, 0);
      scene.model.quat() = gfx::Quat(1, 0, 0, 0);
      break;
    }
    case 'i': {
      printf("vsr_cga3D_app mono print\n");
      monoPrint();
      break;
    }
    case 'o': {
      printf("vsr_cga3D_app stereo print\n");
      stereoPrint();
      break;
    }
    case 88:
    case 'x': {
      bSetMouse = !bSetMouse;
      printf("vsr_cga3D_app save mouse position %d\n", bSetMouse);
      break;
    }
    case 'm':
      printf("vsr_cga3D_app mono / stereo toggle\n");
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

