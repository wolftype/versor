/*!  @file
 *   bit operations on basis blades (compile-time or run-time)
 *   
 *   a compile-time version of the bit representation of basis blades 
 *   (bit representation method introduced by daniel fontijne)
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


/*!-----------------------------------------------------------------------------
 * the **versor** namespace 
 * 
 *-----------------------------------------------------------------------------*/
namespace vsr{ 
  
  /*!-----------------------------------------------------------------------------
   *  constexpr bit manipulations 
   *
   *-----------------------------------------------------------------------------*/
  namespace bits {
  
using namespace std;
  
/*-----------------------------------------------------------------------------
 *  PRINTING
 *-----------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------
 *  MAKE BIT REPRESENTATIONS
 *-----------------------------------------------------------------------------*/

/// BLADE BIT REPRESENTATION TYPE (16 DIMENSIONS is plenty since they can be nested at 8. . .)
typedef short type; 

constexpr type blade(){ return 0; }

/// Make bit representation of a blade from a list of shorts 
template<class X, class...XS>
constexpr type blade( X x, XS ... xs){
  return x | blade(xs...);  
}

/// Grade of blade (count number of "on" bits)
constexpr type grade(type a, type c = 0){
  return a > 0 ? 
  ( a & 1 ? grade( a >> 1, c + 1) : grade( a >> 1, c ) ) : c;
}

/// Highest Dimension Represented by blade (finds leftmost bit)
constexpr type dimOf( type b, type dim=0 ){
  return (b > 0) ? dimOf( b >> 1, dim+1 ) : dim; 
}

/// Flip Check when multiplying two blades a and b
constexpr bool signFlip(type a, type b, int c = 0){
  return (a >> 1 ) > 0 ? 
  ( signFlip( a >> 1 , b, c + grade( ( a >> 1 ) & b ) ) ) : 
  ( ( c & 1 ) ? true : false ); 
}   

/// Inner left contraction check if is legit
constexpr bool inner(type a, type b) {
  return !( ( grade(a) > grade(b) ) || ( grade( a ^ b ) != ( grade(b) - grade(a) )  ) );
}

/// Outer product check is legit
constexpr bool outer( type a, type b){
  return !( a & b );
} 

/// Product is xor
constexpr type product(type a, type b){
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
constexpr bool reverse(type a){
  return cpow( -1, (grade(a) * (grade(a)-1) / 2.0) ) == -1;
}
/// Whether involution causes a sign flip
constexpr bool involute(type a){
  return cpow( -1, grade(a) ) == -1;
}
/// Whether conjugation causes a sign flip
constexpr bool conjugate(type a){
  return cpow( -1,(grade(a) * (grade(a)+1) / 2.0) ) == -1;   
}


/*-----------------------------------------------------------------------------
 *  COMPARISON FOR SORTING ALGORITHMS
 *-----------------------------------------------------------------------------*/
 /// Compare two blades and order them based on grade and value 
template <type A, type B>
constexpr bool compare(){
    return ( grade(A) == grade(B) ) ?  A < B : grade(A) < grade(B); //return A < B::HEAD;
}

constexpr bool lessThan(type A, type B){
    return ( grade(A) == grade(B) ) ?  A < B : grade(A) < grade(B); //return A < B::HEAD;
}
template <type A, type B>
constexpr bool greaterThan(){
    return ( grade(A) == grade(B) ) ?  A > B : grade(A) > grade(B); //return A < B::HEAD;
}


/*-----------------------------------------------------------------------------
 *  BLADE UTILITIES
 *-----------------------------------------------------------------------------*/
/// pseudoscalar of dimension dim
constexpr type pss(type dim){
  return ( dim == 0 ) ? 0 : (1 << (dim-1)) | pss( dim-1 ); 
}

/// origin vec of dimension dim 
template< type dim >
constexpr type origin(){
	return (1<<(dim-2));
}  

/// infinity blade of dimension dim 
template< type dim > 
constexpr type infinity(){
	return (1<<(dim-1));
} 

/// EP blade of dimension dim 
template< type dim >  
constexpr type EP(){
	return (1<<(dim-2));
}  

/// EM blade of dimension dim  
template< type dim > 
constexpr type EM(){
	return (1<<(dim-1));
}

/// E plane blade in dimension dim 
template< type dim > 
constexpr type eplane(){
	return (1 << (dim-1)) | ( 1 << (dim-2) ); 
}

/// check for split
template< type a, type dim >    
constexpr bool checkMink() {
	return ( (a & eplane<dim>()) == 0) || ( ( a & eplane<dim>()) == eplane<dim>() );
}  

} //bits:: 


} //vsr::

#endif
