/*
 * =====================================================================================
 *
 *       Filename:  vsr_cyclide.h
 *
 *    Description:  cyclidic surface patches, surface nets, volume patches, and volume nets
 *
 *        Version:  1.0
 *        Created:  07/07/2015 12:48:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#ifndef vsr_cyclide_INC
#define vsr_cyclide_INC

#include <vsr/form/vsr_tangent.h>

namespace vsr {
namespace cga {

/// Cyclide Principal Patch (four tangent frames)
/// @todo rename to just Cyclide
struct CyclideQuad
{

  /// Four TangentFrames
  TangentFrame tframe[4];

  /// Curvature tensors across surface
  Pair mLogU, mLogV;

  /// Curvature tensors in normal direction
  Pair mLogUz, mLogVz;

  Pair mLogVx;
  Pair mLogUy;

  /// Get/Set ith TangentFrame
  TangentFrame &operator[] (int idx) { return tframe[idx]; }
  /// Get ith TangentFrame
  TangentFrame operator[] (int idx) const { return tframe[idx]; }

  /// Default Constructor
  CyclideQuad () {}

  /// Construct from four Point positions
  CyclideQuad (const Point &a, const Point &b, const Point &c, const Point &d)
  {
    pos (a, b, c, d);
  }

  /// Construct from four Contact elements, and a rotation
  CyclideQuad (const Contact &a, const Contact &b, const Contact &c,
               const Contact &d, float spin = 0)
  {
    set (a, b, c, d, spin);
  }

  /// Construct from four TangentFrames elements
  CyclideQuad (const TangentFrame &ta, const TangentFrame &tb,
               const TangentFrame &tc, const TangentFrame &td)
  {
    set (ta, tb, tc, td);
  }

  void set (const TangentFrame &ta, const TangentFrame &tb,
            const TangentFrame &tc, const TangentFrame &td)
  {
    tframe[0] = ta.unit ();
    tframe[1] = tb.unit ();
    tframe[2] = tc.unit (), tframe[3] = td.unit ();
    calcSurfaces ();
    log ();
  }

  void set (const Contact &a, const Contact &b, const Contact &c,
            const Contact &d, float spin = 0)
  {
    pos (a.point, b.point, c.point, d.point);


    Rot rot = Gen::ratio (Vec::z, a.vec ());
    Rot rot2 = Gen::rot (Biv::xy.spin (rot) * spin);

    calcFrame (TangentFrame (Frame (a.point, rot2)), false, false, false, false,
               0);

    log ();
  }


  void pos (const Point &a, const Point &b, const Point &c, const Point &d)
  {
    tframe[0].pos () = a;
    tframe[1].pos () = b;
    tframe[2].pos () = c;
    tframe[3].pos () = d;
  }

  /// unused CommonSphere
  DualSphere commonSphere () { return (mLogU ^ mLogV).dual (); }

  /// Circle foundation of principal patch
  Circle circle () const
  {
    return tframe[0].pos () ^ tframe[1].pos () ^ tframe[2].pos ();
  }

  /// Calculate Coordinate surfaces based on positions
  void calcFrame ()
  {

    //FIRST ONE
    tframe[0].bitan[0] = tframe[0].tx ().undual ();
    tframe[0].bitan[1] = tframe[0].ty ().undual ();
    tframe[0].bitan[2] = tframe[0].tz ().undual ();

    for (int j = 0; j < 3; ++j)
      {
        tframe[0].sphere[j] = tframe[0].bitan[j] ^ tframe[1].pos ();
        tframe[0].tan[j] = tframe[0].bitan[j].dual ();
      }

    // Subsequent
    for (int i = 1; i < 4; ++i)
      {
        int next = (i < 3) ? i + 1 : 0;
        for (int j = 0; j < 3; ++j)
          {
            tframe[i].bitan[j] =
              Tangent::at (tframe[i - 1].sphere[j], tframe[i].pos ());
            tframe[i].sphere[j] = tframe[i].bitan[j] ^ tframe[next].pos ();
            tframe[i].tan[j] = tframe[i].bitan[j].dual ();
          }
      }
  }

  /// TangentFrames are set, now calculate surfaces
  void calcSurfaces ()
  {
    for (int i = 0; i < 4; ++i)
      {
        int next = (i < 3) ? i + 1 : 0;
        for (int j = 0; j < 3; ++j)
          {
            tframe[i].sphere[j] = tframe[i].bitan[j] ^ tframe[next].pos ();
          }
      }
  }

  /// Calculate frame positions using new formula
  /// tan[0], bitan[0] are used to construct sphere[0] surfaces of constant u
  /// tan[1], bitan[1] are used to construct sphere[1] surfaces of constant v
  /// tan[2], bitan[2] are used to construct sphere[2] surfaces of constant w
  void calcFrame2 ()
  {

    // First frame
    tframe[0].tan[0] = tframe[0].tx ();
    tframe[0].tan[1] = tframe[0].ty ();
    tframe[0].tan[2] = tframe[0].tz ();

    for (int j = 0; j < 3; ++j)
      {
        tframe[0].bitan[j] = tframe[0].tan[j].undual ();
        tframe[0].sphere[j] = tframe[0].bitan[j] ^ tframe[1].pos ();
      }

    // Subsequent frames
    for (int i = 1; i < 4; ++i)
      {
        int next = (i < 3) ? i + 1 : 0;
        for (int j = 0; j < 3; ++j)
          {
            tframe[i].bitan[j] =
              Tangent::at (tframe[i - 1].sphere[j], tframe[i].pos ());
            tframe[i].sphere[j] = tframe[i].bitan[j] ^ tframe[next].pos ();
            tframe[i].tan[j] = tframe[i].bitan[j].dual ();
          }
      }
    //reorient sphere normals if necessary
    for (int i = 0; i < 4; ++i)
      {
        for (int j = 0; j < 3; ++j)
          {
            auto loc = Round::location (tframe[i].sphere[j].dual ());
            auto rad = Round::radius (tframe[i].sphere[j].dual ());
            tframe[i].sphere[j] = Construct::sphere (loc, rad).undual ();
          }
      }
  }

  /// Which direction to interpolate (if corner is outside sphere, then pass fact to Gen::log)
  bool altU ()
  {
    auto dotU = (tframe[0].pos () <= tframe[2].sphere[0].dual ())[0];
    return dotU > 0;
  }

  /// Which direction to interpolate
  bool altV ()
  {
    auto dotV = (tframe[0].pos () <= tframe[1].sphere[1].dual ())[0];
    return dotV > 0;
  }

  /// Which direction to interpolate
  bool altXv ()
  {
    auto dotU = (tframe[0].pos () <= tframe[1].sphere[0].dual ())[0];
    return dotU > 0;
  }

  // Which direction to interpolate
  bool altYu ()
  {
    auto dotU = (tframe[0].pos () <= tframe[2].sphere[1].dual ())[0];
    return dotU > 0;
  }

  bool altWU ()
  {
    auto dotW = (tframe[0].pos () <= tframe[2].sphere[2].dual ())[0];
    return dotW > 0;
  }

  bool altWV ()
  {
    auto dotW = (tframe[0].pos () <= tframe[1].sphere[2].dual ())[0];
    return dotW > 0;
  }

  /// Calculate logs based on ratios of constant coordinate surface spheres
  /// @todo this alt works, but why?  in scratch cases, only works without Gen::ratio
  /// i.e. directly with (-b/a).runit()
  void log ()
  {

    /// mLogU and mLogV are what we typically use
    // log in u direction -- ratio of surfaces of constant x (sphere[0])
    mLogU = Gen::log (Gen::ratio (tframe[0].sphere[0].dual (),
                                  tframe[2].sphere[0].dual (), true),
                      altU ());
    // mLogU = Gen::log( - tframe[2].sphere[0].dual() / tframe[0].sphere[0].dual() ), altU() );

    // log in v direction -- ratio of surfaces of constant y (sphere[1])
    mLogV = Gen::log (Gen::ratio (tframe[3].sphere[1].dual (),
                                  tframe[1].sphere[1].dual (), true),
                      altV ());

    /// mLogUy and mLogVx are alternatives
    // log in u directrion (across constant y)
    mLogUy = Gen::log (Gen::ratio (tframe[0].sphere[1].dual (),
                                   tframe[2].sphere[1].dual (), true),
                       altYu ());
    // log in v direction  (across constant x)
    mLogVx = Gen::log (Gen::ratio (tframe[3].sphere[0].dual (),
                                   tframe[1].sphere[0].dual (), true),
                       altXv ());


    // Calc log of normals (third direction)
    logZ ();
  }

  /// Calculates Curvature across normals in U and V direction
  /// @todo is this used?  prob not
  void logZ ()
  {
    mLogUz = Gen::log (Gen::ratio (tframe[0].sphere[2].dual (),
                                   tframe[2].sphere[2].dual (), true),
                       altWU ());
    mLogVz = Gen::log (Gen::ratio (tframe[3].sphere[2].dual (),
                                   tframe[1].sphere[2].dual (), true),
                       altWV ());
  }

  /// Generate simple Rotor in U direction
  Bst xfu (VSR_PRECISION u) const { return Gen::bst (mLogU * u); }

  /// Generate simple Rotor in V direction
  Bst xfv (VSR_PRECISION v) const { return Gen::bst (mLogV * v); }

  /// Generate simple Rotor in U direction (of z surface)
  Bst xfuz (VSR_PRECISION wu) const { return Gen::bst (mLogUz * wu); }
  /// Generate simple Rotor in U direction (of z surface)
  Bst xfvz (VSR_PRECISION wv) const { return Gen::bst (mLogVz * wv); }

  Bst xfvx (VSR_PRECISION amt) const { return Gen::bst (mLogVx * amt); }
  Bst xfuy (VSR_PRECISION amt) const { return Gen::bst (mLogUy * amt); }

  /// @todo unknown use, possibly eliminate
  Pair ortho (VSR_PRECISION u, VSR_PRECISION v)
  {
    return tframe[0].sphere[2].dual ().spin (xfuz (u))
           ^ tframe[3].sphere[2].dual ().spin (xfvz (v));
  }

  /// Generate compound conformal rotor at u,v
  Con xf (VSR_PRECISION u, VSR_PRECISION v) const { return xfv (v) * xfu (u); }

  /// Generate compound conformal rotor in alterate surfaces
  Con xfxz (VSR_PRECISION u, VSR_PRECISION v) const
  {
    return xfvz (v) * xfu (u);
  }
  Con xfyz (VSR_PRECISION u, VSR_PRECISION v) const
  {
    return xfuz (u) * xfv (v);
  }

  /// Circles in U direction
  Circle cirU (VSR_PRECISION u)
  {
    return tframe[0].calcCurve (1).spin (xfu (u));
  }
  /// Circles in V direction
  Circle cirV (VSR_PRECISION v)
  {
    return tframe[3].calcCurve (0).spin (xfv (v));
  }

  Circle cirW (VSR_PRECISION u, VSR_PRECISION v)
  {
    auto p = eval (u, v);
    auto parA = p <= mLogU;  // dual sphere
    auto parB = p <= mLogV;  // dual sphere

    //return (parA ^ parB).dual();
    auto pair = p <= (parA ^ parB).dual ();  //pair
    return pair ^ commonSphere ();
  }

  Circle cirWB (VSR_PRECISION u, VSR_PRECISION v)
  {
    auto p = eval (u, v);
    auto parA = p <= mLogU;  // dual sphere
    auto parB = p <= mLogV;  // dual sphere

    return (parA ^ parB).undual ();
    //auto pair = p <= (parA^parB).dual(); //pair
    //return pair ^ commonSphere();
  }


  /// Apply conformal rotor to a type T at u,v
  template <class T>
  T apply (const T &t, VSR_PRECISION u, VSR_PRECISION v) const
  {
    return t.spin (xf (u, v));
  }
  //  /// Apply conformal rotor to a type T at u,v
  template <class T>
  T applyXZ (const T &t, VSR_PRECISION u, VSR_PRECISION v) const
  {
    return t.spin (xfxz (u, v));
  }
  /// Apply conformal rotor to a type T at u,v
  template <class T>
  T applyYZ (const T &t, VSR_PRECISION u, VSR_PRECISION v) const
  {
    return t.spin (xfyz (u, v));
  }

  /// Apply conformal rotor to a point at u,v
  Point evalXZ (VSR_PRECISION u, VSR_PRECISION v) const
  {
    return Round::loc (tframe[0].pos ().spin (xfxz (u, v)));
  }
  /// Apply conformal rotor to a point at u,v
  Point evalYZ (VSR_PRECISION u, VSR_PRECISION v) const
  {
    return Round::loc (tframe[0].pos ().spin (xfyz (u, v)));
  }

  // Get normal at u,v
  Vec evalNormal (VSR_PRECISION u, VSR_PRECISION v)
  {
    auto tan = apply (tframe[0].tan[2], u, v);
    return -Round::direction (tan).copy<Vec> ().unit ();
  }

  /// Apply conformal rotor to a point at u,v
  Point eval (VSR_PRECISION u, VSR_PRECISION v) const
  {
    return Round::loc (tframe[0].pos ().spin (xf (u, v)));
  }

  /// offset CyclideQuad in third dimension by amt t, and curvatures cx,cy
  /// @todo test this
  CyclideQuad offset (float t, float cx, float cy)
  {
    //create Boost
    auto bst = tframe[0].xycurve (cx, cy);
    auto bstx = apply (bst, 1, 0);
    auto bsty = apply (bst, 0, 1);
    auto bstxy = apply (bst, 1, 1);

    auto pnt = tframe[0].pos ().translate (tframe[0].z () * t);
    auto bpnt = Round::loc (pnt.spin (bst));
    auto bpntx = Round::loc (pnt.spin (bstx));
    auto bpnty = Round::loc (pnt.spin (bsty));
    auto bpntxy = Round::loc (pnt.spin (bstxy));
    auto tf = TangentFrame (bpnt, tframe[0]);
    auto tfx = TangentFrame (bpntx, tframe[0]);
    auto tfy = TangentFrame (bpnty, tframe[0]);
    auto tfxy = TangentFrame (bpntxy, tframe[0]);

    return CyclideQuad (tf, tfx, tfxy, tfy);
    //  return CyclideQuad(bpnt,bpnt)
  }


  /// blend of two osculating spheres
  //static CyclideQuad Blend()

  /// set by attaching to TangentFrame f,
  /// set booleans: flipx, flipy, corner or continuous, 90 or -90, attach at idx of f)
  void calcFrame (const TangentFrame &f, bool bFlipX, bool bFlipY, bool bCorner,
                  bool bNeg, int begin)
  {

    if (bCorner)
      {

        if (bFlipY)
          {
            tframe[begin].bitan[1] = f.bitan[2] * (bNeg ? -1 : 1);
            tframe[begin].bitan[2] = -f.bitan[1];
            tframe[begin].bitan[0] = f.bitan[0];
          }
        if (bFlipX)
          {
            tframe[begin].bitan[0] = f.bitan[2] * (bNeg ? -1 : 1);
            tframe[begin].bitan[2] = -f.bitan[0];
            tframe[begin].bitan[1] = f.bitan[1];
          }
      }
    else
      {

        for (int i = 0; i < 3; ++i)
          {
            tframe[begin].bitan[i] =
              f.bitan[i];  // * ( (i&1)?1:-1);//.spin(rot);
          }

        if (bFlipY)
          tframe[begin].bitan[1] *= -1;
        if (bFlipX)
          tframe[begin].bitan[0] *= -1;
      }

    //first frame
    for (int j = 0; j < 3; ++j)
      {
        int next = begin < 3 ? begin + 1 : 0;
        tframe[begin].sphere[j] = tframe[begin].bitan[j] ^ tframe[next].pos ();
        tframe[begin].tan[j] = tframe[begin].bitan[j].dual ();
      }

    // Subsequent frames
    for (int i = 1; i < 4; ++i)
      {
        int cur = begin + i;
        cur = cur % 4;
        int prev = (cur > 0) ? cur - 1 : 3;
        int next = (cur < 3) ? cur + 1 : 0;
        for (int j = 0; j < 3; ++j)
          {
            tframe[cur].bitan[j] =
              Tangent::at (tframe[prev].sphere[j], tframe[cur].pos ());
            tframe[cur].sphere[j] = tframe[cur].bitan[j] ^ tframe[next].pos ();
            tframe[cur].tan[j] = tframe[cur].bitan[j].dual ();
          }
      }

    // edges
    //for (int i=0;i<4;++i) tframe[i].calcCurves();
  }


  // attach to current on side specified by int edge, and two booleans
  /// bCorner: whether to break orthogonally @todo change to bSmooth
  /// bNeg: whether to flip the direction
  CyclideQuad attach (const DualSphere &sphere, int edge, bool bCorner = false,
                      bool bNeg = false)
  {
    CyclideQuad &cyc = *this;  //mCyclide[idx];
    CyclideQuad c;
    switch (edge)
      {
        case 0:
          {
            bool bFlipX = true;
            bool bFlipY = false;
            auto cd =
              Contact (cyc.tframe[0].pos (), cyc.tframe[0].sphere[2].dual ());
            if (bNeg)
              cd.tnv *= -1;
            auto ca = Contact (cd, sphere);
            auto cc =
              Contact (cyc.tframe[1].pos (), cyc.tframe[0].sphere[2].dual ());
            if (bNeg)
              cc.tnv *= -1;
            auto cb = Contact (cc, sphere);
            c.pos (ca.point, cb.point, cc.point, cd.point);
            c.calcFrame (cyc.tframe[0], bFlipX, bFlipY, bCorner, bNeg, 3);
            c.log ();
            break;
          }
        case 1:
          {
            bool bFlipX = false;
            bool bFlipY = true;
            auto ca =
              Contact (cyc.tframe[1].pos (), cyc.tframe[0].sphere[2].dual ());
            auto cb = Contact (ca, sphere);
            auto cd =
              Contact (cyc.tframe[2].pos (), cyc.tframe[2].sphere[2].dual ());
            auto cc = Contact (cd, sphere);
            c.pos (ca.point, cb.point, cc.point, cd.point);
            c.calcFrame (cyc.tframe[1], bFlipX, bFlipY, bCorner, bNeg, 0);
            c.log ();
            break;
          }
        case 2:
          {
            bool bFlipX = true;
            bool bFlipY = false;
            auto ca =
              Contact (cyc.tframe[3].pos (), cyc.tframe[2].sphere[2].dual ());
            auto cd = Contact (ca, sphere);
            auto cb =
              Contact (cyc.tframe[2].pos (), cyc.tframe[2].sphere[2].dual ());
            auto cc = Contact (cb, sphere);
            c.pos (ca.point, cb.point, cc.point, cd.point);
            c.calcFrame (cyc.tframe[3], bFlipX, bFlipY, bCorner, bNeg, 0);
            c.log ();
            break;
          }
        case 3:
          {
            bool bFlipX = false;
            bool bFlipY = false;
            bFlipY = true;
            auto cb =
              Contact (cyc.tframe[0].pos (), cyc.tframe[0].sphere[2].dual ());
            auto ca = Contact (cb, sphere);
            auto cc =
              Contact (cyc.tframe[3].pos (), cyc.tframe[2].sphere[2].dual ());
            auto cd = Contact (cc, sphere);
            c.pos (ca.point, cb.point, cc.point, cd.point);
            c.calcFrame (cyc.tframe[0], bFlipX, bFlipY, bCorner, bNeg, 1);
            c.log ();
            break;
          }
      }

    return c;
  }

  //  /// Apply conformal rotor to a point at u,v amt w
  //  Point eval( VSR_PRECISION u, VSR_PRECISION v, VSR_PRECISION w) const {
  //    return Round::loc( tframe[0].pos().spin( xf(u,v,w) ) );
  //  }
};


/**
* @brief A net of cyclide principal patches
*/
struct CircularNet
{

