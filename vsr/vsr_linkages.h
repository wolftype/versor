/*
 * =====================================================================================
 *
 *       Filename:  vsr_linkages.h
 *
 *    Description:  3d linkages
 *
 *        Version:  1.0
 *        Created:  02/10/2014 17:30:30
 *       Compiler:  gcc4.7 or higher or clang 3.2 or higher
 *
 *         Author:  pablo colapinto
 *   Organization:  
 *
 * =====================================================================================
 */

/*!
 * @file
 *
 * \brief A collection of 3d linkages
 *
 * */


#ifndef  vsr_linkages_INC
#define  vsr_linkages_INC

#include "vsr_chain.h"

namespace vsr{


  
  /*!
   *  \brief  The bennett 4 bar linkage
   *
  */
  class Bennett : public Chain {
     
     VT mLengthA;
     VT mLengthB;

     VT mTheta;
     VT mPhi;

     VT mAmt;

     public:

     Bennett( VT theta, VT lengthA = 1.0, VT lengthB = 1.0 )
     : Chain("RRRR"), mTheta(theta), mLengthA( lengthA ), mLengthB( lengthB ), mAmt(0) {
      
        init();    
     }
     
    void init() { 
      
        mPhi = asin( sin(mTheta) * mLengthB / mLengthA); 
    
        mLink[0].pos(0,mLengthA,0); 
        mLink[2].pos(0,mLengthA,0);

        mLink[1].pos(0,mLengthB,0);
        mLink[3].pos(0,mLengthB,0);

        Biv la =  Biv::xz * mTheta/2.0; 
        Biv lb =  Biv::xz * mPhi/2.0;

        mLink[0].rot() = Gen::rot(la);
        mLink[2].rot() = Gen::rot(la);

        mLink[1].rot() = !Gen::rot(lb);
        mLink[3].rot() = !Gen::rot(lb);

    }

     VT lengthA() const { return mLengthA; } 
     VT lengthB() const { return mLengthB; } 
     VT& lengthA() { return mLengthA; } 
     VT& lengthB() { return mLengthB; } 

     VT theta() const { return mTheta; }
     VT phi() const { return mPhi; } 

     Circle circleMeet(){
       return ( Ro::dls(mFrame[1].pos(), mLengthB) ^ Ro::dls(mFrame[3].pos(), mLengthA) ).dual();
     }

     Pair pairMeet(){
       return ( mFrame[1].dxy() ^ circleMeet().dual() ).dual();
     }

     Circle orbit(){
       return ( mFrame[3].dxy() ^ Ro::dls( mJoint[0].pos(), mLengthB ) ).dual();
     }

     Bennett& operator()( VT amt ){

        mAmt = amt;
       
        bool bSwitch = sin(amt) < 0 ? true : false;

        resetJoints();

        mJoint[0].rot() = Gen::rot( Biv::xy * amt/2.0 );
        fk(1);
        
        mFrame[3].mot( mBaseFrame.mot() * !mLink[3].mot() );
        
        //calculate intersection
        auto dualMeet = Ro::dls_pnt(mFrame[1].pos(), mLengthB) ^ Ro::dls_pnt(mFrame[3].pos(), mLengthA);
       // Draw( dualMeet.dual() );
        Pair p = ( mFrame[1].dxy() ^ dualMeet).dual();
        
        mFrame[2].pos() = Ro::loc( Ro::split(p,!bSwitch) );           
        
        Rot ry = mFrame[1].rot(); 
        for (int i = 1; i < 4; ++i){
         // Vec y = mFrame[i].y();
          Vec y = Vec::y.spin(ry);
          int next = i < 3 ? i + 1 : 0;
          auto target = mFrame[next].vec();
          Vec dv = ( target-mFrame[i].vec() ).unit();
          mJoint[i].rot() = Gen::rot(  Biv::xy * acos( ( dv <= y )[0] )/2.0 * (bSwitch ?  -1 : 1 ) );
          ry = ry * mJoint[i].rot() * mLink[i].rot();
        }

        fk();

        return *this;
     }


    

    
     /*!
      *  \brief  A linked Bennett mechanism, determined by ratio of original
      */
     Bennett linkRatio(VT th, VT a = .5, VT b =.5, VT la = 0, VT lb = 0){
      
      Bennett b2(mTheta * th, mLengthA * a, mLengthB * b);
      b2.baseFrame() = Frame( mFrame[2].mot() * !mJoint[2].rot() );

      b2(mAmt);
      
      if (la==0) la = mLengthA; //else la = mLengthA;
      if (lb==0) lb = mLengthB; //else lb *= mLengthB;

      Bennett b3(mTheta * th, la, lb);

      b3.baseFrame() = Frame( b2[2].mot() * !b2.joint(2).rot() );
      
      return b3( Gen::iphi( b2.joint(2).rot() ) );

     }

