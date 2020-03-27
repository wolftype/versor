/*
 * =====================================================================================
 *
 *       Filename:  vsr_tangent.h
 *
 *    Description:  tangent frames, contact methods, and coordinate surface six-spheres
                    in vsr::cga
 *
 *        Version:  1.0
 *        Created:  04/21/2016 16:18:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#ifndef vsr_tangent_INC
#define vsr_tangent_INC

#include <vsr/form/vsr_cga3D_frame.h>

namespace vsr {
namespace cga {

/// A much simpler rep
//  @todo determine how much this should just be operational and storage free
//  for instance are the DualSpheres needed?
struct TFrame
{

  TFrame () : tu (Tnv(1,0,0)), tv (Tnv(0,1,0)), tw(Tnv(0,0,1))
  {
    flatSurfaces();
  }
  TFrame (const Pair& _tu, const Pair &_tv, const Pair &_tw)
    : tu(_tu), tv(_tv), tw(_tw)
  {
    flatSurfaces();
  }

   void flatSurfaces ()
   {
     svu = svw = Inf(-1) <= tv;
     suv = suw = Inf(-1) <= tu;
     swu = swv = Inf(-1) <= tw;
   }

  // Tangents (orthonormal)
  Pair tu;
  Pair tv;
  Pair tw;
  // Spheres of constant p in direction q (spq)
  // @todo should these be set by the usurf functions below?
  // as is right now these default to planes
  // @Todo indexable?
  DualSphere svu;
  DualSphere swu;
  DualSphere suv;
  DualSphere swv;
  DualSphere suw;
  DualSphere svw;

  /// Surface of constant u with curvature ku
  DualSphere usurf (float ku)
  {
    return DualSphere (Inf (-1) <= tu).spin (Gen::bst (tu * -ku / 2.0));
  }

  /// Surface of constant v with curvature kv
  DualSphere vsurf (float kv)
  {
    return DualSphere (Inf (-1) <= tv).spin (Gen::bst (tv * -kv / 2.0));
  }

  /// Surface of constant w with curvature kw
  DualSphere wsurf (float kw)
  {
    return DualSphere (Inf (-1) <= tw).spin (Gen::bst (tw * -kw / 2.0));
  }

  // Make the surfaces (needed to calculate rotor)
  void surfaces (float kvu, float kwu, float kuv, float kwv, float kuw,
                 float kvw)
  {
    svu = vsurf (kvu);
    swu = wsurf (kwu);
    suv = usurf (kuv);
    swv = wsurf (kwv);
    suw = usurf (kuw);
    svw = vsurf (kvw);
  }

  Vec du () { return -Round::dir (tu).copy<Vec> ().unit (); }
  Vec dv () { return -Round::dir (tv).copy<Vec> ().unit (); }
  Vec dw () { return -Round::dir (tw).copy<Vec> ().unit (); }

  Point pos () const { return Round::location (tu); }

  // Generate conformal transformation along u curve (fusion of kvu and kwu)
  Con uc (float kvu, float kwu, float dist)
  {
    return Gen::bst ((tv * kvu + tw * kwu) * -.5) * Gen::trs (du () * dist);
  }
  // Generate conformal transformation along v curve (fusion of kuv and kwv)
  Con vc (float kuv, float kwv, float dist)
  {
    return Gen::bst ((tu * kuv + tw * kwv) * -.5) * Gen::trs (dv () * dist);
  }
  // Generate conformal transformation along w curve (fusion of kuw and kvw)
  Con wc (float kuw, float kvw, float dist)
  {
    return Gen::bst ((tu * kuw + tv * kvw) * -.5) * Gen::trs (dw () * dist);
  }

  template <typename R>
  TFrame xf (const R &k, bool uflip, bool vflip, bool wflip)
  {
    Pair ttu = tu.spin (k);
    Pair ttv = tv.spin (k);
    Pair ttw = tw.spin (k);
    Pair ptu = Pair (-Round::dir (ttu).copy<Vec> ().unit ().copy<Tnv> ())
                 .trs (Round::location (ttu));
    Pair ptv = Pair (-Round::dir (ttv).copy<Vec> ().unit ().copy<Tnv> ())
                 .trs (Round::location (ttv));
    Pair ptw = Pair (-Round::dir (ttw).copy<Vec> ().unit ().copy<Tnv> ())
                 .trs (Round::location (ttw));
    return TFrame (ptu * (uflip ? -1 : 1), ptv * (vflip ? -1 : 1),
                   ptw * (wflip ? -1 : 1));
  }

  void uflip()
  {
    tu *= -1;
  }

  void vflip()
  {
    tv *= -1;
  }

  void wflip()
  {
    tw *= -1;
  }
  // some extra stuff here -- the pair log generators bringing surf to surf
  // note surfaces() must have been called
  // this sweeps the v direction curve "over" along u
  // @todo rename these to duv, duw, etc
  Pair duv (const TFrame &tf){
    return Gen::log (-(tf.suv/suv).bunit()) / 2.0;
  }
  // this sweeps the w direction curve "over" along u
  Pair duw (const TFrame &tf){
    return Gen::log (-(tf.suw/suw).bunit()) / 2.0;
  }
  // this sweeps the u direction curve "up" along v
  Pair dvu (const TFrame &tf){
    return Gen::log (-(tf.svu/svu).bunit()) / 2.0;
  }
  // this sweeps the w direction curve "up" along v
  Pair dvw (const TFrame &tf){
    return Gen::log (-(tf.svw/svw).bunit()) / 2.0;
  }
  // this sweeps the u direction curve "in" along w
  Pair dwu (const TFrame &tf){
    return Gen::log (-(tf.swu/swu).bunit()) / 2.0;
  }
  // this sweeps the v direction curve "in" along w
  Pair dwv (const TFrame &tf){
    return Gen::log (-(tf.swv/swv).bunit()) / 2.0;
  }

};


// Multiple TFrames, in a volume
struct TVolume {

  enum class Corner {
    ORIGIN = 0,
    U = 1,
    V = 2,
    W = 4,
    UV = 3,
    UW = 5,
    VW = 6,
    UVW = 7
  };

  enum class Face {
    LEFT,
    RIGHT,
    BOTTOM,
    TOP,
    BACK,
    FRONT
  };

  struct Mapping {

       std::vector<Con> mCon;
       int mRes;

       Mapping (int res) : mRes (res){
         mCon.resize(res * res * res);
       }

       Con& at (int i, int j, int k){
         return mCon [i * mRes * mRes + j * mRes + k];
       }

       const Con& at (int i, int j, int k) const {
         return mCon [i * mRes * mRes + j * mRes + k];
       }

    };

    float mK[9];
    float mSpacing[3];
    TFrame mTFrame[8];
    Pair mGen[12];

    TVolume (){
      zeroInit();
    };

    TVolume (const TVolume& tv, Face face)
    {
      zeroInit();
      switch (face)
      {
        case Face::LEFT:
           {
             break;
           }
        case Face::RIGHT:
           {
             tf() = tv.uf();
             wf() = tv.uwf();
             vf() = tv.uvf();
             vwf() = tv.uvwf();

             tf().uflip();
             wf().uflip();
             vf().uflip();
             vwf().uflip();

             calcSurfacesFromFace (Face::LEFT);

             break;
           }
         default:
           break;
      }

    };

    TVolume (float kvu, float kwu,
             float kuv, float kwv,
             float kuw, float kvw,
             float kv1u, float ku1w, float kw1v,
             float uSpacing, float vSpacing, float wSpacing)
    {
      mK[0] = kvu;
      mK[1] = kwu;
      mK[2] = kuv;
      mK[3] = kwv;
      mK[4] = kuw;
      mK[5] = kvw;
      mK[6] = kv1u;
      mK[7] = ku1w;
      mK[8] = kw1v;

      mSpacing[0] = uSpacing;
      mSpacing[1] = vSpacing;
      mSpacing[2] = wSpacing;

      calcSurfaces();
    };

    void zeroInit (){
      mK[0] = 0;
      mK[1] = 0;
      mK[2] = 0;
      mK[3] = 0;
      mK[4] = 0;
      mK[5] = 0;
      mK[6] = 0;
      mK[7] = 0;
      mK[8] = 0;

      mSpacing[0] = 3.0;
      mSpacing[1] = 3.0;
      mSpacing[2] = 3.0;

      calcSurfaces();
    }

    TFrame& tf(){ return mTFrame[0]; }
    TFrame& uf(){ return mTFrame[1]; }
    TFrame& vf(){ return mTFrame[2]; }
    TFrame& wf(){ return mTFrame[4]; }
    TFrame& uvf(){ return mTFrame[3]; }
    TFrame& uwf(){ return mTFrame[5]; }
    TFrame& vwf(){ return mTFrame[6]; }
    TFrame& uvwf(){ return mTFrame[7]; }

    const TFrame& tf() const { return mTFrame[0]; }
    const TFrame& uf() const { return mTFrame[1]; }
    const TFrame& vf() const { return mTFrame[2]; }
    const TFrame& wf() const { return mTFrame[4]; }
    const TFrame& uvf() const { return mTFrame[3]; }
    const TFrame& uwf() const { return mTFrame[5]; }
    const TFrame& vwf() const { return mTFrame[6]; }
    const TFrame& uvwf() const { return mTFrame[7]; }

    float& kvu(){ return mK[0]; }
    float& kwu(){ return mK[1]; }
    float& kuv(){ return mK[2]; }
    float& kwv(){ return mK[4]; }
    float& kuw(){ return mK[3]; }
    float& kvw(){ return mK[5]; }
    float& kv1u(){ return mK[6]; }
    float& ku1w(){ return mK[7]; }
    float& kw1v(){ return mK[8]; }

    const float& kvu() const { return mK[0]; }
    const float& kwu() const { return mK[1]; }
    const float& kuv() const { return mK[2]; }
    const float& kwv() const { return mK[4]; }
    const float& kuw() const { return mK[3]; }
    const float& kvw() const { return mK[5]; }
    const float& kv1u() const { return mK[6]; }
    const float& ku1w() const { return mK[7]; }
    const float& kw1v() const { return mK[8]; }

    float& uSpacing () { return mSpacing[0]; }
    float& vSpacing () { return mSpacing[1]; }
    float& wSpacing () { return mSpacing[2]; }

    const float& uSpacing () const { return mSpacing[0]; }
    const float& vSpacing () const { return mSpacing[1]; }
    const float& wSpacing () const { return mSpacing[2]; }

    Pair& duvw0() { return mGen[0]; }
    Pair& duwv0() { return mGen[1]; }

    Pair& dvwu0() { return mGen[2]; }
    Pair& dvuw0() { return mGen[3]; }

    Pair& dwuv0() { return mGen[4]; }
    Pair& dwvu0() { return mGen[5]; }

    Pair& duvw1() { return mGen[6]; }
    Pair& duwv1() { return mGen[7]; }

    Pair& dvwu1() { return mGen[8]; }
    Pair& dvuw1() { return mGen[9]; }

    Pair& dwuv1() { return mGen[10]; }
    Pair& dwvu1() { return mGen[11]; }

    const Pair& duvw0() const { return mGen[0]; }
    const Pair& duwv0() const { return mGen[1]; }

    const Pair& dvwu0() const { return mGen[2]; }
    const Pair& dvuw0() const { return mGen[3]; }

    const Pair& dwuv0() const { return mGen[4]; }
    const Pair& dwvu0() const { return mGen[5]; }

    const Pair& duvw1() const { return mGen[6]; }
    const Pair& duwv1() const { return mGen[7]; }

    const Pair& dvwu1() const { return mGen[8]; }
    const Pair& dvuw1() const { return mGen[9]; }

    const Pair& dwuv1() const { return mGen[10]; }
    const Pair& dwvu1() const { return mGen[11]; }

    //calculats based on four frames
    void calcSurfacesFromFace(const Face& face)
    {
      switch (face){
        case Face::LEFT:
          {
            tf().suv = -vf().pos() <= tf().tu;
            tf().swv = -vf().pos() <= tf().tw;
            tf().suw = -wf().pos() <= tf().tu;
            tf().svw = -wf().pos() <= tf().tv;
            vf().suw = -wf().pos() <= vf().tu;
            vf().svw = -wf().pos() <= vf().tv;
            wf().suv = -vwf().pos() <= wf().tu;
            wf().swv = -vwf().pos() <= wf().tw;

            // Conformal Rotors along u,v,w curves, passing in two curvatures
            Con uc = tf().uc (kvu(), kwu(), uSpacing());

            tf().svu = tf().vsurf (kvu());
            tf().swu = tf().wsurf (kwu());
            vf().svu = vf().vsurf (kv1u());
            uf().suw = uf().usurf (ku1w());

           uf() = tf().xf (uc, true, false, false);
//           uf() = tf().xf (uc, false, false, false);

            //ortho
            uf().suv = -vf().svu <= uf().tu;
            uf().swv = -vf().svu <= uf().tw;
            wf().swu = -uf().suw <= wf().tw;
            wf().svu = -uf().suw <= wf().tv;
            uf().svw = -wf().swu <= uf().tv;
            vf().swu = -uf().suv <= vf().tw;

            duvw0() = tf().duv (uf());
            duwv0() = tf().duw (uf());
            dvwu0() = tf().dvw (vf());
            dvuw0() = tf().dvu (vf());
            dwuv0() = tf().dwu (wf());
            dwvu0() = tf().dwv (wf());

            uvf() = vf().xf( Gen::boost(duvw0()), true, false, false);
            uwf() = uf().xf( Gen::boost(dwuv0()), false, false, true);

            //uvf() = vf().xf( Gen::boost(duvw0()), false, false, false);
            //uwf() = uf().xf( Gen::boost(dwuv0()), false, false, false);

            auto topPlane = vf().pos() ^ uvf().pos() ^ vwf().pos() ^ Inf(1);
            auto frontPlane = wf().pos() ^ vwf().pos() ^ uwf().pos() ^ Inf(1);
            auto rightPlane = uf().pos() ^ uwf().pos() ^ uvf().pos() ^ Inf(1);
            auto p = (topPlane.dual() ^ frontPlane.dual() ^ rightPlane.dual()).dual();
            p /= p[3];
            Point np = p.null();

            //these each have two defined surfaces
            uvf().suw = -np <= uvf().tu;
            uvf().svw = -np <= uvf().tv;
            vwf().svu = -np <= vwf().tv;
            vwf().swu = -np <= vwf().tw;
            uwf().swv = -np <= uwf().tw;
            uwf().suv = -np <= uwf().tu;

            //grab the tangent frame at np too
            //uvwf().tu = (np <= uvf().suw.undual()).dual();
            //uvwf().tv = (np <= uvf().svw.undual()).dual();
            //uvwf().tw = (np <= vwf().swu.undual()).dual();

            uvwf().tu = (np ^ uvf().suw);
            uvwf().tv = (np ^ uvf().svw);
            uvwf().tw = (np ^ vwf().swu);
            //and with that, we have all the surfaces that can be defined with
            //nine coefficients -- 24 surfaces!  Which pair up into 12 Generators
            //now we find the remaining 6 generators
            // du of v direction at w=1 (sweeps left to right FRONT) |
            duvw1() = wf().duv(uwf());
            // du of w direction at v=1 (sweeps left to right TOP) /
            duwv1() = vf().duw(uvf());
            // dv of w direction at u=1 (sweeps bottom to top RIGHT) /
            dvwu1() = uf().dvw(uvf());
            // dv of u direction at w=1 (sweeps bottom to top FRONT) _
            dvuw1() = wf().dvu(vwf());
            // dw of u direction at v=1 (sweeps back to front TOP) _
            dwuv1() = vf().dwu(vwf());
            // dw of v direction at u=1 (sweeps back to front RIGHT) |
            dwvu1() = uf().dwv(uwf());
            break;
          }
          default:
            break;
        }
    }

    // caculates based on nine curvature coefficients and spacing
    void calcSurfaces ()
    {
      // Conformal Rotors along u,v,w curves, passing in two curvatures
      Con uc = tf().uc (kvu(), kwu(), uSpacing());
      Con vc = tf().vc (kuv(), kwv(), vSpacing());
      Con wc = tf().wc (kuw(), kvw(), wSpacing());

      //make the actual surfaces and store them
      tf().surfaces (kvu(), kwu(), kuv(), kwv(), kuw(), kvw());

      // New frames in respective directions, bools specify whether to "flip"
      uf() = tf().xf (uc, true, false, false);
      vf() = tf().xf (vc, false, true, false);
      wf() = tf().xf (wc, false, false, true);

//      uf() = tf().xf (uc, false, false, false);
//      vf() = tf().xf (vc, false, false, false);
//      wf() = tf().xf (wc, false, false, false);

      // Here we can make three surfaces by bending
      // "Top Going Right"
      vf().svu = vf().vsurf (kv1u());
      // "Right Going Forward"
      uf().suw = uf().usurf (ku1w());
      // "Front Going Up"
      wf().swv = wf().wsurf (kw1v());

      //Now all other surfaces must be orthogonal to these
      //"Back Right Edge Going Up"
      //At u1, const u1 and const w0 in v dir are both ortho to const v1 in u dir
      uf().suv = -vf().svu <= uf().tu;
      uf().swv = -vf().svu <= uf().tw;
      //"Front Bottom Edge Going Over"
      //At w1, const w1 and const v0 in u dir are both ortho to const u1 in w dir
      wf().swu = -uf().suw <= wf().tw;
      wf().svu = -uf().suw <= wf().tv;
      //"Top Left Edge Going Forward"
      //At v1, const v1 and const u0 are both ortho to const w1 in v dir
      vf().svw = -wf().swv <= vf().tv;
      vf().suw = -wf().swv <= vf().tu;

      //now, what about the other surfaces to match the three bends?
      //"Bottom Going Forward"
      uf().svw = -wf().swu <= uf().tv;
      //"Back Going Right"
      vf().swu = -uf().suv <= vf().tw;
      //"Left Going Up"
      wf().suv = -vf().svw <= wf().tu;

      // first letter indicates direction of sweep.
      // second letter indicates curvature line that is swept.
      // duv is a ratio of suvs, so sweeps right
      //
      // du of v direction (sweeps left to right BACK vertical) |
      duvw0() = tf().duv (uf());
      // du of w direction (sweeps left to right BOTTOM depth) /
      duwv0() = tf().duw (uf());
      // dv of w direction (sweeps bottom to top LEFT depth) /
      dvwu0() = tf().dvw (vf());
      // dv of u direction (sweeps bottom to top BACK horizontal) _
      dvuw0() = tf().dvu (vf());
      // dw of u direction (sweeps back to front BOTTOM horizontal) _
      dwuv0() = tf().dwu (wf());
      // dw of v direction (sweeps back to front LEFT veritical) |
      dwvu0() = tf().dwv (wf());

      // we need to calculate the other frames
      // to calculate the other surfaces
      uvf() = vf().xf( Gen::boost(duvw0()), true, false, false);
      vwf() = wf().xf( Gen::boost(dvwu0()), false, true, false);
      uwf() = uf().xf( Gen::boost(dwuv0()), false, false, true);

//      uvf() = vf().xf( Gen::boost(duvw0()), false, false, false);
//      vwf() = wf().xf( Gen::boost(dvwu0()), false, false, false);
//      uwf() = uf().xf( Gen::boost(dwuv0()), false, false, false);

      auto topPlane = vf().pos() ^ uvf().pos() ^ vwf().pos() ^ Inf(1);
      auto frontPlane = wf().pos() ^ vwf().pos() ^ uwf().pos() ^ Inf(1);
      auto rightPlane = uf().pos() ^ uwf().pos() ^ uvf().pos() ^ Inf(1);
      auto p = (topPlane.dual() ^ frontPlane.dual() ^ rightPlane.dual()).dual();
      p /= p[3];
      Point np = p.null();

      //these each have two defined surfaces
      uvf().suw = -np <= uvf().tu;
      uvf().svw = -np <= uvf().tv;
      vwf().svu = -np <= vwf().tv;
      vwf().swu = -np <= vwf().tw;
      uwf().swv = -np <= uwf().tw;
      uwf().suv = -np <= uwf().tu;

      //grab the tangent frame at np too
//      uvwf().tu = (np <= uvf().suw.undual()).dual();
//      uvwf().tv = (np <= uvf().svw.undual()).dual();
//      uvwf().tw = (np <= vwf().swu.undual()).dual();

        uvwf().tu = (np ^ uvf().suw);
        uvwf().tv = (np ^ uvf().svw);
        uvwf().tw = (np ^ vwf().swu);
      //and with that, we have all the surfaces that can be defined with
      //nine coefficients -- 24 surfaces!  Which pair up into 12 Generators
      //now we find the remaining 6 generators
      // du of v direction at w=1 (sweeps left to right FRONT) |
      duvw1() = wf().duv(uwf());
      // du of w direction at v=1 (sweeps left to right TOP) /
      duwv1() = vf().duw(uvf());
      // dv of w direction at u=1 (sweeps bottom to top RIGHT) /
      dvwu1() = uf().dvw(uvf());
      // dv of u direction at w=1 (sweeps bottom to top FRONT) _
      dvuw1() = wf().dvu(vwf());
      // dw of u direction at v=1 (sweeps back to front TOP) _
      dwuv1() = vf().dwu(vwf());
      // dw of v direction at u=1 (sweeps back to front RIGHT) |
      dwvu1() = uf().dwv(uwf());

    }

   Con calcMapping (float ti, float tj, float tk){
      // Back to Front
      Boost wvu0 = Gen::bst (dwvu0() * tk);
      Boost wvu1 = Gen::bst (dwvu1() * tk);
      Boost wuv0 = Gen::bst (dwuv0() * tk);
      Boost wuv1 = Gen::bst (dwuv1() * tk);

      DualSphere su0v = tf().suv.spin (wvu0);
      DualSphere su1v = uf().suv.spin (wvu1);
      DualSphere sv0u = tf().svu.spin (wuv0);
      DualSphere sv1u = vf().svu.spin (wuv1);

      Pair duv  = Gen::log (-(su1v/su0v).bunit())/2.0;
      Pair dvu  = Gen::log (-(sv1u/sv0u).bunit())/2.0;
      Boost uv = Gen::bst(duv * ti);
      Boost vu = Gen::bst(dvu * tj);

      return vu * uv * wvu0;
   }

   Mapping calcMapping (int res)
   {

     Mapping result (res);

     for (int i = 0; i < res; ++i)
     {
        float ti = (float)i/(res-1);
        // Back to Front
        Boost wvu0 = Gen::bst (dwvu0() * ti);
        Boost wvu1 = Gen::bst (dwvu1() * ti);
        Boost wuv0 = Gen::bst (dwuv0() * ti);
        Boost wuv1 = Gen::bst (dwuv1() * ti);

        DualSphere su0v = tf().suv.spin (wvu0);
        DualSphere su1v = uf().suv.spin (wvu1);
        DualSphere sv0u = tf().svu.spin (wuv0);
        DualSphere sv1u = vf().svu.spin (wuv1);

        Pair duv  = Gen::log (-(su1v/su0v).bunit())/2.0;
        Pair dvu  = Gen::log (-(sv1u/sv0u).bunit())/2.0;

       for (int j = 0; j < res; ++j)
       {
         float tj = (float)j/(res-1);
         Boost uv = Gen::bst(duv * tj);

         for (int k = 0; k < res; ++k)
         {
           float tk = (float)k/(res-1);
           Boost vu = Gen::bst(dvu * tk);

           Con Kw = vu * uv * wvu0;
           result.at (j,k,i) = Kw;

         }
       }
     }

     return result;
   }

   struct Coord {
      VSR_PRECISION u,v,w;
   };
   Coord inverseMapping (const Point &p, const Face& face)
   {
     switch (face){
       case Face::LEFT:
         {
            auto sv  = -p <= dvwu0();
            auto vpair  = Gen::log ( -(sv/tf().svw).bunit()) * .5;
            auto tv = vpair <= !dvwu0();

            auto sw =  -p <= dwvu0();
            auto wpair  = Gen::log ( -(sw/tf().swv).bunit()) * .5;
            auto tw = wpair <= !dwvu0();
            return {0.0, tv[0], tw[0]};

           break;
         }
       case Face::RIGHT:
         {
            auto sv  = -p <= dvwu1();
            auto vpair  = Gen::log ( -(sv/uf().svw).bunit()) * .5;
            auto tv = vpair <= !dvwu1();

            auto sw =  -p <= dwvu1();
            auto wpair  = Gen::log ( -(sw/uf().swv).bunit()) * .5;
            auto tw = wpair <= !dwvu1();
            return {1.0, tv[0], tw[0]};

           break;
         }
       case Face::BACK:
         {
            auto sv  = -p <= dvuw0();
            auto vpair  = Gen::log ( -(sv/tf().svu).bunit()) * .5;
            auto tv = vpair <= !dvuw0();

            auto su =  -p <= duvw0();
            auto upair  = Gen::log ( -(su/tf().suv).bunit()) * .5;
            auto tu = upair <= !duvw0();
            return {tu[0], tv[0], 1.0};

           break;
         }
       case Face::FRONT:
         {
            auto sv  = -p <= dvuw1();
            auto vpair  = Gen::log ( -(sv/wf().svu).bunit()) * .5;
            auto tv = vpair <= !dvuw1();

            auto su =  -p <= duvw1();
            auto upair  = Gen::log ( -(su/wf().suv).bunit()) * .5;
            auto tu = upair <= !duvw1();
            return {tu[0], tv[0], 1.0};

           break;
         }
       default:
            return {0.0, 0.0, 0.0};

     }

   }

};

/// 3D Frame of Tangent Vectors (Point Pairs)
///
///  @todo make a six-sphere system vsr_coord
///  @todo currently inherits from Frame --
///  might want to be able to set parent class Frame::pos and Frame::rot
///  from tangent info stored here.
///  @todo possibly make more method-based, and less state-based
///  @todo how many spheres about a circle?  any number?
struct TangentFrame : public Frame
{
  /// Coordinate Surface Spheres (Sigma)
  ///  Sphere through next point with tangents as below (needed or put in next level?)
  Sphere sphere[3];
  /// kappa Tangent Vectors (null point pairs)
  Pair tan[3];
  /// Kappa  Tangent Bivectors (null circles)
  Circle bitan[3];

  /// @todo Derivatives in every possible direction
  //float curvature[6];

  /// Default Construct -- Store Frame as Tangents
  TangentFrame () : Frame () { store (); }

  /// Copy Construct from Frame -- Store Frame as Tangents
  TangentFrame (const Frame &f) : Frame (f) { store (); }

  /// Construct at position p w.r.t TangentFrame
  TangentFrame (const Point &p, const TangentFrame tf) { set (p, tf); }

  /// Assignment to Frame
  TangentFrame &operator= (const Frame &f)
  {
    this->rot () = f.rot ();
    this->pos () = f.pos ();
    return store ();
  }

  /// Assignment to TangentFrame (needed?)
  TangentFrame &operator= (const TangentFrame &f)
  {
    this->rot () = f.rot ();
    this->pos () = f.pos ();

    for (int j = 0; j < 3; ++j)
      {
        tan[j] = f.tan[j];
        bitan[j] = f.bitan[j];  // swap duality here
        sphere[j] = f.sphere[j];
      }

    return *this;
  }


  /// Save current Frame State
  TangentFrame &store ()
  {

    tan[0] = tx ();
    tan[1] = ty ();
    tan[2] = tz ();

    for (int j = 0; j < 3; ++j)
      {
        bitan[j] = tan[j].undual ();  // swap duality here
      }

    return *this;
  }

  /// Set current tangents at position from relative TangentFrame
  void set (const Point &p, const TangentFrame &tf)
  {
    this->pos () = p;
    // hmm, could potentially work in any dimension
    for (int j = 0; j < 3; ++j)
      {
        //this outer product order really matters for symmetry
        sphere[j] = tf.bitan[j] ^ p;
        bitan[j] = Tangent::at (sphere[j], p);
        tan[j] = bitan[j].dual ();
      }
  }

  /// Flip direction of ith Tangent (useful when chaining systems)
  TangentFrame &flip (int idx)
  {
    tan[idx] *= -1;
    bitan[idx] *= -1;
    return *this;
  }

  /// Make Unit Length (unneeded?)
  TangentFrame unit () const
  {
    TangentFrame tf = *this;
    for (int idx = 0; idx < 3; ++idx)
      {
        // normalize euclidean representation and reformulate
        // need to normalize coordinate surface?
        tf.sphere[idx] = sphere[idx] / (Round::direction (sphere[idx])[0]);
        tf.bitan[idx] =
          Circle (
            Round::direction (bitan[idx]).copy<Biv> ().bunit ().copy<Tnb> ())
            .translate (pos ());
        tf.tan[idx] = tf.bitan[idx].dual ();
      }
    return tf;
  }


  /// Calculate Edges as intersections of constant coordinates (legacy)
  Circle calcCurve (int idx)
  {
    Circle c;
    switch (idx)
      {
        case 0:
          c = (sphere[1].dual () ^ sphere[2].dual ()).undual ();
        case 1:
          c = (sphere[0].dual () ^ sphere[2].dual ()).undual ();
        case 2:
          c = (sphere[0].dual () ^ sphere[1].dual ()).undual ();
      }
    return c;
  }

  /// Get x DirectionVector
  Drv xdir () { return -Round::direction (tan[0]); }
  /// Get y DirectionVector
  Drv ydir () { return -Round::direction (tan[1]); }
  /// Get z DirectionVector
  Drv zdir () { return -Round::direction (tan[2]); }

  ///@todo rename this to xcop, ycop, zcop (for Curvature OPerator)
  //to disambiguate from xcurve
  /// Generate Boost Relative to x Tangent Vector
  Bst xcurve (float amt) { return Gen::bst (tan[0] * amt * -.5); }
  /// Generate Boost Relative to y Tangent Vector
  Bst ycurve (float amt) { return Gen::bst (tan[1] * amt * -.5); }
  /// Generate Boost Relative to z Tangent Vector
  Bst zcurve (float amt) { return Gen::bst (tan[2] * amt * -.5); }

  ///@todo rename these to xycop, yzcop, xzcop (for Curvature OPerator)
  /// Generate Boost Relative to const x and const y Tangent Vector
  Bst xycurve (float amtX, float amtY)
  {
    return Gen::bst ((tan[0] * amtX + tan[1] * amtY) * -.5);
  }

  /// Generate Boost Relative to x and z TangentVector (null Pair) (dx/dy + dz/dy)
  Bst xzcurve (float amtX, float amtZ)
  {
    return Gen::bst ((tan[0] * amtX + tan[2] * amtZ) * -.5);
  }
  /// Generate Boost Relative to y and z TangentVector (null Pair)
  Bst yzcurve (float amtY, float amtZ)
  {
    return Gen::bst ((tan[1] * amtY + tan[2] * amtZ) * -.5);
  }

  /// x = Constant Coordinate Surface from Boost generator
  DualSphere xsurface (const Bst &b)
  {
    return DualSphere (Round::carrier (bitan[0]).dual ()).spin (b);
  }
  /// y = Constant Coordinate Surface from Boost generator
  DualSphere ysurface (const Bst &b)
  {
    return DualSphere (Round::carrier (bitan[1]).dual ()).spin (b);
  }
  /// z = Constant Coordinate Surface from Boost generator
  DualSphere zsurface (const Bst &b)
  {
    return DualSphere (Round::carrier (bitan[2]).dual ()).spin (b);
  }

  /// An x = Constant Coordinate Surface from curvature scalar
  DualSphere xsurface (float amt)
  {
    return DualSphere (Round::carrier (bitan[0]).dual ().unit ())
      .spin (xcurve (amt));
  }

  /// A y = Constant Coordinate Surface from curvature scalar
  DualSphere ysurface (float amt)
  {
    return DualSphere (Round::carrier (bitan[1]).dual ().unit ())
      .spin (ycurve (amt));
  }

  /// A z = Constant Coordinate Surface from curvature scalar
  DualSphere zsurface (float amt)
  {
    return DualSphere (Round::carrier (bitan[2]).dual ().unit ())
      .spin (zcurve (amt));
  }

  /// Get position of TangentFrame
  Point point () const { return pos (); }

  /// Point along x direction
  Point xpoint (float amt) { return point ().translate (xdir () * amt); }
  /// Point along y direction
  Point ypoint (float amt) { return point ().translate (ydir () * amt); }
  /// Point along z direction
  Point zpoint (float amt) { return point ().translate (zdir () * amt); }

  /// TangentFrame yzcurved
  TangentFrame xbend (float amtY, float amtZ, float dist = 1)
  {
    auto bst = yzcurve (amtY, amtZ);
    auto pt = Round::location (xpoint (dist).spin (bst));
    return TangentFrame (pt, *this);
  }

  /// TangentFrame xzcurved
  TangentFrame ybend (float amtX, float amtZ, float dist = 1)
  {
    auto bst = xzcurve (amtX, amtZ);
    auto pt = Round::location (ypoint (dist).spin (bst));
    return TangentFrame (pt, *this);
  }

  /// TangentFrame xycurved
  TangentFrame zbend (float amtX, float amtY, float dist = 1)
  {
    auto bst = xycurve (amtX, amtY);
    auto pt = Round::location (zpoint (dist).spin (bst));
    return TangentFrame (pt, *this);
  }

  /// Use Const X Surface to Find fourth TangentFrame on a circle
  TangentFrame xclose (float amt, const Point &pa, const Point &pb)
  {
    return circleClose (xsurface (amt), pa, pb);
  }
  /// Use Const Y Surface to Find fourth TangentFrame on a circle
  TangentFrame yclose (float amt, const Point &pa, const Point &pb)
  {
    return circleClose (ysurface (amt), pa, pb);
  }
  /// Use Const Z Surface to Find fourth TangentFrame on a circle
  TangentFrame zclose (float amt, const Point &pa, const Point &pb)
  {
    return circleClose (zsurface (amt), pa, pb);
  }

  /// Given four tangent frames, find corner
  TangentFrame close (const TangentFrame &ta, const TangentFrame &tb,
                      const TangentFrame &tc, const TangentFrame &td)
  {

    auto pt = point ();

    auto cir = pt ^ ta.point () ^ tb.point ();
    auto cir2 = pt ^ tc.point () ^ td.point ();

    auto pair = (Round::surround (cir) ^ cir2.dual ()).dual ();

    auto tpa = Construct::pointA (pair);
    auto tpb = Construct::pointB (pair);

    auto p1 = FERROR ((pt <= tpa).wt ()) ? tpb : tpa;

    return TangentFrame (p1, *this);
  }

  /// Given a sphere and two points, make a circle with the two points and find its intersection with sphere
  TangentFrame circleClose (const DualSphere &s, const Point &pa,
                            const Point &pb)
  {

    auto pt = point ();
    auto cir = pt ^ pa ^ pb;
    auto pair = (s ^ cir.dual ()).dual ();

    /// @todo speed up this extraction
    auto tpa = Construct::pointA (pair);
    auto tpb = Construct::pointB (pair);

    auto p1 = FERROR ((pt <= tpa).wt ()) ? tpb : tpa;

    return TangentFrame (p1, *this);
  }
};

/**
* @brief Contact provides methods for calculations with a normal tangent vector at a point on a sphere

  contains a DualSphere, a Point position on the sphere,
  and a null Pair tangent vector specifying the normal on
  the sphere at that point.  Normal may be pointing inwards.
*/
struct Contact
{

