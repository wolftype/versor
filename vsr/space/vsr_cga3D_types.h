#ifndef VSR_CGA3D_TYPES_INCLUDED
#define VSR_CGA3D_TYPES_INCLUDED

#include "core/vsr_mv.h"

namespace vsr{


//using Algebra = CGA<5>;
//using Metric = typename Algbera::MetricList;
//using Impl = typename Algebra::impl;
//
////Basis building blocks
//using ScaBasis =    typename basis::sca;
//using PntBasis =    typename Blade1<Algebra::Dim>::VEC;
//using PssBasis =    typename basis::pss<Algebra::Dim>;
//using EucPssBasis = typename basis::pss<Algebra::Dim-2>;
//using VecBasis =    typename Blade1<Algebra::Dim-2>::VEC;
//using OriBasis =    typename basis::origin<Algebra::Dim>; 
//using InfBasis =    typename basis::infinity<Algebra::Dim>; 
//using MnkBasis =    typename basis::eplane<Algebra::Dim> ; 
//
////Suppress Re-Instantiation of Combinatorics (explicitly instantiated in cpp file)
//extern template struct CGProd<PntBasis,PntBasis,Metric>;
//extern template struct COProd<PntBasis,PntBasis,Metric>;
//extern template struct CIProd<PntBasis,PntBasis,Metric>;


//extern template struct MV<Algebra, ScaBasis>;

namespace cga3D{
//3D CONFORMAL 
/* typedef Ne<5,1> e1; */ 
/* typedef Ne<5,2> e2; */ 
/* typedef Ne<5,3> e3; */ 
/* typedef Ne<5,4> no; */
/* typedef Ne<5,5> ni; */


using Sca = NSca<5>;
using Vec = NVec<5>;//typename CGA<5>::Types::Vec;
using Biv = NBiv<5>;//typename CGA<5>::Types::Biv;
using Rot = NRot<5>;//typename CGA<5>::Types::Rot;
using Tri = NTri<5>;//typename CGA<5>::Types::EucPss;

using Ori = NOri<5>;//typename CGA<5>::Types::Ori;
using Inf = NInf<5>;//typename CGA<5>::Types::Inf;
using Mnk = NMnk<5>;//typename CGA<5>::Types::Mnk;
using Pss = NPss<5>;//typename CGA<5>::Types::Pss; 

using Pnt = NPnt<5>;//typename CGA<5>::Types::Pnt; 
using Par = NPar<5>;//typename CGA<5>::Types::Par; 
using Cir = NCir<5>;//typename CGA<5>::Types::Cir;
using Sph = NSph<5>;//typename CGA<5>::Types::Sph;

using Flp = NFlp<5>;//typename CGA<5>::Types::Flp;
using Dll = NDll<5>;//typename CGA<5>::Types::Dll;
using Lin = NLin<5>;//typename CGA<5>::Types::Lin;
using Dlp = NDlp<5>;//typename CGA<5>::Types::Dlp;
using Pln = NPln<5>;//typename CGA<5>::Types::Pln;

using Drv = NDrv<5>;//typename CGA<5>::Types::Drv;
using Tnv = NTnv<5>;//typename CGA<5>::Types::Tnv;
using Drb = NDrb<5>;//typename CGA<5>::Types::Drb;
using Tnb = NTnb<5>;// typename CGA<5>::Types::Tnb;
using Drt = NDrt<5>;// typename CGA<5>::Types::Drt;
using Tnt = NTnt<5>;//typename CGA<5>::Types::Tnt;

using Trs = NTrs<5>;//typename CGA<5>::Types::Trs;     //Translator 
using Mot = NMot<5>;//typename CGA<5>::Types::Mot;     //Motor 
using Trv = NTrv<5>;//typename CGA<5>::Types::Trv;     //Transversor 
using Bst = NBst<5>;//typename CGA<5>::Types::Bst;     //Boost 
using Con = NCon<5>;//typename CGA<5>::Types::Con;     //General Conformal Rotor 
using Dil = NDil<5>;//typename CGA<5>::Types::Dil;     //Dilator 
using Tsd = NTsd<5>;//typename CGA<5>::Types::Tsd;     //Translated Dilator   


//FULL NAMES
typedef  Sca Scalar;
typedef  Biv Bivector;
typedef  Tri Trivector ;
typedef  Rot Rotor;
typedef  Ori Origin;
typedef  Inf Infinity;
typedef  Mnk Minkowski;
typedef  Pss Pseudoscalar;
typedef  Pnt Point;
typedef  Par Pair;
typedef  Cir Circle;
typedef  Sph Sphere;
typedef  Pnt DualSphere;
typedef  Drv DirectionVector;
typedef  Drb DirectionBivector;
typedef  Drt DirectionTrivector;
typedef  Tnv TangentVector;
typedef  Tnb TangentBivector;
typedef  Tnt TangentTrivector;
typedef  Dll DualLine;
typedef  Lin Line;
typedef  Flp FlatPoint;
typedef  Pln Plane;
typedef  Dlp DualPlane;
typedef  Trs Translator;
typedef  Mot Motor;
typedef  Trv Transversor;
typedef  Bst Boost;
typedef  Dil Dilator;
typedef  Tsd TranslatedDilator;


////Euclidean Elements
//typedef NSca<5> Sca;      //Scalar    (s)
//typedef NVec<5> Vec;      //Vector    (e1, e2, e3)
////typedef NVec2D<5> Vec2D;  //2D Vector (e1,e2)
//typedef NBiv<5> Biv;      //Bivector  (e12, e13, e23)
//typedef NTri<5> Tri;      //Trivector (e123)
//typedef NRot<5> Rot;      //Rotor     (s, e12, e13, e23) 
//
//typedef NOri<5> Ori;      //Origin        (no)
//typedef NInf<5> Inf;      //Infinity      (ni)
//typedef NMnk<5> Mnk;      //E Plane       (noni)
//typedef NPss<5> Pss;      //PseudoScalar  (e123noni)
//
//typedef NPnt<5> Pnt; 	    //Point         (e1, e2, e3, no, ni)  
//typedef Pnt Dls; 		      //Dual Sphere   (e1, e2, e3, no, ni) 
//
//typedef NPar<5> Par;	    //Point Pair    (e12,e13,e23,e1no,e2no,e3no,e1ni,e2ni,e3ni,noni)
//typedef NCir<5> Cir;	    //Circle        
//typedef NSph<5> Sph;	    //Sphere          
//                    	
//typedef NDrv<5> Drv;	    //Direction Vector
//typedef NTnv<5> Tnv;	    //Tangent Vector   
//typedef NDrb<5> Drb;     //Direction Bivector
//typedef NTnb<5> Tnb;     //Tangent Bivector  
//typedef NDrt<5> Drt;     //Direction Trivector
//typedef NTnt<5> Tnt;     //Tangent Trivector  
//                    	
//typedef NDll<5> Dll;     //Dual Line        
//typedef NLin<5> Lin;     //Dual Line    
//typedef NFlp<5> Flp;     //Flat Plane
//typedef NPln<5> Pln;     //Plane 
//typedef NDlp<5> Dlp;     //Plane   
//                        
//typedef NTrs<5> Trs;     //Translator 
//typedef NMot<5> Mot;     //Motor 
//typedef NTrv<5> Trv;     //Transversor 
//typedef NBst<5> Bst;     //Boost 
//typedef NDil<5> Dil;     //Dilator 
//typedef NTsd<5> Tsd;     //Translated Dilator      



  }//cga3D::

/* using namespace cga3D; */

/* extern template Pnt Pnt::sp( const Rot& )const; */                         
/* extern template Pnt Pnt::sp( const Trs& )const; */                         
/* extern template Pnt Pnt::sp( const Mot& )const; */                       
/* extern template Pnt Pnt::sp( const Bst& )const; */
/* extern template Pnt Pnt::sp( const Tsd& )const; */
/* extern template Pnt Pnt::sp( const Dil& )const; */
                                                         
/* extern template Par Par::sp( const Rot& )const; */
/* extern template Par Par::sp( const Trs& )const; */
/* extern template Par Par::sp( const Mot& )const; */
/* extern template Par Par::sp( const Bst& )const; */
/* extern template Par Par::sp( const Tsd& )const; */
/* extern template Par Par::sp( const Dil& )const; */
                                                       
/* extern template Cir Cir::sp( const Rot& )const; */
/* extern template Cir Cir::sp( const Trs& )const; */
/* extern template Cir Cir::sp( const Mot& )const; */
/* extern template Cir Cir::sp( const Bst& )const; */
/* extern template Cir Cir::sp( const Tsd& )const; */
/* extern template Cir Cir::sp( const Dil& )const; */

} // vsr:: 


#endif
