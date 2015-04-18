/*
 * =====================================================================================
 *
 *       Filename:  vsr_mv.h
 *
 *    Description:  multivector
 *
 *        Version:  1.0
 *        Created:  04/07/2015 15:54:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#ifndef MV_H_INCLUDED
#define MV_H_INCLUDED  

#include "vsr_xlists.h"   ///<-- list processing functions
#include "vsr_products.h" ///<-- compile time processing of instructions ("arrows" or "morphisms")
#include "vsr_algebra.h"  ///<-- algebra implementation (EGA, MGA, CGA)

#include <math.h>   
#include <iostream>  

namespace vsr{ 
    
  

    /*-----------------------------------------------------------------------------
     *  A Geometric Algebra is templated on 
     *     a metric (e.g. RMetric<3,0> for euclidean 3 space or RMetric<4,1,true> for conformal 3 space )
     *     a field value type (i.e. real, complex, or some other arithmetic element).
     *        The value type can be anything that multiplies and adds, including another Multivector,
     *        allowing for tensor metrics C x C, etc a la Bott periodicity.
    
          To Do: enable conversions between algebras!
     *-----------------------------------------------------------------------------*/
    //template< class TMetric , typename T=VSR_PRECISION >
    template< class TMetric, class T >
    struct ga {
         
        using impl = AlgebraImpl< ga<TMetric, T>, TMetric::is_euclidean, TMetric::is_conformal >;    ///<-- implementation details
        using types = basetype<impl>;

        ///Next Higher Algebra over the same field
        using up = ga< typename TMetric::up, T>;  
        
        using metric = TMetric;                          ///<-- Metric, whether Euclidean, Projective, Conformal, etc
      //  using MetricList = typename TMetric::Type ;      ///<-- Metric as List of Values
        typedef T valuetype;                             ///<-- Field over which Algebra is Defined
        static const int dim = TMetric::type::Num;          ///<-- Dimension of Algebra

        using vector = typename Blade1<dim>::VEC;        ///<-- 1-blade vector element

    };

    /*!-----------------------------------------------------------------------------
     *  THE UNIVERSAL MULTIVECTOR CLASS (built from an algbera TALGEBRA on a sub basis A )
     *-----------------------------------------------------------------------------*/
    template<typename TAlgebra, typename A>
    struct MV{
      
      typedef TAlgebra algebra;                ///<-- mother algebra
      typedef A basis;                         ///<-- basis of this type
      static const int Num = basis::Num;       ///<-- number of bases


     // using metric =  typename Algebra::MetricList;
      using VT =      typename algebra::valuetype;
      using impl =    typename algebra::impl;
      using Space =   typename algebra::types;            ///<-- call ::Space:: to get to another Type in same Algebra

      template<class B> using MVB = MV<TAlgebra,B>;       ///<-- another basis within same algebra

      using Dual = MVB< typename impl::template GPBasis< A, typename basis_t::pss<algebra::dim> > >;   ///<-- Define Dual Type
      using DualE = MVB< typename impl::template GPBasis< A, typename basis_t::epss<algebra::dim> > >; ///<-- Define Euclidean Dual Type
    
    
      /// Data 
      VT val[basis::Num];
      /// Data Access
      typedef const VT array_type[basis::Num]; 
      array_type& begin() const { return val; } 
      constexpr VT operator[] (int idx) const{
        return val[idx]; 
      }
      VT& operator[] (int idx){
        return val[idx]; 
      }
    
      /// Constructors
      template<typename...Args>     
      constexpr explicit MV(Args...v) :
      val{ static_cast<VT>(v)...} {} 

      /// From different Basis within same Algebra
      template<typename B>
      constexpr MV( const MV<algebra,B>& b) { *this = b.template cast<MV<algebra,A>>(); }


      /// Getters, Setters
      template<bits::type IDX> VT get() const;
      template<bits::type IDX> VT& get(); 
      template<bits::type IDX> MV& set(VT v); 
    
      /// Reset
      MV& reset(VT v = VT()){
        std::fill( &(val[0]), &(val[0]) + basis::Num, v); 
        return *this;
      }
    
      /// Unary Operation Conjugation
      MV conjugation() const;
      /// Unary Operation Conjugation Shorthand
      MV conj() const { return conjugation(); }
      /// Unary Operation Involution
      MV involution() const; 
      /// Unary Operation Involution Shorthand
      MV inv() const { return involution(); }
    
      /// Casting to type B
      template<class B> B cast() const; 
      /// Copying to type B
      template<class B> B copy() const; 
    
      /// Comparison
      bool operator == (const MV& mv) const{
        for (int i = 0; i < Num; ++i) {
          if (val[i] != mv[i]) return false;
        }
          return true;
      }  
    
      /// Printing
      void print(){
        basis::print();
        VPrint::Call( *this );
      }

      
      /*-----------------------------------------------------------------------------
       *  Dispatched functions (called by AlgebraImpl see core/vsr_algebra.h)
       *-----------------------------------------------------------------------------*/
      /// Geometric Product
      template<class B>
      auto operator * ( const MVB<B>& b) const -> decltype(impl::gp(*this,b)) {
        return impl::gp(*this, b);
      }

      /// Outer Product
      template<class B>
      auto operator ^ ( const MVB<B>&  b) const-> decltype(impl::op(*this,b)) {
        return impl::op(*this, b);
      }

      /// Inner Product
      template<class B>
      auto operator <= ( const MVB<B>&  b) const-> decltype(impl::ip(*this,b)) {
        return impl::ip(*this, b);
      }
      /// Rotor (even) transformation
      template<typename B>
      MV spin( const MVB<B>& b ) const { return impl::sp(*this,b); }
      /// Versor (Odd) Transformation
      template<typename B>
      MV reflect( const MVB<B>& b ) const { return impl::re(*this,b); }

      template<typename B> MV sp( const MVB<B>& b) const { return spin(b); }
      template<typename B> MV re( const MVB<B>& b) const { return reflect(b); }

      /// Reversion  
      MV operator ~() const {
        return Reverse<A>::Type::template Make(*this) ;
      }
      
      /// Inversion
      MV operator !() const {    
        MV tmp = ~(*this); 
        VT v = ((*this) * tmp)[0];    
        return (v==0) ? tmp : tmp / v;
      }
     
      // Division 
      template<class B>
      auto operator / (const MVB<B>& b) const RETURNS(
        (  *this * !b )
      )
  
      /*-----------------------------------------------------------------------------
       *  Transformations (note mostly here for conformal metric)
       *  for instance null() only works in conformal metric (returns identity in others)
       *  these are all defined in vsr_generic_op.h or in vsr_cga3D_op.h
       *-----------------------------------------------------------------------------*/
       MV<algebra, typename algebra::vector > null() const; 
        

       template<class B>
       MV rot ( const MVB<B>& b ) const; 
       template<class B>
       MV rotate ( const MVB<B>& b ) const; 

       template<class B>
       MV trs ( const MVB<B>& b ) const; 
       template<class B>
       MV translate ( const MVB<B>& b ) const; 

       template<class ... Ts>
       MV trs ( Ts ... v ) const; 
       template<class ... Ts>
       MV translate ( Ts ... v ) const;  
       
       template<class B>
       MV trv ( const MVB<B>& b ) const; 
       template<class B>
       MV transverse ( const MVB<B>& b ) const; 

       template<class ... Ts>
       MV trv ( Ts ... v ) const; 
       template<class ... Ts>
       MV transverse ( Ts ... v ) const;  

       template<class B>
       MV mot ( const MVB<B>& b) const;
       template<class B>
       MV motor ( const MVB<B>& b) const;
       template<class B>
       MV twist ( const MVB<B>& b) const;

       template<class B>
       MV bst ( const MVB<B>& b ) const; 
       template<class B>
       MV boost ( const MVB<B>& b ) const; 

       template<class B>
       MV dil ( const MVB<B>& b, VSR_PRECISION t ) const; 
       template<class B>
       MV dilate ( const MVB<B>& b,  VSR_PRECISION t ) const; 

    
      /*-----------------------------------------------------------------------------
       *  Duality
       *-----------------------------------------------------------------------------*/
      auto dual() const -> Dual {
        return impl::gp( *this , typename Space::Pss(-1) );
      }       
  
      auto undual() const -> Dual { 
        return impl::gp( *this ,typename Space::Pss(1) );
      }

      auto duale() const -> DualE{
        return impl::gp( *this ,typename Space::Euc(-1) );
      }       
  
      auto unduale() const -> DualE{ 
        return impl::gp( *this ,typename Space::Euc(1) );
      }


      /*-----------------------------------------------------------------------------
       *  Weights and Units
       *-----------------------------------------------------------------------------*/
      VT wt() const{ return (*this <= *this)[0]; }
      VT rwt() const{ return (*this <= ~(*this))[0]; }
      VT norm() const { VT a = rwt(); if(a<0) return 0; return sqrt( a ); } 
      VT rnorm() const{ VT a = rwt(); if(a<0) return -sqrt( -a ); return sqrt( a );  }  
     
      MV unit() const { VT t = sqrt( fabs( (*this <= *this)[0] ) ); if (t == 0) return MV(); return *this / t; }
      MV runit() const { VT t = rnorm(); if (t == 0) return  MV(); return *this / t; }
      MV tunit() const { VT t = norm(); if (t == 0) return MV(); return *this / t; }  

     
     /*-----------------------------------------------------------------------------
      *  Sums
      *-----------------------------------------------------------------------------*/
      template<class B>
      auto operator + (const MVB<B>& b) -> decltype( impl::sum(*this, b) ) {
        return impl::sum(*this,b);
      }   
  

      MV operator + (const MV& a) const {
        MV tmp;
        for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] + a[i];
        return tmp;
      }  
      
      MV operator - (const MV& a) const {
        MV tmp;
        for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] - a[i];
        return tmp;
      }
       
      MV operator -() const{
        MV tmp = *this;
        for (int i = 0; i < A::Num; ++i){ tmp[i] = -tmp[i]; }
        return tmp;
      }  
      
      MV& operator -=(const MV& b){ 
        for (int i = 0; i < A::Num; ++i) (*this)[i] -= b[i];
        return *this;
      }   
      MV& operator +=(const MV& b){ 
        for (int i = 0; i < A::Num; ++i) (*this)[i] += b[i];
        return *this;
      }  
      
      MV operator / (VSR_PRECISION f) const{   
        MV tmp = *this;
        for (int i = 0; i < A::Num; ++i){ tmp[i] /= f; }
        return tmp;
      }
      
      MV& operator /= (VSR_PRECISION f){
        for (int i = 0; i < A::Num; ++i){ (*this)[i] /= f; }
        return *this;
      }
        
      MV operator * (VSR_PRECISION f) const {
        MV tmp = *this;
        for (int i = 0; i < A::Num; ++i){ tmp[i] *= f; }
        return tmp;
      }
      MV& operator *= (VSR_PRECISION f){
        for (int i = 0; i < A::Num; ++i){ (*this)[i] *= f; }
        return *this;
      }  
      
      auto operator + (VSR_PRECISION a) const -> decltype( impl::sumv( a,  *this) )  {
        return impl::sumv(a, *this); 
      }   
    
      static MV x,y,z,xy,xz,yz;
    };

   
