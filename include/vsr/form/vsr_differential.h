/*
 * =====================================================================================
 *
 *       Filename:  vsr_differential.h
 *
 *    Description:  differential geometry routines (experimental!)
 *
 *        Version:  1.0
 *        Created:  11/04/2014 14:58:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *
 * =====================================================================================
 */


#ifndef vsr_differential_INC
#define vsr_differential_INC

#include <vsr/space/vsr_cga3D_op.h>  //<-- assume cga3d for now
#include <vsr/form/vsr_graph.h>

namespace vsr {
namespace cga {

//simplicial variable (2D facets in 3D space)
//we will eventually generalize this . . .
//
//but, for now the coordinate gradients can be used to calculate
//cotangent laplace-beltrami
//
//essentially, cos of a = (ra * ra + rab) * 2 * area
//
//note, the laplacian of position is a vector laplacian, as opposed
//to a scalar laplacian.  we are in euclidean coords, so straight forward
//i.e. the degree to which a point varies from the average of its neighbors
//i.e. mean curvature
struct Simplicial2
{

  Vec ea, eb;                  ///< emanating edges (ccw)
  Biv tpss;                    ///< tmp
  Biv pss;                     ///< Tangent Pseudoscalar
  Vec ra, rb;                  ///< reciprocal frames (a suitable basis for FEM)
  VSR_PRECISION area, la, lb;  ///< area and edge lengths
  VSR_PRECISION theta;         ///< angle at a0
  VSR_PRECISION ca, cb, cc;    ///< cotangent of angles
  VSR_PRECISION raa, rbb, rab;  ///< inner products of gradients (cot-like)
  VSR_PRECISION rca, rcb;       ///< inner products of gradients (cot-like)
  // Vec lap;                      ///< laplacian of position

  Simplicial2 () {}

  /// Constructor takes three coordinates,
  /// and makes edge vectors, their lengths, tangent pseudoscalar,
  /// area, reciprocal frames
  template <class T>
  Simplicial2 (const T &a, const T &b, const T &c)
  {
    ea = b - a;
    la = ea.norm ();
    eb = c - a;
    lb = eb.norm ();
    tpss = ea ^ eb;
    area = tpss.rnorm () * .5;
    pss = !(tpss);
    ra = (eb <= pss);
    rb = (-ea <= pss);

    raa = ra.wt ();
    rbb = rb.wt ();
    rab = (ra <= rb)[0];

    rca = raa + rab;
    rcb = rbb + rab;

    //    lap = (ea * (raa + rab) + eb * (rbb + rab)) * 2 * area;

    //superfluous cotan, just here for checking l2 norm of gradient identities
    //ca should equal rca, cb should equal rcb.
    //theta is used here to check gaussian against angle deficit
    Vec ta = -ea.unit ();
    Vec tb = -eb.unit ();
    Vec tc = (ea - eb).unit ();
    auto alpha = acos ((tb <= tc)[0]);
    auto beta = acos ((ta <= -tc)[0]);
    auto gamma = acos ((-ta <= -tb)[0]);
    theta = gamma;
    ca = 1.0 / tan (alpha);
    cb = 1.0 / tan (beta);
    cc = 1.0 / tan (gamma);
  }

  void print ()
  {
    ea.print ();
    eb.print ();
  }

  // Laplacian of some blade-valued function defined on points
  // A) Difference along edges (d/da and d/db)
  // B) (projected onto edges?)
  // C) multiplied by raa+rab and rbb+rab
  // @todo
  // eliminate 2.0 muliplier (since it will be divided anyway)
  // divide by 3 so other vertices can contribute their share
  template <class T>
  T laplacian0 (const T &n, const T &na, const T &nb)
  {
    T dna = na - n;
    T dnb = nb - n;
    //    // coefficients (partial derivatives along edges)
    //    auto pda = ea <= dna;
    //    auto pdb = eb <= dnb;
    //    return (dna * rca) + (dnb * rcb) * 2.0 * area;
    return (dna * rca) + (dnb * rcb) * 2.0 * area;
  }

