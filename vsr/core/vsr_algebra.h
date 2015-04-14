/*
 * =====================================================================================
 *
 *       Filename:  vsr_algebra.h
 *
 *    Description:  algebra implementation (euclidean, metric, and conformal)
 *
 *        Version:  1.0
 *        Created:  04/07/2015 14:37:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#ifndef  vsr_algebra_INC
#define  vsr_algebra_INC

#include "core/vsr_products.h"

namespace vsr {



  /*-----------------------------------------------------------------------------
   *  SUMMING WITHIN ANY ALGEBRA
   *-----------------------------------------------------------------------------*/

   //forward?
   template<class Algbera, class B> struct MV;
       
   template <typename Algebra>
   struct AlgebraImplBase{

      template <class B> using TMV = MV<Algebra, B>;
 

      /// Sum of Similar types
      template<class B> 
      static TMV<B> 
      sum( const TMV<B> & a, const TMV<B>& b) {
        TMV<B> c;
        for (int i = 0; i < B::Num; ++i) c[i] = a[i] + b[i];
        return c;
      } 
      /// Difference of Similar types
      template<class B> 
      static TMV<B> 
      diff( const TMV<B> & a, const TMV<B>& b) {
        TMV<B> c;
        for (int i = 0; i < B::Num; ++i) c[i] = a[i] - b[i];
        return c;
      } 
     
      /// Sum of Different types
      template<class B1, class B2>
      static TMV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type>
      sum( const TMV<B1> & a, const TMV<B2>& b) {
        typedef TMV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type> Ret; 
        return sum( a.template cast<Ret>() ,  b.template cast<Ret>() );
      } 
      /// Difference of Different types
      template<class B1, class B2>
      static TMV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type>
      diff( const TMV<B1> & a, const TMV<B2>& b) {
        typedef TMV<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type> Ret; 
        return diff( a.template cast<Ret>() ,  b.template cast<Ret>() );
      } 
     
      /// Sum some scalar value 
      template<class B>
      static TMV<typename ICat< typename NotType< Basis<0>, B >::Type, Basis<0> >::Type>
      sumv( VSR_PRECISION a, const  TMV<B>& b) {
        typedef TMV<typename ICat< typename NotType< Basis<0>, B >::Type, Basis<0> >::Type> Ret;
        return sum( Ret(a) , b.template cast<Ret>() );
      }
  
      
   };


    /*-----------------------------------------------------------------------------
    *  ALGEBRA SPECIFIC PRODUCTS
    *-----------------------------------------------------------------------------*/
   template<typename Algebra, bool Euclidean, bool Conformal>
   struct AlgebraImpl : AlgebraImplBase<Algebra> {};

   /*-----------------------------------------------------------------------------
    *  EUCLIDEAN (NO NEGATIVE METRIC)
    *-----------------------------------------------------------------------------*/
   template<typename Algebra>
   struct AlgebraImpl<Algebra,true, false> : AlgebraImplBase<Algebra> {


       template <class B> using TMV = MV<Algebra, B>;
       
       template<class A, class B> using GPBasis = typename EGProd<A,B >::Bases;
       template<class A, class B> using OPBasis = typename EOProd<A,B >::Bases;
       template<class A, class B> using IPBasis = typename EIProd<A,B >::Bases;
       template<class A, class B> using GP = TMV<GPBasis<typename A::Bases,typename B::Bases>>;
       template<class A, class B> using OP = TMV<OPBasis<typename A::Bases,typename B::Bases>>;
       template<class A, class B> using IP = TMV<IPBasis<typename A::Bases,typename B::Bases>>;  

       /*-----------------------------------------------------------------------------
        *  Euclidean Product Functions (unknown return type)
        *-----------------------------------------------------------------------------*/
        template<class A, class B>
        SCA gp(const A& a, const B& b) -> TMV<typename EGProd<typename A::Bases, typename B::Bases>::Bases> {
          typedef EGProd<typename A::Bases, typename B::Bases> X;
          typedef TMV<typename X::Bases> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA op(const A& a, const B& b) -> TMV<typename EOProd<typename A::Bases, typename B::Bases>::Bases> {
          typedef EOProd<typename A::Bases, typename B::Bases> X;
          typedef TMV<typename X::Bases> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA ip(const A& a, const B& b) -> TMV<typename EIProd<typename A::Bases, typename B::Bases>::Bases> {
          typedef EIProd<typename A::Bases, typename B::Bases> X;
          typedef TMV<typename X::Bases> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        /// Spin a by b, return a
        template<class A, class B>
        static constexpr A sp(const A& a, const B& b) {
          typedef EGProd<typename B::Bases, typename A::Bases> X;
          typedef TMV<typename X::Bases> Type;
          return REProd< Type, B, A >::Call( egp(b, a), Reverse<typename B::Bases>::Type::template Make(b) );
        }
    
        /// Reflect a by b, return a
        template<class A, class B>
        static constexpr A re(const A& a, const B& b) {
         typedef EGProd<typename B::Bases, typename A::Bases> X;
         typedef TMV<typename X::Bases> Type;
         return REProd< Type, B, A>::Call( egp(b, a.involution() ), Reverse<typename B::Bases>::Type::template Make(b) );
        }

        
        /*-----------------------------------------------------------------------------
         *  EUCLIDEAN SPATIAL ELEMENTS
         *-----------------------------------------------------------------------------*/
        struct Types {

            //use as e<1,2,3> will return e123 blade
            template<bits::Type ... N> using e = TMV<Basis< bits::blade((1<<(N-1))...)>>;  
            
            using Sca = TMV< Basis<0>>;  
            using Vec = TMV<typename Blade1<Algebra::Dim>::VEC>; 
            using Pss = TMV<Basis<bits::pss(Algebra::Dim)>>;
            using Biv = OP<Vec,Vec>;
            using Tri = OP<Vec,Biv>;
            using Rot = decltype( Biv() + 1 );
            using EucPss = Pss; //necessary to avoid errors when compiling euclidean duale() method

            template<typename A> using BasisOf = typename A::Bases;

      };
   };
    

   /*-----------------------------------------------------------------------------
    *  Metric Product Functions 
    *-----------------------------------------------------------------------------*/
   /*-----------------------------------------------------------------------------
    *  USE METRIC (e.g for spacetime algebra)
    *-----------------------------------------------------------------------------*/
   template<typename Algebra>
   struct AlgebraImpl<Algebra,false, false> : AlgebraImplBase<Algebra> {

       using Metric = typename Algebra::MetricList;

       template <class B> using TMV = MV<Algebra, B>;
       
       template<class A, class B> using GPBasis = typename MGProd<A,B,Metric>::Bases;
       /*-----------------------------------------------------------------------------
        *  Metric Product Functions (unknown return type)
        *-----------------------------------------------------------------------------*/
        template<class A, class B>
        SCA gp(const A& a, const B& b) -> TMV<typename MGProd<typename A::Bases, typename B::Bases, typename Algebra::MetricList>::Bases> {
          typedef MGProd<typename A::Bases, typename B::Bases, typename Algebra::MetricList> X;
          typedef TMV<typename X::Bases> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA op(const A& a, const B& b) -> TMV<typename MOProd<typename A::Bases, typename B::Bases, typename Algebra::MetricList>::Bases> {
          typedef MOProd<typename A::Bases, typename B::Bases, typename Algebra::MetricList> X;
          typedef  TMV<typename X::Bases> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA ip(const A& a, const B& b) -> TMV<typename MIProd<typename A::Bases, typename B::Bases, typename Algebra::MetricList>::Bases> {
          typedef MIProd<typename A::Bases, typename B::Bases, typename Algebra::MetricList> X;
          typedef  TMV<typename X::Bases> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
    
        /// Spin a by b, return a
        template<class A, class B>
        static constexpr A sp(const A& a, const B& b) {
          typedef MGProd<typename B::Bases, typename A::Bases, typename Algebra::MetricList> X;
          typedef TMV<typename X::Bases> Type;
          return RMGProd< Type, B, A, typename Algebra::MetricList >::Call( gp(b, a), Reverse<typename B::Bases>::Type::template Make(b) );
        }
    
        /// Reflect a by b, return a
        template<class A, class B>
        static constexpr A re(const A& a, const B& b) {
         typedef MGProd<typename B::Bases, typename A::Bases, typename Algebra::MetricList> X;
         typedef TMV<typename X::Bases> Type;
         return RMGProd< Type, B, A, typename Algebra::MetricList>::Call( gp(b, a.involution() ), Reverse<typename B::Bases>::Type::template Make(b) );
        } 
  };
 
   /*-----------------------------------------------------------------------------
    *  CONFORMAL
    *-----------------------------------------------------------------------------*/
   template<typename Algebra>
   struct AlgebraImpl<Algebra,false, true> : AlgebraImplBase<Algebra> {

       using Metric = typename Algebra::MetricList;

       //lift basis into algebraic multivector (monad)
       template <class B> using TMV = MV<Algebra, B>;

       
       template<class A, class B> using GPBasis = typename CGProd<A,B, Metric >::Bases;
       template<class A, class B> using OPBasis = typename COProd<A,B, Metric >::Bases;
       template<class A, class B> using IPBasis = typename CIProd<A,B, Metric >::Bases;
       template<class A, class B> using GP = TMV<GPBasis<typename A::Bases,typename B::Bases>>;
       template<class A, class B> using OP = TMV<OPBasis<typename A::Bases,typename B::Bases>>;
       template<class A, class B> using IP = TMV<IPBasis<typename A::Bases,typename B::Bases>>;    

       /*-----------------------------------------------------------------------------
        *  Conformal Product Functions (unknown return type)
        *-----------------------------------------------------------------------------*/
        template<class A, class B>
        SCA gp(const A& a, const B& b) -> TMV<typename CGProd<typename A::Bases, typename B::Bases, Metric>::Bases> {
          typedef CGProd<typename A::Bases, typename B::Bases, Metric> X;
          typedef TMV<typename X::Bases> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA op(const A& a, const B& b) -> TMV<typename COProd<typename A::Bases, typename B::Bases, Metric>::Bases> {
          typedef COProd<typename A::Bases, typename B::Bases, Metric> X;
          typedef  TMV<typename X::Bases> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA ip(const A& a, const B& b) -> TMV<typename CIProd<typename A::Bases, typename B::Bases, Metric>::Bases> {
          typedef CIProd<typename A::Bases, typename B::Bases, Metric> X;
          typedef  TMV<typename X::Bases> Type;
          return X::Arrow::template Make<Type>(a,b);
        }

    
        /*-----------------------------------------------------------------------------
         *  Known Return Type
         *-----------------------------------------------------------------------------*/
        /// Spin a by b, return a
        template<class A, class B>
        static constexpr A sp(const A& a, const B& b) {
          typedef CGProd<typename B::Bases, typename A::Bases, Metric> X;
          typedef TMV<typename X::Bases> Type;
          return RCGProd< Type, B, A, Metric >::Call( gp(b, a), Reverse<typename B::Bases>::Type::template Make(b) );
        }
    
        /// Reflect a by b, return a
        template<class A, class B>
        static constexpr A re(const A& a, const B& b) {
         typedef CGProd<typename B::Bases, typename A::Bases, Metric> X;
         typedef TMV<typename X::Bases> Type;
         return RCGProd< Type, B, A, Metric>::Call( gp(b, a.involution() ), Reverse<typename B::Bases>::Type::template Make(b) );
        } 

      /*-----------------------------------------------------------------------------
       *  CONFORMAL SPATIAL ELEMENTS META-UTILITY 
       *-----------------------------------------------------------------------------*/
      struct Types{ 
       
        //static const int Dim = DIM;      
        template<bits::Type ... N> using e = TMV<Basis< bits::blade((1<<(N-1))...)>>;  
        
        using Sca =    TMV< typename basis::sca >;       
        using Pnt =    TMV< typename Blade1<Algebra::Dim>::VEC >;
        using Pss =    TMV< typename basis::pss<Algebra::Dim> >;
        using EucPss = TMV< typename basis::pss<Algebra::Dim-2> >;
        using Vec =    TMV< typename Blade1<Algebra::Dim-2>::VEC >;
        using Ori =    TMV< typename basis::origin<Algebra::Dim> >; 
        using Inf =    TMV< typename basis::infinity<Algebra::Dim> >; 
        using Mnk =    TMV< typename basis::eplane<Algebra::Dim> >; 
      
        using Biv = OP<Vec,Vec>;
        using Tri = EucPss;
        
        using Tnv = OP<Vec,Ori>;
        using Drv = OP<Vec,Inf>;
        using Tnb = OP<Biv,Ori>;
        using Drb = OP<Biv,Inf>;
        using Tnt = OP<Tri,Ori>;
        using Drt = OP<Tri,Inf>;

        using Par = OP<Pnt,Pnt>;
        using Cir = OP<Par,Pnt>;
        using Sph = OP<Cir,Pnt>;

        using Flp = OP<Pnt,Inf>;
        using Dll = decltype( Biv() + Drv() );
        using Lin = OP<Pnt,Flp>;
        using Dlp = OP<Vec,Inf>;
        using Pln = OP<Cir,Inf>;

        using Rot = decltype ( Biv() + 1 );
        using Trs = decltype ( Drv() + 1 );
        using Trv = decltype ( Tnv() + 1 );
        using Dil = decltype ( Mnk() + 1 );
        using Tsd = decltype ( Flp() + 1 );
        using Mot = decltype ( Rot() * Trs() );
        using Bst = decltype ( Par() + 1 );
        using Con = decltype ( Par() * Par() );

        template<typename A> using BasisOf = typename A::Bases;
        
      };


   };
 


} //vsr::

#endif   /* ----- #ifndef vsr_algebra_INC  ----- */
