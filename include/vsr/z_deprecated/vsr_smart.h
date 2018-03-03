/*
 * =====================================================================================
 *
 *       Filename:  vsr_smart.h
 *
 *    Description:  smart objects and such
 *
 *        Version:  1.0
 *        Created:  11/15/2013 13:23:36
 *       Revision:  none
 *       Compiler:  clang3.2 or gcc4.7
 *
 *         Author:  Pablo Colapinto, wolftype (gmail)
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  vsr_smart_INC
#define  vsr_smart_INC

namespace vsr{

  /*
   * =====================================================================================
   *        Class:  SmartObj
   *  Description:  IS a data type and a pointer to some node on a graph
   *  as well as a list of listeners it can register with?
   * =====================================================================================
   */

   template< class T >
   struct SmartObj : public T {
      
      template< class ANY >
      SmartObj(const ANY& p = ANY()) : T(p) {}
      
      void * node = NULL;
   };

}

#endif   /* ----- #ifndef vsr_smart_INC  ----- */