/*-----------------------------------------------------------------------------
 *  CONVERSIONS (CASTING, COPYING)
 *-----------------------------------------------------------------------------*/
template<typename Algebra, typename B> template<class A> 
A MV<Algebra,B>::cast() const{
 return Cast< typename A::basis, B >::Type::template doCast<A>( *this );
}  

template<typename Algebra, typename B> template<class A> 
A MV<Algebra,B>::copy() const{
	A tmp;
	for (int i = 0; i < A::basis::Num; ++i) tmp[i] = (*this)[i];
	return tmp;
}
              

/*-----------------------------------------------------------------------------
 *  GETTERS AND SETTERS
 *-----------------------------------------------------------------------------*/
template<typename Algebra, typename B> template<bits::type IDX> 
typename Algebra::valuetype & MV<Algebra,B>::get(){
 return val[ find(IDX, B()) ];
}
template<typename Algebra, typename B>   template<bits::type IDX> 
typename Algebra::valuetype MV<Algebra,B>::get() const{
 return val[ find(IDX, B()) ];
}
template<typename Algebra, typename B> template<bits::type IDX> 
MV<Algebra,B>& MV<Algebra,B>::set( typename Algebra::valuetype v){
	get<IDX>() = v;
	return *this;
}  

template<typename Algebra, typename B> 
MV<Algebra,B> MV<Algebra,B>::x = MV<Algebra,B>().template set<1>(1);

