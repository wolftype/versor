/*
 * =====================================================================================
 *
 *       Filename:  xQuadric.cpp
 *
 *    Description:  quadric geometric algebra
 *
 *        Version:  1.0
 *        Created:  04/04/2014 18:16:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vsr_conformal.h"


using namespace vsr;

typedef NSca<3> Sca1D;
typedef NVec<3> Vec1D;
typedef NOri<3> Ori1D;
typedef NPnt<3> Pnt1D;
typedef NInf<3> Inf1D;
typedef NPar<3> Par1D;
typedef NCir<3> Cir1D;


typedef decltype( Sca1D() + Pnt1D() + Par1D() + Cir1D() ) All;


typedef EGAMV<2,MV<Basis<1>,All>> E1;
typedef EGAMV<2,MV<Basis<2>,All>> E2;

typedef NEVec<2, All> Quadric;


Pnt1D Null(float x){
  return Ori1D(1) + Vec1D(x) + Inf1D((x*x)/2);
}

int main(){

  E1 e1( All(0,0,0,-1) );

  Pnt1D pntA = Null(-1);
  Pnt1D pntB = Null(1);
 // Pnt1D pntC = Null(2);

//  pntA.print();

  Quadric quad(pntA, pntB);

//  quad.print();

  auto qq = quad * quad;

  qq.print();

  auto r = e1 <= quad;

//  r.print();

  All().print();

  return 0;
}
