/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_cubicLattice.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/03/2013 12:51:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */
#include "form/vsr_cubicLattice.h"

namespace vsr{

    //  template class CubicLattice< NPnt<5> >;


      template<>
      void CubicLattice< NPnt<5> > :: initPoints(){
         for (int i = 0; i < mWidth; ++i){
            for (int j = 0; j < mHeight; ++j){
                for (int k = 0; k < mDepth; ++k){
                    int tidx = idx(i,j,k); 
                    mPoint[ tidx ]  = nga::Round::point(px(i),  py(j),  pz(k) ); 
                   // mPoint[tidx].print();
                }
            }
         }
      }

      template class CubicLattice< NPnt<5> >;


}

