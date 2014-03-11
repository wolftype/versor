/*
 * =====================================================================================
 *
 *       Filename:  vsr_generic_instructions.h
 *
 *    Description:  generic instructions (defined over reals or over other metric spaces)
 *
 *        Version:  1.0
 *        Created:  03/10/2014 12:25:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef GENERIC_INSTRUCTIONS_H_INCLUDED
#define GENERIC_INSTRUCTIONS_H_INCLUDED

#include "vsr_basis.h"

namespace vsr{


/*-----------------------------------------------------------------------------
 *  BINARY OPERATOR INSTRUCTIONS
 *-----------------------------------------------------------------------------*/
template<bool F, TT A, TT B, int IDXA, int IDXB>
struct Inst{
  static const TT Res = A ^ B; 
  static const bool IP = inner(A,B);
  static const bool OP = outer(A,B);  
  static const int idxA = IDXA;
  static const int idxB = IDXB;
  
  template<class TA, class TB>
  static constexpr auto Exec( const TA& a, const TB& b) RETURNS (
    a[idxA] * b[idxB] //.template get<idxA>() * b.template get<idxB>(
  )
  
  static void print(){
    printf(" a[%d] * b[%d] /*%s*/\t", idxA, idxB, estring(Res).c_str() );
  }  
};   

//sign flip version
template<TT A, TT B, int IDXA, int IDXB>
struct Inst<true, A,B,IDXA,IDXB>{
  static const TT Res = A ^ B; 
  static const bool IP = inner(A,B);
  static const bool OP = outer(A,B);  
  static const int idxA = IDXA;
  static const int idxB = IDXB;
  
  template<class TA, class TB>
  static constexpr auto Exec( const TA& a, const TB& b) RETURNS (
    -a[idxA] * b[idxB] //(-a.template get<idxA>() * b.template get<idxB>())
  )
  
  static void print(){
    printf("-a[%d] * b[%d] /*%s*/\t",idxA, idxB,  estring(Res).c_str());
  }  
}; 

//Like above but without the product calculations inside it (for CGA)
template<bool F, TT R, int IDXA, int IDXB>
struct Instruct{
 
  static const TT Res = R;
  static const int idxA = IDXA;
  static const int idxB = IDXB;
  template<class TA, class TB>
  
  static constexpr auto Exec( const TA& a, const TB& b) RETURNS(
    (a[idxA] * b[idxB])
  )

  static void print(){
    printf(" a[%d] * b[%d] /*%s*/\t", idxA, idxB, estring(Res).c_str());
  }
}; 
template< TT R, int IDXA, int IDXB>
struct Instruct<true, R, IDXA, IDXB>{
  static const TT Res = R;
  static const int idxA = IDXA;
  static const int idxB = IDXB;
  template<class TA, class TB>
  
  static constexpr auto Exec( const TA& a, const TB& b) RETURNS (
    (-a[idxA] * b[idxB])
  )

  static void print(){
    printf("-a[%d] * b[%d] /*%s*/\t", idxA, idxB,  estring(Res).c_str());
  }
};


/*-----------------------------------------------------------------------------
 *  UNARY OPERATOR INSTRUCTIONS
 *-----------------------------------------------------------------------------*/
template<bool F, int IDX>
struct InstFlip{
  template<class TA>
  static constexpr auto Exec(const TA& a) RETURNS ( a[IDX] ) //(a.template get<IDX>()) )
};    

template<int IDX>
struct InstFlip<true, IDX>{
  template<class TA>
  static constexpr auto Exec(const TA& a) RETURNS ( -a[IDX] )//(-a.template get<IDX>()) )
};



/*-----------------------------------------------------------------------------
 *  CASTING
 *-----------------------------------------------------------------------------*/
//casting will be wonky (but already is)
template<int IDX>
struct InstCast{ 
    template<class TA> 
     static constexpr auto Exec(const TA& a) RETURNS (
      a[IDX]//a.template get<IDX>()
    )
};
template<>
struct InstCast<-1>{       
    template<class TA> 
    static constexpr double Exec(const TA& a){
    return 0;
  } 
};

}

#endif
