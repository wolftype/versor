/*! @file

      template metaprogrammed product logic for Algebras

 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 *  homepage: versor.mat.ucsb.edu
 *
 * */



/*
 * =====================================================================================
 *
 *       Filename:  vsr_products.h
 *
 *    Description:  instructions for all products (euclidean, metric, conformal)
 *
 *        Version:  1.0
 *        Created:  04/06/2015 16:46:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#ifndef  vsr_products_INC
#define  vsr_products_INC

#include <vsr/detail/vsr_xlists.h>
#include <vsr/detail/vsr_split_met.h>

namespace vsr {


/*-----------------------------------------------------------------------------
 *  E U C L I D E A N
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *  EUCLIDEAN GEOMETRIC PRODUCT COMPILE-TIME TYPE METAPROGRAMMING ROUTINES
 *-----------------------------------------------------------------------------*/

// template<int TProduct, typename product, int idxA, int idxB> struct sub_impl{};
//
// template<typename product, int idxA, int idxB>
// struct sub_impl<0,product,idxA,idxB>{
//   using arrow = binary_function_token< product::is_positive, idxA, idxB>;
//   using type = XList< arrow >;
// };
// template<typename product>
// struct sub_impl<1,product,idxA,idxB>{
//   using arrow = binary_function_token< product::is_positive, idxA, idxB>;
//   using type = typename Maybe< product::has_outer, XList< arrow >, XList<> >::Type;
// };
// template<typename product>
// struct sub_impl<2,product,idxA,idxB>{
//   using arrow = binary_function_token< product::is_positive, idxA, idxB>;
//   using type = typename Maybe< product::has_inner, XList< arrow >, XList<> >::Type;
// };
//
//
////tmp (in progress and currently unused)
// template<int TProduct, bits::type A, class B, int idxA, int idxB>
// struct sub{
//    using impl = typename sub_impl<TProduct, product<A, B::HEAD, idxA, idxB>::type;
//    typedef typename XCat< impl, typename sub<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type;
// };



/// Geometric Product Type Calculation Sub Loop
template<bits::type A, class B, int idxA, int idxB>
struct SubEGP{
  typedef typename XCat<
                        XList< Inst<bits::signFlip( A, B::HEAD ) ? -1 : 1, A, B::HEAD, idxA, idxB> >,
                        typename SubEGP<A, typename B::TAIL, idxA, idxB+1>::Type
                       >::Type Type;
};
/// Geometric Product Type Calculation Sub Loop End Case
template<bits::type A, int idxA, int idxB>
struct SubEGP<A, Basis<>, idxA, idxB >{
  typedef XList<> Type;
};
/// Geometric Product Type Calculation Main Loop
template<class A, class B, int idxA=0, int idxB=0>
struct EGP{
  typedef typename XCat<
                        typename SubEGP<A::HEAD,B, idxA,idxB>::Type,
                        typename EGP<typename A::TAIL, B, idxA+1,idxB>::Type
                       >::Type Type;
};
/// Geometric Product Type Calculation Main Loop End Case
template<class B, int idxA, int idxB>
struct EGP<Basis<>,B, idxA,idxB> {
  typedef XList<> Type;
};


/*-----------------------------------------------------------------------------
 *  EUCLIDEAN OUTER PRODUCT COMPILE-TIME TYPE METAPROGRAMMING ROUTINES
 *-----------------------------------------------------------------------------*/
/// Outer Product Type Calculation Sub Loop
template<bits::type A, class B, int idxA, int idxB>
struct SubEOP{
  typedef Inst<bits::signFlip( A, B::HEAD ) ? -1 : 1, A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::OP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubEOP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type;
};
/// Outer Product Type Calculation Sub Loop End Case
template<bits::type A, int idxA, int idxB>
struct SubEOP<A, Basis<>, idxA, idxB >{
  typedef XList<> Type;
};
/// Outer Product Type Calculation Main Loop
template<class A, class B, int idxA=0, int idxB=0>
struct EOP{
  typedef typename XCat<
                        typename SubEOP<A::HEAD,B, idxA,idxB>::Type,
                        typename EOP<typename A::TAIL, B, idxA+1,idxB>::Type
                       >::Type Type;
};
/// Outer Product Type Calculation Main Loop End Case
template<class B, int idxA, int idxB>
struct EOP<Basis<>,B, idxA,idxB> {
  typedef XList<> Type;
};


