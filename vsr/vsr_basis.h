/*!  @File
 *   bit operations on basis blades (compile-time or run-time)
 *   a compile-time version of the bit representation of basis blades 
 *   method introduced by daniel fontijne
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
  

/*!-----------------------------------------------------------------------------
 *  CONSTEXPR BIT MANIPULATIONS
 *-----------------------------------------------------------------------------*/
namespace Bits {
/// BLADE BIT REPRESENTATION TYPE (16 DIMENSIONS is plenty since they can be nested at 8. . .)
typedef short Type; 

constexpr Type blade(){ return 0; }

/// Make bit representation of a blade from a list of unsigned integers
template<class X, class...XS>
constexpr Type blade( X x, XS ... xs){
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
constexpr Type grade (Type a, Type c = 0){
  return a > 0 ? ( a & 1 ? grade( a >> 1, c + 1) :  grade( a >> 1, c ) ) : c;
}

/// Highest Dimension Represented by b (find leftmost bit)
constexpr Type dimOf( Type b, Type dim=0 ){
  return (b > 0) ? dimOf( b >> 1, dim+1 ) : dim; 
}

/// Whether to Flip Signs when multiplying two blades a and b
constexpr bool signFlip(Type a, Type b, int c = 0){
  return (a >> 1 ) > 0 ? ( signFlip( a >> 1 , b, c + grade( ( a >> 1 ) & b ) ) ) : ( ( c & 1 ) ? true : false ); 
}   

/// Whether left contraction inner product is legit
constexpr bool inner(Type a, Type b) {
  return !( ( grade(a) > grade(b) ) || ( grade( a ^ b ) != ( grade(b) - grade(a) )  ) );
}

/// Whether outer product is legit
constexpr bool outer( Type a, Type b){
  return !( a & b );
} 

/// Product is just an xor!
constexpr Type product(Type a, Type b){
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
constexpr bool reverse(Type a){
  return cpow( -1, (grade(a) * (grade(a)-1) / 2.0) ) == -1;
}
/// Whether involution causes a sign flip
constexpr bool involute(Type a){
  return cpow( -1, grade(a) ) == -1;
}
/// Whether conjugation causes a sign flip
constexpr bool conjugate(Type a){
  return cpow( -1,(grade(a) * (grade(a)+1) / 2.0) ) == -1;   
}


/*-----------------------------------------------------------------------------
 *  COMPARISON FOR SORTING ALGORITHMS
 *-----------------------------------------------------------------------------*/
 /// Compare two blades and order them based on grade and value 
template <Type A, Type B>
constexpr bool compare(){
    return ( grade(A) == grade(B) ) ?  A < B : grade(A) < grade(B); //return A < B::HEAD;
}

//pseudoscalr of dimension dim
constexpr Type pss(Type dim){
  return ( dim == 0 ) ? 0 : (1 << (dim-1)) | pss( dim-1 ); 
}

//get origin vec of dimension dim 
template< Type dim >
constexpr Type origin(){
	return (1<<(dim-2));
}  
//get inty vec of dimension dim 
template< Type dim > 
constexpr Type infinity(){
	return (1<<(dim-1));
} 
//get EP vec of dimension dim 
template< Type dim >  
constexpr Type EP(){
	return (1<<(dim-2));
}  
//get EM vec of dimension dim  
template< Type dim > 
constexpr Type EM(){
	return (1<<(dim-1));
}
//get basis of eplane in dimension dim 
template< Type dim > 
constexpr Type eplane(){
	return (1 << (dim-1)) | ( 1 << (dim-2) ); 
}
//check for split
template< Type a, Type dim >    
constexpr bool checkMink() {
	return ( (a & eplane<dim>()) == 0) || ( ( a & eplane<dim>()) == eplane<dim>() );
}  
} //Bits:: 

} //vsr::

#endif
