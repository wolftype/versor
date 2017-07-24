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

#include <vsr/space/vsr_cga2D_types.h>

namespace vsr { namespace cga2D {


struct Construct {

   template<class V>
   static Point point(const V& v){
     return nga::Round::null( v[0], v[1]  );
   }

   static Point point(VSR_PRECISION x, VSR_PRECISION y){
     return nga::Round::null(x,y);
   }

   //Dual Circle at x, y with radius r
   static Point circle(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION rad){
     return nga::Round::dls( Vec(x,y), rad );
   }

   //Dual Circle at v with radius r
   template<class V>
   static Point circle(const V& v, VSR_PRECISION r){
     return nga::Round::dls( Vec(v[0],v[1]), r );
   }

   //Dual Circle at v through p
   template<class V>
   static Point circle(const V& v, const Point& p){
     return nga::Round::at( point(v[0],v[1]), p);
   }

   static Pair meet( const Line& lin, const Circle& cir){
     return ( lin.dual() ^ cir.dual() ).undual();
   }
};


}} //vsr::cga2d::

#endif   /* ----- #ifndef vsr_cga2D_op_INC  ----- */
