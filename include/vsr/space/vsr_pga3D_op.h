
/*  versor - geometric algbera library
 *  https://github.com/wolftype/versor
 *  c++11 (gcc >= 4.7 or clang >= 3.2)
 *
 *	(c)  p a b l o   c o l a p i n t o
 */

#include <vsr/space/vsr_pga3D_types.h>

namespace vsr {
namespace pga {


struct Op
{
/*-----------------------------------------------------------------------------
 *  OP for extracting info from multivectors useful for drawing routines
 *-----------------------------------------------------------------------------*/

/// Returns Angle (degrees) and Axis Vector
static Rot AA (const Vector &s)
{
  Rotor r = nga::Gen::ratio (Vec::z, s.unit ());
  return nga::Gen::aa (r);
}

/// Returns Angle (degrees) and Axis Vector
static Rot AA (const Bivector &s)
{
  Rotor r = nga::Gen::ratio (Vec::z, s.duale ().unit ());
  return nga::Gen::aa (r);
}

/// Returns Angle (degrees) and Axis Vector
static Rot AA (const Plane &s)
{
  Rot r = nga::Gen::ratio (Vec::z, Vec (s).unit ());
  return nga::Gen::aa (r);
}

//Vector on plane closest to origin
static Vec Pos (const Plane &s)
{
  return Vec(s).unit() * Inf(s);
}

};

/*-----------------------------------------------------------------------------
*  GEN
*-----------------------------------------------------------------------------*/
Rot Gen::rot (const Biv &b)
{
  return nga::Gen::rot (b);
}
Rot Gen::rotor (const Biv &b)
{
  return nga::Gen::rot (b);
}

Grt Gen::rot (const Line& d)
{
  return nga::Gen::rot (d);
}

Grt Gen::rotor (const Line& d)
{
  return nga::Gen::rot (d);
}

Line Gen::log (const Grt &g)
{
  return nga::Gen::log (g);
}

Bst Gen::bst (const Pair &p)
{
  return nga::Gen::bst (p);
}
Bst Gen::boost (const Pair &p)
{
  return nga::Gen::bst (p);
}

Rot Gen::ratio (const Vec &v, const Vec &v2)
{
  return nga::Gen::ratio (v, v2);
}

Biv Gen::log (const Rot &r)
{
  return nga::Gen::log (r);
}

/*! Generate a Rotor (i.e quaternion) from spherical coordinates
        @param[in] theta in xz plane from (1,0,0) in range [0,PI]
        @param[in] phi in rotated xy plane in range []
    */
Rot Gen::rot (double theta, double phi)
{
  Rot rt = Gen::rot (Biv::xz * theta / 2.0);
  Rot rp = Gen::rot (Biv::xy.sp (rt) * phi / 2.0);
  return rp * rt;
}

/*! Generate a rotor from euler angles */
Rot Gen::rot (double y, double p, double r)
{
  Rot yaw = Gen::rot (Biv::xz * y / 2.0);
  Rot pitch = Gen::rot (Biv::yz.spin (yaw) * p / 2.0);
  Rot tmp = pitch * yaw;
  Rot roll = Gen::rot (Biv::xy.spin (tmp) * r / 2.0);

  return roll * tmp;
}


/*! Generate a Motor from a Dual Line Axis
       @param Dual Line Generator (the axis of rotation, including pitch and period)
   */
Mot Gen::mot (const Line  &line)
{

  Line b = line;
  Biv B (b[0], b[1], b[2]);  //Biv B(dll);

  Mot::value_t w = B.wt ();

  VSR_PRECISION c = (sqrt (fabs (w)));
  VSR_PRECISION sc = sin (c);
  VSR_PRECISION cc = cos (c);

  if (ERROR (w, .00000001))
    return Mot (1, 0, 0, 0, b[3], b[4], b[5], 0);  // translation only!

  B = B.unit ();
  Vec t (b[3], b[4], b[5]);

  Vec tv;
  tv = Op::pj (t, B);
  Vec tw;
  tw = Op::rj (t, B);

  tv *= Math::sinc (c);

  Vec tt = tw * cc + tv;

  auto ts = B * tw;  //Vec_Biv

  return Mot (cc, B[0] * sc, B[1] * sc, B[2] * sc, tt[0], tt[1], tt[2],
              ts[3] * sc);
}

Mot Gen::motor (const Line &line)
{
  return mot (line);
}

/*! Dual Line Generator from a Motor
        An implementation of J.Lasenby et al "Applications of Conformal Geometric Algebra in
        Computer Vision and Graphics"
        @param Motor m (a concatenation of rotation and translation)
    */
Line Gen::log (const Mot &m)
{

  Drv cperp, cpara;
  Lin rq, q, tq;
  Biv b;

  q = m;  //extract grade 2 part

  VSR_PRECISION ac = acos (m[0]);  //angle of rotor
  VSR_PRECISION den = Math::sinc (ac);
  VSR_PRECISION den2 = ac * ac * den;

  b = ((Ori (1) <= (q * Inf (1))) / den
       * -1.0);  //bivector part - negative necessary . dll? . .
  tq = (b * q);  //Make motor and extract Grade 2 part

  if (FERROR (den2))
    {  // Pure translation
       //  printf("%f, %f, %f\n", ac, den, den2 );
       //  printf("den2 = 0 in motor log\n");
      //cperp = b * -1.0;
      cperp =
        q;  //b * tq * -1.0;// * -1.0; or q //note this used to be cpara... (but was inaccurate)
    }
  else
    {
      cperp = (b * Drt (m[7]))
              / ((den2) * -1.0);  //perpendicular (along line of axis)
      cpara =
        (b * tq) / ((den2) * -1.0);  //parallel      (in plane of rotation)
    }

  Drv c = cperp + cpara;

  rq += b;
  rq += c;

  return rq;
}

/*! Dual Line Generator of Motor That Twists Dual Line a to Dual Line b;

    */
Line Gen::log (const Line &a, const Line &b, VSR_PRECISION t)
{
  Mot m = b / a;
  VSR_PRECISION n = m.rnorm ();
  if (n != 0)
    m /= n;
  return Gen::log (m) * (t / 2.0);
}

/*! Generate Motor That Twists Dual Line a to Dual Line b;

      */
Mot Gen::ratio (const Dll &a, const Dll &b, VSR_PRECISION t)
{
  //Mot m = b/a; VSR_PRECISION n = m.rnorm(); if (n!=0) m /= n; else cout << "zero mot" << endl;
  return Gen::mot (log (a, b, t));  //Gen::log( m ) * (t/2.0) );
}

/*! Generate Motor That Twists Motor a to motor b by amt t;
      */
Mot Gen::ratio (const Mot &a, const Mot &b, VSR_PRECISION t)
{
  return Gen::mot (Gen::log (b / a) * t);
}


/*! Generate Simple Boost rotor from ratio of two dual spheres
       @todo investigate need to negate ratio
    */
Bst Gen::ratio (const DualSphere &a, const DualSphere &b, bool bFlip)
{
  Bst tbst = (b / a).runit ();

  //if (tbst[0]<0)
  if (bFlip)
    tbst = -tbst;  //true unless the one sphere is inside the other
  auto ss = 2 * (1 + tbst[0]);
  auto n = (ss >= 0 ? sqrt (ss) : -sqrt (-ss));
  return FERROR (n) ? Bst () : (tbst + 1) / n;
}

/*! atanh2 function for logarithm of general rotors*/
Pair Gen::atanh2 (const Pair &p, VSR_PRECISION cs, bool bCW, bool bTwoPI)
{
  VSR_PRECISION norm = 1;

  auto tp = p.wt ();
  auto sq = sqrt (fabs (tp));
  if (tp > FPERROR)
  {
//    if (bCW)
//      norm = (asinh (sq)-(bTwoPI ? TWOPI: PI)) / sq  ;
//    else
      norm = asinh (sq) / sq;
  }
  else if (tp < -FPERROR)
    {
      if (bCW)
        //norm = -((bTwoPI ? TWOPI : PI) - atan2 (sq, cs)) / sq;  //alt direction
        norm = (atan2 (sq, cs) - (bTwoPI ? TWOPI : PI)) / sq;  //alt direction
      else
        norm = atan2 (sq, cs) / sq;
    }
  return p * norm;
}

/*! Log of a simple rotor (uses atanh2, passes in boolean for direction of interpolation) */
Line Gen::log (const Motor &b, bool bCW, bool bTwoPI)
{
  return atanh2 (Line(b), b[0], bCW, bTwoPI);
}

/*-----------------------------------------------------------------------------
   *  ROTORS
   *-----------------------------------------------------------------------------*/
/*!
     *  \brief  generate a rotor transformation from a euclidean bivector
     */
Rotor Gen::xf (const Biv &b)
{
  return Gen::rot (b);
}
/*!
     *  \brief  generate a motor transformation from a dual line
     */
Motor Gen::xf (const Line &dll)
{
  return Gen::mot (line);
}

/// Point from x,y,z
Point Construct::point (VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z)
{
  return Point(-1, z, y, x);
}

/// Point from vec
Point Construct::point (const Vec &v)
{
  return Consruct::point (v[0], v[1], v[2]); 
}

/// Point on line l closest to p
Point Construct::point (const Line &line, const Point &p)
{
  return line ^ (line <= p);
}

/// Point on plane closest to p
Point Construct::point (const Plane &plane, const Point &p)
{
  return plane ^ (plane <= p);;
}


/*-----------------------------------------------------------------------------
 *  LINES
 *-----------------------------------------------------------------------------*/

/// Line from two Vecs
Line Construct::line (const Vec &a, const Vec &b)
{
  return Drv(b[0], b[1], b[2]) <= point(a);
}


///Direct line through origin
Line Construct::line (VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z)
{
  return Vec(x, y, z).duale();
}

//Line Construct::line (const Point &a, const Point &b)
//{
//  return a ^ b ^ Inf (1);
//}
/////Direct line through point a in direction b
//Line Construct::line (const Point &a, const Vec &b)
//{
//  return a ^ b ^ Inf (1);
//}


//VSR_PRECISION Construct::distance (const Lin &lin, const Pnt &pnt)
//{
//  return (pnt <= lin.dual ())[0] * -2.0;
//}


/* Line line(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z){ */
/*   return point(a[0], a[1], a[2]) ^ Vec(b[0], b[1], b[2]) ^ Inf(1); */
/* } */

#pragma mark COINCIDENCE_FUNCTIONS

//normalized and nulled point intersection of dualline and dual plane
Point Construct::meet (const Dll &dll, const DualPlane &dlp)
{
  auto pnt = (dll ^ dlp);
  return - pnt / pnt[0]; 
}


////Point intersection of two lines
//Point Construct::meet (const Line &la, const Line &lb)
//{
//  Line r = la.reflect (lb);
//  Line r2 = (la - r.unit ()).unit ();
//  Point pori = Flat::loc (r2, Ori (1), false);
//  Point tp = pori.re (lb);
//  return (((tp / tp[3]) + pori) / 2.0).null ();
//}
/* //test for origin */
/* auto t = Ori(1) ^ la; Sca(t.wt()).vprint(); */
/* (t.dual() ^ lb.dual() ).dual().vprint(); */
/* if (t.wt() != 0 ) { */
/*  return ( t.dual() ^ lb.dual() ).dual(); */
/* } else { */
/*  auto t2 = (Ori(1) ^ lb ); */
/*  if ( t2.wt() != 0 ) return ( la.dual() ^ t2.dual() ).dual(); */
/*  else return Flp(); */
/* } */
//}

#pragma mark HIT_TESTS

}  //pga::
}  //vsr::