  /// std::vector of CyclideQuads;
  vector<CyclideQuad> mCyclide;

  /// Number of Cyclides in Net
  int size () { return mCyclide.size (); }

  CircularNet (const CyclideQuad &c) { mCyclide.push_back (c); }

  CyclideQuad &operator[] (int idx) { return mCyclide[idx]; }
  CyclideQuad operator[] (int idx) const { return mCyclide[idx]; }

  //idx specifies cyclide, edge specifies which edge
  void addAt (const DualSphere &sphere, int idx, int edge, bool bCorner = false,
              bool bNeg = false)
  {
    CyclideQuad &cyc = mCyclide[idx];
    switch (edge)
      {
        case 0:
          {
            bool bFlipX = true;
            bool bFlipY = false;
            auto cd =
              Contact (cyc.tframe[0].pos (), cyc.tframe[0].sphere[2].dual ());
            if (bNeg)
              cd.tnv *= -1;
            auto ca = Contact (cd, sphere);
            auto cc =
              Contact (cyc.tframe[1].pos (), cyc.tframe[0].sphere[2].dual ());
            if (bNeg)
              cc.tnv *= -1;
            auto cb = Contact (cc, sphere);
            CyclideQuad c (ca.point, cb.point, cc.point, cd.point);
            c.calcFrame (cyc.tframe[0], bFlipX, bFlipY, bCorner, bNeg, 3);
            c.log ();
            mCyclide.push_back (c);
            break;
          }
        case 1:
          {
            bool bFlipX = false;
            bool bFlipY = true;
            auto ca =
              Contact (cyc.tframe[1].pos (), cyc.tframe[0].sphere[2].undual ());
            auto cb = Contact (ca, sphere);
            auto cd =
              Contact (cyc.tframe[2].pos (), cyc.tframe[2].sphere[2].undual ());
            auto cc = Contact (cd, sphere);
            CyclideQuad c (ca.point, cb.point, cc.point, cd.point);
            c.calcFrame (cyc.tframe[1], bFlipX, bFlipY, bCorner, bNeg, 0);
            c.log ();
            mCyclide.push_back (c);
            break;
          }
        case 2:
          {
            bool bFlipX = true;
            bool bFlipY = false;
            auto ca =
              Contact (cyc.tframe[3].pos (), cyc.tframe[2].sphere[2].dual ());
            auto cd = Contact (ca, sphere);
            auto cb =
              Contact (cyc.tframe[2].pos (), cyc.tframe[2].sphere[2].dual ());
            auto cc = Contact (cb, sphere);
            CyclideQuad c (ca.point, cb.point, cc.point, cd.point);
            c.calcFrame (cyc.tframe[3], bFlipX, bFlipY, bCorner, bNeg, 0);
            c.log ();
            mCyclide.push_back (c);
            break;
          }
        case 3:
          {
            bool bFlipX = false;
            bool bFlipY = false;
            bFlipY = true;
            auto cb =
              Contact (cyc.tframe[0].pos (), cyc.tframe[0].sphere[2].dual ());
            auto ca = Contact (cb, sphere);
            auto cc =
              Contact (cyc.tframe[3].pos (), cyc.tframe[2].sphere[2].dual ());
            auto cd = Contact (cc, sphere);
            CyclideQuad c (ca.point, cb.point, cc.point, cd.point);
            c.calcFrame (cyc.tframe[0], bFlipX, bFlipY, bCorner, bNeg, 1);
            c.log ();
            mCyclide.push_back (c);
            break;
          }
      }
  }

