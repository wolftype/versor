/*
 * =====================================================================================
 *
 *       Filename:  vsr_conformal.h
 *
 *    Description:  conformal metric (splits into diagonal metric, calculates, then pops back)
 *
 *        Version:  1.0
 *        Created:  03/14/2014 20:25:02
 *       Revision:  none
 *       Compiler:  gcc4.7 and above or clang 3.2 and above
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  pretty awesome
 *
 * =====================================================================================
 */


#ifndef  vsr_conformal_INC
#define  vsr_conformal_INC

#include "vsr_euclidean.h"
#include "vsr_split_met.h"

namespace vsr{

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
template<Bits::Type A, class B, class Metric, int idxA, int idxB>  
struct SubCGP{
  typedef typename SplitProd< A, B::HEAD, Metric>::Type Split;  
  typedef typename SplitInstructions< Split, idxA, idxB >::Type XL;   

  typedef typename XCat< 
    XL, 
    typename SubCGP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type 
  >::Type Type; 
};

/// Geometric Product Type Calculation Sub Loop End Case
template<Bits::Type A, class Metric, int idxA, int idxB>  
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
template<Bits::Type A, class B, class Metric, int idxA, int idxB>  
struct SubCOP{        
  typedef typename SplitOProd< A, B::HEAD, Metric>::Type Split; 
  typedef typename SplitInstructions< Split, idxA, idxB >::Type XL;

  typedef typename XCat< 
    XL, 
    typename SubCOP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type 
  >::Type Type;
};

/// Outer Product Type Calculation Sub Loop End Case
template<Bits::Type A, class Metric, int idxA, int idxB>  
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
template<Bits::Type A, class B, class Metric, int idxA, int idxB>  
struct SubCIP{ 
  typedef typename SplitIProd< A, B::HEAD, Metric>::Type Split;
  typedef typename SplitInstructions< Split, idxA, idxB >::Type XL;
   
  typedef typename XCat< 
    XL, 
    typename SubCIP<A, typename B::TAIL, Metric, idxA, idxB+1>::Type 
  >::Type Type;
};
/// Inner Product Type Calculation Sub Loop End Case
template<Bits::Type A, class Metric, int idxA, int idxB>  
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
 *  Product Construction Instructions
 *-----------------------------------------------------------------------------*/
template<class A, class B, class Metric>
struct CGProdInstructions{
  typedef typename CGP<A,B,Metric>::Type List;
  typedef Product<List> Fun; 
};

template<class A, class B, class Metric>
struct COProdInstructions{
  typedef typename COP<A,B,Metric>::Type List;
  typedef Product<List> Fun; 
};

template<class A, class B, class Metric>
struct CIProdInstructions{
  typedef typename CIP<A,B,Metric>::Type List;
  typedef Product<List> Fun; 
};


/*-----------------------------------------------------------------------------
 *  Product Type Automatic Instantiation
 *-----------------------------------------------------------------------------*/
template<class A, class B, class Metric>
struct CGProd { 
    typedef CGProdInstructions<typename A::Bases, typename B::Bases, Metric> Instructions;   
    typedef MV< typename Instructions::Fun::Type, typename A::ValueType > Type; 
    static constexpr Type Call(const A& a, const B& b) {
      return Instructions::Fun::DO::template Make<Type>(a, b);
    }
};

template<class A, class B, class Metric>
struct COProd { 
    typedef COProdInstructions<typename A::Bases, typename B::Bases, Metric> Instructions;   
    typedef MV< typename Instructions::Fun::Type, typename A::ValueType > Type; 
    static constexpr Type Call(const A& a, const B& b) {
      return Instructions::Fun::DO::template Make<Type>(a, b);
    }
};

template<class A, class B, class Metric>
struct CIProd { 
    typedef CIProdInstructions<typename A::Bases, typename B::Bases, Metric> Instructions;   
    typedef MV< typename Instructions::Fun::Type, typename A::ValueType > Type; 
    static constexpr Type Call(const A& a, const B& b) {
      return Instructions::Fun::DO::template Make<Type>(a, b);
    }
};

/*-----------------------------------------------------------------------------
 *  Geometric Product Type Explicit Control  
 *-----------------------------------------------------------------------------*/
template<class A, class B, class R, class M>
struct RCGProd{
  typedef typename CGP<typename A::Bases, typename B::Bases, M>::Type InstList;      
  typedef typename Index< InstList, typename R::Bases>::Type DO;   
  static constexpr R Call(const A& a, const B& b) {
    return DO::template Make<R>(a, b);
  } 
};

/*-----------------------------------------------------------------------------
 *  Product Functions
 *-----------------------------------------------------------------------------*/
template<class M, class A, class B>
CA cgp(const A& a, const B& b) RETURNS(
  (CGProd<A,B,M>::Call(a, b))
) 
template<class M, class A, class B>
CA cop(const A& a, const B& b) RETURNS(
  (COProd<A,B,M>::Call(a, b))
)
template<class M, class A, class B>
CA cip(const A& a, const B& b) RETURNS(
  (CIProd<A,B,M>::Call(a, b))
) 


/*-----------------------------------------------------------------------------
 *  Transformation Functions
 *-----------------------------------------------------------------------------*/
/// Spin a by b, return a'
template<class M, class A, class B>
constexpr A csp(const A& a, const B& b) {
  return RCGProd<typename CGProd<B,A,M>::Type, B, A, M>::Call( cgp<M>(b, a), Reverse<typename B::Bases>::Type::template Make(b) );
}

/// Reflect a by b, return a'
template<class M, class A, class B>
constexpr A cre(const A& a, const B& b) {
  return RCGProd<typename CGProd<B,A,M>::Type, B, A, M>::Call( cgp<M>(b, a.involution()), Reverse<typename B::Bases>::Type::template Make(b) );
}
 
 
/*-----------------------------------------------------------------------------
 *  CONFORMAL SPACE UTILITY 
 *-----------------------------------------------------------------------------*/
template<Bits::Type DIM>
struct CGA{ 
  static const int Dim = DIM;           
  typedef typename RMetric<DIM-1,1>::Type M;   
 
