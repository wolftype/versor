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

namespace vsr{  namespace cga {

  /*!
   *  \brief  The bennett 4 bar linkage
   *  
   *         lb
   *   2-------------1
   *    |           |
   * la |           | la
   *    |           |
   *   3-------------0
   *         lb
   *
   *   - alternating link lengths have equal lengths
   *   - alternating link transformations have equal xz rotations
   *   - everything else is figured out analytically.
  */
  class Bennett : public Chain {
     
     VSR_PRECISION mLengthA; ///< Length of first alternating sides
     VSR_PRECISION mLengthB; ///< Length of second alternating sides

     VSR_PRECISION mOffsetA; ///< offset along z axis of first alternating sides
     VSR_PRECISION mOffsetB; ///< offset along z axis of second alternating sides

     VSR_PRECISION mTheta;  ///< skew of first alternating links
     VSR_PRECISION mPhi;    ///< skew of second alternating links

     VSR_PRECISION mPhase;    ///< phase of rotation

     public:

     Bennett() : Chain() {}

     Bennett( VSR_PRECISION theta, VSR_PRECISION lengthA, VSR_PRECISION lengthB  )
     : Chain("RRRR"), mTheta(theta), mLengthA( lengthA ), mLengthB( lengthB ), mPhase(0) {
        init();    
     }

     Bennett( VSR_PRECISION theta, VSR_PRECISION lengthA, VSR_PRECISION lengthB,  VSR_PRECISION offset  )
     : Chain("RRRR"), mTheta(theta), mLengthA( lengthA ), mLengthB( lengthB ), mOffsetA(offset), mOffsetB(offset), mPhase(0) {
        init();    
     }
     

     void set( VSR_PRECISION theta, VSR_PRECISION lengthA, VSR_PRECISION lengthB, VSR_PRECISION offset=0.0){
       mTheta = theta; mLengthA = lengthA; mLengthB = lengthB; mOffsetA = offset; mOffsetB = offset;
       init();
     }
     
    void init() { 
      
        mPhi = asin( sin(mTheta) * mLengthB / mLengthA); 
    
        mLink[0].pos(0,mLengthA,  0);//mOffsetA); 
        mLink[2].pos(0,mLengthA,  0);//mOffsetA);

        mLink[1].pos(0,mLengthB,  0);//mOffsetB);
        mLink[3].pos(0,mLengthB,  0);//mOffsetB);

        Biv la =  Biv::xz * mTheta/2.0; 
        Biv lb =  Biv::xz * mPhi/2.0;

        mLink[0].rot() = Gen::rot(la);
        mLink[2].rot() = Gen::rot(la);

        mLink[1].rot() = Gen::rot(-lb);
        mLink[3].rot() = Gen::rot(-lb);

    }

     VSR_PRECISION lengthA() const { return mLengthA; } 
     VSR_PRECISION lengthB() const { return mLengthB; } 
     VSR_PRECISION& lengthA() { return mLengthA; } 
     VSR_PRECISION& lengthB() { return mLengthB; } 


     VSR_PRECISION offsetA() const { return mOffsetA; } 
     VSR_PRECISION offsetB() const { return mOffsetB; } 
     VSR_PRECISION& offsetA() { return mOffsetA; } 
     VSR_PRECISION& offsetB() { return mOffsetB; } 

     VSR_PRECISION theta() const { return mTheta; }
     VSR_PRECISION phi() const { return mPhi; } 

    /*!
      Meet of 1st and 3rd spheres of possibilities
    */
     Circle circleMeet(){
       return ( nextSphere(1) ^ prevSphere(3) ).dual();//Round::dls(mFrame[1].pos(), mLengthB) ^ Round::dls(mFrame[3].pos(), mLengthA) ).dual();
     }

     Pair pairMeet(){
       return ( nextPlane(1) ^ circleMeet().dual() ).dual();
     }