  void update ()
  {
    for (auto &i : mCyclide)
      {
        i.log ();
      }
  }
};

/// @todo check unit() length Tangents . . .
struct CyclidicVolume
{

  CyclideQuad cyclide[2];

  SixSphere mOrigin;

  struct Data
  {
    Point pos;
    Vec normal;
  };

  /// Default Constructor
  CyclidicVolume () { set(); }
  /// Construct from SixSphere coordinates
  CyclidicVolume (const SixSphere &o) : mOrigin (o) { set (); }

  /// Set origin and parallel curvature offsets
  CyclidicVolume &origin (const SixSphere &s, float tzx = 0, float tzy = 0,
                          float txy = 0)
  {
    mOrigin = s;
    set (tzx, tzy, txy);
    return *this;
  }

  SixSphere &origin () { return mOrigin; }

  /// set by curvature offsets
  void set (float tzx = 0.f, float tzy = 0.f, float txy = 0.f)
  {
    cyclide[0].set (mOrigin.frame, mOrigin.y (), mOrigin.xy (txy),
                    mOrigin.x ());
    cyclide[1].set (mOrigin.z (), mOrigin.zy (tzy), mOrigin.xyz (tzx, tzy),
                    mOrigin.zx (tzx));
  }

  /// Pass in starting frame, and curvatures @todo -- depecrate
  void
  setAbsolute (Frame f, float YX,
               float ZX,              // constant X=0 in the Y and Z directions
               float XY, float ZY,    // constant Y=0 in the X and Z directions
               float XZ, float YZ,    // constant Z=0 in the X and Y directions
               float Z1X, float Z1Y,  // constant Z=1 in the X and Y directions
               float X1Y,             // constant X=1 in the Y direction
               float lengthX, float lengthY, float lengthZ)
  {  // Length of Sides

    ///1. Starting TangentFrame
    mOrigin.frame = f;
    mOrigin.set (YX, ZX, XY, ZY, XZ, YZ, lengthX, lengthY, lengthZ);

    TangentFrame tf0 (f);

    ///2. Tangent frames in x, y, and z directions
    auto tfx = mOrigin.x ();  // formerlytf0.xbend(YX, ZX, lengthX);//
    auto tfy = mOrigin.y ();  // formerlytf0.ybend(XY, ZY, lengthY);//
    auto tfz = mOrigin.z ();  // formerlytf0.zbend(XZ, YZ, lengthZ);//

    ///3. Given three frames, cut through circle patch to find fourth
    TangentFrame tfxz =
      tfz.unit ().zclose (Z1X, mOrigin.frame.pos (), tfx.pos ());
    TangentFrame tfyz =
      tfz.unit ().zclose (Z1Y, mOrigin.frame.pos (), tfy.pos ());
    TangentFrame tfxy =
      tfx.unit ().xclose (X1Y, mOrigin.frame.pos (), tfy.pos ());
    TangentFrame tfxyz = tfxy.unit ().close (tfx, tfxz, tfy, tfyz);

    cyclide[0].set (mOrigin.frame, tfy, tfxy, tfx);
    cyclide[1].set (tfz, tfyz, tfxyz, tfxz);
  }

