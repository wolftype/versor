/*
 * =====================================================================================
 *
 *       Filename:  xMetricTensor.cpp
 *
 *    Description:  generic types (not just over real numbers . . .)
 *
 *        Version:  1.0
 *        Created:  03/10/2014 12:38:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vsr_types.h"
#include "vsr_xlists.h"
#include "vsr_generic_instructions.h"

using namespace vsr;

template<TT A, class B, int idxA, int idxB>  
struct SubEGP{
  typedef typename XCat< XList< Inst<signFlip( A, B::HEAD ), A, B::HEAD, idxA, idxB> >, typename SubEGP<A, typename B::TAIL, idxA, idxB+1>::Type >::Type Type; 
};

template<TT A, int idxA, int idxB>  
struct SubEGP<A, MVBasis<>, idxA, idxB >{
  typedef XList<> Type;
};

template<class A, class B, int idxA=0, int idxB=0>  
struct EGP{
  typedef typename XCat< typename SubEGP<A::HEAD,B, idxA,idxB>::Type, typename EGP<typename A::TAIL, B, idxA+1,idxB>::Type >::Type Type; 
};
template<class B, int idxA, int idxB>  
struct EGP<MVBasis<>,B, idxA,idxB> {
  typedef XList<> Type; 
}; 

template<class A, class B>
struct EProdInstructions{
   
  typedef typename EGP<A, B>::Type List;  
  typedef typename Reduce<List>::Type Type;
  typedef typename Index<List, Type>::Type DO;
  
};

template<class A, class B>
struct EProd {
  
  typedef EProdInstructions< typename A::MVType, typename B::MVType> Instructions;
  typedef MV< typename Instructions::Type, typename A::ValueType > Type; 

  static constexpr Type GP(const A& a, const B& b) {
    return Instructions::DO::template Make<Type>(a, b);
  }   

};

template<class A, class B>
CA egp(const A& a, const B& b) RETURNS(
  ( EProd<A,B>::GP(a, b) )
)

template<class A>
struct EGAMV : public A {
    
  typedef A Type;
  typedef EGAMV Self_Type;

  constexpr EGAMV(const A& a) : A(a) {}   
  template< class ... Args >
  constexpr EGAMV(Args...v) : A(v...) {}  

  template<class B>
  EGAMV< typename EProd< Self_Type, B >::Type > operator * ( const B& b) const{
    return egp(*this, b);
  }

  EGAMV operator + (const EGAMV& a) const {
    EGAMV tmp;
    for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] + a[i];
    return tmp;
  } 
  
  EGAMV operator - (const EGAMV& a) const {
    EGAMV tmp;
    for (int i = 0; i < A::Num; ++i) tmp[i] = (*this)[i] - a[i];
    return tmp;
  }
   
  EGAMV operator -() const{
    EGAMV tmp = *this;
    for (int i = 0; i < A::Num; ++i){ tmp[i] = -tmp[i]; }
    return tmp;
  }   

  template<class B>
  EGAMV test(const B& b) const { return egp(*this, b); }
  
};


int main(){

  static const TT DIM = 3;
  
  //RAW COMBINATORICS
  typedef Blade1<DIM>::VEC Vec;
  typedef Blade1<8>::VEC Vec8;

  typedef typename EProdInstructions<Vec,Vec>::Type Rot;

  //NEXT DEFINE THE PRODUCT SPACE
  typedef EGAMV< MV<Rot, float> > RotSpace;

  //R8 over the Quaternions
  typedef EGAMV< MV<Vec8, RotSpace> > VecOfRotors;

  RotSpace r1(1,2,3,4);
  RotSpace r2(1,.3,3,4);
  RotSpace r3(.1,22,3,.4);


  VecOfRotors vor( r1, r2, r3, r2, r1, r3, r3, r2 );

  //vor.test(r).print();

 // auto r2 = r + r;
 // r2.print();
 auto c = vor * vor;

  //Rot r= EProd< Vecff, Vecff>::GP( Vecff( Vecf(1,2,3), Vecf(2,3,4), Vecf(5,6,7) ),  Vecff( Vecf(1,2,3), Vecf(2,3,4), Vecf(5,6,7) ) );

 
  //vor.print();
  c.print();
  
//  auto r = EProd<Vecf, Vecf>::GP( Vecf(1,2,3), Vecf(3,4,5) );

 // r.print();

//  cout << r << endl; 

  //Rot test;
  //Rot::MVType::print();

  //MVInstance<Vec

 // Vec().print();


}
