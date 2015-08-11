/*
 * =====================================================================================
 *
 *       Filename:  vsr_csg.h
 *
 *    Description:  some tools for constructive solid geometry (experimental)
 *
 *        Version:  1.0
 *        Created:  06/08/2015 18:15:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_cga3D_frame.h"
#include "space/vsr_cga3D_round.h"

namespace vsr { namespace cga {


  vector<Point> points( const Circle& k, int num, float theta=0, float phi=TWOPI){

    vector<Point> pnt;
    
    for (int i =0;i<num;++i){
      VSR_PRECISION t = theta + phi * (VSR_PRECISION) i/num;
      pnt.push_back( Construct::point(k,t) );  
    }

    return pnt;
  }


} } //vsr::cga::