  CyclideQuad top ()
  {
    return {mOrigin.zy (), mOrigin.y (), mOrigin.xy (), mOrigin.xyz ()};
  }
  CyclideQuad bottom ()
  {
    return {mOrigin.z (), mOrigin.frame, mOrigin.x (), mOrigin.zx ()};
  }
  CyclideQuad left ()
  {
    return {mOrigin.frame, mOrigin.y (), mOrigin.zy (), mOrigin.z ()};
  }
  CyclideQuad right ()
  {
    return {mOrigin.zx (), mOrigin.xyz (), mOrigin.xy (), mOrigin.x ()};
  }


  /// Get log in z direction
  Pair zlog (float u)
  {
    auto sph1 = cyclide[0].apply (cyclide[0][0].sphere[2], u, 0).dual ();
    auto sph2 = cyclide[1].apply (cyclide[1][0].sphere[2], u, 0).dual ();
    auto tpos = cyclide[0].eval (u, 0);
    auto bAlt = (tpos <= sph2)[0] > 0;
    return Gen::log (Gen::ratio (sph1, sph2, true), bAlt);
  }

  /// Apply 3D Conformal Map to input type
  template <class T>
  T apply (const T &input, float u, float v, float w)
  {
    return input.spin (Gen::bst (zlog (u) * w) * cyclide[0].xf (u, v));
  }