/*-----------------------------------------------------------------------------
 *  EUCLIDEAN SCALAR PRODUCT COMPILE-TIME TYPE METAPROGRAMMING ROUTINES
 *-----------------------------------------------------------------------------*/
/// Scalar Product Type Calculation Sub Loop
template<bits::type A, class B, int idxA, int idxB>
struct SubESP{
  typedef Inst<bits::signFlip( A, B::HEAD ) ? -1 : 1, A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::SP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubESP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type;
};
/// Scalar Product Type Calculation Sub Loop End Case
template<bits::type A, int idxA, int idxB>
struct SubESP<A, Basis<>, idxA, idxB >{
  typedef XList<> Type;
};
/// Scalar Product Type Calculation Main loop
template<class A, class B, int idxA=0, int idxB=0>
struct ESP{
  typedef typename XCat<
                        typename SubESP<A::HEAD,B, idxA,idxB>::Type,
                        typename ESP<typename A::TAIL, B, idxA+1,idxB>::Type
                       >::Type Type;
};
/// Scalar Product Type Calculation Main Loop End Case
template<class B, int idxA, int idxB>
struct ESP<Basis<>,B, idxA,idxB> {
  typedef XList<> Type;
};

/*-----------------------------------------------------------------------------
 *  EUCLIDEAN INNER PRODUCT COMPILE-TIME TYPE METAPROGRAMMING ROUTINES
 *-----------------------------------------------------------------------------*/
/// Inner Product Type Calculation Sub Loop
template<bits::type A, class B, int idxA, int idxB>
struct SubEIP{
  typedef Inst<bits::signFlip( A, B::HEAD ) ? -1 : 1, A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::IP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubEIP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type;
};
/// Inner Product Type Calculation Sub Loop End Case
template<bits::type A, int idxA, int idxB>
struct SubEIP<A, Basis<>, idxA, idxB >{
  typedef XList<> Type;
};
/// Inner Product Type Calculation Main loop
template<class A, class B, int idxA=0, int idxB=0>
struct EIP{
  typedef typename XCat<
                        typename SubEIP<A::HEAD,B, idxA,idxB>::Type,
                        typename EIP<typename A::TAIL, B, idxA+1,idxB>::Type
                       >::Type Type;
};
/// Inner Product Type Calculation Main Loop End Case
template<class B, int idxA, int idxB>
struct EIP<Basis<>,B, idxA,idxB> {
  typedef XList<> Type;
};



/*-----------------------------------------------------------------------------
 *  EUCLIDEAN Product Construction Instructions
 *-----------------------------------------------------------------------------*/
template<class A, class B>
struct EGProd{
  typedef typename EGP<A,B>::Type List;     //<-- Make Combinatoric List of Instructions
  typedef Product<List> Fun;                //<-- Use List to make
  typedef typename Fun::Type basis;         //<-- Basis of Return Type
  typedef typename Fun::DO Arrow;           //<-- The morphism f:axb -> C
};
template<class A, class B>
struct EOProd{
  typedef typename EOP<A,B>::Type List;
  typedef Product<List> Fun;
  typedef typename Fun::Type basis;
  typedef typename Fun::DO Arrow;
};
template<class A, class B>
struct EIProd{
  typedef typename EIP<A,B>::Type List;
  typedef Product<List> Fun;
  typedef typename Fun::Type basis;
  typedef typename Fun::DO Arrow;
};
template<class A, class B>
struct ESProd{
  typedef typename ESP<A,B>::Type List;
  typedef Product<List> Fun;
  typedef typename Fun::Type basis;
  typedef typename Fun::DO Arrow;
};

/*-----------------------------------------------------------------------------
 * EUCLIDEAN Product Type of A and B cast to R (Explicit Return Type Control)
 *-----------------------------------------------------------------------------*/
template<class A, class B, class R>
struct REGProd{
  typedef typename EGP<A,B>::Type List;
  typedef typename Index< List, R>::Type Arrow;
  typedef R Type;
};



/*-----------------------------------------------------------------------------
 *  M E T R I C    (diagonal)
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *  METRIC GEOMETRIC PRODUCT COMPILE-TIME TYPE METAPROGRAMMING ROUTINES
 *
 *  Note: to change to add support for degenerate metric,
 *  1 - no need to change MSign
 *  2 - will need to change BFlip to an unsigned byte metric (0=0, 1=1, 2=-1)
 *  3 - Inst will need to return an empty list if metric is 0
 *
 *  That should be it?
 *
 *-----------------------------------------------------------------------------*/