  DualSphere sphere;
  Point point;
  Pair tnv;

  /// Construct from Point and DualPlane
  Contact (const Point &p, const DualPlane &dlp)
      : point (p), sphere (dlp), tnv ((p <= dlp.dual ()).dual ())
  {
  }

  /// Construct from Point and DualSphere
  Contact (const Point &p, const DualSphere &s)
      : point (p), sphere (s), tnv (Tangent::at (s.undual (), p).dual ())
  {
  }

  /// Construct from Point on some other source sphere and some target sphere
  /// (makes orthogonal plunge into target sphere)
  Contact (const Point &p, const DualSphere &source, const DualSphere &target)
      : sphere (target)
  {
    auto tnvsource = Tangent::at (source.undual (), p).dual ();  // A Point Pair
    point = Construct::pointA (((target ^ tnvsource).dual () ^ target).dual ());
    tnv = Tangent::at (target.undual (), point).dual ();
  }

  /// Construct from another contact and a target
  Contact (const Contact &source, const DualSphere &target) : sphere (target)
  {
    point =
      Construct::pointA (((target ^ source.tnv).dual () ^ target).dual ());
    tnv = Tangent::at (target.undual (), point).dual ();
  }

  /// Get undualized tangentbivector (circle)
  Cir bitan () const { return tnv.dual (); }  //*** dualization changed ***/
  /// Get Vector direction of TangentVector
  Vec vec () const { return -Round::direction (tnv).copy<Vec> ().unit (); }
  /// Get Bivector dual of vector direction
  Biv biv () const { return vec ().dual (); }  //*** dualization changed ***/
};


