//
//  vsr_knot.h
//  Versor
/*
    KNOTS -- building up from Dorst and Valkenburg's paper on Square Roots of Rotors and Logarithms through Polar Decomposition

*/
//  Created by Pablo Colapinto on 11/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Versor_vsr_knot_h
#define Versor_vsr_knot_h

#include <vsr/form/vsr_fiber.h>

namespace vsr { namespace cga {

/// TorusKnot (newer version)
struct TKnot {

  double mP, mQ;
  HopfFiber mHopf;  
  vector<double> mEnergy;
  vector<Point> mPoint;
  vector<Circle> mCircle;
};

/// A sort of Coupled Boost
/// @todo clean this up -- pass number of iterations to apply() function . . .
/// separate out functions from memory containers
struct TorusKnot  {

    //typically integers
    double P, Q;

    //double cable;

    //A Circle base with methods for finding the links around which to knot . . .
    HopfFiber HF;

    /// A vector of circles in the knot orbit
    vector<Cir> cir;
    /// A vector of points in the knot orbit
    vector<Pnt> pnt;

    vector<double> energies;    ///< Energy at idx (from pnt 0)

    void clear(){
      cir.clear();
      pnt.clear();
      energies.clear();
    }

    double amt; //RES default

    TorusKnot& add(const Pnt& p){
        pnt.push_back(p); return *this;
    }

    TorusKnot& add(const Cir& c){
        cir.push_back(c); return *this;
    }

    int iter() {
        return ( P == 0 || Q == 0 ) ?  1.0/amt : P * Q / (amt * Round::size(HF.cir(), false) );
    }

    Par par() {
        double a = P == 0 ? 0 : PI/P; double b = Q == 0 ? 0 : PI/Q;
        return HF.fiberA().dual() * a + HF.fiberB().dual() * b;
    }

    Par par(float t) {
        //double a = P == 0 ? 0 : PI/P; double b = Q == 0 ? 0 : PI/Q;
        return (HF.fiberA().dual() * P + HF.fiberB().dual() * Q)*t;
    }

    Par dpar(int num){
      if (num==0) return Par();
      float t = PI / num;
      return ( (HF.fiberA().dual().runit() * P) + (HF.fiberB().dual().runit() * Q) ) * t;
    }
    
    Bst dbst(int num){
      return Gen::bst( dpar(num) );
    }

    Bst bst() {
        return Gen::bst( par() * amt  );
    }

    Bst bst(double t) { amt = t; return bst(); }

    TorusKnot(double p = 3, double q = 2, double a = .01) : P(p), Q(q), amt(a) {}

    void set(double p, double q) { P = p; Q =q; }

  /// Calculate full orbit from point p, renormalizing every step
  /// save positions in pnt vector and tube slices in cir vector
  vector<Pnt>& calc( const Pnt& p){
    pnt.clear(); cir.clear();

    Pnt np = p;
    Bst tbst = bst();
    int tnum = iter();
    add(np);
    for (int i = 0; i < tnum; ++ i ){
      np = Round::loc( np.sp( tbst ) );
      add(np);
    }

    //Tube Neighborhood
    for (int i = 0; i <= tnum; ++i ){
      int idx = i < tnum ? i + 1 : 0;
      Par tpar = pnt[i] ^ pnt[idx];
      Cir c = tpar.dual();
      add ( c );
    }
    return pnt;
  }

  vector<Pnt>& apply( const Point& p, int num, bool bRenormalize=true){
    pnt.clear(); cir.clear();
    auto tbst = dbst(num);
    auto tp = p;
    //std::vector<Point> result;
  //  add ( Round::loc(tp));
    if (bRenormalize){ //renormalize during each step
     for (int i =0;i <(int)num;++i){
       tp = Round::loc( tp.spin( tbst ) );
       //result.push_back( tp );
       add(tp);
     }
    } else {
     for (int i =0;i <(int)num;++i){
       tp = tp.spin( tbst );
       add (Round::loc(tp));
       //result.push_back( Round::loc(tp) );  // or after each step
     }
    }

    //Tube Neighborhood
    for (int i = 0; i < (int)num; ++i ){
      int idx = i < num - 1? i + 1 : 0;
      Par tpar = pnt[i] ^ pnt[idx];
      Cir c = tpar.dual();
      add ( c );
    }

    //energies
   // auto d = energy(0,num);

    return pnt; 
   
  }