     Circle orbit(){
       return ( nextPlane(3) ^ prevSphere(0) ).dual();//( mFrame[3].dxy() ^ Round::dls( mJoint[0].pos(), mLengthB ) ).dual();
     }

     
     /*-----------------------------------------------------------------------------
      * Eval 
      *-----------------------------------------------------------------------------*/
     Bennett& operator()( VSR_PRECISION amt ){

        mPhase = amt;      
        bool bSwitch = sin(amt) < 0 ? true : false;

        resetJoints();

        mJoint[0].rot() = Gen::rot( Biv::xy * amt/2.0 );
        fk(1);
        
        mFrame[3].mot( mBaseFrame.mot() * !mLink[3].mot() );
        
        //calculate intersection
        //auto dualMeet = nextSphere(1) ^ prevSphere(3);
        auto dualMeet = nga::Round::dls_pnt(mFrame[1].pos(), mLengthB) ^ nga::Round::dls_pnt(mFrame[3].pos(), mLengthA);
        //Pair p = (nextPlane(1)^dualMeet).dual();
        Pair p = ( mFrame[1].dxy() ^ dualMeet).dual();
        
        //pick one
        mFrame[2].pos() = Round::location( Round::split(p, !bSwitch) );           
        
        Rot ry = mFrame[1].rot(); 
        for (int i = 1; i < 4; ++i){
          Vec y = Vec::y.spin(ry);
          int next = i < 3 ? i + 1 : 0;
          auto target = mFrame[next].vec(); 

          auto linkRot = Gen::ratio( mLink[i].vec().unit(), Vec::y );
          
          Vec dv = (target-mFrame[i].vec()).unit().spin( linkRot );

          //mJoint[i].rot() = Gen::ratio(y,dv);
          mJoint[i].rot() = Gen::rot(  Biv::xy * acos( ( dv <= y )[0] )/2.0 * (bSwitch ?  -1 : 1 ) );
          
         // ry = mJoint[i].rot() * ry;
          ry = ry * mJoint[i].rot() * mLink[i].rot();
        }

 //      calcJoints(1,true);

        fk();

        return *this;
     }

    
    
     /*!
      *  \brief  A linked Bennett mechanism, determined by ratio of original
      */
     Bennett linkRatio(VSR_PRECISION th, VSR_PRECISION a = .5, VSR_PRECISION b =.5, VSR_PRECISION la = 0, VSR_PRECISION lb = 0){
      
      Bennett b2(mTheta * th, mLengthA * a, mLengthB * b);
      b2.baseFrame() = Frame( mFrame[2].mot() * !mJoint[2].rot() );

      b2(mPhase);
      
      if (la==0) la = mLengthA; //else la = mLengthA;
      if (lb==0) lb = mLengthB; //else lb *= mLengthB;

      Bennett b3(mTheta * th, la, lb);

      b3.baseFrame() = Frame( b2[2].mot() * !b2.joint(2).rot() );
      
      return b3( nga::Gen::iphi( b2.joint(2).rot() ) );

     }

     /*!
      *  \brief  A linked Bennett mechanism at joint N determined by ratio of original
      *  We first create a sublinkage inside the first, and then use the [2] frame to set
      *  the base of our resulting linkage.
      *
      *  @param Nth link (default 2)
      *  @param linkage skew RELATIVE to parent
      *  @param length along edge a (default .5)
      *  @param length along edge b (default .5)
      *  @param new length a (default same as parent)
      *  @param new length b (default same as parent)
      */
     Bennett linkAt(int N=2, VSR_PRECISION th=1, VSR_PRECISION a = .5, VSR_PRECISION b =.5, VSR_PRECISION la = 0, VSR_PRECISION lb = 0){
      
      //necessary boolean to reverse direction 
      bool bSwitch = sin(mPhase) < 0 ? true : false;

      //make linkage relative to original
      Bennett b2(mTheta * th, mLengthA * a, mLengthB * b);      
      //set baseframe of new linkage to nth frame and undo local in-socket transformation
      b2.baseFrame() = Frame( mFrame[N].mot() * !mJoint[N].rot() );
      //set phase of new linkage mechanism from nth joint's rotation
      b2( bSwitch ? -nga::Gen::iphi( mJoint[N].rot())  : nga::Gen::iphi( mJoint[N].rot() ) );
      //use linkAt_ to debug this point

      if (la==0) la = mLengthA; //else la = mLengthA;
      if (lb==0) lb = mLengthB; //else lb *= mLengthB;

      //repeat
      Bennett b3(mTheta * th, la, lb);
      b3.baseFrame() = Frame( b2[2].mot() * !b2.joint(2).rot() );
      for (int i=0;i<b3.num();++i){
        b3[i].scale() = (*this)[i].scale();
      }
      return b3( bSwitch ? -nga::Gen::iphi( b2.joint(2).rot() ) : nga::Gen::iphi( b2.joint(2).rot() )  );

     }

