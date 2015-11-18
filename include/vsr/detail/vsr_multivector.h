/*! @file
    generic multivector class, templated on a geometric algebra and a field
      
    using Vec = vsr::algebra< metric<3>, float>::types::Vec;
    Vec v(1,0,0);
     
 * =====================================================================================
 *
 *       Filename:  vsr_multivector.h
 *
 *    Description:  generic multivector class, templated on a geometric algebra and a field
 *
 *        Version:  1.0
 *        Created:  04/07/2015 15:54:28
 *       Revision:  none
 *       Compiler:  gcc4.7 or above or clang 3.2 or above
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#ifndef MV_H_INCLUDED
#define MV_H_INCLUDED  


#include "vsr_algebra.h"  //<-- algebra implementation details (EGA, MGA, CGA)

#include <math.h>   
#include <iostream>  



/*!-----------------------------------------------------------------------------
 *  the **versor** library namespace
 
 *-----------------------------------------------------------------------------*/
namespace vsr{ 
 
 
     /*!
     *  Generic Geometric Number Types (templated on an algebra and a basis )

          Multivector is the main value-storing class.  All Geometric Algebra Types
          are template instantiations of this class.  
          
          The types of **values** contained here (float, double, etc) are 
          deterimined by the algebra.  The way in which the data is combined with other 
          methods is determined by their respective basis, in coordination with the algebra.

     */
    template<typename algebra_type, typename basis_type>
    struct Multivector{
      
      using algebra = algebra_type;                       ///< \ref algebra (a metric and field)
      using basis = basis_type;                           ///< basis is an algebraic data type created with compile-time list processing  
      
      static const int Num = basis::Num;                  ///< number of bases

      using value_t = typename algebra::value_t;          ///< the field over which the algebra is defined (e.g. float or double)
      using space =   typename algebra::types;            ///< @ingroup typesystem call <MultivectorType>::space::<MultivectorName> to declare another named Type within the same algebra 

      template<class B> using MultivectorB = Multivector<algebra,B>;       ///< another Type within same algebra

      /// the Dual Type (product of this and algebra::types::pseudoscalar)
      using Dual = typename algebra::template make_gp< typename blade<algebra::dim,algebra::dim>::type, basis>; 
      /// the Euclidan subspace Dual Type (product of this and algebra::types::euclidean_pseudoscalar)
      using DualE = typename algebra::template make_gp< Basis<bits::pss(algebra::dim-2)>, basis>; 
    
      value_t val[Num];                                   ///< %Data Array
      typedef const value_t array_type[Num];              ///< %Data Array Type

      array_type& begin() const { return val; }           ///< Pointer to first data

      /// %Get value at idx
      constexpr value_t operator[] (int idx) const{
        return val[idx]; 
      }
      /// %Set value at idx
      value_t& operator[] (int idx){
        return val[idx]; 
      }
    
      /// Construct from list of args (cannot be longer than Num)
      template<typename...Args>     
      constexpr explicit Multivector(Args...v) :
      val{ static_cast<value_t>(v)...} {} 

      /// Construct from different Basis within same Algebra
      template<typename B>
      constexpr Multivector( const Multivector<algebra,B>& b) : Multivector(b.template cast<Multivector<algebra,basis>>()) {}

      /// Construct from different algebra signature and different basis
      template<class alg, typename B>
      constexpr Multivector( const Multivector<alg, B>& b ) : Multivector( b.template cast<Multivector<algebra,basis>>() ) {}

      /// Immutable get value of blade type IDX (\todo make blades user-defined literals?)
      template<bits::type IDX> value_t get() const;
      /// Mutable get value of blade type IDX 
      template<bits::type IDX> value_t& get(); 
      /// %Set value of blade type IDX
      template<bits::type IDX> Multivector& set(value_t v); 
    
      /// Reset
      Multivector& reset(value_t v = value_t()){
        std::fill( &(val[0]), &(val[0]) + basis::Num, v); 
        return *this;
      }
    