  //currently only works on pairs and circles
  template<typename T>
  vector<T> apply(const T& input, int num, bool bRenormalize=true){
    vector<T> result;
    auto tbst = dbst(num);
    auto tmp = input;
    if (bRenormalize){ //renormalize during each step
     for (int i =0;i <(int)num;++i){
       tmp = Round::renormalize( tmp.spin( tbst ) );
       result.push_back( tmp );
     }
    } else {
     for (int i =0;i <(int)num;++i){
       tmp = tmp.spin( tbst );
       result.push_back( Round::renormalize(tmp) );  // or after each step
     }
    }
    return result;
  }


  //calculate full orbit from point p without renormalizing at each step (no tube)
  double calc0( const Pnt& p){
    pnt.clear(); cir.clear();
    Pnt np = p;
    Bst tbst = bst();
    int tnum = iter();

    for (int i = 0; i < tnum; ++ i ){
      np = np.sp( tbst ) ;
      add( Round::loc( np) );
    }

    //Tube Neighborhood
    for (int i = 0; i < tnum; ++i ){
      int idx = i < tnum -1 ? i + 1 : 0;
      Par tpar = pnt[i] ^ pnt[idx];
      Cir c = tpar.dual();
      add ( c );
    }

    //energies
    return energy(0,tnum);
  }