    //alt
     Bennett linkAt_(int N, VSR_PRECISION th=1, VSR_PRECISION a = .5, VSR_PRECISION b =.5, VSR_PRECISION la = 0, VSR_PRECISION lb = 0){
        
      bool bSwitch = sin(mPhase) < 0 ? true : false;
      
      Bennett b2(mTheta * th, mLengthA * a, mLengthB * b);      
      //set baseframe to nth frame and undo local transformation
      b2.baseFrame() = Frame( mFrame[N].mot() * !mJoint[N].rot() );
      //set phase of new linkage mechanism from nth joint's rotation
      return b2( bSwitch ? -nga::Gen::iphi( mJoint[N].rot())  : nga::Gen::iphi( mJoint[N].rot() ) );
      
     }     

  };


    
  /*!
   *  \brief  PANTOGRAPH for scissor-like kinematics
   */
    class Pantograph  {
      
      Chain mChainA, mChainB;

      VSR_PRECISION mRatio, mDecay;

      public:

        Pantograph(int n=1) : mChainA(n), mChainB(n), mRatio(1), mDecay(0)
        {
        }

        void alloc(int n) { mChainA.alloc(n); mChainB.alloc(n); }
        void reset(){ mChainA.reset(); mChainB.reset(); }
        void fk() { mChainA.fk(); mChainB.fk(); }

        void ratio(VSR_PRECISION amt) { mRatio = amt; }
        void decay(VSR_PRECISION amt) { mDecay = amt; }

        VSR_PRECISION ratio() const { return mRatio; }
        VSR_PRECISION decay() const { return mDecay; }

        void operator()() {

            bool flip = false;
            VSR_PRECISION tRatio = mRatio;

            mChainA.fk();
            mChainB.fk(); 

            Dlp dlp = mChainA[0].dxy().unit();               // /Dual xy plane

            for (int i = 0; i < mChainA.num(); i+=2 ){
            
              Dls da = mChainA.nextSphere(i);
              Dls db = mChainB.nextSphere(i);

              Dls ta = flip ? da : da.dil( mChainA[i].pos(), log(tRatio) );
              Dls tb = !flip ? db : db.dil( mChainB[i].pos(), log(tRatio) );

              Par tp = ta ^ tb;
              bool isLegit = Round::size( tp, true ) > 0;  

              if ( isLegit ){
              
                  Par p = ( ta ^ tb ^ dlp ).dual(); 
                  Pnt pnt = Round::split(p,flip);

                  mChainA[i+1].pos() = pnt;
                  mChainB[i+1].pos() = pnt;

                  double a = ( 1.0 / tRatio );
                  double b = tRatio;
            
                  if (i < mChainA.num() - 2){
                    mChainA[i+2].pos() =  Round::null( mChainA[i].vec() + 
                    ( ( mChainA[i+1].vec() - mChainA[i].vec() ) * (1 + ( (flip) ? b : a ) ) ) );
                    mChainB[i+2].pos() =  Round::null( mChainB[i].vec() + 
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


} }

#endif   /* ----- #ifndef vsr_linkages_INC  ----- */