  /// Get mapped point at coordinate u,v,w
  Point at (float u, float v, float w)
  {
    return Round::location (apply (cyclide[0][0].point (), u, v, w));
  }

  /// Get coordinate of 3D point m
  template <class T>
  void coordinate (const T &m)
  {
    auto pt = cyclide[0][0].point ();
    auto x = (m[0] - pt[0]) / mOrigin.lengthX;
    auto y = (m[1] - pt[1]) / mOrigin.lengthY;
    auto z = (m[2] - pt[2]) / mOrigin.lengthZ;
  }


  /// six faces worth of data (@todo move somewhere more general)
  vector<vector<Data>> faces (int w, int h, int d)
  {
    vector<vector<Data>> res (6);

    auto pt = cyclide[0][0].point ();
    auto normx = cyclide[0][0].tan[0];
    auto normy = cyclide[0][0].tan[1];
    auto normz = cyclide[0][0].tan[2];

    for (int i = 0; i <= w; ++i)
      {
        float tu = (float) i / w;
        auto log = zlog (tu);
        for (int j = 0; j <= h; ++j)
          {
            float tv = (float) j / h;
            auto uvbst = cyclide[0].xf (tu, tv);

            int numd;
            if (i == 0 || i == w || j == 0 || j == h)
              numd = d;
            else
              numd = 1;

            // calc position and normals and add to correct face
            for (int k = 0; k <= numd; ++k)
              {
                float tw = (float) k / numd;
                auto bst = Gen::bst (log * tw) * uvbst;
                auto tres = Round::location (pt.spin (bst));

                if (k == 0)
                  res[0].push_back (
                    {tres,
                     Round::direction (normz.spin (bst)).copy<Vec> ().unit ()});
                if (i == 0)
                  res[1].push_back (
                    {tres,
                     Round::direction (normx.spin (bst)).copy<Vec> ().unit ()});
                if (j == 0)
                  res[2].push_back (
                    {tres,
                     Round::direction (normy.spin (bst)).copy<Vec> ().unit ()});
                if (k == numd)
                  res[3].push_back ({tres, -Round::direction (normz.spin (bst))
                                              .copy<Vec> ()
                                              .unit ()});
                if (i == w)
                  res[4].push_back ({tres, -Round::direction (normx.spin (bst))
                                              .copy<Vec> ()
                                              .unit ()});
                if (j == h)
                  res[5].push_back ({tres, -Round::direction (normy.spin (bst))
                                              .copy<Vec> ()
                                              .unit ()});
              }
          }
      }
    return res;
  }

