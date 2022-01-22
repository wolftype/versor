/*
 * =====================================================================================
 *
 *       Filename:  xAlgebra.cpp
 *
 *    Description:  examples of various algebras
 *
 *        Version:  1.0
 *        Created:  04/14/2015 18:57:17
 *       Revision:  none
 *       Compiler:  gcc or clang with c++11 support
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include <type_traits>
#include <vsr/detail/vsr_multivector.h>

/*-----------------------------------------------------------------------------

  A geometric algebra is constructed by first defining a bilinear form
  (i.e. a "metric tensor") over a field.

  The algebra is itself a type, and is defined by the p,q,r metric, which is the
  number of dimensions that square to +1, -1, and 0.

  Additionally, we define the field, which is any ring that can be multiplied
  and summed, we typically use floats or doubles.  But this could be dual
  numbers, complex numbers, rotors, or even an entire algebra.

  In the code that follows, you'll notice there is also a boolean which lets
  the compiler know whether to conformalize the metric, that is, re-interpret
  the minkowkski metric (p=1, q=1) in terms of a null-metric.

  This is all done at compile-time..

 *-----------------------------------------------------------------------------*/

//a euclidean algebra of 3d space (basis elements include vectors, areas, volumes, rotors (aka quaternions) )
//...................................<p>, field  >
using ega = vsr::algebra< vsr::metric<3,0,0,false>, double >;

//4d
using ega4 = vsr::algebra< vsr::metric<4,0,0,false>, double >;

//a metric "spacetime algebra" of 4d space
//...................................<p,q>, field  >  ( p is number of basis roots of 1 and q number of roots of -1)
using sta = vsr::algebra< vsr::metric<1,3,0,false>, double >;

//a conformal algebra of 3d space + 2 (we add two new dimensions to the "signature" and set conformal flag to "true")
//...................................<p,q,conf?>  field >
using cga = vsr::algebra< vsr::metric<4,1,0,true>, double >;

//a twistor algebra of conformalized spacetime
//...................................<p,q>, field  >
using twistor = vsr::algebra< vsr::metric<2,4,0,true>, double >;

//minkowksi
using mga = vsr::algebra< vsr::metric<1,1,0, false>, double>;

//a plane-based algebra of euclidean 3D space
//...................................<p,q>, field  >
//using pga = vsr::algebra< vsr::metric<3,0,1,false>, double >;


/*-----------------------------------------------------------------------------

  the field over which the algebra is defined (in the above case "double")
  can be any type of number that returns the same type under multiplication and addition
  and is commutative, that is a*b = b*a (additionally it must have characteristic != 2 )

  for instance, we can define a 2 dimensional geometric algebra over the complex numbers

 *-----------------------------------------------------------------------------*/

//  a complex number is isomorphic to a "rotor" in 2 dimensional euclidean geometric algebra
template<class T> using complex_t = typename vsr::algebra< vsr::metric<2,0,0,false>, T >::types::rotor;

// c2 is now a four-dimensioal complex number in the tensored space i.e. with complex arguments
using c2 = complex_t< complex_t<double> >;


/*-----------------------------------------------------------------------------

  template alias-driven "syntactic sugar" exists for common spaces such as euclidean and conformal.
  in the first line of code above we could have written:

    using ega = vsr::euclidean<3,double>;

  or using the default field (i.e. "double") defined by the macro VSR_PRECISION:

    using ega = vsr::euclidean<3>;

  similarly,

    using cga = vsr::conformal<5>;

  Also, "::types::rotor" is syntactic sugar for ::make_sum< blade<2,0>::type, blade<2,2>::type >)
  which generates a multivector type for the algebra

 *-----------------------------------------------------------------------------*/
  static_assert(std::is_same< ega, vsr::euclidean<3> >::value, "euclidean geometric algebra of 3D space");
  static_assert(std::is_same< cga, vsr::conformal<5> >::value, "conformal geometric algebra of 3D space");

int main(){


  static_assert(std::is_same<ega::types::vector::basis, typename vsr::blade<3,1>::type >::value, "ega3d vector basis");
  static_assert(std::is_same<ega::types::bivector::basis, typename vsr::blade<3,2>::type >::value, "ega3d bivector basis");


  printf("some euclidean 3D types\n");

  printf("vector\n");
  ega::types::vector::basis::print();
  printf("bivector\n");
  ega::types::bivector::basis::print();
  printf("trivector\n");
  ega::types::trivector::basis::print();
  printf("rotor\n");
  ega::types::rotor::basis::print();

  printf("some spacetime 4D types\n");

  printf("vector\n");
  sta::make_grade<1>::basis::print();

  printf("some combinatorics: product of rotors a and b in euclidean algebra: \n");
  ega::impl::gp_arrow_t< ega::types::rotor::basis, ega::types::rotor::basis >::Arrow::print();
  printf("some combinatorics: product of vectors a and b in space time algebra: \n");
  sta::impl::gp_arrow_t< sta::make_grade<1>::basis, sta::make_grade<1>::basis >::Arrow::print();
  printf("some combinatorics: product of vectors a and b in euclidean 4d algebra: \n");
  vsr::euclidean<4>::impl::gp_arrow_t< sta::make_grade<1>::basis, sta::make_grade<1>::basis >::Arrow::print();
  printf("some combinatorics: product of bivectors a and b in twistor algebra: \n");
  twistor::impl::gp_arrow_t< sta::make_grade<2>::basis, sta::make_grade<2>::basis >::Arrow::print();


  printf("some combinatorics: inner product of rotors a and b in euclidean 3D algebra: \n");
  ega::impl::ip_arrow_t< ega::types::rotor::basis, ega::types::rotor::basis>::Arrow::print();

  printf("some combinatorics: scalar product of rotors a and b in euclidean 3D algebra: \n");
  ega::impl::sp_arrow_t< ega::types::rotor::basis, ega::types::rotor::basis>::Arrow::print();

  printf("some combinatorics: inner product of twist motors a and b in conformal 5D algebra: \n");
  cga::impl::ip_arrow_t< cga::types::motor::basis, cga::types::motor::basis>::Arrow::print();

  printf("some combinatorics: scalar product of twist motors a and b in conformal 5D algebra: \n");
  cga::impl::sp_arrow_t< cga::types::motor::basis, cga::types::motor::basis>::Arrow::print();

  printf("some conformal 3D types\n");

  printf("point\n");
  cga::types::point::basis::print();
  printf("pair\n");
  cga::types::pair::basis::print();
  printf("circle\n");
  cga::types::circle::basis::print();
  printf("sphere\n");
  cga::types::sphere::basis::print();


  printf("some complex 2D types\n");
  complex_t<double> ta( 1.0, 2.5 );
  complex_t<double> tb( 2.0, 3.0 );
  c2 a(ta,tb);
  a.print();
  (a*a).print();

  mga::types::Pss().print();

//  printf("some PGA 3D types\n");
//  auto pva = pga::types::vector(1,1,1,1);
//  auto pvb = pga::types::vector(2,3,4,5);

  auto eva = ega4::types::vector(1,1,1,1);
  auto evb = ega4::types::vector(2,3,4,5);
//
  //(eva * evb).print();
  //(pva * pvb).print();

  ega4::impl::gp_arrow_t< ega4::types::vec, ega4::types::vec>::Arrow::print();
//  pga::impl::gp_arrow_t< pga::types::vec, pga::types::vec>::Arrow::print();

  ega4::impl::gp_arrow_t< ega4::types::biv, ega4::types::biv>::Arrow::print();
  //pga::impl::gp_arrow_t< pga::types::biv, pga::types::biv>::Arrow::print();




  return 0;

}