template<typename Algebra, typename B> 
MV<Algebra,B> MV<Algebra,B>::y  = MV<Algebra,B>().template set<2>(1);

template<typename Algebra, typename B> 
MV<Algebra,B> MV<Algebra,B>::z  = MV<Algebra,B>().template set<4>(1);

template<typename Algebra, typename B> 
MV<Algebra,B> MV<Algebra,B>::xy = MV<Algebra,B>().template set<3>(1);

template<typename Algebra, typename B> 
MV<Algebra,B> MV<Algebra,B>::xz = MV<Algebra,B>().template set<5>(1);

template<typename Algebra, typename B> 
MV<Algebra,B> MV<Algebra,B>::yz = MV<Algebra,B>().template set<6>(1);
 



/*-----------------------------------------------------------------------------
 *  UNARY OPERATIONS
 *-----------------------------------------------------------------------------*/
template<typename Algebra, typename B> 
MV<Algebra,B> MV<Algebra,B>::conjugation() const{
	return Conjugate<B>::Type::template Make(*this);
}
template<typename Algebra, typename B> 
MV<Algebra,B> MV<Algebra,B>::involution() const{
	return Involute<B>::Type::template Make(*this);
} 



template<typename algebra> using GASca = MV< algebra, typename algebra::types::sca >;
template<typename algebra> using GAVec = MV< algebra, typename algebra::types::vec >;
template<typename algebra> using GABiv = MV< algebra, typename algebra::types::biv >;
template<typename algebra> using GATri = MV< algebra, typename algebra::types::tri >;
template<typename algebra> using GAPnt = MV< algebra, typename algebra::types::pnt >;
template<typename algebra> using GADls = MV< algebra, typename algebra::types::dls >;
template<typename algebra> using GAPar = MV< algebra, typename algebra::types::par >;
template<typename algebra> using GACir = MV< algebra, typename algebra::types::cir >;
template<typename algebra> using GASph = MV< algebra, typename algebra::types::sph >;
template<typename algebra> using GAFlp = MV< algebra, typename algebra::types::flp >;
template<typename algebra> using GADll = MV< algebra, typename algebra::types::dll >;
template<typename algebra> using GALin = MV< algebra, typename algebra::types::lin >;
template<typename algebra> using GADlp = MV< algebra, typename algebra::types::dlp >;
template<typename algebra> using GAPln = MV< algebra, typename algebra::types::pln >;
template<typename algebra> using GAMnk = MV< algebra, typename algebra::types::mnk >;
template<typename algebra> using GAInf = MV< algebra, typename algebra::types::inf >;
template<typename algebra> using GAOri = MV< algebra, typename algebra::types::ori >;
template<typename algebra> using GAPss = MV< algebra, typename algebra::types::pss >;
template<typename algebra> using GATnv = MV< algebra, typename algebra::types::tnv >;
template<typename algebra> using GADrv = MV< algebra, typename algebra::types::drv >;
template<typename algebra> using GATnb = MV< algebra, typename algebra::types::tnb >;
template<typename algebra> using GADrb = MV< algebra, typename algebra::types::drb >;
template<typename algebra> using GATnt = MV< algebra, typename algebra::types::tnt >;
template<typename algebra> using GADrt = MV< algebra, typename algebra::types::drt >;
template<typename algebra> using GARot = MV< algebra, typename algebra::types::rot >;
template<typename algebra> using GATrs = MV< algebra, typename algebra::types::trs >;
template<typename algebra> using GADil = MV< algebra, typename algebra::types::dil >;
template<typename algebra> using GAMot = MV< algebra, typename algebra::types::mot >;
template<typename algebra> using GABst = MV< algebra, typename algebra::types::bst >;
template<typename algebra> using GATrv = MV< algebra, typename algebra::types::trv >;
template<typename algebra> using GACon = MV< algebra, typename algebra::types::con >;
template<typename algebra> using GATsd = MV< algebra, typename algebra::types::tsd >;
template<typename algebra> using GAEucPss = MV< algebra, typename algebra::types::eucpss  >;

