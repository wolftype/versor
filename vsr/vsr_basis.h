/*!  @File
 *   bit operations on basis blades (compile-time or run-time)
 *   a compile-time version of the method introduced daniel fontijne
 *  
 *  VERSOR
 *
 *  Author: Pablo Colapinto
 *  Gmail:  wolftype
 *
 *  homepage: versor.mat.ucsb.edu
 *
 * */


#ifndef BASIS_H_INCLUDED
#define BASIS_H_INCLUDED

#include <stdio.h>  
#include <bitset>  
#include <string>
#include <sstream> 

namespace vsr{   
  
using namespace std;
  

/// BLADE BIT REPRESENTATION TYPE (16 DIMENSIONS is plenty since they can be nested at 8. . .)
typedef short TT; 

constexpr TT blade(){ return 0; }

/// Make bit representation of a blade from a list of unsigned integers
template<class X, class...XS>
constexpr TT blade( X x, XS ... xs){
  return x | blade(xs...);  
}


/// Convert blade to string of 0s and 1s
template<int N>
inline string bitString( int b ){
  bitset<N> tmp = b;
  return tmp.template to_string<char,char_traits<char>,allocator<char> >();  
}

/// Default bitstring 
inline void bsprint(int x){
  printf("%s\n", bitString<6>(x).c_str() );
}

/// Convert blade to "e1.." string 
inline string estring(int x){
  if ( x == 0 ){  return "s"; }
  stringstream s; s << "e";
  int i = 1;
  while ( x != 0){
      if (x&1) s << i;
      x = x >> 1;
      i++;
  }
  return s.str();
}

inline void beprint(int x){
 printf("%s\t", estring(x).c_str() ); 
 }

/// Get grade of blade (count number of "on" bits)
constexpr TT grade (TT a, TT c = 0){
  return a > 0 ? ( a & 1 ? grade( a >> 1, c + 1) :  grade( a >> 1, c ) ) : c;
}

/// Highest Dimension Represented by b (find leftmost bit)
constexpr TT dimOf( TT b, TT dim=0 ){
  return (b > 0) ? dimOf( b >> 1, dim+1 ) : dim; 
}

/// Whether to Flip Signs when multiplying two blades a and b
constexpr bool signFlip(TT a, TT b, int c = 0){
  return (a >> 1 ) > 0 ? ( signFlip( a >> 1 , b, c + grade( ( a >> 1 ) & b ) ) ) : ( ( c & 1 ) ? true : false ); 
}   

/// Whether left contraction inner product is legit
constexpr bool inner(TT a, TT b) {
  return !( ( grade(a) > grade(b) ) || ( grade( a ^ b ) != ( grade(b) - grade(a) )  ) );
}

/// Whether outer product is legit
constexpr bool outer( TT a, TT b){
  return !( a & b );
} 

/// Product is just an xor!
constexpr TT product(TT a, TT b){
  return a ^ b;
}   

/// Utitilty for raising x to the Nth power at compile-time
constexpr int cpow(int x, int N)
{
    return (N>0) ? (x*cpow(x, N-1)) : (1.0);
}


/*-----------------------------------------------------------------------------
 *  SIGN FLIP BOOLEANS
 *-----------------------------------------------------------------------------*/

/// Whether reversion causes a sign flip
constexpr bool reverse(TT a){
  return cpow( -1, (grade(a) * (grade(a)-1) / 2.0) ) == -1;
}
/// Whether involution causes a sign flip
constexpr bool involute(TT a){
  return cpow( -1, grade(a) ) == -1;
}
/// Whether conjugation causes a sign flip
constexpr bool conjugate(TT a){
  return cpow( -1,(grade(a) * (grade(a)+1) / 2.0) ) == -1;   
}


/*-----------------------------------------------------------------------------
 *  COMPARISON FOR SORTING ALGORITHMS
 *-----------------------------------------------------------------------------*/
 /// Compare two blades and order them based on grade and value 
template <TT A, TT B>
constexpr bool compare(){
    return ( grade(A) == grade(B) ) ?  A < B : grade(A) < grade(B); //return A < B::HEAD;
}


constexpr TT _vsr_make_pss(TT dim){
  return ( dim == 0 ) ? 0 : (1 << (dim-1)) | _vsr_make_pss( dim-1 ); 
}

} //vsr::

#endif