      /// Conjugation Unary Operation
      Multivector conjugation() const;
      /// Conjugation Unary Operation Shorthand
      Multivector conj() const { return conjugation(); }
      /// Involution Unary Operation 
      Multivector involution() const; 
      /// Involution Unary Operation Shorthand
      Multivector inv() const { return involution(); }
    
      /// Casting to type B
      template<class B> B cast() const; 
      /// Copying to type B
      template<class B> B copy() const; 
    
      /// Comparison \todo use dot product, see \ref Group
      bool operator == (const Multivector& mv) const{
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
      /// Geometric Product \\(a*b\\)
      template<class B>
      auto operator * ( const MultivectorB<B>& b) const -> decltype( algebra::gp(*this,b)) {
        return algebra::gp(*this, b);
      }

      /// Geometric Product in place Transformation
      template<class B>
      Multivector& operator *= ( const MultivectorB<B>& b) {
        *this = *this * b;
        return *this;
      }

      /// Outer Product \\(a \\wedge b\\)
      template<class B>
      auto operator ^ ( const MultivectorB<B>&  b) const-> decltype( algebra::op(*this,b)) {
        return algebra::op(*this, b);
      }

      /// Inner Product \\(a \\cdot b \\)
      /// Uses Hestenes Inner Contraction (a must be of lower grade than b)
      template<class B>
      auto operator <= ( const MultivectorB<B>&  b) const-> decltype( algebra::ip(*this,b)) {
        return algebra::ip(*this, b);
      }

      /// Commutator Product  \\(a \\times b\\)
      template<class B>
      auto operator % (const MultivectorB<B>& b ) const -> decltype( algebra::gp(*this,b) ){
        return ( (*this * b) - (b * (*this) ) ) * .5;
      }

      /// Rotor (even) transformation \\(RA\\tilde{R}\\)
      template<typename B>
      Multivector spin( const MultivectorB<B>& b ) const { return algebra::spin(*this,b); }
      /// Versor (Odd) Transformation \\(R\\hat{A}\\tilde{R}\\)
      template<typename B>
      Multivector reflect( const MultivectorB<B>& b ) const { return algebra::reflect(*this,b); }

      template<typename B> Multivector sp( const MultivectorB<B>& b) const { return spin(b); }
      template<typename B> Multivector re( const MultivectorB<B>& b) const { return reflect(b); }

      /// Reversion \\(\\tilde{A}\\) 
      Multivector operator ~() const {
        return Reverse< basis >::Type::template Make(*this) ;
      }
      
      /// Inversion \\(\\tilde{A}/A\\tilde{A}\\) 
      Multivector operator !() const {    
        Multivector tmp = ~(*this); 
        value_t v = ((*this) * tmp)[0];    
        return (v==0) ? tmp : tmp / v;
      }
     
      // Division \\(A/B\\)
      template<class B>
      auto operator / (const MultivectorB<B>& b) const RETURNS(
        (  *this * !b )
      )
  
      /*-----------------------------------------------------------------------------
       *  Transformations (note mostly here for conformal metric)
       *  for instance null() only works in conformal metric (returns identity in others)
       *  these are all defined in vsr_generic_op.h or in vsr_cga3D_op.h
       *-----------------------------------------------------------------------------*/
       
       /// Conformal Mapping \\(\\boldsymbol(x)\\to n_o + \\boldsymbol{x} + \\boldsymbol{x}^2n_\\infty \\)
       Multivector<algebra, typename algebra::vector_basis > null() const; 
        
       ///@{ 
       /// rotate in b plane
       template<class B>
       Multivector rot ( const MultivectorB<B>& b ) const; 
       template<class B>
       Multivector rotate ( const MultivectorB<B>& b ) const; 
       ///@}

      template<class B>
      Multivector trs ( const MultivectorB<B>& b ) const; 
      template<class B>
      Multivector translate ( const MultivectorB<B>& b ) const; 


       template<class ... Ts>
       Multivector trs ( Ts ... v ) const; 
       template<class ... Ts>
       Multivector translate ( Ts ... v ) const;  
       
       template<class B>
       Multivector trv ( const MultivectorB<B>& b ) const; 
       template<class B>
       Multivector transverse ( const MultivectorB<B>& b ) const; 

       template<class ... Ts>
       Multivector trv ( Ts ... v ) const; 
       template<class ... Ts>
       Multivector transverse ( Ts ... v ) const;  

       template<class B>
       Multivector mot ( const MultivectorB<B>& b) const;
       template<class B>
       Multivector motor ( const MultivectorB<B>& b) const;
       template<class B>
       Multivector twist ( const MultivectorB<B>& b) const;

       template<class B>
       Multivector bst ( const MultivectorB<B>& b ) const; 
       template<class B>
       Multivector boost ( const MultivectorB<B>& b ) const; 

       template<class B>
       Multivector dil ( const MultivectorB<B>& b, VSR_PRECISION t ) const; 
       template<class B>
       Multivector dilate ( const MultivectorB<B>& b,  VSR_PRECISION t ) const; 

    
      /*-----------------------------------------------------------------------------
       *  Duality
       *-----------------------------------------------------------------------------*/
      auto dual() const -> Dual {
        return algebra::gp( *this , typename space::Pss(-1) );
      }       
  
      auto undual() const -> Dual { 
        return algebra::gp( *this ,typename space::Pss(1) );
      }

      auto duale() const -> DualE{
        return algebra::gp( *this ,typename space::Euc(-1) );
      }       
  
      auto unduale() const -> DualE{ 
        return algebra::gp( *this ,typename space::Euc(1) );
      }


      /*-----------------------------------------------------------------------------
       *  Weights and Units
       *-----------------------------------------------------------------------------*/
      value_t wt() const{ return (*this <= *this)[0]; }
      value_t rwt() const{ return (*this <= ~(*this))[0]; }
      value_t norm() const { value_t a = rwt(); if(a<0) return 0; return sqrt( a ); } 
      value_t rnorm() const{ value_t a = rwt(); if(a<0) return -sqrt( -a ); return sqrt( a );  }  
     
      Multivector unit() const { value_t t = sqrt( fabs( (*this <= *this)[0] ) ); if (t == 0) return Multivector(); return *this / t; }
      Multivector runit() const { value_t t = rnorm(); if (t == 0) return  Multivector(); return *this / t; }
      Multivector tunit() const { value_t t = norm(); if (t == 0) return Multivector(); return *this / t; }  

     
     /*-----------------------------------------------------------------------------
      *  Sums
      *-----------------------------------------------------------------------------*/
      template<class B>
      auto operator + (const MultivectorB<B>& b) -> decltype( algebra::sum(*this, b) ) {
        return algebra::sum(*this,b);
      }   
  

      Multivector operator + (const Multivector& a) const {
        Multivector tmp;
        for (int i = 0; i < Num; ++i) tmp[i] = (*this)[i] + a[i];
        return tmp;
      }  
      
      Multivector operator - (const Multivector& a) const {
        Multivector tmp;
        for (int i = 0; i < Num; ++i) tmp[i] = (*this)[i] - a[i];
        return tmp;
      }
       
      Multivector operator -() const{
        Multivector tmp = *this;
        for (int i = 0; i < Num; ++i){ tmp[i] = -tmp[i]; }
        return tmp;
      }  
      
      Multivector& operator -=(const Multivector& b){ 
        for (int i = 0; i < Num; ++i) (*this)[i] -= b[i];
        return *this;
      }   
      Multivector& operator +=(const Multivector& b){ 
        for (int i = 0; i < Num; ++i) (*this)[i] += b[i];
        return *this;
      }  
      
      Multivector operator / (VSR_PRECISION f) const{   
        Multivector tmp = *this;
        for (int i = 0; i < Num; ++i){ tmp[i] /= f; }
        return tmp;
      }
      
      Multivector& operator /= (VSR_PRECISION f){
        for (int i = 0; i < Num; ++i){ (*this)[i] /= f; }
        return *this;
      }
        
      Multivector operator * (VSR_PRECISION f) const {
        Multivector tmp = *this;
        for (int i = 0; i < Num; ++i){ tmp[i] *= f; }
        return tmp;
      }
      Multivector& operator *= (VSR_PRECISION f){
        for (int i = 0; i < Num; ++i){ (*this)[i] *= f; }
        return *this;
      }  
     
      //causing problems if not templated
     // template<class T> 
     // auto operator + (T a) const -> decltype( algebra::sumv( a,  *this) )  {
     //   return algebra::sumv(a, *this); 
     // }   
         MultivectorB<typename ICat< typename NotType< Basis<0>, basis_type >::Type, Basis<0> >::Type>
         operator + ( VSR_PRECISION a) const {
           typedef MultivectorB<typename ICat< typename NotType< Basis<0>, basis_type >::Type, Basis<0> >::Type> Ret;
           return algebra::sum( Ret(a) , this->template cast<Ret>() );
         }
      
    
      static Multivector x,y,z,xy,xz,yz;

      friend ostream& operator << (ostream& os, const Multivector& m){
        for (int i=0; i < Num; ++i){
          os << m[i] << "\t";//std::endl;
        }
        return os;
      }
    };


   
/*-----------------------------------------------------------------------------
 *  CONVERSIONS (CASTING, COPYING)
 *-----------------------------------------------------------------------------*/
template<typename Algebra, typename B> template<class A> 
A Multivector<Algebra,B>::cast() const{
 return Cast< typename A::basis, B >::Type::template doCast<A>( *this );
}  

template<typename Algebra, typename B> template<class A> 
A Multivector<Algebra,B>::copy() const{
	A tmp;
	for (int i = 0; i < A::basis::Num; ++i) tmp[i] = (*this)[i];
	return tmp;
}
              

/*-----------------------------------------------------------------------------
 *  GETTERS AND SETTERS
 *-----------------------------------------------------------------------------*/
template<typename Algebra, typename B> template<bits::type IDX> 
typename Algebra::value_t & Multivector<Algebra,B>::get(){
 return val[ find(IDX, B()) ];
}
template<typename Algebra, typename B>   template<bits::type IDX> 
typename Algebra::value_t Multivector<Algebra,B>::get() const{
 return val[ find(IDX, B()) ];
}
template<typename Algebra, typename B> template<bits::type IDX> 
Multivector<Algebra,B>& Multivector<Algebra,B>::set( typename Algebra::value_t v){
	get<IDX>() = v;
	return *this;
}  

template<typename Algebra, typename B> 
Multivector<Algebra,B> Multivector<Algebra,B>::x = Multivector<Algebra,B>().template set<1>(1);

template<typename Algebra, typename B> 
Multivector<Algebra,B> Multivector<Algebra,B>::y  = Multivector<Algebra,B>().template set<2>(1);

template<typename Algebra, typename B> 
Multivector<Algebra,B> Multivector<Algebra,B>::z  = Multivector<Algebra,B>().template set<4>(1);

template<typename Algebra, typename B> 
Multivector<Algebra,B> Multivector<Algebra,B>::xy = Multivector<Algebra,B>().template set<3>(1);

template<typename Algebra, typename B> 
Multivector<Algebra,B> Multivector<Algebra,B>::xz = Multivector<Algebra,B>().template set<5>(1);

template<typename Algebra, typename B> 
Multivector<Algebra,B> Multivector<Algebra,B>::yz = Multivector<Algebra,B>().template set<6>(1);
 



/*-----------------------------------------------------------------------------
 *  UNARY OPERATIONS
 *-----------------------------------------------------------------------------*/
template<typename Algebra, typename B> 
Multivector<Algebra,B> Multivector<Algebra,B>::conjugation() const{
	return Conjugate<B>::Type::template Make(*this);
}
template<typename Algebra, typename B> 
Multivector<Algebra,B> Multivector<Algebra,B>::involution() const{
	return Involute<B>::Type::template Make(*this);
} 



template<typename algebra> using GASca = Multivector< algebra, typename algebra::types::sca >;
template<typename algebra> using GAVec = Multivector< algebra, typename algebra::types::vec >;
template<typename algebra> using GABiv = Multivector< algebra, typename algebra::types::biv >;
template<typename algebra> using GATri = Multivector< algebra, typename algebra::types::tri >;
template<typename algebra> using GAPnt = Multivector< algebra, typename algebra::types::pnt >;
template<typename algebra> using GADls = Multivector< algebra, typename algebra::types::dls >;
template<typename algebra> using GAPar = Multivector< algebra, typename algebra::types::par >;
template<typename algebra> using GACir = Multivector< algebra, typename algebra::types::cir >;
template<typename algebra> using GASph = Multivector< algebra, typename algebra::types::sph >;
template<typename algebra> using GAFlp = Multivector< algebra, typename algebra::types::flp >;
template<typename algebra> using GADll = Multivector< algebra, typename algebra::types::dll >;
template<typename algebra> using GALin = Multivector< algebra, typename algebra::types::lin >;
template<typename algebra> using GADlp = Multivector< algebra, typename algebra::types::dlp >;
template<typename algebra> using GAPln = Multivector< algebra, typename algebra::types::pln >;
template<typename algebra> using GAMnk = Multivector< algebra, typename algebra::types::mnk >;
template<typename algebra> using GAInf = Multivector< algebra, typename algebra::types::inf >;
template<typename algebra> using GAOri = Multivector< algebra, typename algebra::types::ori >;
template<typename algebra> using GAPss = Multivector< algebra, typename algebra::types::pss >;
template<typename algebra> using GATnv = Multivector< algebra, typename algebra::types::tnv >;
template<typename algebra> using GADrv = Multivector< algebra, typename algebra::types::drv >;
template<typename algebra> using GATnb = Multivector< algebra, typename algebra::types::tnb >;
template<typename algebra> using GADrb = Multivector< algebra, typename algebra::types::drb >;
template<typename algebra> using GATnt = Multivector< algebra, typename algebra::types::tnt >;
template<typename algebra> using GADrt = Multivector< algebra, typename algebra::types::drt >;
template<typename algebra> using GARot = Multivector< algebra, typename algebra::types::rot >;
template<typename algebra> using GATrs = Multivector< algebra, typename algebra::types::trs >;
template<typename algebra> using GADil = Multivector< algebra, typename algebra::types::dil >;
template<typename algebra> using GAMot = Multivector< algebra, typename algebra::types::mot >;
template<typename algebra> using GABst = Multivector< algebra, typename algebra::types::bst >;
template<typename algebra> using GATrv = Multivector< algebra, typename algebra::types::trv >;
template<typename algebra> using GACon = Multivector< algebra, typename algebra::types::con >;
template<typename algebra> using GATsd = Multivector< algebra, typename algebra::types::tsd >;
template<typename algebra> using GAEucPss = Multivector< algebra, typename algebra::types::eucpss  >;

template<typename algebra>
struct GAE{
    template <bits::type ... NN> using e = typename algebra::types::template e<NN...>;// Multivector<algebra, typename algebra::types::e_basis::template e<NN...> >; 
};

template<bits::type N, typename T=VSR_PRECISION> using euclidean = algebra< metric<N>,T>;
template<bits::type N, typename T=VSR_PRECISION> using conformal = algebra< metric<N-1,1,true>,T>;

/*-----------------------------------------------------------------------------
 *  EUCLIDEAN TEMPLATE ALIAS UTILITY
 *-----------------------------------------------------------------------------*/

template<bits::type N, typename T=VSR_PRECISION> using NESca = GASca< euclidean<N,T> >; 
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




