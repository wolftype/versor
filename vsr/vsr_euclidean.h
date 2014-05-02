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
 *  GEOMETRIC PRODUCT COMPILE-TIME TYPE CALCULATION ROUTINES
 *-----------------------------------------------------------------------------*/
/// Geometric Product Type Calculation Sub Loop
template<Bits::Type A, class B, int idxA, int idxB>  
struct SubEGP{
  typedef typename XCat< XList< Inst<Bits::signFlip( A, B::HEAD ), A, B::HEAD, idxA, idxB> >, typename SubEGP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type; 
};
/// Geometric Product Type Calculation Sub Loop End Case
template<Bits::Type A, int idxA, int idxB>  
struct SubEGP<A, Basis<>, idxA, idxB >{
  typedef XList<> Type;
};
/// Geometric Product Type Calculation Main Loop
template<class A, class B, int idxA=0, int idxB=0>  
struct EGP{
  typedef typename XCat< typename SubEGP<A::HEAD,B, idxA,idxB>::Type, typename EGP<typename A::TAIL, B, idxA+1,idxB>::Type >::Type Type; 
};
/// Geometric Product Type Calculation Main Loop End Case
template<class B, int idxA, int idxB>  
struct EGP<Basis<>,B, idxA,idxB> {
  typedef XList<> Type; 
}; 


/*-----------------------------------------------------------------------------
 *  OUTER PRODUCT COMPILE-TIME TYPE CALCULATION ROUTINES
 *-----------------------------------------------------------------------------*/
/// Outer Product Type Calculation Sub Loop
template<Bits::Type A, class B, int idxA, int idxB>  
struct SubEOP{ 
  typedef Inst<Bits::signFlip( A, B::HEAD ), A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::OP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubEOP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type; 
};
/// Outer Product Type Calculation Sub Loop End Case
template<Bits::Type A, int idxA, int idxB>  
struct SubEOP<A, Basis<>, idxA, idxB >{
  typedef XList<> Type;
};
/// Outer Product Type Calculation Main Loop
template<class A, class B, int idxA=0, int idxB=0>  
struct EOP{
  typedef typename XCat< typename SubEOP<A::HEAD,B, idxA,idxB>::Type, typename EOP<typename A::TAIL, B, idxA+1,idxB>::Type >::Type Type; 
};
/// Outer Product Type Calculation Main Loop End Case
template<class B, int idxA, int idxB>  
struct EOP<Basis<>,B, idxA,idxB> {
  typedef XList<> Type; 
};



/*-----------------------------------------------------------------------------
 *  INNER PRODUCT COMPILE-TIME TYPE CALCULATION ROUTINES
 *-----------------------------------------------------------------------------*/
/// Inner Product Type Calculation Sub Loop
template<Bits::Type A, class B, int idxA, int idxB>  
struct SubEIP{ 
  typedef Inst<Bits::signFlip( A, B::HEAD ), A, B::HEAD, idxA, idxB> INST;
  typedef typename Maybe< INST::IP, XList< INST >, XList<> >::Type ELEM;
  typedef typename XCat< ELEM, typename SubEIP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type; 
};
/// Inner Product Type Calculation Sub Loop End Case
template<Bits::Type A, int idxA, int idxB>  
struct SubEIP<A, Basis<>, idxA, idxB >{
  typedef XList<> Type;
};
/// Inner Product Type Calculation Main loop
template<class A, class B, int idxA=0, int idxB=0>  
struct EIP{
  typedef typename XCat< typename SubEIP<A::HEAD,B, idxA,idxB>::Type, typename EIP<typename A::TAIL, B, idxA+1,idxB>::Type >::Type Type; 
};
/// Inner Product Type Calculation Main Loop End Case
template<class B, int idxA, int idxB>  
struct EIP<Basis<>,B, idxA,idxB> {
  typedef XList<> Type; 
};



/*-----------------------------------------------------------------------------
 *  Product Construction Instructions
 *-----------------------------------------------------------------------------*/
template<class A, class B>
struct EGProdInstructions{
  typedef typename EGP<A,B>::Type List; 
  typedef Product<List> Fun;
};
template<class A, class B>
struct EOProdInstructions{
  typedef typename EOP<A,B>::Type List; 
  typedef Product<List> Fun;
};
template<class A, class B>
struct EIProdInstructions{
  typedef typename EIP<A,B>::Type List; 
  typedef Product<List> Fun;
};


/*-----------------------------------------------------------------------------
 *  Product Type Automatic Instantiation
 *-----------------------------------------------------------------------------*/