template<typename algebra>
struct GAE{
    template <bits::type ... NN> using e = MV<algebra, typename algebra::types::e::template e<NN...> >; 
};

template<bits::type N, typename T=VSR_PRECISION> using euclidean = ga< Metric<N>,T>;
template<bits::type N, typename T=VSR_PRECISION> using conformal = ga< Metric<N-1,1,true>,T>;

/*-----------------------------------------------------------------------------
 *  EUCLIDEAN TEMPLATE ALIAS UTILITY
 *-----------------------------------------------------------------------------*/

template<bits::type N, typename T=VSR_PRECISION> using NEVec = GAVec< euclidean<N,T> >; 
template<bits::type N, typename T=VSR_PRECISION> using NEBiv = GABiv< euclidean<N,T> >; 
template<bits::type N, typename T=VSR_PRECISION> using NETri = GATri< euclidean<N,T> >; 
template<bits::type N, typename T=VSR_PRECISION> using NERot = GARot< euclidean<N,T> >; 

template<bits::type dim, typename T=VSR_PRECISION> using euclidean_vector = GAVec< euclidean<dim,T> >; 
template<bits::type dim, typename T=VSR_PRECISION> using euclidean_bivector = GABiv< euclidean<dim,T> >; 
template<bits::type dim, typename T=VSR_PRECISION> using euclidean_trivector = GATri< euclidean<dim,T> >; 
template<bits::type dim, typename T=VSR_PRECISION> using euclidean_rotor = GARot< euclidean<dim,T> >; 