template<bits::type A, class B, class Metric, int idxA, int idxB>
struct SubMGP{
  static const int MVAL = MSign< Metric, A & B::HEAD, bits::signFlip(A , B::HEAD) ? -1 : 1 >::Val;
  typedef Inst< MVAL, A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::GP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubMGP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type;
};
template<bits::type A, class Metric, int idxA, int idxB>
struct SubMGP<A, Basis<>, Metric, idxA, idxB >{
  typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>
struct MGP{
  typedef typename XCat< typename SubMGP<A::HEAD,B, Metric, idxA,idxB>::Type, typename MGP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type;
};
template<class B, class Metric, int idxA, int idxB>
struct MGP<Basis<>,B, Metric, idxA,idxB> {
  typedef XList<> Type;
};


/*-----------------------------------------------------------------------------
 *  METRIC OUTER PRODUCT COMPILE-TIME TYPE METAPROGRAMMING ROUTINES
 *-----------------------------------------------------------------------------*/

template<bits::type A, class B, class Metric, int idxA, int idxB>
struct SubMOP{
  static const int MVAL = MSign< Metric, A & B::HEAD, bits::signFlip(A , B::HEAD) ? -1 : 1 >::Val;
  typedef Inst< MVAL, A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::OP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubMOP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type;
};
template<bits::type A, class Metric, int idxA, int idxB>
struct SubMOP<A, Basis<>, Metric, idxA, idxB >{
  typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>
struct MOP{
  typedef typename XCat< typename SubMOP<A::HEAD,B, Metric, idxA,idxB>::Type, typename MOP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type;
};
template<class B, class Metric, int idxA, int idxB>
struct MOP<Basis<>,B, Metric,idxA,idxB> {
  typedef XList<> Type;
};


/*-----------------------------------------------------------------------------
 *  METRIC INNER PRODUCT COMPILE-TIME TYPE METAPROGRAMMING ROUTINES
 *-----------------------------------------------------------------------------*/

template<bits::type A, class B,  class Metric, int idxA, int idxB>
struct SubMIP{
  static const int MVAL = MSign< Metric, A & B::HEAD, bits::signFlip(A , B::HEAD) ? -1 : 1 >::Val;
  typedef Inst< MVAL, A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::IP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubMIP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type;
};
template<bits::type A, class Metric, int idxA, int idxB>
struct SubMIP<A, Basis<>, Metric, idxA, idxB >{
  typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>
struct MIP{
  typedef typename XCat< typename SubMIP<A::HEAD,B, Metric, idxA,idxB>::Type, typename MIP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type;
};
template<class B, class Metric,  int idxA, int idxB>
struct MIP<Basis<>,B, Metric, idxA,idxB> {
  typedef XList<> Type;
};

/*-----------------------------------------------------------------------------
 *  METRIC SCALAR PRODUCT COMPILE-TIME TYPE METAPROGRAMMING ROUTINES
 *-----------------------------------------------------------------------------*/

template<bits::type A, class B,  class Metric, int idxA, int idxB>
struct SubMSP{
  static const int MVAL = MSign< Metric, A & B::HEAD, bits::signFlip(A , B::HEAD) ? -1 : 1 >::Val;
  typedef Inst< MVAL, A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::SP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubMSP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type;
};
template<bits::type A, class Metric, int idxA, int idxB>
struct SubMSP<A, Basis<>, Metric, idxA, idxB >{
  typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>
struct MSP{
  typedef typename XCat< typename SubMSP<A::HEAD,B, Metric, idxA,idxB>::Type, typename MSP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type;
};
template<class B, class Metric,  int idxA, int idxB>
struct MSP<Basis<>,B, Metric, idxA,idxB> {
  typedef XList<> Type;
};


/*-----------------------------------------------------------------------------
 *  METRIC Product Construction
 *-----------------------------------------------------------------------------*/

template<class A, class B, class Metric>
struct MGProd{
  typedef typename MGP<A,B,Metric>::Type List;
  typedef Product<List> Fun;
  typedef typename Fun::Type basis;
  typedef typename Fun::DO Arrow;
};

template<class A, class B, class Metric>
struct MOProd{
  typedef typename MOP<A,B,Metric>::Type List;
  typedef Product<List> Fun;
  typedef typename Fun::Type basis;
  typedef typename Fun::DO Arrow;
};

template<class A, class B, class Metric>
struct MIProd{
  typedef typename MIP<A,B,Metric>::Type List;
  typedef Product<List> Fun;
  typedef typename Fun::Type basis;
  typedef typename Fun::DO Arrow;
};

template<class A, class B, class Metric>
struct MSProd{
  typedef typename MSP<A,B,Metric>::Type List;
  typedef Product<List> Fun;
  typedef typename Fun::Type basis;
  typedef typename Fun::DO Arrow;
};

/*-----------------------------------------------------------------------------
 * METRIC Product Explicit Control
 *-----------------------------------------------------------------------------*/
template<class A, class B, class R, class M>
struct RMGProd{
  typedef typename MGP<A,B,M>::Type List;
  typedef typename Index< List, R>::Type Arrow;
  typedef R Type;
};





/*-----------------------------------------------------------------------------
 *  C O N F O R M A L
 *-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 *  SPLITTING OF INSTRUCTIONS (PUSHING AND POPPING of Minkowskian Diagonal Metric)
 *-----------------------------------------------------------------------------*/
template<class S, int idxA, int idxB>
struct SplitInstructions{
  typedef Instruct<(S::HEAD::SIGN < 0), S::HEAD::BIT, idxA, idxB> I;
  typedef typename XCat<
    XList<I>,
    typename SplitInstructions<typename S::TAIL, idxA, idxB>::Type
  >::Type Type;
};

template<int idxA, int idxB>
struct SplitInstructions<XList<>, idxA, idxB> {
  typedef XList<> Type;
};

/*-----------------------------------------------------------------------------
 *  GEOMETRIC PRODUCT COMPILE-TIME TYPE CALCULATION ROUTINES
 *-----------------------------------------------------------------------------*/
/// Geometric Product Type Calculation Sub Loop
template<bits::type A, class B, class Metric, int idxA, int idxB>
struct SubCGP{
  typedef typename SplitProd< A, B::HEAD, Metric>::Type Split;
  typedef typename SplitInstructions< Split, idxA, idxB >::Type XL;

