/*
 * =====================================================================================
 *
 *       Filename:  vsr_map.h
 *
 *    Description:  map a function over a set  see also topo
 *
 *        Version:  1.0
 *        Created:  03/02/2015 18:20:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  vsr_map_INC
#define  vsr_map_INC

#include "vsr_generic_op.h"

namespace vsr {

  namespace functor {


    /*!
     *  \brief  evaluate lambda iter times for each member of input
     */
      template<class T, class S, class Lambda>
      vector<T> eval(const vector<S>& in, Lambda func, int iter, float range=1.0){
        vector<T> out;
        for(auto& c : in){
          for (int j=0;j<iter;++j){
            float t = range*(float)j/iter;
            out.push_back( func(c,t) );
          }
        }
        return out;
      }

  }

}//vsr::
#endif   /* ----- #ifndef vsr_map_INC  ----- */
