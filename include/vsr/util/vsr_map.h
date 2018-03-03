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

#include "detail/vsr_generic_op.h"

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

     /*!
     *  \brief  evaluate lambda for each member of input (with optional args)
     */
      template<class T, class Lambda, class S, class ... Args>
      vector<T> fmap(Lambda func, const vector<S>& in, Args ... args){
        vector<T> out;
        for(auto& c : in){
          out.push_back( func(c,args...) );
        }
        return out;
      }


  } //functor::

}//vsr::
#endif   /* ----- #ifndef vsr_map_INC  ----- */
