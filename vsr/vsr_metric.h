/*
 * =====================================================================================
 *
 *       Filename:  vsr_metric.h
 *
 *    Description:  ND geometric algebra with arbitary diagonal metric
 *
 *        Version:  1.0
 *        Created:  03/14/2014 19:55:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  vsr_metric_INC
#define  vsr_metric_INC


#include "vsr_xlists.h"
#include "vsr_instructions.h"
#include "vsr_types.h"
#include "vsr_versions.h"

template<TT A, class B, class Metric, int idxA, int idxB>  
struct SubMGP{
  static const bool BFlip = MSign< Metric, A & B::HEAD, signFlip(A , B::HEAD) ? -1 : 1 >::Val == -1;  
  typedef typename XCat< XList< Inst< BFlip, A, B::HEAD, idxA, idxB> >, typename SubMGP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, class Metric, int idxA, int idxB>  
struct SubMGP<A, MV<>, Metric, idxA, idxB >{
  typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>  
struct MGP{
  typedef typename XCat< typename SubMGP<A::HEAD,B, Metric, idxA,idxB>::Type, typename MGP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, class Metric, int idxA, int idxB>  
struct MGP<MV<>,B, Metric, idxA,idxB> {
  typedef XList<> Type; 
}; 

template<TT A, class B, class Metric, int idxA, int idxB>  
struct SubMOP{
  static const bool BFlip = MSign< Metric, A & B::HEAD, signFlip(A , B::HEAD) ? -1 : 1 >::Val == -1;    
  typedef Inst< BFlip, A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::OP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubMOP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, class Metric, int idxA, int idxB>  
struct SubMOP<A, MV<>, Metric, idxA, idxB >{
  typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>  
struct MOP{
  typedef typename XCat< typename SubMOP<A::HEAD,B, Metric, idxA,idxB>::Type, typename MOP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, class Metric, int idxA, int idxB>  
struct MOP<MV<>,B, Metric,idxA,idxB> {
  typedef XList<> Type; 
};

template<TT A, class B,  class Metric, int idxA, int idxB>  
struct SubMIP{ 
  static const bool BFlip = MSign< Metric, A & B::HEAD, signFlip(A , B::HEAD) ? -1 : 1 >::Val == -1; 
  typedef Inst< BFlip, A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::IP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubMIP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, class Metric, int idxA, int idxB>  
struct SubMIP<A, MV<>, Metric, idxA, idxB >{
  typedef XList<> Type;
};

template<class A, class B, class Metric, int idxA=0, int idxB=0>  
struct MIP{
  typedef typename XCat< typename SubMIP<A::HEAD,B, Metric, idxA,idxB>::Type, typename MIP<typename A::TAIL, B, Metric, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, class Metric,  int idxA, int idxB>  
struct MIP<MV<>,B, Metric, idxA,idxB> {
  typedef XList<> Type; 
};

template<class A, class B, class Metric>
struct MProdInstructions{
  typedef typename MGP<A,B,Metric>::Type List;
  typedef Product<List> Fun; 
};

template<class A, class B, class Metric>
struct MOProdInstructions{
  typedef typename MOP<A,B,Metric>::Type List;
  typedef Product<List> Fun; 
};

template<class A, class B, class Metric>
struct MIProdInstructions{
  typedef typename MIP<A,B,Metric>::Type List;
  typedef Product<List> Fun; 
};

template<class A, class B, class Metric>{
struct MProd { 
    MProdInstructions<A,B,Metric> Instructions;   
    static constexpr Type Call(const A& a, const B& b) {
      return Instructions::Fun::DO::template Make<Type>(a, b);
    }
};

template<class A, class B, class Metric>{
struct MOProd { 
    MOProdInstructions<A,B,Metric> Instructions;   
    static constexpr Type Call(const A& a, const B& b) {
      return Instructions::Fun::DO::template Make<Type>(a, b);
    }
};

template<class A, class B, class Metric>{
struct MIProd { 
    MIProdInstructions<A,B,Metric> Instructions;   
    static constexpr Type Call(const A& a, const B& b) {
      return Instructions::Fun::DO::template Make<Type>(a, b);
    }
};

template<class M, class A, class B > 
CA mgp(const A& a, const B& b) RETURNS(
  ( MProd<A,B,M>()::Call(a, b) )
)    
template<class M, class A, class B > 
CA mop(const A& a, const B& b) RETURNS(
  ( MOProd<A,B,M>::Call(a, b) )
)
template<class M, class A, class B >
CA mip(const A& a, const B& b) RETURNS(
  ( MIProd<A,B,M>::Call(a, b) )
)


#endif   /* ----- #ifndef vsr_metric_INC  ----- */
