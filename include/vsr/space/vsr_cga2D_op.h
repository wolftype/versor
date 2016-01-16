/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga2D_op.h
 *
 *    Description:  cga2d specific operations
 *
 *        Version:  1.0
 *        Created:  01/15/2016 21:49:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#ifndef  vsr_cga2D_op_INC
#define  vsr_cga2D_op_INC

#include "vsr_cga2D_types.h"

namespace vsr { namespace cga2D {


struct Construct {

   template<class V>
   static Point point(const V& v){
     return nga::Round::null( v[0], v[1]  );
   }

   static Point point(VSR_PRECISION x, VSR_PRECISION y){
     return nga::Round::null(x,y);
   }
 
   static Point circle(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION rad){
     return nga::Round::dls( Vec(x,y), rad ); 
   }

   template<class V>
   static Point circle(const V& v, VSR_PRECISION rad){
     return nga::Round::dls( Vec(v[0],v[1]), rad ); 
   }
   
   static Pair meet( const Line& lin, const Circle& cir){
     return ( lin.dual() ^ cir.dual() ).dual();
   }
};

}} //vsr::cga2d::

#endif   /* ----- #ifndef vsr_cga2D_op_INC  ----- */