/// SixSphere Coordinate System curvatures in every direction (see Moon and Spencer for coordinate system diagram)
/// @todo simplify this representation -- tan and bitan should be methods?
struct SixSphere
{

  TangentFrame frame;
  struct Curve
  {
    float a = 0;
    float b = 0;
  } curve[3];  ///< Three Coordinate Curves, each with two curvatures

  float lengthX, lengthY, lengthZ;

  SixSphere () {}
  SixSphere (const Frame &f) : frame (f) {}
  //  SixSphere& operator= (const Frame& f){
  //    this->pos() = f.pos();  this->rot() = f.rot();
  //    store();
  //    return *this;
  //  }

  /// Set curvatures in every direction
  void set (float YX, float ZX,  // constant X in the Y and Z directions
            float XY, float ZY,  // constant Y in the X and Z directions
            float XZ, float YZ,  // constant Z in the X and Y directions
            float lx = 1, float ly = 1,
            float lz = 1)  //distances in each direction
  {
    curve[0].a = YX;
    curve[0].b = ZX;
    curve[1].a = XY;
    curve[1].b = ZY;
    curve[2].a = XZ;
    curve[2].b = YZ;

    lengthX = lx;
    lengthY = ly;
    lengthZ = lz;
  }

  /// Tangent Frame in x direction
  TangentFrame x ()
  {
    return frame.xbend (curve[0].a, curve[0].b, lengthX).unit ();
  }
  /// Tangent Frame in y direction
  TangentFrame y ()
  {
    return frame.ybend (curve[1].a, curve[1].b, lengthY).unit ();
  }
  /// Tangent Frame in z direction
  TangentFrame z ()
  {
    return frame.zbend (curve[2].a, curve[2].b, lengthZ).unit ();
  }
  /// Tangent Frame in x+y direction (with optional additional curve input)
  TangentFrame xy (float c = 0.f)
  {
    float XY = cxy ();
    float X1Y = XY == 0 ? 0 : XY > 0 ? -(1.f / ((1.f / XY) - lengthX))
                                     : 1.f / ((1.f / -XY) + lengthX);
    return x ().xclose (X1Y + c, frame.pos (), y ().pos ()).unit ();
  }
  /// Tangent Frame in x+z direction (with optional additional curve input)
  TangentFrame zx (float c = 0.f)
  {
    float ZX = czx ();
    float Z1X = ZX == 0 ? 0 : ZX > 0 ? -(1.f / ((1.f / ZX) - lengthZ))
                                     : 1.f / ((1.f / -ZX) + lengthZ);
    return z ().zclose (Z1X + c, frame.pos (), x ().pos ()).unit ();
  }
  /// Tangent Frame in y+z direction (with optional additional curve input)
  TangentFrame zy (float c = 0.f)
  {
    float ZY = czy ();
    float Z1Y = ZY == 0 ? 0 : ZY > 0 ? -(1.f / ((1.f / ZY) - lengthZ))
                                     : 1.f / ((1.f / -ZY) + lengthZ);
    return z ().zclose (Z1Y + c, frame.pos (), y ().pos ()).unit ();
  }
  /// Tangent Frame in x+y+z direction
  TangentFrame xyz (float cx = 0.f, float cy = 0.f)
  {
    return xy ().close (x (), zx (cx), y (), zy (cy)).unit ();
  }

  float &cyx () { return curve[0].a; }
  float &czx () { return curve[0].b; }
  float &cxy () { return curve[1].a; }
  float &czy () { return curve[1].b; }
  float &cxz () { return curve[2].a; }
  float &cyz () { return curve[2].b; }

  //TangentFrame xy(float amtX1Y){ return x().xclose( amtX1Y, pos(), y().pos() ); }
  //TangentFrame xyz(){ return }
};
}
}  //vsr::cga::

#endif /* ----- #ifndef vsr_tangent_INC  ----- */