  double energy(int idx, int num){

      double totalEnergy = 0;     ///< Running sum TOTAL ENERGY OF KNOT
      energies.clear();           ///< energies relative to idx
      energies.push_back(0);      ///< Initial

    //forward and reverse arc measurements
      vector<double> globalA; //Distance
      //vector<double> globalB;
      vector<double> local; //Distance

      double totalA = 0;

      //integrated sums
      for (int i = 0; i < num; ++i){

          //Idx of Neighbor
          int idxA = i < num - 1 ? i + 1 : 0;

          double ta = Round::dist( pnt[i], pnt[idxA]);
          local.push_back(ta);

          totalA += ta;

          globalA.push_back( totalA );
      }

      for (int i = 0; i < num; ++i){

          double ta = 0;

          for (int j = 0; j < num; ++j){

              if ( i != j ) {
                  double chord = Round::sqd( pnt[i], pnt[j] );

                  double diffA = fabs( globalA[j] - globalA[i] );
                  double diffB = fabs( (totalA - globalA[j] ) + globalA[i] );//fabs( globalB[j] - globalA[i] );

                  double diff = diffA < diffB ? diffA : diffB;

                  double tener = ( ( 1.0 / chord ) - (1.0 / (diff * diff) ) ) * local[j]; //weighted by distance
                  //push back for i = 0
                  if (i == idx ) energies.push_back(tener);
                  ta += tener;
              }
          }

          totalEnergy += ta * local[i];
     }

        return totalEnergy;
    }



};


// /*! Generates an Orbit around a Real, Imaginary, or Null Point Pair */
// class Orbit : public Frame {
//     Frame sF, mF;
//     double mM, mAmt;
//     bool bReal, bNull;
//     public:
//
//
//
//         Orbit() : Frame(), sF(PT(0,0,0)), mM(1), mAmt(.01), bReal(false), bNull(false)
//         { calc(); }
//
//         Orbit( const Frame& f): Frame(f), sF(PT(0,0,0)), mM(1), mAmt(.01), bReal(false), bNull(false)
//         { calc(); }
//
//         bool& real() { return bReal; }
//         bool& null() { return bNull; }
//         double& m() { return mM; }
//         double& amt() { return mAmt; }
//
//         Frame& sf() { return sF; }
//         Frame& mf() { return mF; }
//
//
//         bool real() const { return bReal; }
//         bool null() const { return bNull; }
//         double m() const { return mM; }
//         double amt() const { return mAmt; }
//
//         Frame sf() const { return sF; }
//         Frame f() const { return mF; }
//
//         Par par() const {  return ( ( bNull ) ? mF.tx() : mF.px(bReal) ) * PI/mM ; }
//         Cir cir() const { return mF.px(bReal).undual(); }
//
//         void calc() {
//             mF = sF;
//             mF.mot( sF.mot() * mot() );
//         }
//
//         Bst bst() { return Gen::bst( par() * mAmt ); }
//
//         Par par(const Orbit& o, double t){
//             Orbit no = Frame::Twist( f(), o.f(), t );
//
//             return no.par();
//             //return par() * (1-t) + o.par() * t;
//         }
//
//
// };

// class Knot : public Frame {
//
//     Frame sFa, sFb;
//     Frame mFa, mFb;
//
//     Par mPar;
//
//     double mM, mN;
//     double mAmt, mDense;
//     double mDist;
//
//     bool bRealA, bRealB, bNullA, bNullB;
//
//     public:
//
//     Knot() : Frame(),
//     mAmt(.01), mDense(1.0), mM(2), mN(2),
//     sFa( PT(0,-.5,0) ), sFb( PT(0,.5,0) ),
//     bRealA(false), bRealB(false), bNullA(false), bNullB(false)
//     { cout << mFa.scale() << endl; calc(); }
//
//     double& dist() { return mDist; }
//     double dist() const { return mDist; }
//     double& amt() { return mAmt; }
//     double amt() const { return mAmt; }
//     double& m() { return mM; }
//     double& n() { return mN; }
//     double m() const { return mM; }
//     double n() const { return mN; }
//
//     bool& ra() { return bRealA; }
//     bool& rb() { return bRealB; }
//     bool ra() const { return bRealA; }
//     bool rb() const { return bRealB; }
//     bool& ba() { return bNullA; }
//     bool& bb() { return bNullB; }
//     bool ba() const { return bNullA; }
//     bool bb() const { return bNullB; }
//
//     Frame& sfa() { return sFa; }
//     Frame& sfb() { return sFb; }
//     Frame sfa() const { return sFa; }
//     Frame sfb() const { return sFb; }
//     Frame& fa() { return mFa; }
//     Frame& fb() { return mFb; }
//     Frame fa() const { return mFa; }
//     Frame fb() const { return mFb; }
//
//     Par pa(bool real = false) const {  return bNullA ? mFa.tx() : mFa.px(real); }
//     Par pb(bool real = false ) const { return bNullB ? mFb.tz() : mFb.pz(real); }
//
//     Cir ca(bool real = false) const { return mFa.px(real).undual(); }
//     Cir cb(bool real = false ) const { return mFb.pz(real).undual();}
//
//     int num() { return mM * mN / mAmt; }
//
//     void calc() {
//         sFa.pos() = PT(0,-mDist/2.0,0);
//         sFb.pos() = PT(0,mDist/2.0,0);
//
//         Mot ma = sFa.mot() * mot();
//         Mot mb = sFb.mot() * mot();
//         mFa.mot( ma ); mFb.mot( mb );
//         mPar = pa(bRealA) * ( mM == 0 ? 0 :PI/mM ) + pb(bRealB) * (  mN == 0 ? 0 :PI/mN );
//     }
//
//     Par par() const { return mPar; }
//     Par& par() { return mPar; }
//
//     Par para() const { return pa(bRealA) * PI/mM; }
//     Par parb() const { return pb(bRealB) * PI/mN; }
//
//     //Separated out
//     Bst bsta() const { return Gen::bst( para() * mAmt ); }
//     Bst bstb() const { return Gen::bst( parb() * mAmt ); }
//     Bst bstc() const { return bsta() * bstb(); }
//
//     Bst bst() { return Gen::bst( par() * mAmt ); }
//
//     //? not sure what this would do or should do
//     Knot operator * (const Knot& k){
//
//     }
//
//     static Bst bst(const Knot& ka, const Knot& kb, double t){
//         return Gen::bst( ka.par() * (1-t) + kb.par() * t );
//     }
//
//
//     template<class T>
//     T operator () (const T& t) { return t.sp( bst() ); }
//
//
// };

} }//vsr::cga

#endif