  typedef MV<typename Blade1<DIM>::VEC> TPnt; 
  typedef GA::pss<DIM> Pss;  
  typedef GA::pss<(DIM-2)> EucPss;           //Euclidean Pseudoscaler 
  typedef typename Blade1<DIM-2>::VEC   Vec;         //Euclidean Vector
  typedef GA::origin<DIM> Ori;
  typedef GA::infinity<DIM> Inf; 
  typedef GA::eplane<DIM> Mnk;
};

/*!-----------------------------------------------------------------------------
 * CONFORMAL MULTIVECTOR 
 *
 * Conformal multivectors templated on dimension and list of Bits::blades
 *-----------------------------------------------------------------------------*/
template<Bits::Type DIM, class A>
struct CGAMV : public A {

  static const int Dim = DIM;
  typedef A Type;                         ///< Type of Multivector
  typedef typename A::ValueType VT;       ///< Multivector is defined over this type 
  typedef typename CGA<DIM>::M M;        ///< Ambient Metric

  /// Construct from a list of args
  template< typename ... Args >
  constexpr CGAMV(Args...v) : A(v...) {}
  /// Construct from one instance of type A  
  constexpr CGAMV(const A& a) : A(a) {}   
  /// Construct from one instance of type A::ValueType (for tensored metrics)
  constexpr CGAMV(const typename A::ValueType& a) : A(a) {}  

  /// Construct from another Euclidean Type and/or Dimension
  template<Bits::Type BDIM, class B>
  constexpr CGAMV(const CGAMV<BDIM,B>& b) : A( b.template cast<A>() ) {}
   
  /// Geometric Product
  template<class B>
  CGAMV<DIM, typename CGProd<CGAMV, B, M>::Type> operator * ( const CGAMV<DIM, B>& b) const{
    return cgp<M>(*this, b);
  }

  /// Outer Product
  template<class B>
  CGAMV<DIM, typename COProd<CGAMV, B, M>::Type> operator ^ ( const CGAMV<DIM, B>&  b) const{
    return cop<M>(*this, b);
  }

  /// Inner Product
  template<class B>
  CGAMV<DIM, typename CIProd<CGAMV, B, M>::Type > operator <= ( const CGAMV<DIM, B>&  b) const{
    return cip<M>(*this, b);
  }
  
  /// Reversion  
  CGAMV operator ~() const {
    return Reverse<typename A::Bases>::Type::template Make(*this) ;
  }
  
  /// Inversion
  CGAMV operator !() const {    
    CGAMV tmp = ~(*this); 
    VT v = ((*this) * tmp)[0];    
    return (v==0) ? tmp : tmp / v;
  }
 
  // Division 
  template<class B>
  auto operator / (const B& b) const RETURNS(
    (  *this * !b )
  )
  
  CGAMV conj() const { return this -> conjugation(); }
  CGAMV inv() const { return this -> involution(); }
  
  template<typename B>
  CGAMV sp( const B& b) const { return csp<M>(*this, b); }    
  template<typename B>
  CGAMV re( const B& b) const { return cre<M>(*this, b); } 
    
  template<typename B>
  CGAMV spin( const B& b ) const { return sp(b); }
  template<typename B>
  CGAMV reflect( const B& b ) const { return re(b); }
   
