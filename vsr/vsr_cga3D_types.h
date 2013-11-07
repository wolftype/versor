#ifndef VSR_CGA3D_TYPES_INCLUDED
#define VSR_CGA3D_TYPES_INCLUDED

#include "vsr_products.h"



namespace vsr{

namespace cga3D{

//3D CONFORMAL 
typedef Ne<5,1> e1; 
typedef Ne<5,2> e2; 
typedef Ne<5,3> e3; 
typedef Ne<5,4> no;
typedef Ne<5,5> ni;

//Euclidean Elements
typedef NSca<5> Sca;      //Scalar    (s)
typedef NVec<5> Vec;      //Vector    (e1, e2, e3)
typedef NVec2D<5> Vec2D;  //2D Vector (e1,e2)
typedef NBiv<5> Biv;      //Bivector  (e12, e13, e23)
typedef NTri<5> Tri;      //Trivector (e123)
typedef NRot<5> Rot;      //Rotor     (s, e12, e13, e23) 

typedef NOri<5> Ori;      //Origin        (no)
typedef NInf<5> Inf;      //Infinity      (ni)
typedef NMnk<5> Mnk;      //E Plane       (noni)
typedef NPss<5> Pss;      //PseudoScalar  (e123noni)

typedef NPnt<5> Pnt; 	    //Point         (e1, e2, e3, no, ni)  
typedef Pnt Dls; 		      //Dual Sphere   (e1, e2, e3, no, ni) 

typedef NPar<5> Par;	    //Point Pair    (e12,e13,e23,e1no,e2no,e3no,e1ni,e2ni,e3ni,noni)
typedef NCir<5> Cir;	    //Circle        
typedef NSph<5> Sph;	    //Sphere          
                    	
typedef NDrv<5> Drv;	    //Direction Vector
typedef NTnv<5> Tnv;	    //Tangent Vector   
typedef NDrb<5> Drb;     //Direction Bivector
typedef NTnb<5> Tnb;     //Tangent Bivector  
typedef NDrt<5> Drt;     //Direction Trivector
typedef NTnt<5> Tnt;     //Tangent Trivector  
                    	
typedef NDll<5> Dll;     //Dual Line        
typedef NLin<5> Lin;     //Dual Line    
typedef NFlp<5> Flp;     //Flat Plane
typedef NPln<5> Pln;     //Plane 
typedef NDlp<5> Dlp;     //Plane   
                        
typedef NTrs<5> Trs;     //Translator 
typedef NMot<5> Mot;     //Motor 
typedef NTrv<5> Trv;     //Transversor 
typedef NBst<5> Bst;     //Boost 
typedef NDil<5> Dil;     //Dilator 
typedef NTsd<5> Tsd;     //Translated Dilator      



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

  }//cga3D::

} // vsr:: 


#endif
