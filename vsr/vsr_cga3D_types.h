#ifndef VSR_CGA3D_TYPES_INCLUDED
#define VSR_CGA3D_TYPES_INCLUDED

#include "vsr_products.h"

namespace vsr{

//3D Euclidean
// typedef NESca<3> Sca; 
// typedef NEVec<3> Vec; 
// typedef NEBiv<3> Biv; 
// typedef NETri<3> Tri; 
// typedef NERot<3> Rot;
// typedef NEe<1> e1; 
// typedef NEe<2> e2; 
// typedef NEe<3> e3;



//3D CONFORMAL 
typedef Ne<5,1> e1; 
typedef Ne<5,2> e2; 
typedef Ne<5,3> e3; 

typedef NSca<5> Sca; 
typedef NVec<5> Vec; 
typedef NVec2D<5> Vec2D; 
typedef NBiv<5> Biv; 
typedef NTri<5> Tri; 
typedef NRot<5> Rot;

typedef NOri<5> Ori; //Origin
typedef NInf<5> Inf; //Infinity
typedef NMnk<5> Mnk; //E Plane
typedef NPss<5> Pss; //E Plane

typedef NPnt<5> Pnt; 	 //Homogenous Point in 3D  
typedef Pnt Dls; 		 //Dual Sphere    

typedef NPar<5> Par;	 //Point Pair
typedef NCir<5> Cir;	 //Circle
typedef NSph<5> Sph;	 //Sphere
                    	
typedef NDrv<5> Drv;	 //Direction Vector
typedef NTnv<5> Tnv;	 //Tangent Vector   
typedef NDrb<5> Drb;	 //Direction Bivector
typedef NTnb<5> Tnb;	 //Tangent Bivector  
typedef NDrt<5> Drt;	 //Direction Trivector
typedef NTnt<5> Tnt;	 //Tangent Trivector  
                    	
typedef NDll<5> Dll;	 //Dual Line        
typedef NLin<5> Lin;	 //Dual Line    
typedef NFlp<5> Flp;	 //Flat Plane
typedef NPln<5> Pln;	 //Plane 
typedef NDlp<5> Dlp;	 //Plane   
                    	
typedef NTrs<5> Trs;	 //Translator 
typedef NMot<5> Mot;	 //Motor 
typedef NTrv<5> Trv;	 //Transversor 
typedef NBst<5> Bst;	 //Boost 
typedef NDil<5> Dil;	 //Dilator 
typedef NTsd<5> Tsd;	 //Translated Dilator      




} //  


#endif