     /*!
      *  \brief  A linked Bennett mechanism at joint N determined by ratio of original
      */
     Bennett linkAt(int N=0, VT th=1, VT a = .5, VT b =.5, VT la = 0, VT lb = 0){
      
      Bennett b2(mTheta * th, mLengthA * a, mLengthB * b);
      b2.baseFrame() = Frame( mFrame[N].mot() * !mJoint[N].rot() );

      b2( Gen::iphi( mJoint[N].rot() ) );

     // return b2(mAmt);
      
      if (la==0) la = mLengthA; //else la = mLengthA;
      if (lb==0) lb = mLengthB; //else lb *= mLengthB;

      Bennett b3(mTheta * th, la, lb);

      b3.baseFrame() = Frame( b2[2].mot() * !b2.joint(2).rot() );
      
      return b3( Gen::iphi( b2.joint(2).rot() ) );

     }

  };


    
  /*!
   *  \brief  PANTOGRAPH for scissor-like kinematics
   */
    class Pantograph  {
      
      Chain mChainA, mChainB;

      VT mRatio, mDecay;

      public:

        Pantograph(int n=1) : mChainA(n), mChainB(n), mRatio(1), mDecay(0)
        {
        }

        void alloc(int n) { mChainA.alloc(n); mChainB.alloc(n); }
        void reset(){ mChainA.reset(); mChainB.reset(); }
        void fk() { mChainA.fk(); mChainB.fk(); }

        void ratio(VT amt) { mRatio = amt; }
        void decay(VT amt) { mDecay = amt; }

        VT ratio() const { return mRatio; }
        VT decay() const { return mDecay; }

        void operator()() {

            bool flip = false;
            VT tRatio = mRatio;

            mChainA.fk();
            mChainB.fk(); 

            Dlp dlp = mChainA[0].dxy().unit();               // /Dual xy plane

            for (int i = 0; i < mChainA.num(); i+=2 ){
            
              Dls da = mChainA.nextDls(i);
              Dls db = mChainB.nextDls(i);

              Dls ta = flip ? da : da.dil( mChainA[i].pos(), log(tRatio) );
              Dls tb = !flip ? db : db.dil( mChainB[i].pos(), log(tRatio) );

              Par tp = ta ^ tb;
              bool isLegit = Ro::size( tp, true ) > 0;  

              if ( isLegit ){
              
                  Par p = ( ta ^ tb ^ dlp ).dual(); 
                  Pnt pnt = Ro::split(p,flip);

                  mChainA[i+1].pos() = pnt;
                  mChainB[i+1].pos() = pnt;

                  double a = ( 1.0 / tRatio );
                  double b = tRatio;
            
                  if (i < mChainA.num() - 2){
                    mChainA[i+2].pos() =  Ro::null( mChainA[i].vec() + 
                    ( ( mChainA[i+1].vec() - mChainA[i].vec() ) * (1 + ( (flip) ? b : a ) ) ) );
                    mChainB[i+2].pos() =  Ro::null( mChainB[i].vec() + 
                    ( ( mChainB[i+1].vec() - mChainB[i].vec() ) * (1 + ( (flip) ? a : b ) ) ) );
                  }

                  flip = !flip;
            
                  tRatio *= (1-mDecay);
              }
            }

            mChainA.calcJoints();
            mChainB.calcJoints();

        }

        Chain& chainA() { return mChainA; }
        Chain& chainB() { return mChainB; }

    };


}
#endif   /* ----- #ifndef vsr_linkages_INC  ----- */
