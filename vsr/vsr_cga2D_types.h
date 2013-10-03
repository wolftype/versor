#ifndef VSR_CGA3D_TYPES_INCLUDED
#define VSR_CGA3D_TYPES_INCLUDED

#include "vsr_products.h"
#include "vsr_generic_op.h"    

namespace vsr{


//2D CONFORMAL 
typedef Ne<4,1> e1; 
typedef Ne<4,2> e2; 
typedef Ne<4,3> e3; 

typedef NSca<4> Sca; 
typedef NVec<4> Vec; 
//typedef NVec2D<4> Vec2D; 
typedef NBiv<4> Biv; 
typedef NTri<4> Tri; 
typedef NRot<4> Rot;

typedef NOri<4> Ori; //Origin
typedef NInf<4> Inf; //Infinity
typedef NMnk<4> Mnk; //E Plane
typedef NPss<4> Pss; //E Plane

typedef NPnt<4> Pnt; 	 //Homogenous Point in 3D  
typedef Pnt Dls; 		 //Dual Sphere    

typedef NPar<4> Par;	 //Point Pair
typedef NCir<4> Cir;	 //Circle
typedef NSph<4> Sph;	 //Sphere
                    	
typedef NDrv<4> Drv;	 //Direction Vector
typedef NTnv<4> Tnv;	 //Tangent Vector   
typedef NDrb<4> Drb;	 //Direction Bivector
typedef NTnb<4> Tnb;	 //Tangent Bivector  
typedef NDrt<4> Drt;	 //Direction Trivector
typedef NTnt<4> Tnt;	 //Tangent Trivector  
                    	
typedef NDll<4> Dll;	 //Dual Line        
typedef NLin<4> Lin;	 //Dual Line    
typedef NFlp<4> Flp;	 //Flat Plane
typedef NPln<4> Pln;	 //Plane 
typedef NDlp<4> Dlp;	 //Plane   
                    	
typedef NTrs<4> Trs;	 //Translator 
typedef NMot<4> Mot;	 //Motor 
typedef NTrv<4> Trv;	 //Transversor 
typedef NBst<4> Bst;	 //Boost 
typedef NDil<4> Dil;	 //Dilator 
typedef NTsd<4> Tsd;	 //Translated Dilator   

//ADDITIONAL NON-GENERIC OPS  

Pnt Point(VT x, VT y){
	return Ro::null( Vec(x,y) );
}
Par Pair( const Pnt& a, const Pnt& b){
	return a ^ b;
}
Cir Circle(const Pnt& a, const Pnt& b, const Pnt& c){
	return a ^ b ^ c; 
}
Pnt Circle(VT x, VT y, VT rad){
	return Ro::dls( Vec(x,y), rad ); 
}


} //  


#endif