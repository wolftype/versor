
/*! @file 
    
      arrow tokens (deep implementation detail)

 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 *  homepage: versor.mat.ucsb.edu
 *
 * */

#ifndef INSTRUCTIONS_H_INCLUDED
#define INSTRUCTIONS_H_INCLUDED

#include "vsr_basis.h"

namespace vsr{

/** The value type of data (default is double) */
#define VSR_PRECISION double

/*-----------------------------------------------------------------------------
 *  Lowest level product for when to Flip product, calculate inner, or calculate outer
 *-----------------------------------------------------------------------------*/
template<bits::type A, bits::type B>
struct product {
  static const bits::type result = A ^ B; 
  static const bool is_positive = !bits::signFlip(A,B);
  static const bool has_inner = bits::inner(A,B);
  static const bool has_outer = bits::outer(A,B);  
};

/*-----------------------------------------------------------------------------
 *  use of product to calculate value
 *-----------------------------------------------------------------------------*/
template<bool positive, int idxA, int idxB>
struct binary_function_token {
  template<class TA, class TB>
  static constexpr typename TA::value_t Exec(const TA& a, const TB& b){
    return a[idxA] * b[idxB] * (positive ? 1 : -1);
  }
  static void print(){
    printf(" %s a[%d] * b[%d] \t", positive?"":"-", idxA, idxB );
  }    
};

//template<bool positive, int idxA, int idxB> 
//struct print< binary_function_token<positive, idxA, idxB> >{
//  static void print(){
//    printf(" a[%d] * b[%d] /*%s*/\t", idxA, idxB, bits::estring(product::result).c_str() );
//  }      
//};

template<bool positive, int idx>
struct unary_function_token {
  template<class TA>
  static constexpr typename TA::value_t Exec(const TA& a){
    return a[idx] * (positive ? 1 : -1);
  }
   static void print(){
    printf("%s\ta[%d]\t",(positive ? "" :"-") ,idx);
  }
};


/*-----------------------------------------------------------------------------
 *  Single Word of an Instruction -- multiples a[IDXA] by b[IDXB]
 *-----------------------------------------------------------------------------*/
template<bool Flip, int IDXA, int IDXB>
struct Instruction{
  static const int Sign = Flip ? -1 : 1;
 
  template<class TA, class TB>
  static constexpr typename TA::algebra::value_t Exec( const TA& a, const TB& b){
    return Sign * a[IDXA] * b[IDXB];
  } 
  
  static void print(){
    printf("%d * a[%d] * b[%d]", Sign, IDXA, IDXB );
  }  
}; 


template<bool F, bits::type A, bits::type B, int IDXA, int IDXB>
struct Inst{
  static const bits::type Res = A ^ B; 
  static const bool IP = bits::inner(A,B);
  static const bool OP = bits::outer(A,B);  
 
  template<class TA, class TB>
  static constexpr typename TA::algebra::value_t Exec( const TA& a, const TB& b){
    return a[IDXA] * b[IDXB];
  } 
  
  static void print(){
    printf(" a[%d] * b[%d] /*%s*/\t", IDXA, IDXB, bits::estring(Res).c_str() );
  }  
};   

template<bits::type A, bits::type B, int IDXA, int IDXB>
struct Inst<true, A,B,IDXA,IDXB>{
  static const bits::type Res = A ^ B; 
  static const bool IP = bits::inner(A,B);
  static const bool OP = bits::outer(A,B);  
 
  template<class TA, class TB>
  static constexpr typename TA::algebra::value_t Exec( const TA& a, const TB& b){
    return -a[IDXA] * b[IDXB];
  } 
  
  static void print(){
    printf("-a[%d] * b[%d] /*%s*/\t",IDXA, IDXB,  bits::estring(Res).c_str());
  }  
}; 

//ok, like Inst but without the product calculations inside it (assumes RULE has been invoked)
template<bool F, bits::type R, int IDXA, int IDXB>
struct Instruct{
  static const bits::type Res = R;
  static const int idxA = IDXA;
  static const int idxB = IDXB;

  template<class TA, class TB>
  static constexpr typename TA::algebra::value_t Exec( const TA& a, const TB& b){
    return a[idxA] * b[idxB];
  } 
  static void print(){
    printf(" a[%d] * b[%d] /*%s*/\t", idxA, idxB, bits::estring(Res).c_str());
  }
}; 
template< bits::type R, int IDXA, int IDXB>
struct Instruct<true, R, IDXA, IDXB>{
  static const bits::type Res = R;
  static const int idxA = IDXA;
  static const int idxB = IDXB;
  
  template<class TA, class TB>
  static constexpr typename TA::algebra::value_t Exec( const TA& a, const TB& b){
    return -a[idxA] * b[idxB];
  } 
  static void print(){
    printf("-a[%d] * b[%d] /*%s*/\t", idxA, idxB,  bits::estring(Res).c_str());
  }
};


/*-----------------------------------------------------------------------------
 *  Single Token of a Sign Flip Instruction
 *-----------------------------------------------------------------------------*/
template<bool F, int IDX>
struct InstFlip{
  template<class TA>
  static constexpr typename TA::algebra::value_t Exec(const TA& a){
    return a[IDX];
  }  
  static void print(){
    printf("a[%d]\t",IDX);
  }

};    
template<int IDX>
struct InstFlip<true, IDX>{
  template<class TA>
  static constexpr typename TA::algebra::value_t Exec(const TA& a){
    return -a[IDX];
  }
  static void print(){
    printf("-a[%d]\t",IDX);
  }
};


/*-----------------------------------------------------------------------------
 *  Single Token of a Casting Instruction
 *-----------------------------------------------------------------------------*/
template<int IDX>
struct InstCast{ 
    template<class TA> 
     static constexpr typename TA::algebra::value_t Exec(const TA& a){
    return a[IDX];
  } 
};
template<>
struct InstCast<-1>{       
    template<class TA> 
    static constexpr typename TA::algebra::value_t Exec(const TA& a){
    return 0;
  } 
};

}

#endif