  VT wt() const{ return (*this <= *this)[0]; }
  VT rwt() const{ return (*this <= ~(*this))[0]; }
  VT norm() const { VT a = rwt(); if(a<0) return 0; return sqrt( a ); } 
  VT rnorm() const{ VT a = rwt(); if(a<0) return -sqrt( -a ); return sqrt( a );  }  
  
  CGAMV unit() const { VT t = sqrt( fabs( (*this <= *this)[0] ) ); if (t == 0) return A(); return *this / t; }
  CGAMV runit() const { VT t = rnorm(); if (t == 0) return  A(); return *this / t; }
  CGAMV tunit() const { VT t = norm(); if (t == 0) return A(); return *this / t; }  

  template<Bits::Type BDIM, class B>
  auto operator + (const CGAMV<BDIM, B>& b) -> CGAMV< (DIM>BDIM)?DIM:BDIM, decltype( sum( A(), B() ) ) > {
    return sum(*this,b);
  }   
  
  CGAMV operator + (const CGAMV& a) const {
    CGAMV tmp;
    for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] + a[i];
    return tmp;
  }  
  
  CGAMV operator - (const CGAMV& a) const {
    CGAMV tmp;
    for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] - a[i];
    return tmp;
  }
   
  CGAMV operator -() const{
    CGAMV tmp = *this;
    for (int i = 0; i < A::Num; ++i){ tmp[i] = -tmp[i]; }
    return tmp;
  }  
  
  CGAMV& operator -=(const CGAMV& b){ 
    for (int i = 0; i < A::Num; ++i) (*this)[i] -= b[i];
    return *this;
  }   
  CGAMV& operator +=(const CGAMV& b){ 
    for (int i = 0; i < A::Num; ++i) (*this)[i] += b[i];
    return *this;
  }  
  
  CGAMV operator / (VSR_PRECISION f) const{   
    CGAMV tmp = *this;
    for (int i = 0; i < A::Num; ++i){ tmp[i] /= f; }
    return tmp;
  }
  
  CGAMV& operator /= (VSR_PRECISION f){
    for (int i = 0; i < A::Num; ++i){ (*this)[i] /= f; }
    return *this;
  }
    
  CGAMV operator * (VSR_PRECISION f) const {
    CGAMV tmp = *this;
    for (int i = 0; i < A::Num; ++i){ tmp[i] *= f; }
    return tmp;
  }
  CGAMV& operator *= (VSR_PRECISION f){
    for (int i = 0; i < A::Num; ++i){ (*this)[i] *= f; }
    return *this;
  }  
  
  auto operator + (VSR_PRECISION a) const -> CGAMV<DIM,  decltype( sumv( a, A() ) ) >  {
    return sumv(a, *this); 
  }   
  
  static CGAMV x, y, z, xy, xz, yz;  
  
  auto dual() const -> CGAMV< DIM, typename CGProd<A, MV<typename GA::pss<DIM>,VT>, M>::Type > { 
    return cgp<M>( *this , MV<typename GA::pss<DIM>, VT>(-1) );
  }       
  
  auto undual() const -> CGAMV< DIM, typename CGProd< A, MV<typename GA::pss<DIM>, VT>, M >::Type > { 
    return cgp<M>( *this , MV<typename GA::pss<DIM>, VT>(1) );
  }
  
  auto duale() const -> CGAMV< DIM, typename EGProd< A, MV<typename GA::pss<DIM-2>, VT> >::Type > { 
    return cgp<M>( *this , MV<typename GA::pss<DIM-2>, VT>(-1) );
  }       
  
  auto unduale() const -> CGAMV< DIM, typename EGProd< A, MV<typename GA::pss<DIM-2>, VT> >::Type > { 
    return cgp<M>( *this , MV<typename GA::pss<DIM-2>, VT>(1) );
  }    
  
  CGAMV<DIM, MV<typename GA::vec<DIM>, typename A::ValueType>> null() const;


  template<class B>
  static void PrintGP(){
    CGProd<A,B,M>::Instructions::Fun::DO::print();
  }
  
  template<typename T>
  CGAMV rot( const T&) const;
  template<typename T>
  CGAMV rot(VSR_PRECISION a, const T&) const;

  template<typename T>
  CGAMV rotate( const T& ) const;
  template<typename T>
  CGAMV rotate(VSR_PRECISION a, const T& ) const;

              
  template<typename T>
  CGAMV trs( const T& ) const;     
  template<typename T>  
  CGAMV translate( const T& ) const;

  template<class ... T>   
  CGAMV trs( T ... v) const;
  template<class ... T>   
  CGAMV translate( T ... v) const;
  
  template<typename T>
  CGAMV trv( const T& ) const;     
  template<class ... T>   
  CGAMV trv( T ... v) const; 
  template<typename T>
  CGAMV transverse( const T& ) const;     
  template<class ... T>   
  CGAMV transverse( T ... v) const; 

  template<typename T>
  CGAMV mot( const T& ) const; 
  template<typename T>
  CGAMV dil( const T& ) const;
  template<typename P, typename T>
  CGAMV dil( const P&, const T& ) const; 
  template<typename T>
  CGAMV dilate( const T& ) const;
  template<typename P, typename T>
  CGAMV dilate( const P&, const T& ) const; 
  template<typename T>
  CGAMV scale( const T& ) const; //same as dilate
  template<typename P, typename T>
  CGAMV scale( const P&, const T& ) const; 

  template<typename T>
  CGAMV bst( const T& ) const;   
  template<typename T>
  CGAMV boost( const T& ) const;  
  template<typename T>
  CGAMV motor( const T& ) const; 
  template<typename T>
  CGAMV twist( const T& ) const; //same as motor                                                                                      
};   