  //wedged derivative / differential
  template <typename T>
  auto derivative (const T &n, const T &na, const T &nb)
    -> decltype (Vec () * T ())
  {
    //diff along edges
    auto dna = na - n;
    auto dnb = nb - n;

    // weighted reciprocals
    return ((ra * dna) + (rb * dnb));
  }

//  //wedged derivative / differential
//  Vec derivative (const float &n, const float &na, const float &nb)
//  {
//    //diff along edges
//    auto dna = na - n;
//    auto dnb = nb - n;
//
//    // weighted reciprocals
//    return ((ra * dna) + (rb * dnb));
//  }


  // Differential in X direction of some function F with values n, na, nb
  // on the simplex -- returns X'
  //
  // Equivalent to df(X) returns a stretched and rotated X.
  // If F is conformal, all a directions will stretch the same amount
  //
  // if n's are normals, then wedging the result with n is the Shape Bivector
  template <class T>
  Vec differential (const Vec &a, const T &n, const T &na, const T &nb)
  {
    T dna = na - n;
    T dnb = nb - n;

    //Derivate of F is sum of
    auto derivF = (ra * dna) + (rb * dnb);

    return a <= derivF;
  }

  /// Vector Derivative of some T-valued function defined on points
  //  finds difference along edges, dots that with edge vectors,
  //  uses resultant scalar value to weigh reciprocals
  //
  //  @todo can "difference" be a log function?  not sure how this would
  //  affect projection step (i.e. coefficient extraction)
  template <class T>
  vector<Vec> derivative1 (const T &n, const T &na, const T &nb)
  {
    vector<Vec> v;
    //diff along edges
    auto dna = na - n;
    auto dnb = nb - n;
    // coefficients (partial derivatives along edges)
    auto pda = ea <= dna;
    auto pdb = eb <= dnb;
    // weighted reciprocals
    v.push_back (ra * pda);
    v.push_back (rb * pdb);
    return v;
  }

  // what is exterior_derivative
  // rename this exterior derivative, and see curl
  template <class T>
  Biv exterior_derivative (const T &n, const T &na, const T &nb)
  {
    //diff along edges
    auto dna = na - n;
    auto dnb = nb - n;
    // coefficients (partial derivatives along edges)
    auto pda = ea <= dna;
    auto pdb = eb <= dnb;
    // weighted reciprocals
    return ((ra * pda) ^ (rb * pdb)) / area;
  }


  //Derivative of only edge A (for ccw integration around a node)
  //rename to DIVERGENCE
  template <class T>
  Vec derivativeA (const T &na, const T &nb)
  {
    //diff along edge
    auto dna = nb - na;
    // coefficients of change in a direction
    auto wa = dna <= ea;
    // weighted reciprocal
    return ra * wa;  //q: multiply recip by length or multiply by dot? or after
  }


  /// Vector Derivative of some T-valued function defined on points
  //  finds difference along edges, dots that with edge vectors,
  //  uses resultant scalar value to weigh reciprocals
  //  SUMS these together (no)
  template <class T>
  Vec derivative0 (const T &n, const T &na, const T &nb)
  {
    //diff along edges
    auto dna = na - n;
    auto dnb = nb - n;
    // coefficients (amt of change in ea and eb directions)
    auto wa = dna <= ea;
    auto wb = dnb <= eb;
    // return sum of weighted reciprocals
    return ((ra * wa) + (rb * wb));  //q: divid by area? or after sum
  }

  //no
  Vec derivative0 (const float &n, const float &na, const float &nb)
  {
    //diff along edges
    auto dna = na - n;
    auto dnb = nb - n;
    // coefficients
    auto wa = dna;  // <= ea;
    auto wb = dnb;  // <= eb;
    // return sum of weighted reciprocals
    return ((ra * wa) + (rb * wb));  //q: divid by area? or after sum
  }