  /// Create by attaching another volume to side
  /// in drawing below, numbers are faces to attach to (0:x, 1:y, 2:z, 3:-x ...)
  /// and letters are vertices (back, clockwise, then front, clockwise)
  ///   b----------c
  ///  /.   1     /|
  /// / .     5  / |
  /// f----------g  |
  ///3|   a . . .|0 d
  /// |    2     | /
  /// |          |/
  ///  e----------h
  ///        4

  CyclidicVolume attach (int edge, float da, float db, float dc, float dd,
                         float dist = 3)
  {
    CyclidicVolume cv;
    //.......... a  d  b  e  c   h   f   g
    TangentFrame p, x, y, z, xy, xz, yz, xyz;

    switch (edge)
      {
        case 0:
          {

            //attach to right
            float dyx = da;
            float dzx = db;
            float dz1x = dc;
            float dx1y = -dd;

            p = cyclide[0][3];
            p.flip (0);
            x = p.xbend (dyx, dzx, dist);
            y = cyclide[0][2];
            y.flip (0);
            z = cyclide[1][3];
            z.flip (0);
            yz = cyclide[1][2];
            yz.flip (0);

            xz = z.zclose (dz1x, p.pos (), x.pos ());
            xy = x.xclose (dx1y, p.pos (), y.pos ());
            xyz = xy.close (x, xz, y, yz);

            cv.cyclide[0].set (p, y, xy, x);
            cv.cyclide[1].set (z, yz, xyz, xz);

            break;
          }
        case 1:
          {

            p = cyclide[0][1];
            p.flip (1);
            x = cyclide[0][2];
            x.flip (1);
            z = cyclide[1][1];
            z.flip (1);
            xz = cyclide[1][2];
            xz.flip (1);

            y = p.ybend (dd, db, dist);
            yz = z.zclose (dc, p.pos (), y.pos ());
            xy = x.xclose (da, p.pos (), y.pos ());
            xyz = xy.close (x, xz, y, yz);

            cv.cyclide[0].set (p, y, xy, x);
            cv.cyclide[1].set (z, yz, xyz, xz);

            break;
          }
      }

    return cv;
  }
};



