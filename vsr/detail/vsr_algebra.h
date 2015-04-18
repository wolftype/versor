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

#include "detail/vsr_products.h"

namespace vsr {

  /*-----------------------------------------------------------------------------
   *  generates basis blade in dimension dim of grade 
   *-----------------------------------------------------------------------------*/
    template<bits::type dim, bits::type grade>
    struct blade {
        using vec = typename vsr::Blade1<dim>::VEC; 
        using sub = typename blade<dim, grade-1>::type;
        using type = typename EOProd<vec,sub>::basis;
  
    };
  
    template<bits::type dim>
    struct blade<dim, 1>{
        using type = typename vsr::Blade1<dim>::VEC;
    };


  /*-----------------------------------------------------------------------------
   *  SUMMING WITHIN ANY ALGEBRA
   *-----------------------------------------------------------------------------*/

   //forward
   template<class Algebra, class B> struct MV;
       
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

       static const bits::type dim = Algebra::dim;

       template <class A, class B> using sum_t = typename ICat< typename NotType< A, B >::Type, A >::Type;
       template <class A, class B> using  gp_t = typename EGProd<A,B >::basis;
       template <class A, class B> using  op_t = typename EOProd<A,B >::basis;
       template <class A, class B> using  ip_t = typename EIProd<A,B >::basis;

       template <class B> using TMV = MV<Algebra, B>;
      
       template <class A, class B> using GPBasis = typename EGProd<A,B >::basis;
       template <class A, class B> using OPBasis = typename EOProd<A,B >::basis;
       template <class A, class B> using IPBasis = typename EIProd<A,B >::basis;
       template <class A, class B> using GP = TMV<GPBasis<typename A::basis,typename B::basis>>;
       template <class A, class B> using OP = TMV<OPBasis<typename A::basis,typename B::basis>>;
       template <class A, class B> using IP = TMV<IPBasis<typename A::basis,typename B::basis>>;  

