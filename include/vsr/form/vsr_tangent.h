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

#define TMP_FSIGN 1
#define TMP_PSIGN 1
#define TMP_FLIP false

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

  // not negated unless tv are defined no^t and not t^no
   void flatSurfaces ()
   {
     svu = svw = Inf(TMP_PSIGN) <= tv;
     suv = suw = Inf(TMP_PSIGN) <= tu;
     swu = swv = Inf(TMP_PSIGN) <= tw;
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

  /// Surface of constant u with curvature ku (simplified)
  DualSphere usurf (float ku)
  {
    //return DualSphere (Inf (-1) <= tu).spin (Gen::bst (tu * -ku / 2.0));
    return Normalize( (Inf (TMP_PSIGN) <= tu) * ((tu * ku) + 1));
  }

  /// Surface of constant v with curvature kv
  DualSphere vsurf (float kv)
  {
    //return DualSphere (Inf (-1) <= tv).spin (Gen::bst (tv * -kv / 2.0));
    return Normalize( (Inf (TMP_PSIGN) <= tv) * ((tv * kv) + 1));
  }

  /// Surface of constant w with curvature kw
  DualSphere wsurf (float kw)
  {
    //return DualSphere (Inf (-1) <= tw).spin (Gen::bst (tw * -kw / 2.0));
    return Normalize ((Inf (TMP_PSIGN) <= tw) * ((tw * kw) + 1));
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

  static DualSphere Normalize (const DualSphere&s ){
    bool is_flat = FERROR(s[3]);// == 0);
    float flip = (!is_flat && (s[3] < 0)) ? -1.0 : 1.0;
    return ((is_flat) ? DualSphere(s[0], s[1], s[2], 0, s[4]) : s/s[3]) * flip;
  }
  // some extra stuff here -- the pair log generators bringing surf to surf
  // not used
  static Pair CalcGen2 (const Point& p, const DualSphere& beg, const DualSphere&end)
  {
    auto ratio = (end/beg).tunit();
    bool isense = (p <= end)[0] > 0;
    bool iratio = ratio[0] < 0;;
    ratio *= iratio ? (isense ? -1.0 : 1.0) : 1.0;
//    ratio *= (isense ? -1.0 : 1.0);
    return Gen::log(ratio) / 2.0;
  }

  // reading this correctly:
  // starting tangent and ending tangent have similar relative senses to their spheres,
  // do not negate
  // if starting point is on the same side as ending tangent, use alt log
  static Pair CalcGen (const Pair& p1, const Pair& p2, const DualSphere& beg, const DualSphere&end)
  {
    auto ratio = (end/beg).tunit();
    bool flipA = (beg <= p1)[3] > FPERROR;
    bool flipB = (end <= p2)[3] > FPERROR;
    bool flipC =  ((Round::location(p1) <= end)[0] > FPERROR) ;
    //bool flipC =  ((Round::location(p1) <= end)[0] > FPERROR) ;
    bool flipD = flipA != flipB;
    //bool flipE = (flipC) && !flipD;
    bool flipE = flipC == flipB;
    float flip = ((flipD) ) ? -1.0 : 1.0;
    //return Gen::log(ratio * flip, flipC, true) / 2.0;
    //return Gen::log(ratio * flip, flipE, true) / 2.0;
    //return Gen::log(ratio * flip, flipC, true) / 2.0;
    //return Gen::log(ratio * flip, flipC, true) / 2.0;
    return Gen::log(ratio * flip, flipE, true) / 2.0;
  }

  static Pair NormalizePair (const Pair &pair)
  {
    return Pair(-Round::dir (pair).copy<Vec> ().unit ().copy<Tnv> ())
                 .trs (Round::location (pair));
  };
  // note surfaces() must have been called
  // this sweeps the v direction curve "over" along u
  Pair duv (const TFrame &tf){
    return CalcGen(tu, tf.tu, suv, tf.suv);
  }
  // this sweeps the w direction curve "over" along u
  Pair duw (const TFrame &tf){
    return CalcGen(tu, tf.tu, suw, tf.suw);
  }
  // this sweeps the u direction curve "up" along v
  Pair dvu (const TFrame &tf){
    return CalcGen(tv, tf.tv, svu, tf.svu);
  }
  // this sweeps the w direction curve "up" along v
  Pair dvw (const TFrame &tf){
    return CalcGen(tv, tf.tv, svw, tf.svw);
  }
  // this sweeps the u direction curve "in" along w
  Pair dwu (const TFrame &tf){
    //return Gen::log ((tf.swu/swu).tunit()) / 2.0;
    return CalcGen(tw, tf.tw, swu, tf.swu);
  }
  // this sweeps the v direction curve "in" along w
  Pair dwv (const TFrame &tf){
    //return Gen::log ((tf.swv/swv).tunit()) / 2.0;
    return CalcGen(tw, tf.tw, swv, tf.swv);
  }

};

//A frame and a few more info
struct TSection
{
  Pair tu, tv, tu1, tv1, tuv1, tvu1, logU, logV;
  DualSphere su0, sv0, su1, sv1;
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
       int mResU, mResV, mResW;

       Mapping (int resU, int resV, int resW) :
         mResU (resU), mResV (resV), mResW (resW) {
         mCon.resize(resU * resV * resW);
       }

       Con& at (int i, int j, int k){
         return mCon [i * mResV * mResW + j * mResW + k];
       }

       const Con& at (int i, int j, int k) const {
         return mCon [i * mResV * mResW + j * mResW + k];
       }

    };

    float mK[9];
    float mSpacing[3];
    TFrame mTFrame[8];
    Pair mGen[12];
    Boost mBoost[12];

    TVolume (){
      zeroInit();
    };

    TVolume (const TVolume& tv, Face face, float kA = 0.0, float kB = 0.0, float kC=0.0, float kD=0.0)
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
             tf() =  tv.uf();
             wf() =  tv.uwf();
             vf() =  tv.uvf();
             vwf() = tv.uvwf();

             kvu() = kA;
             kwu() = kB;
             kv1u() = kC;
             ku1w() = kD;

             calcSurfacesFromFace (Face::LEFT);

             break;
           }
         default:
           break;
      }
    };

    TVolume (const TVolume &tvol){
      mK[0] = tvol.kvu();
      mK[1] = tvol.kwu();
      mK[2] = tvol.kuv();
      mK[3] = tvol.kwv();
      mK[4] = tvol.kuw();
      mK[5] = tvol.kvw();
      mK[6] = tvol.kv1u();
      mK[7] = tvol.ku1w();
      mK[8] = tvol.kw1v();

      mSpacing[0] = tvol.uSpacing();
      mSpacing[1] = tvol.vSpacing();
      mSpacing[2] = tvol.wSpacing();

      calcSurfaces();
    }