//@todo experiments with other numbers of frames (six and 3...)
struct NCyclide
{

  //    tframe[0].frame = control;
  //
  //    //SET FRAME POSITIONS ALONG CIRCLE
  //    for (int i=0;i<num;++i){
  //      tframe[i].frame.pos() = point( net.cxy(), (float)i/num*TWOPI);
  //    }
  //
  //    //SET FRAME POSITIONS ALONG CIRCLE
  //    for (int i=0;i<4;++i){
  //      tframe[i].frame.pos() = point( net.cxy(), ((float)i/num + pos[i] )*TWOPI);
  //    }
  //
  //    //FIRST ONE
  //    tframe[0].bitan[0] = tframe[0].frame.tx().dual();
  //    tframe[0].bitan[1] = tframe[0].frame.ty().dual();
  //    tframe[0].bitan[2] = tframe[0].frame.tz().dual();
  //
  //    for (int j=0;j<3;++j){
  //     tframe[0].sphere[j] = tframe[0].bitan[j] ^ tframe[1].frame.pos();
  //    }
  //
  //
  //    //SUBSEQUENT
  //    for (int i=1;i<num;++i){
  //     int next = ( i < (num-1) ) ? i+1 : 0;
  //     for (int j=0;j<3;++j){
  //       tframe[i].bitan[j] = Tangent::at( tframe[i-1].sphere[j], tframe[i].frame.pos() );
  //       tframe[i].sphere[j] = tframe[i].bitan[j] ^ tframe[next].frame.pos();
  //       tframe[i].tan[j] = tframe[i].bitan[j].undual();
  //     }
  //    }
  //
  //    for (int i=0;i<num;++i) tframe[i].calcCurves();
};
}
}  //vsr::cga::

#endif /* ----- #ifndef vsr_cyclide_INC  ----- */
