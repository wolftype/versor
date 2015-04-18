/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_log.h
 *
 *    Description:  temp log h file
 *
 *        Version:  1.0
 *        Created:  03/19/2015 16:43:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#ifndef  vsr_cga3D_log_INC
#define  vsr_cga3D_log_INC

#include "vsr_cga3D_op.h"

namespace vsr { namespace cga3D {

typedef decltype(Pair()*Pair()) GenRot; ///<-- general rotor
typedef decltype(Sph() + 1) SqDeriv; ///<-- squared half the exterior derivative

//get the square root of a simple rotor (boost: 1 + par)
//goal is to normalize 1+R.  note this formula works for all simple rotors...
Boost sqroot(const Boost& bst){
    Bst tbst = bst;
    if (bst[0]<0) tbst = -bst; //restrict to positive <R>
    auto ss = 2 * (1+tbst[0]);     
    auto n = (ss >= 0 ? sqrt(ss) : -sqrt(-ss) );
    return FERROR(n) ? Bst() : (tbst+1)/n;
}



//get the square root of a transformation rotor
GenRot sqroot(const GenRot& rot){
  
    GenRot trot = rot;
    if (rot[0]<0) trot = -trot; //restrict to positive <R>
      
    auto rotone = trot + 1;

    VT sca = 1 + trot[0];
    VT sca2 = sca*sca;

    Sphere sph(trot);
    auto sph2 = sph.wt();

    if (sca2 == sph2) {
      printf("infinity of roots -- need to program this...\n");
    }

    auto sca3 = sca2 - sph2;
    auto sqsca3 = sqrt(sca3);

    auto v1 = ( -sph + sca ) / (2*sca3);
    auto v2 = (sph+(sca+sqsca3))/sqrt(sca+sqsca3);
    
    return rotone * v1 * v2;

}


//take a general rotor and create the exterior derivative
Pair extDeriv( const GenRot& rot){
  
  Sphere quad(rot); //grade 4 part

  auto tmp = quad + (-rot[0]); //scalar + quadvector
  auto stmp = Pair( tmp * Pair(rot) ); //quad parts are zero here.

  return stmp * 4;

}

///Take a general bivector and split into commuting pairs
//will give sinh(B+-)
vector<Pair> split(const Pair& par){

  vector<Pair> res;
  
  SqDeriv h2 = par*par;
  auto hh2 = Sphere(h2).wt();
  auto ipar = (-Sphere(h2) + h2[0]) /(h2[0]*h2[0] - hh2);  

  //scalar ||f||^2
  //4th root
//  auto tmp = ((-h2+(2*h2[0]))<=h2)[0];
//  auto f4 = pow( ((-h2+(2*h2[0]))<=h2)[0], 1.0/4.0);

  auto tmp2 = ( (h2*h2) - (h2*2*h2[0]) )[0] ;
  auto ff4 = pow( -tmp2, 1.0/4);
  auto wt = ff4 * ff4;//sqrt(fabs (((-h2+(2*h2[0]))<=h2)[0]));
 
  if ( FERROR(wt) ) {
      if ( FERROR(hh2) ){
        res.push_back(par);
        res.push_back(Par());
        return res;
      } else {
       printf("bivector split norm implemenation needed\n (add random value and retry)");
      }

  }
  
  auto iha = ipar * wt;
  auto fplus = iha + 1;
  auto fminus = -iha + 1;

  Pair pa = par * fplus * .5;
  Pair pb = par * fminus * .5;

  res.push_back(pa);
  res.push_back(pb);
  
  return res;
   
}

Pair atanh2(const Pair& p, VT cs ){

  VT norm=1;

  auto tp = p.wt();
  auto sq = sqrt( fabs(tp) );
  if(tp>0) norm = asinh(sq)/sq;
  else if (tp<0) norm = atan2(sq, cs) / sq;
  return p * norm;

}

vector<Pair> log( const GenRot& rot){

  vector<Pair> res;
  //R^2
  auto sqrot = rot*rot;
  //<R>2
  Pair rot2(sqrot);

  auto ed = extDeriv(rot);
  //half the exterior derivative is sinh(B+) + sinh(B-)
  auto hed = ed / 2.0;
  //so we split that . . .
  auto v = split(hed);
  
  //get cosh (see p96 of ref)
  auto sp = v[0].wt();//(v[0]<=v[0])[0];
  auto sm = v[1].wt();//(v[1]<=v[1])[0];

  VT coshp = FERROR(sm) ? sqrot[0] : -(rot2 <= !v[1])[0];
  VT coshm = FERROR(sp) ? sqrot[0] : -(rot2 <= !v[0])[0];

  //5.27 on p96 of ref
  res.push_back( atanh2(v[0],coshp) * -.5 );
  res.push_back( atanh2(v[1],coshm) * -.5 );

  return res;

}



} }// vsr::cga3D::

#endif   /* ----- #ifndef vsr_cga3D_log_INC  ----- */
