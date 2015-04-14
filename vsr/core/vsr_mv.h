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
 *  VALUE PRINT
 *-----------------------------------------------------------------------------*/
struct VPrint {

  template<typename B>
  static void Call( B& m ){
    for(int i=0; i < B::Num; ++i ) std::cout << m[i] << "\t"; std::cout << std::endl; 
  }

};

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
         
        using type = ga<TMetric, T>;
        using impl = AlgebraImpl< type, TMetric::is_euclidean, TMetric::is_conformal >;    ///<-- implementation details

        ///Next Higher Algebra over the same field
        using up = ga< typename TMetric::up, T>;  
        
        typedef TMetric Metric;                          ///<-- Metric, whether Euclidean, Projective, Conformal, etc
        typedef typename Metric::Type MetricList;        ///<-- Metric as List of Values
        typedef T ValueType;                             ///<-- Field over which Algebra is Defined
        static const int Dim = MetricList::Num;          ///<-- Dimension of Algebra

        using vector = typename Blade1<Dim>::VEC;        ///<-- 1-blade vector element

    };

    /*!-----------------------------------------------------------------------------
     *  THE GENERIC MULTIVECTOR CLASS (built from an algbera TALGEBRA on a sub basis A )
     *-----------------------------------------------------------------------------*/
    template<typename TAlgebra, typename A>
    struct MV{
      
      typedef TAlgebra Algebra;                ///<-- mother algebra
      typedef A Bases;                         ///<-- basis of this type
      static const int Num = Bases::Num;       ///<-- number of bases


      using Metric =  typename Algebra::MetricList;
      using VT =      typename Algebra::ValueType;
      using Product = typename Algebra::impl;
      using Space =   typename Product::Types;            ///<-- call ::Space:: to get another Type in same Algebra

      template<class B> using MVB = MV<TAlgebra,B>;       ///<-- another basis within same algebra

      using Dual = MVB< typename Product::template GPBasis< A, typename basis::pss<Algebra::Dim> > >;   ///<-- Define Dual Type
      using DualE = MVB< typename Product::template GPBasis< A, typename basis::epss<Algebra::Dim> > >; ///<-- Define Euclidean Dual Type
    
    
      /// Data 
      VT val[Bases::Num];
      /// Data Access
      typedef const VT array_type[Bases::Num]; 
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
      constexpr MV( const MV<Algebra,B>& b) { *this = b.template cast<MV<Algebra,A>>(); }


      /// Getters, Setters
      template<bits::Type IDX> VT get() const;
      template<bits::Type IDX> VT& get(); 
      template<bits::Type IDX> MV& set(VT v); 
    
      /// Reset
      MV& reset(VT v = VT()){
        std::fill( &(val[0]), &(val[0]) + Bases::Num, v); 
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
        Bases::print();
        VPrint::Call( *this );
      }

      
      /*-----------------------------------------------------------------------------
       *  Dispatched functions (called by AlgebraImpl see core/vsr_algebra.h)
       *-----------------------------------------------------------------------------*/
      /// Geometric Product
      template<class B>
      auto operator * ( const MVB<B>& b) const -> decltype(Product::gp(*this,b)) {
        return Product::gp(*this, b);
      }

      /// Outer Product
      template<class B>
      auto operator ^ ( const MVB<B>&  b) const-> decltype(Product::op(*this,b)) {
        return Product::op(*this, b);
      }

      /// Inner Product
      template<class B>
      auto operator <= ( const MVB<B>&  b) const-> decltype(Product::ip(*this,b)) {
        return Product::ip(*this, b);
      }
      /// Rotor (even) transformation
      template<typename B>
      MV spin( const MVB<B>& b ) const { return Product::sp(*this,b); }
      /// Versor (Odd) Transformation
      template<typename B>
      MV reflect( const MVB<B>& b ) const { return Product::re(*this,b); }

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
       MV<Algebra, typename Algebra::vector > null() const; 
        

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

    
      /*-----------------------------------------------------------------------------
       *  Duality
       *-----------------------------------------------------------------------------*/
      auto dual() const -> Dual {
        return Product::gp( *this , typename Space::Pss(-1) );
      }       
  
      auto undual() const -> Dual { 
        return Product::gp( *this ,typename Space::Pss(1) );
      }

      auto duale() const -> DualE{
        return Product::gp( *this ,typename Space::EucPss(-1) );
      }       
  
      auto unduale() const -> DualE{ 
        return Product::gp( *this ,typename Space::EucPss(1) );
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
      auto operator + (const MVB<B>& b) -> decltype( Product::sum(*this, b) ) {
        return Product::sum(*this,b);
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
      
      auto operator + (VSR_PRECISION a) const -> decltype( Product::sumv( a,  *this) )  {
        return Product::sumv(a, *this); 
      }   
    
      static MV x,y,z,xy,xz,yz;
    };

   


/*-----------------------------------------------------------------------------
 *  CONVERSIONS (CASTING, COPYING)
 *-----------------------------------------------------------------------------*/
template<typename Algebra, typename B> template<class A> 
A MV<Algebra,B>::cast() const{
 return Cast< typename A::Bases, B >::Type::template doCast<A>( *this );
}  

template<typename Algebra, typename B> template<class A> 
A MV<Algebra,B>::copy() const{
	A tmp;
	for (int i = 0; i < A::Bases::Num; ++i) tmp[i] = (*this)[i];
	return tmp;
}
              

/*-----------------------------------------------------------------------------
 *  GETTERS AND SETTERS
 *-----------------------------------------------------------------------------*/
template<typename Algebra, typename B> template<bits::Type IDX> 
typename Algebra::ValueType & MV<Algebra,B>::get(){
 return val[ find(IDX, B()) ];
}
template<typename Algebra, typename B>   template<bits::Type IDX> 
typename Algebra::ValueType MV<Algebra,B>::get() const{
 return val[ find(IDX, B()) ];
}
template<typename Algebra, typename B> template<bits::Type IDX> 
MV<Algebra,B>& MV<Algebra,B>::set( typename Algebra::ValueType v){
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


template<typename Algebra> using GASca = MV< Algebra, typename Algebra::impl::Types::Sca::Bases >;
template<typename Algebra> using GAVec = MV< Algebra, typename Algebra::impl::Types::Vec::Bases >;
template<typename Algebra> using GABiv = MV< Algebra, typename Algebra::impl::Types::Biv::Bases >;
template<typename Algebra> using GATri = MV< Algebra, typename Algebra::impl::Types::Tri::Bases >;
template<typename Algebra> using GAPnt = MV< Algebra, typename Algebra::impl::Types::Pnt::Bases >;
template<typename Algebra> using GADls = MV< Algebra, typename Algebra::impl::Types::Dls::Bases >;
template<typename Algebra> using GAPar = MV< Algebra, typename Algebra::impl::Types::Par::Bases >;
template<typename Algebra> using GACir = MV< Algebra, typename Algebra::impl::Types::Cir::Bases >;
template<typename Algebra> using GASph = MV< Algebra, typename Algebra::impl::Types::Sph::Bases >;
template<typename Algebra> using GAFlp = MV< Algebra, typename Algebra::impl::Types::Flp::Bases >;
template<typename Algebra> using GADll = MV< Algebra, typename Algebra::impl::Types::Dll::Bases >;
template<typename Algebra> using GALin = MV< Algebra, typename Algebra::impl::Types::Lin::Bases >;
template<typename Algebra> using GADlp = MV< Algebra, typename Algebra::impl::Types::Dlp::Bases >;
template<typename Algebra> using GAPln = MV< Algebra, typename Algebra::impl::Types::Pln::Bases >;
template<typename Algebra> using GAMnk = MV< Algebra, typename Algebra::impl::Types::Mnk::Bases >;
template<typename Algebra> using GAInf = MV< Algebra, typename Algebra::impl::Types::Inf::Bases >;
template<typename Algebra> using GAOri = MV< Algebra, typename Algebra::impl::Types::Ori::Bases >;
template<typename Algebra> using GAPss = MV< Algebra, typename Algebra::impl::Types::Pss::Bases >;
template<typename Algebra> using GAEucPss = MV< Algebra, typename Algebra::impl::Types::EucPss::Bases >;
template<typename Algebra> using GATnv = MV< Algebra, typename Algebra::impl::Types::Tnv::Bases >;
template<typename Algebra> using GADrv = MV< Algebra, typename Algebra::impl::Types::Drv::Bases >;
template<typename Algebra> using GATnb = MV< Algebra, typename Algebra::impl::Types::Tnb::Bases >;
template<typename Algebra> using GADrb = MV< Algebra, typename Algebra::impl::Types::Drb::Bases >;
template<typename Algebra> using GATnt = MV< Algebra, typename Algebra::impl::Types::Tnt::Bases >;
template<typename Algebra> using GADrt = MV< Algebra, typename Algebra::impl::Types::Drt::Bases >;
template<typename Algebra> using GARot = MV< Algebra, typename Algebra::impl::Types::Rot::Bases >;
template<typename Algebra> using GATrs = MV< Algebra, typename Algebra::impl::Types::Trs::Bases >;
template<typename Algebra> using GADil = MV< Algebra, typename Algebra::impl::Types::Dil::Bases >;
template<typename Algebra> using GAMot = MV< Algebra, typename Algebra::impl::Types::Mot::Bases >;
template<typename Algebra> using GABst = MV< Algebra, typename Algebra::impl::Types::Bst::Bases >;
template<typename Algebra> using GATrv = MV< Algebra, typename Algebra::impl::Types::Trv::Bases >;
template<typename Algebra> using GACon = MV< Algebra, typename Algebra::impl::Types::Con::Bases >;
template<typename Algebra> using GATsd = MV< Algebra, typename Algebra::impl::Types::Tsd::Bases >;

template<typename Algebra>
struct GAE{
    template <bits::Type ... NN> using e = MV<Algebra, typename Algebra::impl::Types::template e<NN...> >; 
};

template<bits::Type N, typename T=VSR_PRECISION> using EGA = ga< RMetric<N>,T>;
template<bits::Type N, typename T=VSR_PRECISION> using CGA = ga<RMetric<N-1,1,true>,T>;

/*-----------------------------------------------------------------------------
 *  EUCLIDEAN TEMPLATE ALIAS UTILITY
 *-----------------------------------------------------------------------------*/

template<bits::Type N, typename T=VSR_PRECISION> using NEVec = GAVec< EGA<N,T> >; //typename EGA<N,T>::Types::Vec;
template<bits::Type N, typename T=VSR_PRECISION> using NEBiv = GABiv< EGA<N,T> >; //typename EGA<N,T>::Types::Biv;
template<bits::Type N, typename T=VSR_PRECISION> using NETri = GATri< EGA<N,T> >; //typename EGA<N,T>::Types::Tri;
template<bits::Type N, typename T=VSR_PRECISION> using NERot = GARot< EGA<N,T> >; //typename EGA<N,T>::Types::Rot;

template<typename T=VSR_PRECISION>
struct NE{
    template <bits::Type ... NN> using e = typename GAE< EGA<bits::dimOf( bits::blade((1<<(NN-1))...) ), T >>::template e<NN...>; 
};
//

///*-----------------------------------------------------------------------------
// *  CONFORMAL TEMPLATE ALIAS UTILITY
// *-----------------------------------------------------------------------------*/

template<bits::Type N, typename T=VSR_PRECISION> using NSca = GASca<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Sca;
template<bits::Type N, typename T=VSR_PRECISION> using NVec = GAVec<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Vec;
template<bits::Type N, typename T=VSR_PRECISION> using NBiv = GABiv<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Biv;
template<bits::Type N, typename T=VSR_PRECISION> using NPnt = GAPnt<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Pnt;
template<bits::Type N, typename T=VSR_PRECISION> using NBst = GABst<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Bst;
template<bits::Type N, typename T=VSR_PRECISION> using NPar = GAPar<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Par;
template<bits::Type N, typename T=VSR_PRECISION> using NCir = GACir<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Cir;
template<bits::Type N, typename T=VSR_PRECISION> using NRot = GARot<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Rot;
template<bits::Type N, typename T=VSR_PRECISION> using NTnv = GATnv<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Tnv;
template<bits::Type N, typename T=VSR_PRECISION> using NTrv = GATrv<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Trv;
template<bits::Type N, typename T=VSR_PRECISION> using NTrs = GATrs<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Trs;
template<bits::Type N, typename T=VSR_PRECISION> using NDrv = GADrv<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Drv;
template<bits::Type N, typename T=VSR_PRECISION> using NDil = GADil<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Dil;
template<bits::Type N, typename T=VSR_PRECISION> using NTsd = GATsd<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Tsd;
template<bits::Type N, typename T=VSR_PRECISION> using NOri = GAOri<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Ori;
template<bits::Type N, typename T=VSR_PRECISION> using NInf = GAInf<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Inf;
template<bits::Type N, typename T=VSR_PRECISION> using NDls = GADls<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Dls;
template<bits::Type N, typename T=VSR_PRECISION> using NDll = GADll<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Dll;
template<bits::Type N, typename T=VSR_PRECISION> using NLin = GALin<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Lin;
template<bits::Type N, typename T=VSR_PRECISION> using NMnk = GAMnk<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Mnk;
template<bits::Type N, typename T=VSR_PRECISION> using NPss = GAPss<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Pss;
template<bits::Type N, typename T=VSR_PRECISION> using NSph = GASph<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Sph;
template<bits::Type N, typename T=VSR_PRECISION> using NTri = GATri<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Tri;
template<bits::Type N, typename T=VSR_PRECISION> using NFlp = GAFlp<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Flp;
template<bits::Type N, typename T=VSR_PRECISION> using NPln = GAPln<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Pln;
template<bits::Type N, typename T=VSR_PRECISION> using NDlp = GADlp<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Dlp;
template<bits::Type N, typename T=VSR_PRECISION> using NDrb = GADrb<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Drb;
template<bits::Type N, typename T=VSR_PRECISION> using NTnb = GATnb<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Tnb;
template<bits::Type N, typename T=VSR_PRECISION> using NTnt = GATnt<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Tnt;
template<bits::Type N, typename T=VSR_PRECISION> using NDrt = GADrt<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Drt;
template<bits::Type N, typename T=VSR_PRECISION> using NMot = GAMot<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Mot;
template<bits::Type N, typename T=VSR_PRECISION> using NCon = GACon<CGA<N,T>>;// typename CGA<N,T>::impl::Types::Con;



} //vsr::   

#endif