  /// Gradient of some T-valued function defined on points
  /// ignore ea and eb... hmmm, see curl instead
  template <class T>
  Vec gradient (const T &n, const T &na, const T &nb)
  {
    //diff along edges
    auto dna = na - n;
    auto dnb = nb - n;

    auto sa = dna <= (ra ^ (dna));
    auto sb = dnb <= (rb ^ (dnb));

    // return sum of weighted reciprocals
    return (sa + sb);  //q: divid by area? or after sum
  }

  //vector derivative of scalar-valued function is the gradient
  //@todo divide by area?
  Vec gradient (const float &n, const float &na, const float &nb)
  {
    //diff along edges
    auto dna = na - n;
    auto dnb = nb - n;
    // return sum of weighted reciprocals
    return ((ra * dna) + (rb * dnb));  //q: divid by area? or after sum
  }

  /// Vector Derivative of some T-valued function defined on points
  //  finds direct difference along outer edge, wedges with gradient
  //  (or derivative of position).. actually this almost the CURL
  template <class T>
  Biv exterior_derivative0 (const T &n, const T &na, const T &nb)
  {

    //diff along outer edge
    auto F = nb - na;
    auto deriv = derivative0 (n, na, nb);

    return (deriv ^ F);
  }


  /// Curl of some T-valued function defined on points
  //  finds difference along outer edge, wedges with recips
  template <class T>
  Biv curl (const T &n, const T &na, const T &nb)
  {
    //partial derivatives are differences along edges
    auto dna = na - n;
    auto dnb = nb - n;

    //wedge those with the recips and sum and divide by area
    return (ra ^ dna + rb ^ dnb) / area;
  }

  /// Vector Derivative of some T-valued function defined on points
  //  finds direct difference along outer edge, wedges with gradient
  template <class T>
  auto full_derivative (const T &n, const T &na, const T &nb)
    -> decltype (Vec () * T ())
  {

    //partial derivatives are differences along edges
    auto dna = na - n;
    auto dnb = nb - n;

    //multiply those with the recips and sum and divide by area
    return (ra * dna + rb * dnb) / area;
  }


  float deficit () { return acos ((eb.unit () <= ea.unit ())[0]); }


  //circumcenter
  //@todo barycenter (.5ra + .5rb)
  Point center ()
  {
    return Round::loc (Round::null (0, 0, 0) ^ Round::null (ea)
                       ^ Round::null (eb));
  }

  //assumes s is next ccw, with shared edge eb
  Vec cotan (Simplicial2 &s)
  {
    Vec dual = s.center () - center ();
    float wt = dual.norm () / lb;
    return eb * wt;
  }

  /*-----------------------------------------------------------------------------
    *  Questions:
    *
    *  1. sum weighted bases or return separate?
    *  2. divide result by area?
    *  3. wedge around a node and sum . . . (exterior differential)
    *-----------------------------------------------------------------------------*/

  //weighted reciprocals
  /* Vec wra() { return ra * la; } */
  /* Vec wrb() { return rb * lb; } */

  //given a node, get edge vectors
  /* template<class T> */
  /* static vector<Simplicial2> Domain(const typename HEGraph<T>::Node& n){ */
  /*   vector<Simplicial2> v; */
  /*   const auto& ta = n.data(); */
  /*   for (auto& i : n.valence() ){ */
  /*     auto& tb = i->a(); */
  /*     auto& tc = i->next->a(); */
  /*     v.push_back( Simplicial2(ta,tb,tc) ); */
  /*   } */
  /*   return v; */
  /* } */
};

//generic simplicial variable (in progress)
template <int DIM>
struct Simplicial
{
  using TVEC = NEVec<DIM>;
  //using TPSS = NEPSS<DIM>;
};

//given a facet, get reciprocal
//given a half edge graph, get normals
}
}  //vsr::cga::

#endif /* ----- #ifndef vsr_differential_INC  ----- */