  typedef typename XCat<
    XL,
    typename SubCGP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type
  >::Type Type;
};

/// Geometric Product Type Calculation Sub Loop End Case
template<bits::type A, class Metric, int idxA, int idxB>
struct SubCGP<A, Basis<>, Metric, idxA, idxB >{
  typedef XList<> Type;
};

/// Geometric Product Type Calculation Main Loop
template<class A, class B, class Metric, int idxA=0, int idxB=0>
struct CGP{
  typedef typename XCat<
    typename SubCGP<A::HEAD,B, Metric, idxA,idxB>::Type,
    typename CGP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type
  >::Type Type;
};

/// Geometric Product Type Calculation Main Loop End Case
template<class B, class Metric, int idxA, int idxB>
struct CGP<Basis<>,B, Metric, idxA,idxB> {
  typedef XList<> Type;
};

/*-----------------------------------------------------------------------------
 *  OUTER PRODUCT COMPILE-TIME TYPE CALCULATION ROUTINES
 *-----------------------------------------------------------------------------*/
/// Outer Product Type Calculation Sub Loop
template<bits::type A, class B, class Metric, int idxA, int idxB>
struct SubCOP{
  typedef typename SplitOProd< A, B::HEAD, Metric>::Type Split;
  typedef typename SplitInstructions< Split, idxA, idxB >::Type XL;

  typedef typename XCat<
    XL,
    typename SubCOP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type
  >::Type Type;
};

/// Outer Product Type Calculation Sub Loop End Case
template<bits::type A, class Metric, int idxA, int idxB>
struct SubCOP<A, Basis<>, Metric, idxA, idxB >{
  typedef XList<> Type;
};

/// Outer Product Type Calculation Main Loop
template<class A, class B, class Metric, int idxA=0, int idxB=0>
struct COP{
  typedef typename XCat<
    typename SubCOP<A::HEAD,B, Metric, idxA,idxB>::Type,
    typename COP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type
  >::Type Type;
};

/// Outer Product Type Calculation Main Loop End Case
template<class B, class Metric,  int idxA, int idxB>
struct COP<Basis<>,B, Metric,idxA,idxB> {
  typedef XList<> Type;
};

/*-----------------------------------------------------------------------------
 *  INNER PRODUCT COMPILE-TIME TYPE CALCULATION ROUTINES
 *-----------------------------------------------------------------------------*/
/// Inner Product Type Calculation Sub Loop
template<bits::type A, class B, class Metric, int idxA, int idxB>
struct SubCIP{
  typedef typename SplitIProd< A, B::HEAD, Metric>::Type Split;
  typedef typename SplitInstructions< Split, idxA, idxB >::Type XL;

