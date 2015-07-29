/*!
 * @file 
 
    implementations of algebras (Euclidean, PQ-metric, and conformal)
    

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

#include "vsr_xlists.h"   ///<-- list processing functions
#include "vsr_products.h" ///<-- compile time processing of instructions ("arrows" or "morphisms")

namespace vsr {

   
     // using metric = Metric;
    /*-----------------------------------------------------------------------------
     *  generates basis blade in dimension dim of grade 
    
        note: to do: guard again negative dimensions and negative grades
     *-----------------------------------------------------------------------------*/
      
      /// metafunction to construct a basis blade type from spatial dimension and subspace grade
      template<bits::type dim, bits::type grade>
      struct blade {
          using vec = typename vsr::Blade1<dim>::VEC; 
          using sub = typename blade<dim, grade-1>::type;
          using type = typename EOProd<vec,sub>::basis;
    
      };
    
      /// limiting case of grade 0 (scalar)
      template<bits::type dim>
      struct blade<dim, 0>{
          using type = Basis<0>;
      };
    
      /// all blades within a spatial dim upto and including maxgrade
      template<bits::type dim, bits::type grade=dim>
      struct all_blades{
          using sub = typename all_blades<dim, grade-1>::type;
          using one = typename blade<dim, grade>::type;
          using type = typename ICat< one, sub >::Type;
      };
    
      /// limiting case of grade 0 (scalar)
      template<bits::type dim>
      struct all_blades<dim,0>{
          using type = Basis<0>;
      };  

    //forward declaration of multivector
    template<class algebra, class basis> struct Multivector;
    //forward declaration of algebraImpl
    template<class algebra, bool, bool> struct algebra_impl;
    //forward declaration of named_types
    template<class algebraimpl> struct named_types;

    /*!-----------------------------------------------------------------------------
       An algebra instance is templated on: 
     
       metric_type: a metric (e.g. Metric<3,0> for euclidean 3 space or Metric<4,1,true> for conformal 5D space )
       value_type:  a field value type (i.e. real, complex, or some other arithmetic element).
           
       The value type can be anything that commutatively multiplies and adds in a closed group, 
       allowing for tensor metrics C x C, etc a la Bott periodicity.


         algebra is a vsr::algebra< vsr::metric< P, Q, bConformal>, value_t>
            where: 
            * P and Q are integers representing the **signature** of a diagonal metric
            * bConformal is a boolean value specifing whether the metric should be **split**

         For example:
    
             using ega = vsr::algebra< metric<3>, float>;

     *-----------------------------------------------------------------------------*/
    template< class metric_type, class value_type >
    struct algebra {

        using metric = metric_type;                             ///<-- Metric, with signature, whether Euclidean, Projective, Conformal, etc
          
        using value_t = value_type;                             ///<-- %Field over which Algebra is Defined (e.g. float, double, complex)
        
        static const int dim = metric::type::Num;               ///<-- Dimension of Algebra (2,3,4,5,etc)
 
        /// a multivector is a monadic structure that wraps a basis with a bilinear quadratic form 
        template <class B> using mv_t = Multivector<algebra<metric, value_t>, B>;
 
        /// implementation details for dealing with conformal vs euclidean etc (in vsr_algebra.h)
        using impl = algebra_impl< algebra<metric, value_t>, metric::is_euclidean, metric::is_conformal >;                

        template <class A, class B> using sum_basis_t = typename ICat< typename NotType< A, B >::Type, A >::Type;
        template <class A, class B> using  gp_basis_t = typename impl::template gp_arrow_t<A,B>::basis;
        template <class A, class B> using  op_basis_t = typename impl::template op_arrow_t<A,B>::basis;
        template <class A, class B> using  ip_basis_t = typename impl::template ip_arrow_t<A,B>::basis;

        // lift functions of A and B into the multivector form
        template <class A, class B> using sum_lift_t = mv_t< sum_basis_t< A, B>>;
        template <class A, class B> using gp_lift_t =  mv_t< gp_basis_t<A,B> >;
        template <class A, class B> using op_lift_t =  mv_t< op_basis_t< A, B> >;
        template <class A, class B> using ip_lift_t =  mv_t< ip_basis_t< A, B> >;  

        // bind the contents of multivectors A and B into the functions
        template <class A, class B> using sum_t = sum_lift_t<typename A::basis,typename B::basis>;
        template <class A, class B> using gp_t = gp_lift_t<typename A::basis,typename B::basis>;
        template <class A, class B> using op_t = op_lift_t<typename A::basis,typename B::basis>;
        template <class A, class B> using ip_t = ip_lift_t<typename A::basis,typename B::basis>;           

        /*-----------------------------------------------------------------------------
         *  Sum Functions 
         *-----------------------------------------------------------------------------*/
         /// Sum of Similar types
         template<class B> 
         static mv_t<B> 
         sum( const mv_t<B> & a, const mv_t<B>& b) {
           mv_t<B> c;
           for (int i = 0; i < B::Num; ++i) c[i] = a[i] + b[i];
           return c;
         } 
         /// Difference of Similar types
         template<class B> 
         static mv_t<B> 
         diff( const mv_t<B> & a, const mv_t<B>& b) {
           mv_t<B> c;
           for (int i = 0; i < B::Num; ++i) c[i] = a[i] - b[i];
           return c;
         } 
        
         /// Sum of Different types
         template<class B1, class B2>
         static mv_t<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type>
         sum( const mv_t<B1> & a, const mv_t<B2>& b) {
           typedef mv_t<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type> Ret; 
           return sum( a.template cast<Ret>() ,  b.template cast<Ret>() );
         } 
         /// Difference of Different types
         template<class B1, class B2>
         static mv_t<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type>
         diff( const mv_t<B1> & a, const mv_t<B2>& b) {
           typedef mv_t<typename ICat< typename NotType< B1, B2 >::Type, B1 >::Type> Ret; 
           return diff( a.template cast<Ret>() ,  b.template cast<Ret>() );
         } 
        
         /// Sum some scalar value 
         template<class B>
         static mv_t<typename ICat< typename NotType< Basis<0>, B >::Type, Basis<0> >::Type>
         sumv( VSR_PRECISION a, const  mv_t<B>& b) {
           typedef mv_t<typename ICat< typename NotType< Basis<0>, B >::Type, Basis<0> >::Type> Ret;
           return sum( Ret(a) , b.template cast<Ret>() );
         }  

        /*-----------------------------------------------------------------------------
         *  Product Functions (unknown return type)
         *-----------------------------------------------------------------------------*/
         template<class A, class B>
         static constexpr auto gp(const A& a, const B& b) -> gp_t<A,B> {
           typedef typename impl::template gp_arrow_t<typename A::basis, typename B::basis> x;
           typedef mv_t<typename x::basis> type;
           return x::Arrow::template Make<type>(a,b);
         }
         template<class A, class B>
         static constexpr auto op(const A& a, const B& b) -> op_t<A,B>  {
           typedef typename impl::template op_arrow_t<typename A::basis, typename B::basis> x;
           typedef mv_t<typename x::basis> type;
           return x::Arrow::template Make<type>(a,b);
         }
         template<class A, class B>
         static constexpr auto ip(const A& a, const B& b) -> ip_t<A,B> {
           typedef typename impl::template ip_arrow_t<typename A::basis, typename B::basis> x;
           typedef mv_t<typename x::basis> type;
           return x::Arrow::template Make<type>(a,b);
         }
 
         /// Spin a by b, return type a
         template<class A, class B>
         static constexpr A spin(const A& a, const B& b) {
           typedef gp_basis_t<typename B::basis, typename A::basis > tmp_basis;      
           //............................................lh...........rh.................return type 
           using x = typename impl::template rot_arrow_t<tmp_basis, typename B::basis, typename A::basis>;                 
           return x::Arrow::template Make<A>( gp(b, a), Reverse<typename B::basis>::Type::template Make(b) );
         }
    
         /// Reflect a by b, return type a
         template<class A, class B>
         static constexpr A reflect(const A& a, const B& b) {
          typedef gp_basis_t<typename B::basis, typename A::basis > tmp_basis;
          using x = typename impl::template rot_arrow_t<tmp_basis, typename B::basis, typename A::basis>;
          return x::Arrow::template Make<A>( gp(b, a.involution() ), Reverse<typename B::basis>::Type::template Make(b) );
         } 
 
          /// make a type from sum of basis B1 and B2
         template<typename B1, typename B2> 
         using make_sum = sum_lift_t<B1, B2>;

         /// make a type from geometric product of basis B1 and B2
         template<typename B1, typename B2> 
         using make_gp =   gp_lift_t<B1, B2>;

         /// make a type from outer product of basis B1 and B2
         template<typename B1, typename B2> 
         using make_op =   op_lift_t<B1, B2>;

         /// make a type from inner product of basis B1 and B2
         template<typename B1, typename B2> 
         using make_ip =   ip_lift_t<B1, B2>;

         /// make a type from a grade
         template<bits::type grade>
         using make_grade =  mv_t< typename blade<dim,grade>::type >;

         /// certain prenamed types in euclidean and conformal 
         using types = named_types<impl>;

         ///Next Higher Algebra over the same field
         using up = algebra< typename metric::up, value_t>;  
         
         
         using vector_basis = typename blade<dim,1>::type;          ///<-- 1-blade vector basis (no data)
         using vec = mv_t< vector_basis >;                          ///<-- 1-blade vector type  (stores data)
      

    };



       
    /*-----------------------------------------------------------------------------
    *  ALGEBRA SPECIFIC PRODUCTS
    *-----------------------------------------------------------------------------*/
    template<typename Algebra, bool Euclidean, bool Conformal>
    struct algebra_impl {
       // using algebra = Algebra;  
    };

   /*-----------------------------------------------------------------------------
    *  EUCLIDEAN (NO NEGATIVE METRIC)
    *-----------------------------------------------------------------------------*/
   template<typename Algebra>
   struct algebra_impl<Algebra,true, false>  {

     //  static const bits::type dim = Algebra::dim;

       template <class A, class B> using  gp_arrow_t = EGProd<A,B>;
       template <class A, class B> using  op_arrow_t = EOProd<A,B>;
       template <class A, class B> using  ip_arrow_t = EIProd<A,B>;

       template< class R, class A, class B> using  rot_arrow_t = REGProd< R, A, B>;

   };

 

   /*-----------------------------------------------------------------------------
    *  Metric Product Functions (e.g for spacetime algebra)
    *-----------------------------------------------------------------------------*/
   template<typename Algebra>
   struct algebra_impl<Algebra,false, false>  {
       using algebra = Algebra;
       using metric_type = typename Algebra::metric::type;
      // static const bits::type dim = Algebra::dim;

       template <class A, class B> using  gp_arrow_t = MGProd<A, B, metric_type>;
       template <class A, class B> using  op_arrow_t = MOProd<A, B, metric_type>;
       template <class A, class B> using  ip_arrow_t = MIProd<A, B, metric_type>;

       template< class R, class A, class B> using  rot_arrow_t = RMGProd< R, A, B, metric_type>;

  };
 
   /*-----------------------------------------------------------------------------
    *  CONFORMAL
    *-----------------------------------------------------------------------------*/
   template<typename Algebra>
   struct algebra_impl<Algebra,false, true>  {

       using metric_type = typename Algebra::metric::type;
    //   static const bits::type dim = Algebra::dim;

       template <class A, class B> using  gp_arrow_t = CGProd<A, B, metric_type>;
       template <class A, class B> using  op_arrow_t = COProd<A, B, metric_type>;
       template <class A, class B> using  ip_arrow_t = CIProd<A, B, metric_type>;

       template< class R, class A, class B> using  rot_arrow_t = RCGProd< R, A, B, metric_type>;
  };
            



   /*-----------------------------------------------------------------------------
    *  Default Metric Types don't exist . . . use ::vector
    *-----------------------------------------------------------------------------*/
   template<typename A> 
   struct named_types{
     
      using alg = typename A::algebra;
      //use as e<1,2,3> will return e123 blade
      template<bits::type ... N> using e_basis = Basis< bits::blade((1<<(N-1))...)>;  
      template<bits::type ... N> using e = Multivector<alg, e_basis<N...>>;

      using sca = Basis<0>;  
      using pss = Basis<bits::pss(alg::dim)>;
      using vec = typename Blade1<alg::dim>::VEC;   
      using biv = typename alg::template op_basis_t<vec,vec>;
      using tri = typename alg::template op_basis_t<vec,biv>;
      using rot = typename alg::template sum_basis_t<biv,sca>;
     
     
    };
   
   /*-----------------------------------------------------------------------------
    *  Default Euclidean Basis Types (specialize named_types to your own needs)
    *-----------------------------------------------------------------------------*/
   template<typename alg> 
   struct named_types<algebra_impl<alg,true,false>>{

            using algebra = alg;//AlgebraImpl<alg,true,false>;
            
            //use as e<1,2,3> will return e123 blade
            template<bits::type ... N> using e_basis = Basis< bits::blade((1<<(N-1))...)>;  
            template<bits::type ... N> using e = Multivector<alg, e_basis<N...>>;
            
            using sca = Basis<0>;  
            using pss = Basis<bits::pss(alg::dim)>;
            using vec = typename Blade1<alg::dim>::VEC;   
            using biv = typename algebra::template op_basis_t<vec,vec>;
            using tri = typename algebra::template op_basis_t<vec,biv>;
            using rot = typename algebra::template sum_basis_t<biv,sca>;
            using euc = pss; //necessary redundancy to avoid errors when compiling euclidean duale() method

            using Sca = Multivector<alg,sca >;
            using Pss = Multivector<alg,pss >;
            using Vec = Multivector<alg,vec >;
            using Biv = Multivector<alg,biv >;
            using Tri = Multivector<alg,tri >;
            using Rot = Multivector<alg,rot >;
            using Euc = Multivector<alg,euc >;

            using scalar_basis = Basis<0>;
            using pseudoscalar_basis = Basis<bits::pss(alg::dim)>;
            using vector_basis = typename blade<alg::dim,1>::type;
            using bivector_basis = typename blade<alg::dim,2>::type;
            using trivector_basis = typename blade<alg::dim,3>::type;
            using rotor_basis = typename algebra::template sum_basis_t<bivector_basis, scalar_basis>;
            using euclidean_pseudoscalar_basis = pseudoscalar_basis;

            using scalar = Multivector<alg,scalar_basis >;
            using vector = Multivector<alg, vector_basis >;
            using bivector = Multivector<alg, bivector_basis >;
            using trivector = Multivector<alg,trivector_basis >;
            using rotor = Multivector<alg,rotor_basis>;
            using pseudoscalar = Multivector<alg, pseudoscalar_basis>;

   };



   /*-----------------------------------------------------------------------------
    *  Default Conformal Basis Types
    *-----------------------------------------------------------------------------*/
   template<typename alg> 
   struct named_types<algebra_impl<alg,false,true>>{
            
            using algebra = alg;//AlgebraImpl<alg,false,true>;
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
            
            using biv = typename algebra::template op_basis_t<vec,vec>;
            using tri = typename algebra::template op_basis_t<vec,biv>;
            using rot = typename algebra::template sum_basis_t<biv,sca>;
            using tnv = typename algebra::template op_basis_t<vec,ori>;
            using drv = typename algebra::template op_basis_t<vec,inf>;
            using tnb = typename algebra::template op_basis_t<biv,ori>;
            using drb = typename algebra::template op_basis_t<biv,inf>;
            using tnt = typename algebra::template op_basis_t<tri,ori>;
            using drt = typename algebra::template op_basis_t<tri,inf>;
            using par = typename algebra::template op_basis_t<pnt,pnt>;
            using cir = typename algebra::template op_basis_t<par,pnt>;
            using sph = typename algebra::template op_basis_t<cir,pnt>;
            using flp = typename algebra::template op_basis_t<pnt,inf>;
           // using dll = typename algebra::template sum_basis_t<biv,drv>;
            using lin = typename algebra::template op_basis_t<pnt,flp>;
            using dll = typename algebra::template gp_basis_t<lin,pss>;
            using dlp = typename algebra::template sum_basis_t<vec,inf>;
            using pln = typename algebra::template op_basis_t<cir,inf>;
            using trs = typename algebra::template sum_basis_t<drv, sca>;// 1 );
            using trv = typename algebra::template sum_basis_t<tnv, sca>;// 1 );
            using dil = typename algebra::template sum_basis_t<mnk, sca>;// 1 );
            using tsd = typename algebra::template sum_basis_t<flp, sca>;// 1 );
            using mot = typename algebra::template gp_basis_t<rot, trs>;
            using bst = typename algebra::template sum_basis_t<par,sca>;
            using con = typename algebra::template gp_basis_t<par, par>;
            using dls = pnt;

            //MULTIVECTORS
            using Sca = Multivector<alg,sca>; 
            using Pss = Multivector<alg,pss>;  
            using Euc = Multivector<alg,euc>;
            using Vec = Multivector<alg,vec>; 
            using Pnt = Multivector<alg,pnt>;
            using Ori = Multivector<alg,ori>;
            using Inf = Multivector<alg,inf>;
            using Mnk = Multivector<alg,mnk>;
            using Biv = Multivector<alg,biv>;
            using Tri = Multivector<alg,tri>;
            using Rot = Multivector<alg,rot>;
            using Tnv = Multivector<alg,tnv>;
            using Drv = Multivector<alg,drv>;
            using Tnb = Multivector<alg,tnb>;
            using Drb = Multivector<alg,drb>;
            using Tnt = Multivector<alg,tnt>;
            using Drt = Multivector<alg,drt>;
            using Par = Multivector<alg,par>;
            using Cir = Multivector<alg,cir>;
            using Sph = Multivector<alg,sph>;
            using Flp = Multivector<alg,flp>;
            using Dll = Multivector<alg,dll>;
            using Lin = Multivector<alg,lin>;
            using Dlp = Multivector<alg,dlp>;
            using Pln = Multivector<alg,pln>;
            using Trs = Multivector<alg,trs>;
            using Trv = Multivector<alg,trv>;
            using Dil = Multivector<alg,dil>;
            using Tsd = Multivector<alg,tsd>;
            using Mot = Multivector<alg,mot>;
            using Bst = Multivector<alg,bst>;
            using Con = Multivector<alg,con>;
            using Dls = Multivector<alg,dls>;


            using scalar = Multivector<alg,sca>; 
            using psuedoscalar = Multivector<alg,pss>;  
            using euclidean_pseudoscalar = Multivector<alg,euc>;
            using vector = Multivector<alg,vec>; 
            using point = Multivector<alg,pnt>;
            using origin = Multivector<alg,ori>;
            using infinity = Multivector<alg,inf>;
            using eplane = Multivector<alg,mnk>;
            using bivector = Multivector<alg,biv>;
            using trivector = Multivector<alg,tri>;
            using rotor = Multivector<alg,rot>;
            using tangent_vector = Multivector<alg,tnv>;
            using direction_vector = Multivector<alg,drv>;
            using tangent_bivector = Multivector<alg,tnb>;
            using direction_bivector = Multivector<alg,drb>;
            using tangent_trivector = Multivector<alg,tnt>;
            using direction_trivector = Multivector<alg,drt>;
            using pair = Multivector<alg,par>;
            using circle = Multivector<alg,cir>;
            using sphere = Multivector<alg,sph>;
            using flat_point = Multivector<alg,flp>;
            using dual_line = Multivector<alg,dll>;
            using line = Multivector<alg,lin>;
            using dual_plane = Multivector<alg,dlp>;
            using plane = Multivector<alg,pln>;
            using translator = Multivector<alg,trs>;
            using transversor = Multivector<alg,trv>;
            using dilator = Multivector<alg,dil>;
            using translated_dilator = Multivector<alg,tsd>;
            using motor = Multivector<alg,mot>;
            using boost = Multivector<alg,bst>;
            using conformal_rotor = Multivector<alg,con>;
            using dual_sphere = Multivector<alg,dls>;


            using Scalar = Multivector<alg,sca>; 
            using Psuedoscalar = Multivector<alg,pss>;  
            using EuclideanPseudoscalar = Multivector<alg,euc>;
            using Vector = Multivector<alg,vec>; 
            using Point = Multivector<alg,pnt>;
            using Origin = Multivector<alg,ori>;
            using Infinity = Multivector<alg,inf>;
            using Eplane = Multivector<alg,mnk>;
            using Bivector = Multivector<alg,biv>;
            using Trivector = Multivector<alg,tri>;
            using Rotor = Multivector<alg,rot>;
            using TangentVector = Multivector<alg,tnv>;
            using DirectionVector = Multivector<alg,drv>;
            using TangentBivector = Multivector<alg,tnb>;
            using DirectionBivector = Multivector<alg,drb>;
            using TangentTrivector = Multivector<alg,tnt>;
            using DirectionTrivector = Multivector<alg,drt>;
            using Pair = Multivector<alg,par>;
            using Circle = Multivector<alg,cir>;
            using Sphere = Multivector<alg,sph>;
            using FlatPoint = Multivector<alg,flp>;
            using DualLine = Multivector<alg,dll>;
            using Line = Multivector<alg,lin>;
            using DualPlane = Multivector<alg,dlp>;
            using Plane = Multivector<alg,pln>;
            using Translator = Multivector<alg,trs>;
            using Transversor = Multivector<alg,trv>;
            using Dilator = Multivector<alg,dil>;
            using TranslatedDilator = Multivector<alg,tsd>;
            using Motor = Multivector<alg,mot>;
            using Boost = Multivector<alg,bst>;
            using ConformalRotor = Multivector<alg,con>;
            using DualSphere = Multivector<alg,dls>;
          
            

   };





} //vsr::

#endif   /* ----- #ifndef vsr_algebra_INC  ----- */
