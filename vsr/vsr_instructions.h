#ifndef INSTRUCTIONS_H_INCLUDED
#define INSTRUCTIONS_H_INCLUDED

#include "vsr_basis.h"

namespace vsr{

template<bool F, TT A, TT B, int IDXA, int IDXB>
struct Inst{
	static const TT Res = A ^ B; 
	static const bool IP = inner(A,B);
	static const bool OP = outer(A,B);  
	static const int idxA = IDXA;
	static const int idxB = IDXB;
	
	template<class TA, class TB>
	static constexpr VT Exec( const TA& a, const TB& b){
		return a[idxA] * b[idxB];
	} 
	
	static void print(){
		printf("%d =  a[%d] * b[%d]\t", Res, idxA, idxB);
	}  
};   

template<TT A, TT B, int IDXA, int IDXB>
struct Inst<true, A,B,IDXA,IDXB>{
	static const TT Res = A ^ B; 
	static const bool IP = inner(A,B);
	static const bool OP = outer(A,B);  
	static const int idxA = IDXA;
	static const int idxB = IDXB;
	
	template<class TA, class TB>
	static constexpr VT Exec( const TA& a, const TB& b){
		return -a[idxA] * b[idxB];
	} 
	
	static void print(){
		printf("%d =  -a[%d] * b[%d]\t", Res, idxA, idxB);
	}  
}; 

//ok, like Inst but without the product calculations inside it
template<bool F, TT R, int IDXA, int IDXB>
struct Instruct{
	static const TT Res = R;
	static const int idxA = IDXA;
	static const int idxB = IDXB;
	template<class TA, class TB>
	static constexpr VT Exec( const TA& a, const TB& b){
		return a[idxA] * b[idxB];
	} 
	static void print(){
		printf("%d =  a[%d] * b[%d]\t", Res, idxA, idxB);
	}
}; 
template< TT R, int IDXA, int IDXB>
struct Instruct<true, R, IDXA, IDXB>{
	static const TT Res = R;
	static const int idxA = IDXA;
	static const int idxB = IDXB;
	template<class TA, class TB>
	static constexpr VT Exec( const TA& a, const TB& b){
		return -a[idxA] * b[idxB];
	} 
	static void print(){
		printf("%d =  -a[%d] * b[%d]\t", Res, idxA, idxB);
	}
};

template<bool F, int IDX>
struct InstFlip{
	template<class TA>
	static constexpr VT Exec(const TA& a){
		return a[IDX];
	}
};    
template<int IDX>
struct InstFlip<true, IDX>{
	template<class TA>
	static constexpr VT Exec(const TA& a){
		return -a[IDX];
	}
};

template<int IDX>
struct InstCast{ 
		template<class TA> 
   	static constexpr VT Exec(const TA& a){
		return a[IDX];
	} 
};
template<>
struct InstCast<-1>{       
		template<class TA> 
    static constexpr VT Exec(const TA& a){
		return 0;
	} 
};

}

#endif