//    TVolume& operator = (const TVolume &tvol)
//    {
//      return tvol;
//    }

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

    Point pos() const { return tf().pos(); }

    float& kvu(){ return mK[0]; }
    float& kwu(){ return mK[1]; }
    float& kuv(){ return mK[2]; }
    float& kwv(){ return mK[3]; }
    float& kuw(){ return mK[4]; }
    float& kvw(){ return mK[5]; }
    float& kv1u(){ return mK[6]; }
    float& ku1w(){ return mK[7]; }
    float& kw1v(){ return mK[8]; }

    const float& kvu() const { return mK[0]; }
    const float& kwu() const { return mK[1]; }
    const float& kuv() const { return mK[2]; }
    const float& kwv() const { return mK[3]; }
    const float& kuw() const { return mK[4]; }
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

    Boost& ruvw0() { return mBoost[0]; }
    Boost& ruwv0() { return mBoost[1]; }

    Boost& rvwu0() { return mBoost[2]; }
    Boost& rvuw0() { return mBoost[3]; }

    Boost& rwuv0() { return mBoost[4]; }
    Boost& rwvu0() { return mBoost[5]; }

    Boost& ruvw1() { return mBoost[6]; }
    Boost& ruwv1() { return mBoost[7]; }

    Boost& rvwu1() { return mBoost[8]; }
    Boost& rvuw1() { return mBoost[9]; }

    Boost& rwuv1() { return mBoost[10]; }
    Boost& rwvu1() { return mBoost[11]; }

    const Boost& ruvw0() const { return mBoost[0]; }
    const Boost& ruwv0() const { return mBoost[1]; }

    const Boost& rvwu0() const { return mBoost[2]; }
    const Boost& rvuw0() const { return mBoost[3]; }

    const Boost& rwuv0() const { return mBoost[4]; }
    const Boost& rwvu0() const { return mBoost[5]; }

    const Boost& ruvw1() const { return mBoost[6]; }
    const Boost& ruwv1() const { return mBoost[7]; }

    const Boost& rvwu1() const { return mBoost[8]; }
    const Boost& rvuw1() const { return mBoost[9]; }

    const Boost& rwuv1() const { return mBoost[10]; }
    const Boost& rwvu1() const { return mBoost[11]; }
    //calculats based on four frames
    void calcSurfacesFromFace(const Face& face)
    {
      switch (face){
        case Face::LEFT:
          {
            tf().suv = TFrame::Normalize(vf().pos() <= tf().tu  * TMP_FSIGN);
            tf().swv = TFrame::Normalize(vf().pos() <= tf().tw  * TMP_FSIGN);
            tf().suw = TFrame::Normalize(wf().pos() <= tf().tu  * TMP_FSIGN);
            tf().svw = TFrame::Normalize(wf().pos() <= tf().tv  * TMP_FSIGN);
            vf().suw = TFrame::Normalize(vwf().pos() <= vf().tu  * TMP_FSIGN);
            vf().svw = TFrame::Normalize(vwf().pos() <= vf().tv  * TMP_FSIGN);
            wf().suv = TFrame::Normalize(vwf().pos() <= wf().tu * TMP_FSIGN);
            wf().swv = TFrame::Normalize(vwf().pos() <= wf().tw * TMP_FSIGN);

            // Conformal Rotors along u,v,w curves, passing in two curvatures
            Con uc = tf().uc (kvu(), kwu(), uSpacing());

            uf() = tf().xf (uc, TMP_FLIP, false, false);

            tf().svu = tf().vsurf (kvu());
            tf().swu = tf().wsurf (kwu());
            vf().svu = vf().vsurf (kv1u());

            uf().suw = uf().usurf (ku1w());

            //ortho
            uf().suv = TFrame::Normalize(vf().svu <= uf().tu * TMP_FSIGN);
            uf().swv = TFrame::Normalize(vf().svu <= uf().tw * TMP_FSIGN);
            wf().swu = TFrame::Normalize(uf().suw <= wf().tw * TMP_FSIGN);
            wf().svu = TFrame::Normalize(uf().suw <= wf().tv * TMP_FSIGN);
            uf().svw = TFrame::Normalize(wf().swu <= uf().tv * TMP_FSIGN);
            vf().swu = TFrame::Normalize(uf().suv <= vf().tw * TMP_FSIGN);

            duvw0() = tf().duv (uf());
            duwv0() = tf().duw (uf());
            dvwu0() = tf().dvw (vf());
            dvuw0() = tf().dvu (vf());
            dwuv0() = tf().dwu (wf());
            dwvu0() = tf().dwv (wf());

            uvf() = vf().xf( Gen::boost(duvw0()), TMP_FLIP, false, false);
            uwf() = uf().xf( Gen::boost(dwuv0()), false, false, TMP_FLIP);

            //uvf() = vf().xf( Gen::boost(duvw0()), false, false, false);
            //uwf() = uf().xf( Gen::boost(dwuv0()), false, false, false);

            auto topPlane = vf().pos() ^ uvf().pos() ^ vwf().pos() ^ Inf(1);
            auto frontPlane = wf().pos() ^ vwf().pos() ^ uwf().pos() ^ Inf(1);
            auto rightPlane = uf().pos() ^ uwf().pos() ^ uvf().pos() ^ Inf(1);
            auto p = (topPlane.dual() ^ frontPlane.dual() ^ rightPlane.dual()).dual();
            p /= p[3];
            Point np = p.null();

            //these each have two defined surfaces
            uvf().suw = TFrame::Normalize(np <= uvf().tu * TMP_FSIGN);
            uvf().svw = TFrame::Normalize(np <= uvf().tv * TMP_FSIGN);
            vwf().svu = TFrame::Normalize(np <= vwf().tv * TMP_FSIGN);
            vwf().swu = TFrame::Normalize(np <= vwf().tw * TMP_FSIGN);
            uwf().swv = TFrame::Normalize(np <= uwf().tw * TMP_FSIGN);
            uwf().suv = TFrame::Normalize(np <= uwf().tu * TMP_FSIGN);

            //grab the tangent frame at np too
            //uvwf().tu = (np <= uvf().suw.undual()).dual();
            //uvwf().tv = (np <= uvf().svw.undual()).dual();
            //uvwf().tw = (np <= vwf().swu.undual()).dual();

            uvwf().tu = TFrame::NormalizePair(uvf().suw ^ np);
            uvwf().tv = TFrame::NormalizePair(uvf().svw ^ np);
            uvwf().tw = TFrame::NormalizePair(vwf().swu ^ np);
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

            //boosts
            ruvw0() = Gen::boost (duvw0());
            ruvw1() = Gen::boost (duvw1());

            ruwv0() = Gen::boost (duwv0());
            ruwv1() = Gen::boost (duwv1());

            rvuw0() = Gen::boost (dvuw0());
            rvuw1() = Gen::boost (dvuw1());

            rvwu0() = Gen::boost (dvwu0());
            rvwu1() = Gen::boost (dvwu1());

            rwuv0() = Gen::boost (dwuv0());
            rwuv1() = Gen::boost (dwuv1());

            rwvu0() = Gen::boost (dwvu0());
            rwvu1() = Gen::boost (dwvu1());

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
      uf() = tf().xf (uc, TMP_FLIP, false, false);
      vf() = tf().xf (vc, false, TMP_FLIP, false);
      wf() = tf().xf (wc, false, false, TMP_FLIP);

      // Here we can make three surfaces by bending
      // "Top Going Right"
      // "Right Going Forward"
      // "Front Going Up"
      vf().svu = vf().vsurf (kv1u());
      uf().suw = uf().usurf (ku1w());
      wf().swv = wf().wsurf (kw1v());

      //Now all other surfaces must be orthogonal to these
      //"Back Right Edge Going Up"
      //At u1, const u1 and const w0 in v dir are both ortho to const v1 in u dir
      //"Front Bottom Edge Going Over"
      //At w1, const w1 and const v0 in u dir are both ortho to const u1 in w dir
      //"Top Left Edge Going Forward"
      //At v1, const v1 and const u0 are both ortho to const w1 in v dir
      uf().suv = TFrame::Normalize(vf().svu <= uf().tu * TMP_FSIGN);
      uf().swv = TFrame::Normalize(vf().svu <= uf().tw * TMP_FSIGN);
      wf().swu = TFrame::Normalize(uf().suw <= wf().tw * TMP_FSIGN);
      wf().svu = TFrame::Normalize(uf().suw <= wf().tv * TMP_FSIGN);
      vf().svw = TFrame::Normalize(wf().swv <= vf().tv * TMP_FSIGN);
      vf().suw = TFrame::Normalize(wf().swv <= vf().tu * TMP_FSIGN);

      //now, what about the other surfaces to match the three bends?
      //"Bottom Going Forward"
      //"Back Going Right"
      //"Left Going Up"
      uf().svw = TFrame::Normalize(wf().swu <= uf().tv * TMP_FSIGN);
      vf().swu = TFrame::Normalize(uf().suv <= vf().tw * TMP_FSIGN);
      wf().suv = TFrame::Normalize(vf().svw <= wf().tu * TMP_FSIGN);

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
      uvf() = vf().xf( Gen::boost(duvw0()), TMP_FLIP, false, false);
      vwf() = wf().xf( Gen::boost(dvwu0()), false, TMP_FLIP, false);
      uwf() = uf().xf( Gen::boost(dwuv0()), false, false, TMP_FLIP);

      auto topPlane = vf().pos() ^ uvf().pos() ^ vwf().pos() ^ Inf(1);
      auto frontPlane = wf().pos() ^ vwf().pos() ^ uwf().pos() ^ Inf(1);
      auto rightPlane = uf().pos() ^ uwf().pos() ^ uvf().pos() ^ Inf(1);
      auto p = (topPlane.dual() ^ frontPlane.dual() ^ rightPlane.dual()).dual();
      p /= p[3];
      Point np = p.null();

      //these each have two defined surfaces
      uvf().suw = TFrame::Normalize(np <= uvf().tu * TMP_FSIGN);
      uvf().svw = TFrame::Normalize(np <= uvf().tv * TMP_FSIGN);
      vwf().svu = TFrame::Normalize(np <= vwf().tv * TMP_FSIGN);
      vwf().swu = TFrame::Normalize(np <= vwf().tw * TMP_FSIGN);
      uwf().swv = TFrame::Normalize(np <= uwf().tw * TMP_FSIGN);
      uwf().suv = TFrame::Normalize(np <= uwf().tu * TMP_FSIGN);

      //grab the tangent frame at np too
//      uvwf().tu = (np <= uvf().suw.undual()).dual();
//      uvwf().tv = (np <= uvf().svw.undual()).dual();
//      uvwf().tw = (np <= vwf().swu.undual()).dual();

      uvwf().tu = uvf().suw ^ np * TMP_FSIGN;
      uvwf().tv = uvf().svw ^ np * TMP_FSIGN;
      uvwf().tw = vwf().swu ^ np * TMP_FSIGN;
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

      //boosts
      //
      ruvw0() = Gen::boost (duvw0());
      ruvw1() = Gen::boost (duvw1());

      ruwv0() = Gen::boost (duwv0());
      ruwv1() = Gen::boost (duwv1());

      rvuw0() = Gen::boost (dvuw0());
      rvuw1() = Gen::boost (dvuw1());

      rvwu0() = Gen::boost (dvwu0());
      rvwu1() = Gen::boost (dvwu1());

      rwuv0() = Gen::boost (dwuv0());
      rwuv1() = Gen::boost (dwuv1());

      rwvu0() = Gen::boost (dwvu0());
      rwvu1() = Gen::boost (dwvu1());
    }

   // get the logUV surfaces at depth tw
   TSection tensorAt (float tw){
      // Back to Front (LEFT)
      Boost wvu0 = Gen::bst (dwvu0() * tw);

      // Bottom to Top (LEFT)
      Boost vwu0 = rvwu0();//Gen::bst (dvwu0());

      // Bottom to Top (RIGHT)
      Boost vwu1 = rvwu1();// Gen::bst (dvwu1());

      // Left to Right (BOTTOM)
      Boost uwv0 = ruwv0();//Gen::bst (duwv0());

      // Left to Right (TOP)
      Boost uwv1 = ruwv1();//Gen::bst (duwv1());

      Pair tu = TFrame::NormalizePair(tf().tu.spin (wvu0));
      Pair tv = TFrame::NormalizePair(tf().tv.spin (wvu0));
      Pair tu1  = TFrame::NormalizePair(tu.spin (uwv0));
      Pair tv1  = TFrame::NormalizePair(tv.spin (vwu0));

      Pair tuv1 = TFrame::NormalizePair(tu1.spin (vwu1));
      Pair tvu1 = TFrame::NormalizePair(tv1.spin (uwv1));

      DualSphere su0 = TFrame::Normalize(vf().svw <= tu);
      DualSphere sv0 = TFrame::Normalize(uf().suw <= tv);

   //   DualSphere su1 = (sv0 <= tuv1);
   //   DualSphere sv1 = (su0 <= tvu1);
      DualSphere su1 = (uvf().svw <= tu1);
      DualSphere sv1 = (uvf().suw <= tv1);

      Pair logU = TFrame::CalcGen (tu, tu1, su0, su1);
      Pair logV = TFrame::CalcGen (tv, tv1, sv0, sv1);

      return {tu,tv,tu1,tv1,tuv1,tvu1,logU,logV,su0,sv0,su1,sv1};
   }

   Con calcMappingAt (float ti, float tj, float tk){

      // Back to Front (LEFT)
      Boost wvu0 = Gen::bst (dwvu0() * tk);

      // Bottom to Top (LEFT)
      Boost vwu0 = rvwu0();//Gen::bst (dvwu0());

      // Bottom to Top (RIGHT)
      Boost vwu1 = rvwu1();// Gen::bst (dvwu1());

      // Left to Right (BOTTOM)
      Boost uwv0 = ruwv0();//Gen::bst (duwv0());

      // Left to Right (TOP)
      Boost uwv1 = ruwv1();//Gen::bst (duwv1());

      Pair tu = TFrame::NormalizePair(tf().tu.spin (wvu0));
      Pair tv = TFrame::NormalizePair(tf().tv.spin (wvu0));
      Pair tu1  = TFrame::NormalizePair(tu.spin (uwv0));
      Pair tv1  = TFrame::NormalizePair(tv.spin (vwu0));

      Pair tuv1 = TFrame::NormalizePair(tu1.spin (vwu1));
      Pair tvu1 = TFrame::NormalizePair(tv1.spin (uwv1));

      DualSphere su0 = TFrame::Normalize(vf().svw <= tu);
      DualSphere sv0 = TFrame::Normalize(uf().suw <= tv);

      DualSphere su1 = (sv0 <= tuv1);
      DualSphere sv1 = (su0 <= tvu1);

      Pair logU = TFrame::CalcGen (tu, tu1, su0, su1);
      Pair logV = TFrame::CalcGen (tv, tv1, sv0, sv1);

      return Gen::bst(logV *tj ) * Gen::bst (logU *ti) * wvu0;

    }

   Mapping calcMapping (int resU, int resV, int resW)
   {
     Mapping result (resU, resV, resW);

     // Bottom to Top (LEFT)
     Boost vwu0 = Gen::bst (dvwu0());

     // Bottom to Top (RIGHT)
     Boost vwu1 = Gen::bst (dvwu1());

     // Left to Right (BOTTOM)
     Boost uwv0 = Gen::bst (duwv0());

     // Left to Right (TOP)
     Boost uwv1 = Gen::bst (duwv1());

     for (int i = 0; i < resW; ++i)
     {
        float ti = (float)i/(resW-1);

        // Back to Front (LEFT)
        Boost wvu0 = Gen::bst (dwvu0() * ti);

        Pair tu = TFrame::NormalizePair(tf().tu.spin (wvu0));
        Pair tv = TFrame::NormalizePair(tf().tv.spin (wvu0));
        Pair tu1  = TFrame::NormalizePair(tu.spin (uwv0));
        Pair tv1  = TFrame::NormalizePair(tv.spin (vwu0));
        Pair tuv1 = TFrame::NormalizePair(tu1.spin (vwu1));
        Pair tvu1 = TFrame::NormalizePair(tv1.spin (uwv1));

        DualSphere su0 = TFrame::Normalize(vf().svw <= tu);
        DualSphere sv0 = TFrame::Normalize(uf().suw <= tv);
        DualSphere su1 = (sv0 <= tuv1);
        DualSphere sv1 = (su0 <= tvu1);

        Pair logU = TFrame::CalcGen (tu, tu1, su0, su1);
        Pair logV = TFrame::CalcGen (tv, tv1, sv0, sv1);

        for (int j = 0; j < resU; ++j){
          float tj = (float)j/(resU-1);
          Boost bu = Gen::bst (logU * tj);
          for (int k = 0; k < resV; ++k){
            float tk = (float)k/(resV-1);
            Boost bv = Gen::bst (logV * tk);

            Con Kw = bv * bu * wvu0;
            result.at (j,k,i) = Kw;
          }
        }
     }

     return result;
   }

   Con calcMappingAt2 (float ti, float tj, float tk){
      // Back to Front
      Boost wvu0 = Gen::bst (dwvu0() * tk);
      Boost wvu1 = Gen::bst (dwvu1() * tk);
      Boost wuv0 = Gen::bst (dwuv0() * tk);
      Boost wuv1 = Gen::bst (dwuv1() * tk);

      DualSphere su0v = TFrame::Normalize(tf().suv.spin (wvu0));
      DualSphere su1v = TFrame::Normalize(uf().suv.spin (wvu1));
      DualSphere sv0u = TFrame::Normalize(tf().svu.spin (wuv0));
      DualSphere sv1u = TFrame::Normalize(vf().svu.spin (wuv1));

      auto u0 = (tf().tu.spin (wvu0));
      auto u1 = (uf().tu.spin (wvu1));
      Pair duv = TFrame::CalcGen (u0, u1, su0v, su1v);
      auto v0 = (tf().tv.spin (wuv0));
      auto v1 = (vf().tv.spin (wuv1));
      Pair dvu = TFrame::CalcGen (v0, v1, sv0u, sv1u);

      Boost uv = Gen::bst(duv * ti);
      Boost vu = Gen::bst(dvu * tj);

      return vu * uv * wuv0;//wvu0;
   }

   Mapping calcMapping2 (int resU, int resV, int resW)
   {

     Mapping result (resU, resV, resW);

     for (int i = 0; i < resW; ++i)
     {
        float ti = (float)i/(resW-1);

        // Back to Front (left and right)
        Boost wvu0 = Gen::bst (dwvu0() * ti);
        Boost wvu1 = Gen::bst (dwvu1() * ti);

        // Spheres left and right
        DualSphere su0v = TFrame::Normalize(tf().suv.spin (wvu0));
        DualSphere su1v = TFrame::Normalize(uf().suv.spin (wvu1));

        auto u0 = TFrame::NormalizePair(tf().tu.spin (wvu0));
        auto u1 = TFrame::NormalizePair(uf().tu.spin (wvu1));
        Pair duv = TFrame::CalcGen (u0, u1, su0v, su1v);

        // Point p
        Point upv0 = Round::location (tf().pos().spin(wvu0));
        Point upv1 = Round::location (vf().pos().spin(wvu0));
        //inverse map
        float tv0 = inverseMapping (upv0, TVolume::Face::BOTTOM).w;
        float tv1 = inverseMapping (upv1, TVolume::Face::TOP).w;

        // Back to Front (bottom and top)
        Boost wuv0 = Gen::bst (dwuv0() * tv0);
        Boost wuv1 = Gen::bst (dwuv1() * tv1);

        auto v0 = TFrame::NormalizePair(tf().tv.spin (wuv0));
        auto v1 = TFrame::NormalizePair(vf().tv.spin (wuv1));

        DualSphere sv0u = TFrame::Normalize(tf().svu.spin (wuv0));
        DualSphere sv1u = TFrame::Normalize(vf().svu.spin (wuv1));

        Pair dvu = TFrame::CalcGen (v0, v1, sv0u, sv1u);

       for (int j = 0; j < resU; ++j)
       {
         float tj = (float)j/(resU-1);
         Boost uv = Gen::bst(duv * tj);

         for (int k = 0; k < resV; ++k)
         {
           float tk = (float)k/(resV-1);
           Boost vu = Gen::bst(dvu * tk);

           Con Kw = vu * uv * wvu0;
           result.at (j,k,i) = Kw;
         }
       }
     }

     return result;
   }

   Mapping calcMapping3 (int resU, int resV, int resW)
   {

     Mapping result (resU, resV, resW);

     for (int i = 0; i < resW; ++i)
     {
        float ti = (float)i/(resW-1);

        // left xf
        Boost wvu0 = Gen::bst (dwvu0() * ti);
        // left sphere
        DualSphere su0v = TFrame::Normalize(tf().suv.spin (wvu0));
        // left tu tangents
        auto tu0v0 = TFrame::NormalizePair(tf().tu.spin (wvu0));
        auto tu0v1 = TFrame::NormalizePair(vf().tu.spin (wvu0));
        // left point
        Point pu0v0 = Round::location (tu0v0);
        Point pu0v1 = Round::location (tu0v1);
        //bottom inverse map
        float cu0v0 = inverseMapping (pu0v0, TVolume::Face::BOTTOM).w;
        //top inverse map
        float cu0v1 = inverseMapping (pu0v1, TVolume::Face::TOP).w;

        // bottom xf
        Boost wuv0 = Gen::bst (dwuv0() * cu0v0);
        // top xf
        Boost wuv1 = Gen::bst (dwuv1() * cu0v1);
        // bottom sphere
        DualSphere sv0u = TFrame::Normalize(tf().svu.spin (wuv0));
        // top sphere
        DualSphere sv1u = TFrame::Normalize(vf().svu.spin (wuv1));
        // bottom left tv tangents
        auto tv0u0 = TFrame::NormalizePair(tf().tv.spin (wuv0));
        // top left tv tangents
        auto tv1u0 = TFrame::NormalizePair(vf().tv.spin (wuv1));
        // bottom right tu tangents
        auto tu1v0 = TFrame::NormalizePair(uf().tu.spin (wuv0));
        // top right tu tangents
        auto tu1v1 = TFrame::NormalizePair(uvf().tu.spin (wuv1));

        //right point
        Point pu1v0 = Round::location (tu1v0);
        Point pu1v1 = Round::location (tu1v1);
        //right inverse map
        float cu1v0 = inverseMapping (pu1v0, TVolume::Face::RIGHT).w;
        //right inverse map
        float cu1v1 = inverseMapping (pu1v1, TVolume::Face::RIGHT).w;

        // right xf
        Boost wvu1 = Gen::bst (dwvu1() * cu1v0);
        // right sphere
        DualSphere su1v = TFrame::Normalize (uf().suv.spin (wvu1));

        Pair duv = TFrame::CalcGen (tu0v0, tu1v0, su0v, su1v);
        Pair dvu = TFrame::CalcGen (tv0u0, tv1u0, sv0u, sv1u);

        //cout << cu1v0 << " " << cu1v1 << endl;

       for (int j = 0; j < resU; ++j)
       {
         float tj = (float)j/(resU-1);
         Boost uv = Gen::bst(duv * tj);

         for (int k = 0; k < resV; ++k)
         {
           float tk = (float)k/(resV-1);
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



   static Point IMapVal (const Pair & pa, const Pair& pb, const DualSphere&a,
       const DualSphere &b){
            return Ori(-1) <= TFrame::CalcGen(pa, pb, a, b);
   }

   Coord inverseMapping (const Point &p, const Face& face)
   {
     auto imapval = [](const Pair& pa, const Pair& pb,
         const DualSphere& a, const DualSphere& b){
            return Ori(-1) <= TFrame::CalcGen(pa, pb, a, b);;
       };

     switch (face){
       case Face::LEFT:
         {
            auto sw = TFrame::Normalize(-p <= dwvu0());
            auto swt = TFrame::NormalizePair(sw ^ p);
            auto wpair = imapval (tf().tw, swt, tf().swv, sw);
            auto wpair2 = imapval (tf().tw, wf().tw, tf().swv, wf().swv);
            auto tw = wpair <= !wpair2;
            float fw = FERROR(tw[0]) ? 0 : tw[0];

//            auto sv = TFrame::Normalize(-p <= dvwu0());
//            auto svt = TFrame::NormalizePair(sv ^ p);
//            auto vpair  = imapval(tf().tv, svt,  tf().svw, sv);
//            auto vpair2 = imapval(tf().tv, vf().tv, tf().svw, vf().svw);

           TSection ts = tensorAt (fw);
           auto sv = TFrame::Normalize(-p <= ts.logV);
           auto svt = TFrame::NormalizePair(sv ^ p);

           auto vpair  = IMapVal(ts.tv, svt,  ts.sv0, sv);
           auto vpair2 = IMapVal(ts.tv, ts.tv1, ts.sv0, ts.sv1);

           auto tv = vpair <= !vpair2;

            float fv = FERROR(tv[0]) ? 0 : tv[0];

            return {0.0, fv, fw};
           break;
         }
       case Face::RIGHT:
         {
            auto sv = TFrame::Normalize(-p <= dvwu1());
            auto sw = TFrame::Normalize(-p <= dwvu1());

            auto svt = TFrame::NormalizePair(sv ^ p);
            auto swt = TFrame::NormalizePair(sw ^ p);

            auto vpair  = imapval(uf().tv, svt,  uf().svw, sv);
            auto vpair2 = imapval(uf().tv, uvf().tv, uf().svw, uvf().svw);
            auto tv = vpair <= !vpair2;

            auto wpair = imapval (uf().tw, swt, uf().swv, sw);
            auto wpair2 = imapval (uf().tw, uwf().tw, uf().swv, uwf().swv);
            auto tw = wpair <= !wpair2;

            float fv = FERROR(tv[0]) ? 0 : tv[0];
            float fw = FERROR(tw[0]) ? 0 : tw[0];

            return {1.0, fv, fw};
           break;
         }
       case Face::BACK:
         {
           auto sv = TFrame::Normalize(-p <= dvuw0());
           auto su = TFrame::Normalize(-p <= duvw0());

           auto svt = TFrame::NormalizePair(sv ^ p);
           auto sut = TFrame::NormalizePair(su ^ p);

           auto vpair  = imapval(tf().tv, svt,  tf().svu, sv);
           auto vpair2 = imapval(tf().tv, vf().tv, tf().svu, vf().svu);
           auto tv = vpair <= !vpair2;

           auto upair = imapval (tf().tu, sut, tf().suv, su);
           auto upair2 = imapval (tf().tu, uf().tu, tf().suv, uf().suv);
           auto tu = upair <= !upair2;

           float fv = FERROR(tv[0]) ? 0 : tv[0];
           float fu = FERROR(tu[0]) ? 0 : tu[0];

           return {fu, fv, 1.0};
           break;
         }
       case Face::BOTTOM:
         {
            auto su = TFrame::Normalize(-p <= duwv0());
            auto sw = TFrame::Normalize(-p <= dwuv0());

            auto sut = TFrame::NormalizePair(su ^ p);
            auto swt = TFrame::NormalizePair(sw ^ p);

            auto upair  = imapval(tf().tu, sut,  tf().suw, su);
            auto upair2 = imapval(tf().tu, uf().tu, tf().suw, uf().suw);
            auto tu = upair <= !upair2;

            auto wpair = imapval (tf().tw, swt, tf().swu, sw);
            auto wpair2 = imapval (tf().tw, wf().tw, tf().swu, wf().swu);
            auto tw = wpair <= !wpair2;

            float fu = FERROR(tu[0]) ? 0 : tu[0];
            float fw = FERROR(tw[0]) ? 0 : tw[0];

            return {fu, 0.0, fw};
         }
       case Face::TOP:
         {
            auto su = TFrame::Normalize(-p <= duwv1());
            auto sw = TFrame::Normalize(-p <= dwuv1());

            auto sut = TFrame::NormalizePair(su ^ p);
            auto swt = TFrame::NormalizePair(sw ^ p);

            auto upair  = imapval(vf().tu, sut,  vf().suw, su);
            auto upair2 = imapval(vf().tu, uvf().tu, vf().suw, uvf().suw);
            auto tu = upair <= !upair2;

            auto wpair = imapval (vf().tw, swt, vf().swu, sw);
            auto wpair2 = imapval (vf().tw, vwf().tw, vf().swu, vwf().swu);
            auto tw = wpair <= !wpair2;


            float fu = FERROR(tu[0]) ? 0 : tu[0];
            float fw = FERROR(tw[0]) ? 0 : tw[0];

            return {fu, 1.0, fw};
         }
       case Face::FRONT:
         {
            auto sv = TFrame::Normalize(-p <= dvuw1());
            auto su = TFrame::Normalize(-p <= duvw1());

            auto svt = TFrame::NormalizePair(sv ^ p);
            auto sut = TFrame::NormalizePair(su ^ p);

            auto vpair  = imapval(wf().tv, svt,  wf().svu, sv);
            auto vpair2 = imapval(wf().tv, vwf().tv, wf().svu, vwf().svu);
            auto tv = vpair <= !vpair2;

            auto upair = imapval (wf().tu, sut, wf().suv, su);
            auto upair2 = imapval (wf().tu, uwf().tu, wf().suv, uwf().suv);
            auto tu = upair <= !upair2;

            float fv = FERROR(tv[0]) ? 0 : tv[0];
            float fu = FERROR(tu[0]) ? 0 : tu[0];

            return {fu, fv, 1.0};

            break;
         }
       default:
            return {0.0, 0.0, 0.0};

     }

   }

   //simplifiers
   /// Point at index u, v, w
   Point pointAt (int u, int v, int w, const Mapping& mapping)
   {
      return Round::location(pos().spin(mapping.at(u,v,w)));
   }

   /// Point at coordinate
   Point pointAt (float u, float v, float w)
   {
      return Round::location(pos().spin(calcMappingAt(u,v,w)));
   }
   /// Point at coordinate
   Point pointAt (const Coord& coord)
   {
      return Round::location(pos().spin(calcMappingAt(coord.u,coord.v,coord.w)));
   }
};

/// 3D Frame of Tangent Vectors (Point Pairs)
//
// unused, use TFrame instead
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
            Round::direction (bitan[idx]).copy<Biv> ().tunit ().copy<Tnb> ())
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

#undef TMP_FSIGN
#undef TMP_PSIGN
#undef TMP_FLIP
}
}  //vsr::cga::

#endif /* ----- #ifndef vsr_tangent_INC  ----- */
