#ifndef BASIS_H_INCLUDED
#define BASIS_H_INCLUDED

#include <stdio.h>  
#include <bitset>  
#include <string> 

namespace vsr{   
	
using namespace std;
	

typedef double VT; 
typedef short TT;  

constexpr TT blade(){ return 0; }

template<class X, class...XS>
constexpr TT blade( X x, XS ... xs){
	return x | blade(xs...);  
}


template<int N>
inline string bitString( int b ){
	bitset<N> tmp = b;
	return tmp.template to_string<char,char_traits<char>,allocator<char> >();  
}

inline void bsprint(int x){
	printf("%s\n", bitString<6>(x).c_str() );
}
	
constexpr TT grade (TT a, TT c = 0){
	return a > 0 ? ( a & 1 ? grade( a >> 1, c + 1) :  grade( a >> 1, c ) ) : c;
}

constexpr bool signFlip(TT a, TT b, int c = 0){
	return (a >> 1 ) > 0 ? ( signFlip( a >> 1 , b, c + grade( ( a >> 1 ) & b ) ) ) : ( ( c & 1 ) ? true : false ); 
}   

constexpr bool inner(TT a, TT b) {
	return !( ( grade(a) > grade(b) ) || ( grade( a ^ b ) != ( grade(b) - grade(a) )  ) );
}

constexpr bool outer( TT a, TT b){
	return !( a & b );
} 

constexpr TT product(TT a, TT b){
	return a ^ b;
}   

constexpr int cpow(int x, int N)
{
    return (N>0) ? (x*cpow(x, N-1)) : (1.0);
}

constexpr bool reverse(TT a){
	return cpow( -1, (grade(a) * (grade(a)-1) / 2.0) ) == -1;
}
constexpr bool involute(TT a){
	return cpow( -1, grade(a) ) == -1;
}
constexpr bool conjugate(TT a){
	return cpow( -1,(grade(a) * (grade(a)+1) / 2.0) ) == -1;   
}

template <TT A, TT B>
constexpr bool compare(){
		return ( grade(A) == grade(B) ) ?  A < B: grade(A) < grade(B); //return A < B::HEAD;
}

//pss
// template< TT dim >   
constexpr TT pss(TT dim){
	return ( dim == 0 ) ? 0 : (1 << (dim-1)) | pss( dim-1 ); 
}

} //vsr::

#endif