       /*-----------------------------------------------------------------------------
        *  Euclidean Product Functions (unknown return type)
        *-----------------------------------------------------------------------------*/
        template<class A, class B>
        SCA gp(const A& a, const B& b) -> TMV<typename EGProd<typename A::basis, typename B::basis>::basis> {
          typedef EGProd<typename A::basis, typename B::basis> X;
          typedef TMV<typename X::basis> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA op(const A& a, const B& b) -> TMV<typename EOProd<typename A::basis, typename B::basis>::basis> {
          typedef EOProd<typename A::basis, typename B::basis> X;
          typedef TMV<typename X::basis> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA ip(const A& a, const B& b) -> TMV<typename EIProd<typename A::basis, typename B::basis>::basis> {
          typedef EIProd<typename A::basis, typename B::basis> X;
          typedef TMV<typename X::basis> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        /// Spin a by b, return a
        template<class A, class B>
        static constexpr A sp(const A& a, const B& b) {
          typedef EGProd<typename B::basis, typename A::basis> X;
          typedef TMV<typename X::basis> Type;
          return REProd< Type, B, A >::Call( egp(b, a), Reverse<typename B::basis>::Type::template Make(b) );
        }
    
        /// Reflect a by b, return a
        template<class A, class B>
        static constexpr A re(const A& a, const B& b) {
         typedef EGProd<typename B::basis, typename A::basis> X;
         typedef TMV<typename X::basis> Type;
         return REProd< Type, B, A>::Call( egp(b, a.involution() ), Reverse<typename B::basis>::Type::template Make(b) );
        }

        
   };

    

   /*-----------------------------------------------------------------------------
    *  Metric Product Functions 
    *-----------------------------------------------------------------------------*/
   /*-----------------------------------------------------------------------------
    *  USE METRIC (e.g for spacetime algebra)
    *-----------------------------------------------------------------------------*/
   template<typename Algebra>
   struct AlgebraImpl<Algebra,false, false> : AlgebraImplBase<Algebra> {

       using metric = typename Algebra::metric::type;

       template <class B> using TMV = MV<Algebra, B>;
       
       template<class A, class B> using GPBasis = typename MGProd<A,B,metric>::basis;
       /*-----------------------------------------------------------------------------
        *  Metric Product Functions (unknown return type)
        *-----------------------------------------------------------------------------*/
        template<class A, class B>
        SCA gp(const A& a, const B& b) -> TMV<typename MGProd<typename A::basis, typename B::basis, metric>::basis> {
          typedef MGProd<typename A::basis, typename B::basis, metric> X;
          typedef TMV<typename X::basis> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA op(const A& a, const B& b) -> TMV<typename MOProd<typename A::basis, typename B::basis, metric>::basis> {
          typedef MOProd<typename A::basis, typename B::basis, metric> X;
          typedef  TMV<typename X::basis> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA ip(const A& a, const B& b) -> TMV<typename MIProd<typename A::basis, typename B::basis, metric>::basis> {
          typedef MIProd<typename A::basis, typename B::basis, metric> X;
          typedef  TMV<typename X::basis> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
    
        /// Spin a by b, return a
        template<class A, class B>
        static constexpr A sp(const A& a, const B& b) {
          typedef MGProd<typename B::basis, typename A::basis, metric> X;
          typedef TMV<typename X::basis> Type;
          return RMGProd< Type, B, A, metric >::Call( gp(b, a), Reverse<typename B::basis>::Type::template Make(b) );
        }
    
        /// Reflect a by b, return a
        template<class A, class B>
        static constexpr A re(const A& a, const B& b) {
         typedef MGProd<typename B::basis, typename A::basis, metric> X;
         typedef TMV<typename X::basis> Type;
         return RMGProd< Type, B, A, metric>::Call( gp(b, a.involution() ), Reverse<typename B::basis>::Type::template Make(b) );
        } 
  };
 
   /*-----------------------------------------------------------------------------
    *  CONFORMAL
    *-----------------------------------------------------------------------------*/
   template<typename Algebra>
   struct AlgebraImpl<Algebra,false, true> : AlgebraImplBase<Algebra> {

       using metric = typename Algebra::metric::type;

       //lift basis into algebraic multivector (monad)
       template <class B> using TMV = MV<Algebra, B>;
       static const bits::type dim = Algebra::dim;

       template <class A, class B> using sum_t = typename ICat< typename NotType< A, B >::Type, A >::Type;
       template <class A, class B> using  gp_t = typename EGProd<A,B >::basis;
       template <class A, class B> using  op_t = typename EOProd<A,B >::basis;
       template <class A, class B> using  ip_t = typename EIProd<A,B >::basis;

       
       template<class A, class B> using GPBasis = typename CGProd<A,B, metric >::basis;
       template<class A, class B> using OPBasis = typename COProd<A,B, metric >::basis;
       template<class A, class B> using IPBasis = typename CIProd<A,B, metric >::basis;
       template<class A, class B> using GP = TMV<GPBasis<typename A::basis,typename B::basis>>;
       template<class A, class B> using OP = TMV<OPBasis<typename A::basis,typename B::basis>>;
       template<class A, class B> using IP = TMV<IPBasis<typename A::basis,typename B::basis>>;    

       /*-----------------------------------------------------------------------------
        *  Conformal Product Functions (unknown return type)
        *-----------------------------------------------------------------------------*/
        template<class A, class B>
        SCA gp(const A& a, const B& b) -> TMV<typename CGProd<typename A::basis, typename B::basis, metric>::basis> {
          typedef CGProd<typename A::basis, typename B::basis, metric> X;
          typedef TMV<typename X::basis> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA op(const A& a, const B& b) -> TMV<typename COProd<typename A::basis, typename B::basis, metric>::basis> {
          typedef COProd<typename A::basis, typename B::basis, metric> X;
          typedef  TMV<typename X::basis> Type;
          return X::Arrow::template Make<Type>(a,b);
        }
        template<class A, class B>
        SCA ip(const A& a, const B& b) -> TMV<typename CIProd<typename A::basis, typename B::basis, metric>::basis> {
          typedef CIProd<typename A::basis, typename B::basis, metric> X;
          typedef  TMV<typename X::basis> Type;
          return X::Arrow::template Make<Type>(a,b);
        }

    
        /*-----------------------------------------------------------------------------
         *  Known Return Type
         *-----------------------------------------------------------------------------*/
        /// Spin a by b, return a
        template<class A, class B>
        static constexpr A sp(const A& a, const B& b) {
          typedef CGProd<typename B::basis, typename A::basis, metric> X;
          typedef TMV<typename X::basis> Type;
          return RCGProd< Type, B, A, metric >::Call( gp(b, a), Reverse<typename B::basis>::Type::template Make(b) );
        }
    
        /// Reflect a by b, return a
        template<class A, class B>
        static constexpr A re(const A& a, const B& b) {
         typedef CGProd<typename B::basis, typename A::basis, metric> X;
         typedef TMV<typename X::basis> Type;
         return RCGProd< Type, B, A, metric>::Call( gp(b, a.involution() ), Reverse<typename B::basis>::Type::template Make(b) );
        } 


   };
 


   template<typename alg> struct basetype{};
   
   /*-----------------------------------------------------------------------------
    *  Default Euclidean Basis Types (specialize basetype to your own needs)
    *-----------------------------------------------------------------------------*/
   template<typename alg> 
   struct basetype<AlgebraImpl<alg,true,false>>{

            using algebra = AlgebraImpl<alg,true,false>;
            
            //use as e<1,2,3> will return e123 blade
            template<bits::type ... N> using e = Basis< bits::blade((1<<(N-1))...)>;  
            
            using sca = Basis<0>;  
            using pss = Basis<bits::pss(alg::dim)>;
            using vec = typename Blade1<alg::dim>::VEC;   
            using biv = typename algebra::template op_t<vec,vec>;
            using tri = typename algebra::template op_t<vec,biv>;
            using rot = typename algebra::template sum_t<biv,sca>;
            using euc = pss; //necessary redundancy to avoid errors when compiling euclidean duale() method

            using Sca = MV<alg,sca >;
            using Pss = MV<alg,pss >;
            using Vec = MV<alg,vec >;
            using Biv = MV<alg,biv >;
            using Tri = MV<alg,tri >;
            using Rot = MV<alg,rot >;
            using Euc = MV<alg,euc >;

   };



   /*-----------------------------------------------------------------------------
    *  Default Conformal Basis Types
    *-----------------------------------------------------------------------------*/
   template<typename alg> 
   struct basetype<AlgebraImpl<alg,false,true>>{
            
            using algebra = AlgebraImpl<alg,false,true>;
            //use as e<1,2,3> will return e123 blade
            template<bits::type ... N> using e = Basis< bits::blade((1<<(N-1))...)>;  
            
            //BASIS
            using sca = Basis<0>;  
            using pss = Basis<bits::pss(algebra::dim)>;
            using euc =  Basis<bits::pss(algebra::dim-2)>;
            using vec = typename Blade1<algebra::dim-2>::VEC;               
            using pnt = typename Blade1<algebra::dim>::VEC ;
            using ori = typename basis_t::origin<algebra::dim> ; 
            using inf = typename basis_t::infinity<algebra::dim> ; 
            using mnk = typename basis_t::eplane<algebra::dim> ; 
            using biv = typename algebra::template op_t<vec,vec>;
            using tri = typename algebra::template op_t<vec,biv>;
            using rot = typename algebra::template sum_t<biv,sca>;
            using tnv = typename algebra::template op_t<vec,ori>;
            using drv = typename algebra::template op_t<vec,inf>;
            using tnb = typename algebra::template op_t<biv,ori>;
            using drb = typename algebra::template op_t<biv,inf>;
            using tnt = typename algebra::template op_t<tri,ori>;
            using drt = typename algebra::template op_t<tri,inf>;
            using par = typename algebra::template op_t<pnt,pnt>;
            using cir = typename algebra::template op_t<par,pnt>;
            using sph = typename algebra::template op_t<cir,pnt>;
            using flp = typename algebra::template op_t<pnt,inf>;
            using dll = typename algebra::template sum_t<biv,drv>;
            using lin = typename algebra::template op_t<pnt,flp>;
            using dlp = typename algebra::template sum_t<vec,inf>;
            using pln = typename algebra::template op_t<cir,inf>;
            using trs = typename algebra::template sum_t<drv, sca>;// 1 );
            using trv = typename algebra::template sum_t<tnv, sca>;// 1 );
            using dil = typename algebra::template sum_t<mnk, sca>;// 1 );
            using tsd = typename algebra::template sum_t<flp, sca>;// 1 );
            using mot = typename algebra::template gp_t<rot, trs>;
            using bst = typename algebra::template sum_t<par,sca>;
            using con = typename algebra::template gp_t<par, par>;
            using dls = pnt;

            //MULTIVECTORS
            using Sca = MV<alg,sca>; 
            using Pss = MV<alg,pss>;  
            using Euc = MV<alg,euc>;
            using Vec = MV<alg,vec>; 
            using Pnt = MV<alg,pnt>;
            using Ori = MV<alg,ori>;
            using Inf = MV<alg,inf>;
            using Mnk = MV<alg,mnk>;
            using Biv = MV<alg,biv>;
            using Tri = MV<alg,tri>;
            using Rot = MV<alg,rot>;
            using Tnv = MV<alg,tnv>;
            using Drv = MV<alg,drv>;
            using Tnb = MV<alg,tnb>;
            using Drb = MV<alg,drb>;
            using Tnt = MV<alg,tnt>;
            using Drt = MV<alg,drt>;
            using Par = MV<alg,par>;
            using Cir = MV<alg,cir>;
            using Sph = MV<alg,sph>;
            using Flp = MV<alg,flp>;
            using Dll = MV<alg,dll>;
            using Lin = MV<alg,lin>;
            using Dlp = MV<alg,dlp>;
            using Pln = MV<alg,pln>;
            using Trs = MV<alg,trs>;
            using Trv = MV<alg,trv>;
            using Dil = MV<alg,dil>;
            using Tsd = MV<alg,tsd>;
            using Mot = MV<alg,mot>;
            using Bst = MV<alg,bst>;
            using Con = MV<alg,con>;
            using Dls = MV<alg,dls>;

   };





} //vsr::

#endif   /* ----- #ifndef vsr_algebra_INC  ----- */
