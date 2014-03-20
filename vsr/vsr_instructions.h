#ifndef INSTRUCTIONS_H_INCLUDED
#define INSTRUCTIONS_H_INCLUDED

#include "vsr_basis.h"

namespace vsr{

template<bool F, Bits::Type A, Bits::Type B, int IDXA, int IDXB>
struct Inst{
  static const Bits::Type Res = A ^ B; 
  static const bool IP = Bits::inner(A,B);
  static const bool OP = Bits::outer(A,B);  
  static const int idxA = IDXA;
  static const int idxB = IDXB;
  
  template<class TA, class TB>
  static constexpr typename TA::ValueType Exec( const TA& a, const TB& b){
    return a[idxA] * b[idxB];
  } 
  
  static void print(){
    printf(" a[%d] * b[%d] /*%s*/\t", idxA, idxB, Bits::estring(Res).c_str() );
  }  
};   

template<Bits::Type A, Bits::Type B, int IDXA, int IDXB>
struct Inst<true, A,B,IDXA,IDXB>{
  static const Bits::Type Res = A ^ B; 
  static const bool IP = Bits::inner(A,B);
  static const bool OP = Bits::outer(A,B);  
  static const int idxA = IDXA;
  static const int idxB = IDXB;
  
  template<class TA, class TB>
  static constexpr typename TA::ValueType Exec( const TA& a, const TB& b){
    return -a[idxA] * b[idxB];
  } 
  
  static void print(){
    printf("-a[%d] * b[%d] /*%s*/\t",idxA, idxB,  Bits::estring(Res).c_str());
  }  
}; 

//ok, like Inst but without the product calculations inside it
template<bool F, Bits::Type R, int IDXA, int IDXB>
struct Instruct{
  static const Bits::Type Res = R;
  static const int idxA = IDXA;
  static const int idxB = IDXB;
  template<class TA, class TB>
  static constexpr typename TA::ValueType Exec( const TA& a, const TB& b){
    return a[idxA] * b[idxB];
  } 
  static void print(){
    printf(" a[%d] * b[%d] /*%s*/\t", idxA, idxB, Bits::estring(Res).c_str());
  }
}; 
template< Bits::Type R, int IDXA, int IDXB>
struct Instruct<true, R, IDXA, IDXB>{
  static const Bits::Type Res = R;
  static const int idxA = IDXA;
  static const int idxB = IDXB;
  template<class TA, class TB>
  static constexpr typename TA::ValueType Exec( const TA& a, const TB& b){
    return -a[idxA] * b[idxB];
  } 
  static void print(){
    printf("-a[%d] * b[%d] /*%s*/\t", idxA, idxB,  Bits::estring(Res).c_str());
  }
};

template<bool F, int IDX>
struct InstFlip{
  template<class TA>
  static constexpr typename TA::ValueType Exec(const TA& a){
    return a[IDX];
  }  
  static void print(){
    printf("a[%d]\t",IDX);
  }

};    
template<int IDX>
struct InstFlip<true, IDX>{
  template<class TA>
  static constexpr typename TA::ValueType Exec(const TA& a){
    return -a[IDX];
  }
  static void print(){
    printf("-a[%d]\t",IDX);
  }
};

template<int IDX>
struct InstCast{ 
    template<class TA> 
     static constexpr typename TA::ValueType Exec(const TA& a){
    return a[IDX];
  } 
};
template<>
struct InstCast<-1>{       
    template<class TA> 
    static constexpr typename TA::ValueType Exec(const TA& a){
    return 0;
  } 
};

}

#endif