/*-----------------------------------------------------------------------------
 *  CONFORMAL CANDIDATES
 *-----------------------------------------------------------------------------*/

  template<Bits::Type N, typename T=VSR_PRECISION> using NSca = CGAMV<N, MV<GA::Sca,T>>;
  template<Bits::Type N, typename T=VSR_PRECISION> using NVec = CGAMV<N, MV<typename Blade1<N-2>::VEC ,T>>;
  template<Bits::Type N, typename T=VSR_PRECISION> using NPss = CGAMV<N, MV<typename GA::pss<N>, T>>;
  template<Bits::Type N, typename T=VSR_PRECISION> using NEucPss = CGAMV<N, MV<typename GA::pss<N-2>, T>>;

  template<Bits::Type N, typename T=VSR_PRECISION> using NBiv = decltype( NVec<N,T>() ^ NVec<N,T>() ); 
  template<Bits::Type N, typename T=VSR_PRECISION> using NTri = decltype( NBiv<N,T>() ^ NVec<N,T>() ); 
  template<Bits::Type N, typename T=VSR_PRECISION> using NRot = decltype( NVec<N,T>() * NVec<N,T>() ); 

  template<Bits::Type N, typename T=VSR_PRECISION> using NPnt = CGAMV<N, MV<typename Blade1<N>::VEC ,T>>;
  template<Bits::Type N, typename T=VSR_PRECISION> using NPar = decltype( NPnt<N,T>() ^ NPnt<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NCir = decltype( NPnt<N,T>() ^ NPar<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NSph = decltype( NPnt<N,T>() ^ NCir<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NDls = NPnt<N,T>;
    
  template<Bits::Type N, typename T=VSR_PRECISION> using NOri = CGAMV<N, MV<typename GA::origin<N>,T>>;
  template<Bits::Type N, typename T=VSR_PRECISION> using NInf = CGAMV<N, MV<typename GA::infinity<N>,T>>;
  template<Bits::Type N, typename T=VSR_PRECISION> using NMnk = decltype( NOri<N,T>() ^ NInf<N,T>() );
  
  template<Bits::Type N, typename T=VSR_PRECISION> using NFlp = decltype( NPnt<N,T>() ^ NInf<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NLin = decltype( NPar<N,T>() ^ NInf<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NPln = decltype( NCir<N,T>() ^ NInf<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NDll = decltype( NLin<N,T>() * NPss<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NDlp = decltype( NPln<N,T>() * NPss<N,T>() );

  template<Bits::Type N, typename T=VSR_PRECISION> using NTnv = decltype( NVec<N,T>() ^ NOri<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NDrv = decltype( NVec<N,T>() ^ NInf<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NTnb = decltype( NBiv<N,T>() ^ NOri<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NDrb = decltype( NBiv<N,T>() ^ NInf<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NTnt = decltype( NTri<N,T>() ^ NOri<N,T>() );
  template<Bits::Type N, typename T=VSR_PRECISION> using NDrt = decltype( NTri<N,T>() ^ NInf<N,T>() );
  
  template<Bits::Type N, typename T=VSR_PRECISION> using NTrs = decltype(NDrv<N,T>() + 1);//sumv(1., NDrv<N,T>()));
  template<Bits::Type N, typename T=VSR_PRECISION> using NTrv = decltype(NTnv<N,T>() + 1);
  template<Bits::Type N, typename T=VSR_PRECISION> using NDil = decltype(NMnk<N,T>() + 1);
  template<Bits::Type N, typename T=VSR_PRECISION> using NMot = decltype(NRot<N,T>() * NTrs<N,T>());
  template<Bits::Type N, typename T=VSR_PRECISION> using NBst = decltype(NPar<N,T>() + 1);
  template<Bits::Type N, typename T=VSR_PRECISION> using NTsd = decltype(NFlp<N,T>() + 1);

}//vsr::

#endif   /* ----- #ifndef vsr_conformal_INC  ----- */

