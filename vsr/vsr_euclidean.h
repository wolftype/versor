/*
 * =====================================================================================
 *
 *       Filename:  vsr_euclidean.h
 *
 *    Description:  euclidean products (compile-time fun)
 *
 *        Version:  1.0
 *        Created:  03/10/2014 17:46:59
 *       Revision:  none
 *       Compiler:  gcc4.7 or clang 3.2 or greater
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  ucsb
 *
 *   EUCLIDEAN (METRIC IS ASSUMED TO BE Only Positive , e.g R4, R5, R6, etc) 
 *
 * =====================================================================================
 */


#ifndef  vsr_euclidean_INC
#define  vsr_euclidean_INC

#include "vsr_xlists.h"
#include "vsr_instructions.h"
#include "vsr_types.h"
#include "vsr_versions.h"


namespace vsr{
                             
/*-----------------------------------------------------------------------------
 *  GEOMETRIC PRODUCT COMPILE-TIME ROUTINES
 *-----------------------------------------------------------------------------*/
template<TT A, class B, int idxA, int idxB>  
struct SubEGP{
  typedef typename XCat< XList< Inst<signFlip( A, B::HEAD ), A, B::HEAD, idxA, idxB> >, typename SubEGP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, int idxA, int idxB>  
struct SubEGP<A, MVBasis<>, idxA, idxB >{
  typedef XList<> Type;
};

template<class A, class B, int idxA=0, int idxB=0>  
struct EGP{
  typedef typename XCat< typename SubEGP<A::HEAD,B, idxA,idxB>::Type, typename EGP<typename A::TAIL, B, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, int idxA, int idxB>  
struct EGP<MVBasis<>,B, idxA,idxB> {
  typedef XList<> Type; 
}; 



/*-----------------------------------------------------------------------------
 *  OUTER PRODUCT COMPILE-TIME ROUTINES
 *-----------------------------------------------------------------------------*/
template<TT A, class B, int idxA, int idxB>  
struct SubEOP{ 
  typedef Inst<signFlip( A, B::HEAD ), A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::OP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubEOP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, int idxA, int idxB>  
struct SubEOP<A, MVBasis<>, idxA, idxB >{
  typedef XList<> Type;
};

template<class A, class B, int idxA=0, int idxB=0>  
struct EOP{
  typedef typename XCat< typename SubEOP<A::HEAD,B, idxA,idxB>::Type, typename EOP<typename A::TAIL, B, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, int idxA, int idxB>  
struct EOP<MVBasis<>,B, idxA,idxB> {
  typedef XList<> Type; 
};



/*-----------------------------------------------------------------------------
 *  INNER PRODUCT COMPILE-TIME ROUTINES
 *-----------------------------------------------------------------------------*/
template<TT A, class B, int idxA, int idxB>  
struct SubEIP{ 
  typedef Inst<signFlip( A, B::HEAD ), A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::IP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubEIP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type; 
};
template<TT A, int idxA, int idxB>  
struct SubEIP<A, MVBasis<>, idxA, idxB >{
  typedef XList<> Type;
};

template<class A, class B, int idxA=0, int idxB=0>  
struct EIP{
  typedef typename XCat< typename SubEIP<A::HEAD,B, idxA,idxB>::Type, typename EIP<typename A::TAIL, B, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, int idxA, int idxB>  
struct EIP<MVBasis<>,B, idxA,idxB> {
  typedef XList<> Type; 
};



/*-----------------------------------------------------------------------------
 *  Product Construction Instructions
 *-----------------------------------------------------------------------------*/
template<class T>
struct Product {
  typedef typename Reduce<T>::Type Type;
  typedef typename Index<T, Type>::Type DO;
};

template<class A, class B>
struct EProdInstructions{
  typedef typename EGP<A,B>::Type InstList; 
  typedef Product<InstList> Fun;
};
template<class A, class B>
struct EOProdInstructions{
  typedef typename EOP<A,B>::Type InstList; 
  typedef Product<InstList> Fun;
};
template<class A, class B>
struct EIProdInstructions{
  typedef typename EIP<A,B>::Type InstList; 
  typedef Product<InstList> Fun;
};


/*-----------------------------------------------------------------------------
 *  Product Type Instantiation
 *-----------------------------------------------------------------------------*/
template<class A, class B>
struct EProd {  
  typedef EProdInstructions< typename A::Bases, typename B::Bases> Instructions;
  typedef MV< typename Instructions::Fun::Type, typename A::ValueType > Type; 
  static constexpr Type Call(const A& a, const B& b) {
    return Instructions::Fun::DO::template Make<Type>(a, b);
  }   
};

template<class A, class B>
struct EOProd{
  typedef EOProdInstructions< typename A::Bases, typename B::Bases> Instructions;
  typedef MV< typename Instructions::Fun::Type, typename A::ValueType > Type; 
  static constexpr Type Call(const A& a, const B& b) {
    return Instructions::Fun::DO::template Make<Type>(a, b);
  } 
}; 

template<class A, class B>
struct EIProd{
  typedef EIProdInstructions< typename A::Bases, typename B::Bases> Instructions;
  typedef MV< typename Instructions::Fun::Type, typename A::ValueType > Type; 
  static constexpr Type Call(const A& a, const B& b) {
    return Instructions::Fun::DO::template Make<Type>(a, b);
  } 
}; 


/*-----------------------------------------------------------------------------
 *  Product Functions
 *-----------------------------------------------------------------------------*/
template<class A, class B>
CA egp(const A& a, const B& b) RETURNS(
  ( EProd<A,B>::Call(a, b) )
)
template<class A, class B>
CA eop(const A& a, const B& b) RETURNS(
  ( EOProd<A,B>::Call(a, b) )
)
template<class A, class B>
CA eip(const A& a, const B& b) RETURNS(
  ( EIProd<A,B>::Call(a, b) )
)



/*-----------------------------------------------------------------------------
 *  EUCLIDEAN SPACE
 *-----------------------------------------------------------------------------*/
template<TT DIM>
struct EGA {
  