template<class A, class B>
struct EGProd {  
  typedef EGProdInstructions< typename A::Bases, typename B::Bases> Instructions;
  typedef typename Instructions::Fun::Type Bases;
  typedef MV< Bases, typename A::ValueType > Type; 
//  template<class AA, class BB>
  static constexpr Type Call(const A& a, const B& b) {
    return Instructions::Fun::DO::template Make<Type>(a, b);
  }   
};

template<class A, class B>
struct EOProd{
  typedef EOProdInstructions< typename A::Bases, typename B::Bases> Instructions;
  typedef typename Instructions::Fun::Type Bases;
  typedef MV< Bases, typename A::ValueType > Type; 
//  template<class AA, class BB>
  static constexpr Type Call(const A& a, const B& b) {
    return Instructions::Fun::DO::template Make<Type>(a, b);
  } 
}; 

template<class A, class B>
struct EIProd{
  typedef EIProdInstructions< typename A::Bases, typename B::Bases> Instructions;
  typedef typename Instructions::Fun::Type Bases;
  typedef MV< Bases, typename A::ValueType > Type; 

//  template<class AA, class BB>
  static constexpr Type Call(const A& a, const B& b) {
    return Instructions::Fun::DO::template Make<Type>(a, b);
  } 
}; 

/*-----------------------------------------------------------------------------
 *  Product Type Explicit Control  
 *-----------------------------------------------------------------------------*/
template<class A, class B, class R>
struct REProd{
  typedef typename EGP<typename A::Bases, typename B::Bases>::Type InstList;      
  typedef typename Index< InstList, R>::Type DO;   
  static constexpr R Call(const A& a, const B& b) {
    return DO::template Make<R>(a, b);
  } 
};


/*-----------------------------------------------------------------------------
 *  Product Functions
 *-----------------------------------------------------------------------------*/