  typedef typename XCat<
    XL,
    typename SubCIP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type
  >::Type Type;
};
/// Inner Product Type Calculation Sub Loop End Case
template<bits::type A, class Metric, int idxA, int idxB>
struct SubCIP<A, Basis<>, Metric, idxA, idxB >{
  typedef XList<> Type;
};
/// Inner Product Type Calculation Main Loop
template<class A, class B, class Metric, int idxA=0, int idxB=0>
struct CIP{
  typedef typename XCat<
    typename SubCIP<A::HEAD,B, Metric, idxA,idxB>::Type,
    typename CIP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type
  >::Type Type;
};
/// Inner Product Type Calculation Main Loop End Case
template<class B, class Metric,  int idxA, int idxB>
struct CIP<Basis<>,B, Metric, idxA,idxB> {
  typedef XList<> Type;
};


/*-----------------------------------------------------------------------------
 *  INNER PRODUCT COMPILE-TIME TYPE CALCULATION ROUTINES
 *-----------------------------------------------------------------------------*/
/// Inner Product Type Calculation Sub Loop
template<bits::type A, class B, class Metric, int idxA, int idxB>
struct SubCSP{
  typedef typename SplitSProd< A, B::HEAD, Metric>::Type Split;
  typedef typename SplitInstructions< Split, idxA, idxB >::Type XL;

  typedef typename XCat<
    XL,
    typename SubCSP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type
  >::Type Type;
};
/// Inner Product Type Calculation Sub Loop End Case
template<bits::type A, class Metric, int idxA, int idxB>
struct SubCSP<A, Basis<>, Metric, idxA, idxB >{
  typedef XList<> Type;
};
/// Inner Product Type Calculation Main Loop
template<class A, class B, class Metric, int idxA=0, int idxB=0>
struct CSP{
  typedef typename XCat<
    typename SubCSP<A::HEAD,B, Metric, idxA,idxB>::Type,
    typename CSP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type
  >::Type Type;
};
/// Inner Product Type Calculation Main Loop End Case
template<class B, class Metric,  int idxA, int idxB>
struct CSP<Basis<>,B, Metric, idxA,idxB> {
  typedef XList<> Type;
};


/*-----------------------------------------------------------------------------
 *  Product Construction Instructions
 *-----------------------------------------------------------------------------*/
template<class A, class B, class Metric>
struct CGProd{
  typedef typename CGP<A,B,Metric>::Type List;
  typedef Product<List> Fun;
  typedef typename Fun::Type basis;
  typedef typename Fun::DO Arrow;
};

template<class A, class B, class Metric>
struct COProd{
  typedef typename COP<A,B,Metric>::Type List;
  typedef Product<List> Fun;
  typedef typename Fun::Type basis;
  typedef typename Fun::DO Arrow;
};

template<class A, class B, class Metric>
struct CIProd{
  typedef typename CIP<A,B,Metric>::Type List;
  typedef Product<List> Fun;
  typedef typename Fun::Type basis;
  typedef typename Fun::DO Arrow;

};

template<class A, class B, class Metric>
struct CSProd{
  typedef typename CSP<A,B,Metric>::Type List;
  typedef Product<List> Fun;
  typedef typename Fun::Type basis;
  typedef typename Fun::DO Arrow;
};

/*-----------------------------------------------------------------------------
 *  Geometric Product Type Explicit Control
 *-----------------------------------------------------------------------------*/
//template<class A, class B, class R, class M>
//struct RCGProd{
//  typedef typename CGP<typename A::basis, typename B::basis, M>::Type InstList;
//  typedef typename Index< InstList, typename R::basis>::Type DO;
//  static constexpr R Call(const A& a, const B& b) {
//    return DO::template Make<R>(a, b);
//  }
//};

/*-----------------------------------------------------------------------------
 * METRIC Product Explicit Control
 *-----------------------------------------------------------------------------*/
template<class A, class B, class R, class M>
struct RCGProd{
  typedef typename CGP<A,B,M>::Type List;
  typedef typename Index< List, R>::Type Arrow;
  typedef R Type;
};




} //vsr::

#endif   /* ----- #ifndef vsr_products_INC  ----- */