  typedef MVBasis<0> Sca; 
  template<TT ... N> using e = MVBasis< blade( (1<<(N-1))... ) >;  
  
  using Vec = typename Blade1<DIM>::VEC; 
  using Pss = MVBasis< _vsr_make_pss(DIM) >;
};



/*!-----------------------------------------------------------------------------
 * EUCLIDEAN MULTIVECTOR 
 *-----------------------------------------------------------------------------*/
template<TT NDIM, class A>
struct EGAMV : public A {

  static const int DIM = NDIM;
  typedef A Type;
  typedef typename A::ValueType VT;

  template< class ... Args >
  constexpr EGAMV(Args...v) : A(v...) {}  
  constexpr EGAMV(const A& a) : A(a) {}   
  
  /// Construct from another Euclidean Dimension (make more specific)
//  template<TT BDIM, class B>
//  constexpr EGAMV(const EGAMV<BDIM,B>& b) : A( b.template cast<A>() ) {}
   
  /// Geometric Product
  template<class B>
  EGAMV<DIM, typename EProd< EGAMV, B >::Type > operator * ( const B& b) const{
    return egp(*this, b);
  }

  /// Outer Product
  template<class B>
  EGAMV<DIM, typename EOProd< EGAMV, B >::Type > operator ^ ( const B& b) const{
    return eop(*this, b);
  }

  /// Inner Product
  template<class B>
  EGAMV<DIM, typename EIProd< EGAMV, B >::Type > operator <= ( const B& b) const{
    return eip(*this, b);
  }
  
  /// Reversion  
  EGAMV operator ~() const {
    return Reverse< A >::Type::template Make(*this) ;
  }
  
  /// Inversion
  EGAMV operator !() const {    
    EGAMV tmp = ~(*this); 
    VT v = ((*this) * tmp)[0];    
    return (v==0) ? tmp : tmp / v;
  }
  
  template<class B>
  auto operator / (const B& b) const RETURNS(
    (  *this * !b )
  )
  
  EGAMV conj() const { return this -> conjugation(); }
  EGAMV inv() const { return this -> involution(); }
  
  template<typename B>
  EGAMV sp( const B& b) const { return (b * (*this) * ~b).template cast<A>(); }    
  template<typename B>
  EGAMV re( const B& b) const { return (b * (*this).inv() * !b).template cast<A>(); } 
  
  template<typename B>
  EGAMV reflect( const B& b ) const { return re(b); }
   