template<typename T=VSR_PRECISION>
struct NE{
    template <bits::type ... NN> using e = typename GAE< euclidean<bits::dimOf( bits::blade((1<<(NN-1))...) ), T >>::template e<NN...>; 
};
//

///*-----------------------------------------------------------------------------
// *  CONFORMAL TEMPLATE ALIAS UTILITY
// *-----------------------------------------------------------------------------*/

template<bits::type N, typename T=VSR_PRECISION> using NSca = GASca<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NVec = GAVec<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NBiv = GABiv<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NPnt = GAPnt<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NBst = GABst<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NPar = GAPar<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NCir = GACir<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NRot = GARot<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NTnv = GATnv<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NTrv = GATrv<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NTrs = GATrs<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NDrv = GADrv<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NDil = GADil<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NTsd = GATsd<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NOri = GAOri<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NInf = GAInf<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NDls = GADls<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NDll = GADll<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NLin = GALin<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NMnk = GAMnk<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NPss = GAPss<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NSph = GASph<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NTri = GATri<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NFlp = GAFlp<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NPln = GAPln<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NDlp = GADlp<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NDrb = GADrb<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NTnb = GATnb<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NTnt = GATnt<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NDrt = GADrt<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NMot = GAMot<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using NCon = GACon<conformal<N,T>>;


template<bits::type N, typename T=VSR_PRECISION> using conformal_scalar = GASca<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_vector = GAVec<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Biv = GABiv<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Pnt = GAPnt<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Bst = GABst<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Par = GAPar<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Cir = GACir<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Rot = GARot<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Tnv = GATnv<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Trv = GATrv<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Trs = GATrs<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Drv = GADrv<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Dil = GADil<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Tsd = GATsd<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Ori = GAOri<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Inf = GAInf<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Dls = GADls<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Dll = GADll<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Lin = GALin<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Mnk = GAMnk<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Pss = GAPss<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Sph = GASph<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Tri = GATri<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Flp = GAFlp<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Pln = GAPln<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Dlp = GADlp<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Drb = GADrb<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Tnb = GATnb<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Tnt = GATnt<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Drt = GADrt<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Mot = GAMot<conformal<N,T>>;
template<bits::type N, typename T=VSR_PRECISION> using conformal_Con = GACon<conformal<N,T>>;


} //vsr::   

#endif