template<class A, class B>
CA egp(const A& a, const B& b) RETURNS(
  ( EGProd<A,B>::Call(a, b) )
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
 *  Transformation Functions
 *-----------------------------------------------------------------------------*/
/// Spin a by b, return a
template<class A, class B>
constexpr A esp(const A& a, const B& b) {
  return REProd< typename EGProd<B,A>::Type, B, A >::Call( egp(b, a),   Reverse<typename B::Bases>::Type::template Make(b) );
}

/// Reflect a by b, return a
template<class A, class B>
constexpr A ere(const A& a, const B& b) {
  return REProd< typename EGProd<B,A>::Type, B, A>::Call( egp(b, a.involution() ),   Reverse<typename B::Bases>::Type::template Make(b) );
}



/*-----------------------------------------------------------------------------
 *  EUCLIDEAN SPACE UTILITY MULTIVECTORS
 *-----------------------------------------------------------------------------*/
template<Bits::Type DIM, typename T=VSR_PRECISION> 
struct EGA {
  
  //typedef Basis<0> Sca; 
 // template<Bits::Type ... N> using e = Basis< Bits::blade((1<<(N-1))...) >;  
    
  using Vec = MV<typename Blade1<DIM>::VEC,T>; 
  using Pss = MV<Basis<Bits::pss(DIM)>,T>;
  using Biv = typename EOProd<Vec,Vec>::Type;
  using Rot = typename EGProd<Vec,Vec>::Type;

  /* template<Bits::Type DIM, typename T> using Biv = typename EOProd<Vec<DIM,T>,Vec<DIM,T>>::Type; */
  /* template<Bits::Type DIM, typename T> using Rot = typename EGProd<Vec<DIM,T>,Vec<DIM,T>>::Type; */

};



/*!-----------------------------------------------------------------------------
 * EUCLIDEAN MULTIVECTOR 
 *
 * Euclidean multivectors templated on dimension and list of Bits::blades
 *-----------------------------------------------------------------------------*/
template<Bits::Type NDIM, class A>
struct EGAMV : public A {

  typedef EGA<NDIM> Mode;
  template<class B> using BType = EGAMV<NDIM, B>; 

  static const int DIM = NDIM;
  typedef A Type;
  typedef typename A::ValueType VT;

  /// Construct from a list of args
  template< typename ... Args >
  constexpr EGAMV(Args...v) : A(v...) {}
  /// Construct from one instance of type A  
  constexpr EGAMV(const A& a) : A(a) {}   
  /// Construct from one instance of type A::ValueType (for tensored metrics)
  constexpr EGAMV(const typename A::ValueType& a) : A(a) {}  

  /// Construct from another Euclidean Type and/or Dimension
  template<Bits::Type BDIM, class B>
  constexpr EGAMV(const EGAMV<BDIM,B>& b) : A( b.template cast<A>() ) {}
   
  /// Geometric Product
  template<class B>
  EGAMV<DIM, typename EGProd< EGAMV, B >::Type > operator * ( const EGAMV<DIM, B>& b) const{
    return egp(*this, b);
  }

  /// Outer Product
  template<class B>
  EGAMV<DIM, typename EOProd< EGAMV, B >::Type > operator ^ ( const EGAMV<DIM, B>& b) const{
    return eop(*this, b);
  }

  /// Inner Product
  template<class B>
  EGAMV<DIM, typename EIProd< EGAMV, B >::Type > operator <= ( const EGAMV<DIM, B>& b) const{
    return eip(*this, b);
  }
  
  /// Reversion  
  EGAMV operator ~() const {
    return Reverse<typename A::Bases>::Type::template Make(*this) ;
  }
  
  /// Inversion
  EGAMV operator !() const {    
    EGAMV tmp = ~(*this); 
    VT v = ((*this) * tmp)[0];    
    return (v==0) ? tmp : tmp / v;
  }
 
  // Division 
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

  template<Bits::Type BDIM, class B>
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
  
  auto dual() const -> EGAMV< DIM, typename EGProd< A, MV<typename GA::pss<DIM>, VT> >::Type > { 
    return egp( *this , MV<typename GA::pss<DIM>, VT>(-1) );
  }       
  
  auto undual() const -> EGAMV< DIM, typename EGProd< A, MV<typename GA::pss<DIM>, VT> >::Type > { 
    return egp( *this , MV<typename GA::pss<DIM>, VT>(1) );
  }  
  
  template<typename R>
  EGAMV rot( const R& ) const;

  template<class B>
  static void PrintGP(){
    EGProd<A,B>::Instructions::Fun::DO::print();
  }

  static EGAMV x,y,z,xy,xz,yz; 
                                                                                        
};   

template<Bits::Type DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::x  = A().A::template set<1>(1);
template<Bits::Type DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::y  = A().A::template set<2>(1); 
template<Bits::Type DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::z  = A().A::template set<4>(1); 
template<Bits::Type DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::xy = A().A::template set<3>(1); 
template<Bits::Type DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::xz = A().A::template set<5>(1); 
template<Bits::Type DIM, class A> EGAMV<DIM,A> EGAMV<DIM,A>::yz = A().A::template set<6>(1); 



//EUCLIDEAN CANDIDATES
template<Bits::Type N, typename T=VSR_PRECISION> using NESca = EGAMV<N, MV<GA::Sca,T> >;   
/* template<Bits::Type N, typename T=VSR_PRECISION> using NEVec = EGAMV<N, EGA::Vec<N,T> >; */ 
/* template<Bits::Type N, typename T=VSR_PRECISION> using NEPss = EGAMV<N, EGA::Pss<N,T> >; */ 
/* template<Bits::Type N, typename T=VSR_PRECISION> using NERot = EGAMV<N, EGA::Rot<N,T> >; */

template<Bits::Type N, typename T=VSR_PRECISION> using NEVec = EGAMV<N, typename EGA<N,T>::Vec >; 
template<Bits::Type N, typename T=VSR_PRECISION> using NEPss = EGAMV<N, typename EGA<N,T>::Pss >; 
template<Bits::Type N, typename T=VSR_PRECISION> using NERot = EGAMV<N, typename EGA<N,T>::Rot >;


template<typename T=VSR_PRECISION>
struct NE{
    template <Bits::Type ... NN> using e = EGAMV< Bits::dimOf( Bits::blade((1<<(NN-1))...) ), MV<typename GA::template e<NN...>, T> >; 
};

//template< Bits::Type ... NN, typename T=VSR_PRECISION> using NE = EGAMV< Bits::dimOf( Bits::blade((1<<(NN-1))...) ), MV<typename EGA< Bits::blade((1<<(NN-1))...)>::template e<NN...>, T> >; 
///template< template <Bits::Type N> class S, typename T=VSR_PRECISION> using NE = EGAMV< S::DIM, S, T> >; 

template<Bits::Type N, typename T=VSR_PRECISION> using NEBiv = decltype( NEVec<N>() ^ NEVec<N>() );
template<Bits::Type N, typename T=VSR_PRECISION> using NETri = decltype( NEVec<N>() ^ NEBiv<N>() );
//decltype( NEVec<N>() * NEVec<N>() );


}//vsr::

#endif   /* ----- #ifndef vsr_euclidean_INC  ----- */