  VT wt() const{ return (*this <= *this)[0]; }
  VT rwt() const{ return (*this <= ~(*this))[0]; }
  VT norm() const { VT a = rwt(); if(a<0) return 0; return sqrt( a ); } 
  VT rnorm() const{ VT a = rwt(); if(a<0) return -sqrt( -a ); return sqrt( a );  }  
  
  EGAMV unit() const { VT t = sqrt( fabs( (*this <= *this)[0] ) ); if (t == 0) return A(); return *this / t; }
  EGAMV runit() const { VT t = rnorm(); if (t == 0) return  A(); return *this / t; }
  EGAMV tunit() const { VT t = norm(); if (t == 0) return A(); return *this / t; }  

  template<TT BDIM, class B>
  auto operator + (const EGAMV<BDIM, B>& b) -> EGAMV< (DIM>BDIM)?DIM:BDIM, decltype( sum( A(), B() ) ) > {
    return sum(*this,b);
  }   
  
  EGAMV operator + (const EGAMV& a) const {
    EGAMV tmp;
    for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] + a[i];
    return tmp;
  }  
  
  EGAMV operator - (const EGAMV& a) const {
    EGAMV tmp;
    for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] - a[i];
    return tmp;
  }
   
  EGAMV operator -() const{
    EGAMV tmp = *this;
    for (int i = 0; i < A::Num; ++i){ tmp[i] = -tmp[i]; }
    return tmp;
  }  
  
  EGAMV& operator -=(const EGAMV& b){ 
    for (int i = 0; i < A::Num; ++i) (*this)[i] -= b[i];
    return *this;
  }   
  EGAMV& operator +=(const EGAMV& b){ 
    for (int i = 0; i < A::Num; ++i) (*this)[i] += b[i];
    return *this;
  }  
  
  EGAMV operator / (VSR_PRECISION f) const{   
    EGAMV tmp = *this;
    for (int i = 0; i < A::Num; ++i){ tmp[i] /= f; }
    return tmp;
  }
  
  EGAMV& operator /= (VSR_PRECISION f){
    for (int i = 0; i < A::Num; ++i){ (*this)[i] /= f; }
    return *this;
  }
    
  EGAMV operator * (VSR_PRECISION f) const {
    EGAMV tmp = *this;
    for (int i = 0; i < A::Num; ++i){ tmp[i] *= f; }
    return tmp;
  }
  EGAMV& operator *= (VSR_PRECISION f){
    for (int i = 0; i < A::Num; ++i){ (*this)[i] *= f; }
    return *this;
  }  
  
  auto operator + (VSR_PRECISION a) const -> EGAMV<DIM,  decltype( sumv( a, A() ) ) >  {
    return sumv(a, *this); 
  }   
  
  static EGAMV x, y, z, xy, xz, yz;  
  
  auto dual() const -> EGAMV< DIM, typename EProd< A, MV<typename EGA<DIM>::Pss, VT> >::Type > { 
    return egp( *this , MV<typename EGA<DIM>::Pss, VT>(-1) );
  }       
  
  auto undual() const -> EGAMV< DIM, typename EProd< A, MV<typename EGA<DIM>::Pss, VT> >::Type > { 
    return egp( *this , MV<typename EGA<DIM>::Pss, VT>(1) );
  }  
  
  template<typename T>
  EGAMV rot( const T& ) const;
                                                                                        
};   



//EUCLIDEAN CANDIDATES
template<TT N, typename T=VSR_PRECISION> using NESca = EGAMV<N, MV<typename EGA<N>::Sca,T> >;   
template<TT N, typename T=VSR_PRECISION> using NEVec = EGAMV<N, MV<typename EGA<N>::Vec,T> >; 
template<TT N, typename T=VSR_PRECISION> using NEPss = EGAMV<N, MV<typename EGA<N>::Pss,T> >; 

template<typename T, TT N, TT ... NN> using EucE   = EGAMV<N, MV<typename EGA<N>::template e<NN...>, T> >; 

template<TT N, typename T=VSR_PRECISION> using NEBiv = decltype( NEVec<N>() ^ NEVec<N>() );
template<TT N, typename T=VSR_PRECISION> using NETri = decltype( NEVec<N>() ^ NEBiv<N>() );
template<TT N, typename T=VSR_PRECISION> using NERot = decltype( NEVec<N>() * NEVec<N>() );


}//vsr::

#endif   /* ----- #ifndef vsr_euclidean_INC  ----- */






