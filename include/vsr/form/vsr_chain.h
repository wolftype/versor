/*
 *  Chain.h
 *  VERSOR
 *
 *  Created by xpc on 9/29/10.
 *  Copyright 2010 wolftype. All rights reserved.
 *
 
3D KINEMATIC CHAIN can be closed or open (robot arm)
 
 > if one end is fixed, it is a MECHANISM
 
 > if a MECHANISM transmits power it is a MACHINE
 
*/

#ifndef VSR_CHAIN_H_INCLUDED
#define VSR_CHAIN_H_INCLUDED

#include "vsr_cga3D_frame.h"
#include "vsr_twist.h"
#include "vsr_rigid.h"

#include <map>
#include <string>

namespace vsr{ namespace cga {

  using std::map;

/* namespace Kinematic { */
/*   enum Type{ */
/*       REVOLUTE, PRISMATIC, CYLINDRICAL, HELICAL, PLANAR, SPHERICAL */
/*   } */
/* } */

struct Joint : public Frame {
  
  enum Type{
      REVOLUTE, PRISMATIC, CYLINDRICAL, HELICAL, PLANAR, SPHERICAL
  };

// static map<string, Type> LowerPairMap;


  Type type;

  Joint( Type t = Joint::SPHERICAL, const Frame& f = Frame() ) : Frame(f), type(t) {}

  virtual Frame operator()() const = 0;

};

/* map<string,Joint::Type> Joint::LowerPairMap = { */ 
/*   make_pair("R",Joint::REVOLUTE), */ 
/*   make_pair("P", Joint::PRISMATIC), */ 
/*   make_pair("C", Joint::CYLINDRICAL), */
/*   make_pair("H", Joint::HELICAL), */
/*   make_pair("S", Joint::SPHERICAL) , */
/*   make_pair("X", Joint::PLANAR) }; */


struct Prismatic {
  Prismatic() : mFrame() {}
  Frame operator() (VSR_PRECISION amt) { return mFrame.moveZ(amt); }//pos() = frame.pos().trs ( frame.z() * amt ) ; }
  Frame mFrame;
};

struct Revolute : public Joint {
  Revolute( const Frame& f = Frame() ) : Joint ( Joint::REVOLUTE, f) {}
  Frame operator() (VSR_PRECISION amt) { mAmt = amt; return rotXY( amt ); }
  virtual Frame operator()() const { return rotXY( mAmt ); }
  VSR_PRECISION mAmt;
};

struct Cylindrical {
  Cylindrical() : mFrame() {}
  Frame operator() (VSR_PRECISION slide, VSR_PRECISION rotate) { return mFrame.moveZ(slide).rotXY( rotate ); }
  Frame mFrame;
};

struct Helical {
  Helical( VSR_PRECISION period = PI, VSR_PRECISION pitch = 1.0 ) : mFrame(), mPeriod( period), mPitch(pitch) {}
  Frame operator() (VSR_PRECISION amt) { return Frame( Gen::mot( Twist::Along( mFrame.dlz(), mPeriod, mPitch ) * amt ) * mFrame.mot() ); }
  Frame mFrame;
  VSR_PRECISION mPeriod, mPitch;
};

struct Planar {
  Planar() : mFrame() {}
  Frame operator() (VSR_PRECISION dx, VSR_PRECISION dy, VSR_PRECISION rotate) { Frame f(mFrame); return f.move(dx,dy,0).rotXY( rotate ); }
  Frame mFrame;
};
 
struct  Spherical : public Joint {
  Spherical( const Frame& f = Frame() ) : Joint(Joint::SPHERICAL, f) {}
  VSR_PRECISION theta, phi;
  Frame operator() (VSR_PRECISION rx, VSR_PRECISION ry) { theta = rx; phi = ry; return Frame( pos(), Gen::rot(theta,phi) ); }
  virtual Frame operator() () const { return Frame( pos(), Gen::rot(theta,phi) ); } 
};
//spherical (sph coords)
//planar

  
  /**
  * @brief A sequence of spatial Frames
    @ingroup form
  */
  class Chain : public Frame {
     
     protected:
       
     Frame mBaseFrame;        ///< default zero, to tie chains together, set this to another chain's frame.  
       
     vector<Frame> mJoint;    ///< In Socket Transformation (RDHC, etc) SET THIS directly using joint(i) 
                              ///  (all others follow after calling fk() method)  
     
     vector<Frame> mLink;     ///< Relative Link to NEXT joint         
    
     vector<Frame> mFrame;    ///< Absolute frames of Joints = prevFrame * prevLink * joint

    int mNum;
    
    void _init(){
      for (int i = 0; i < mNum; ++i){
        Vec v(0,1.0,0);
        mLink[i].pos() = Round:: null(v);
      }
       fk();
    }
    
    public:

      Frame& baseFrame() { return mBaseFrame; }
      Frame baseFrame() const { return mBaseFrame; }

      void resetJoints(){
        for (auto& i : mJoint) i.reset();
      }

      void reset(){
        for (auto& i : mJoint) i.reset();
        for (auto& i : mLink) i.reset();
        mBaseFrame.reset();
        _init();
      }
    
      Chain(const string& s) {
        
        //mNum = s.length();
        alloc(s);
        _init();
      }

      Chain(int n = 3) : mNum(n) {
                
          alloc(n);
          _init();    
                  
      }       
        
      
        ~Chain(){
           mFrame.clear();
           mLink.clear();
           mJoint.clear(); 
        }

        void alloc(const string& s){
           mNum = s.length();
           if (mNum>0){

               mFrame.clear();
               mLink.clear();
               mJoint.clear();  

               mJoint  = vector<Frame>(mNum);
               mLink  = vector<Frame>(mNum);
               mFrame = vector<Frame>(mNum);


               for (int i = 0; i < mNum; ++i){
                  
               if( strncmp( &s[i], "R", 1 ) == 0) mJoint.push_back( Revolute() );
               else if (strncmp( &s[i], "S", 1 ) == 0 ) mJoint.push_back( Spherical() );

                      /* case "R":  mJoint.push_back( new Revolute() ); */
                      /* break; */
                      /* case "P":  //mJoint.push_back( new Revolute() ); */
                      /* break; */
                      /* case "C": */
                      /* break; */ 
                      /* case "H": */
                      /* break; */
                      /* case "S": mJoint.push_back( new Spherical() ); */
                      /* break; */
                      /* case "X": */
                      /* break; */
                  /* } */
                
                // mJoint.push_back( Joint::LowerPairMap[ s[i] ] 
               }

              _init();
           }

        }
        
        void alloc(int n){
            mNum = n; 
            if (mNum>0){
 
              mFrame.clear();
              mLink.clear();
              mJoint.clear();  

              mFrame = vector<Frame>(n);
              mLink  = vector<Frame>(n);
              mJoint = vector<Frame>(n);

              /* for (int i = 0; i < mNum; ++i){ */
              /*   Spherical * s = new Spherical(); */
              /*   mJoint.push_back(s); */
              /* } */
             // mJoint = vector<Joint*>(n, new Spherical());///new Frame[n];
              _init(); 
            }           
        }
            
       //set first joint to this frame's position and rotation
        void frameSet(){
            mJoint[0].set( mPos, mRot ); fk();
        }
        
      /* GETTERS AND SETTERS */
      int num() const { return mNum; }
      Frame& link(int k) { return mLink[k]; }            ///< set k's Link To Next Joint
      Frame& joint(int k) { return mJoint[k]; }          ///< set kth joint's In Socket Transformation
      Frame& frame(int k) { return mFrame[k]; }          ///< set Absolute Displacement Motor

      Frame link(int k) const { return mLink[k]; }       ///< Get k's Link To Next joint 
      Frame joint(int k) const { return mJoint[k]; }     ///< Get kth Joint's In Socket Transformation
      Frame frame(int k) const { return mFrame[k]; }     ///< Get Absolute Displacement Motor

      vector<Frame>& frame() { return mFrame; }
      vector<Frame>& links() { return mLink; }
      vector<Frame>& joints() { return mJoint; }
      
      Frame& operator [] (int k) { return mFrame[k]; }        ///< Set kth Absolute Frame
      Frame operator [] (int k) const { return mFrame[k]; }   ///< Get kth Absolute Frame
      
      /* SURROUNDS */
      
      /// Sphere Centered at Joint K Going Through Joint K+1 
      Dls nextSphere(int k) const{
        return Round::dls(mFrame[k].pos(), mLink[k].vec().norm() );//mFrame[k+1].pos());
      }
      /// Sphere Centered at Joint K Going Through Joint K-1
      Dls prevSphere(int k) const{
        return Round::dls(mFrame[k].pos(), mLink[k-1].vec().norm());//mFrame[k-1].pos());
      }
      
      /// Dual Plane of rotation of kth joint (translated by link rejection from yz)
      Dlp nextPlane(int k) const {
        Vec rj = Op::rj( link(k).vec(), Biv::xy );
        return mFrame[k].dxy().translate(rj.spin(mFrame[k].rot()));
      }

      /// see next version Dual Plane of rotation of k-1th joint (translated by link rejection from yz)
//      Dlp prevPlane_(int k) const {
//        auto rj = Op::rj( link(k-1).vec(), Biv::xy );
//        auto uxy = mFrame[k].dxy().spin( !link(k-1).rot() );//<-- "undo" orientation of current xy plane
//        return uxy.translate(-rj);
//      }

      /// Dual Plane of rotation of k-1th joint (translated by link rejection from yz)
      Dlp prevPlane_(int i) const {
        
        Vec rj = Op::rj( link(i-1).vec(), Biv::xy );
        
        auto adjustedRot = prevRot_(i);
        auto normal = Vec::z.spin( adjustedRot); 
        
        return DualPlane(normal).translate(Vec(mFrame[i].pos()) - rj.spin(adjustedRot));


      }

      //testing
      Dlp prevPlane(int i) const {
        
        //rotation in terms of prev frame
        //offset of link
        Vec rj = Op::rj( link(i-1).vec(), Biv::xy );
        
        auto adjustedRot = prevRot(i);
        auto normal = Vec::z.spin( adjustedRot); 
        
        return DualPlane(normal).translate(Vec(mFrame[i].pos()) - rj.spin(adjustedRot));

      }

      Rot prevRot(int i) const {
        
        // direction from prev frame
        auto dir = Vec(mFrame[i].pos()-mFrame[i-1].pos()).unit();

        Vec ty = Op::project( mLink[i-1].vec().unit().spin( !mLink[i-1].rot() ), Biv::xy).unit();        

        Vec pj = Op::project( dir.spin( !mFrame[i].rot() ), Biv::xy ).unit();

        auto rel = Gen::ratio( ty, pj);//Gen::ratio( pjlink, pjunrot );
       // auto lrel = Gen::ratio ( mFrame[i].y(), Vec::y.spin(rel) );
        return  mFrame[i].rot() * rel * !mLink[i-1].rot();  

      }

      /// based on unit direction between frames..
      Rot prevRot_(int i) const {
        
        auto dir = Vec(mFrame[i].pos()).unit();
        Vec ty = Op::project( mLink[i-1].vec().unit().spin( !mLink[i-1].rot() ), Biv::xy).unit();        

        Vec pj = Op::project( dir.spin( !mFrame[i].rot() ), Biv::xy ).unit();
 
        auto rel = Gen::ratio( ty, pj);//Gen::ratio( pjlink, pjunrot );
       // auto lrel = Gen::ratio ( mFrame[i].y(), Vec::y.spin(rel) );
        return  mFrame[i].rot() * rel * !mLink[i-1].rot();  

      }
      
      /// Dual Circle Centered at Joint K Going Through Joint K-1 (in plane of rotation)
      Circle prevCircle(int k) const {  
        return (prevSphere(k) ^ prevPlane(k)).dual();
      }

      
      /// Dual Circle Centered at Joint K Going Through Joint K+1 (in plane of rotation)
      Circle nextCircle(int k) const {  
        return (nextSphere(k) ^ nextPlane(k)).dual();
      }

      /// Dual Circle Centered at Joint K Going Through Joint K-1 (in plane of rotation)
      Circle prevCircle_(int k) const {  
        return (prevSphere(k) ^ prevPlane_(k)).dual();
      }

      /// Sphere at Point p through Joint K
      Dls goalSphere(const Pnt& p, int k){
         return Round::dls(p, mLink[k].vec().norm());
      }
      /// Sphere at point p through last link (default, or set arbitary link)
      Dls lastSphere(const Pnt& p){
        return Round::dls(p,mLink[mNum-1].vec().norm());
      }
        
      /* Possible Points */
      
      /// Pnt at position t along Link idx
      Pnt at(int idx, double t = 0.0){
          return Round:: null( Interp::linear<Vec>( mFrame[idx].vec(), mFrame[idx+1].vec(), t) );
      }
      
      Frame& base() { return mBaseFrame;} //mFrame[0]; }
      Frame& first() { return mFrame[0]; }        
      Frame& last() { return mFrame[mNum -1]; }
      
      /// Vert xy Plane Containing Root Target Point v ( NORMALIZED )
      Dlp xy(const Pnt& p) {
        return Op::dl(frame(0).pos()^Round:: null(0,1,0)^p^Inf(1)).unit();
      }
      /// Horiz xz Plane Containing Target Point v
      Dlp xz(const Pnt& p)  {
        return Dlp(0,1,0,p[1]);
      }

      /// Dual Line Forward: Line from kth frame to kth Link
      Dll linkf(int k) { return Op::dl( mFrame[k].pos() ^ mFrame[k+1].pos() ^ Inf(1) ).runit() ; }      
      /// Dual Line Forward: Line from kth frame to kth+1 joint
      Dll linf(int k) { return Op::dl( mFrame[k].pos() ^ mFrame[k+1].pos() ^ Inf(1) ).runit() ; }
      /// Dual Line Forward
      Dll nextLine(int k) { return linf(k); }
      /// Dual Line Backward: Line from kth frame to kth-1 joint
      Dll linb(int k ) { return Op::dl( mFrame[k].pos() ^ mFrame[k-1].pos() ^ Inf(1) ).runit() ; }
      /// Dual Line Backward
      Dll prevLine(int k) { return linb(k); }
      /// Dual Line From Kth Joint to Input Target (Default is From Last joint)
      Dll lin(const Pnt& p ) { return Op::dl( mFrame[mNum-1].pos() ^ p ^ Inf(1) ).runit() ; }

      /// relative transformation (lagrangian) at kth joint
      Mot rel(int k){
         if (k==0) return mJoint[0].mot();

         return mLink[k-1].mot() * mJoint[k].mot();
      }

      void calcBase(){
         Mot mot = mJoint[0].mot();
         mFrame[0].mot( mBaseFrame.mot() * mot  );
      }
      
      /// Forward Kinematics: Absolute Concatenations of previous frame, previous link, and current joint
      void fk() {  
          Motor mot = mJoint[0].mot();
          mFrame[0].mot( mBaseFrame.mot() * mot );
          for (int i = 1; i < mNum; ++i){    
            Mot rel =  mLink[i-1].mot() * mJoint[i].mot();//mLink[i-1].mot() * mJoint[i].mot();
            mFrame[i].mot( mFrame[i-1].mot() * rel );// * mFrame[i-1].mot() );// mFrame[i-1].mot() * rel ) ;
          }
      }        
        
      /// Forward Kinematics: calculate forward to "end" joint
      void fk(int end){
       
        Mot mot = mJoint[0].mot();
        mFrame[0].mot( mBaseFrame.mot() * mot  );
       
        for (int i = 1; i <= end; ++i){
          Mot m =  mFrame[i-1].mot()  * mLink[i-1].mot() * mJoint[i].mot();
          mFrame[i].mot( m );        
        }
      
      }

      /// Forward Kinematics: calculate forward from "begin" to "end" joint
      void fk(int begin, int end){
              
        for (int i = begin; i < end; ++i){
          Mot m =  mFrame[i-1].mot()  * mLink[i-1].mot() * mJoint[i].mot();
          mFrame[i].mot( m );        
        }
      
      }

            // What is difference between ifabrik and fabrik?
            void ik(int end, int begin){
                
            }
        

            /// Basic "FABRIK" Iterative Distance Solver [see paper "Inverse Kinematic Solutions using Conformal Geometric Algebra", 
            ///  by Aristodou and Lasenby] feed target point, end frame and beginning frame,
            void fabrik(const Pnt& target, int end, int begin, double err = .001){

              auto terr = Round::sqd( mFrame[end].pos(), target);

               Point base = mFrame[begin].pos();

              //repeat until err minimized or iterations maxed
              int iter =0;
              while( terr>err && iter<20 ){
                //1.place end at target
                mFrame[end].pos() = target;
                //2.work backwards
                for (int i=end;i>begin;--i){
                  mFrame[i-1].pos() = Constrain::PointToSphere( mFrame[i-1].pos(), prevSphere(i) );
                }
                //3.place beginning at base
                mFrame[begin].pos() = base;
                //4.work forwards
                for (int i=begin+1;i<end;++i){
                  mFrame[i].pos() = Constrain::PointToSphere( mFrame[i].pos(), nextSphere(i-1) );
                }

                terr = Round::sqd( mFrame[end].pos(), target);
                iter++;
              }

              calcJoints();
            }
           
           
           struct PC{
              Point pnt; Circle cir; Frame frame;
           };

            /// Constrain to xy plane of rotation (in progress)
            vector<PC>  constrainedFabrik(const Pnt& target, int end, int begin, double err = .001, float xyoff=0, float xzoff=0){

              vector<PC> tres;

              double terr = Round::sqd( mFrame[end].pos(), target);

              Point base = mFrame[begin].pos();

              //repeat until err minimized or iterations maxed
              Point tmpEndA = Construct::point(0,0,0);
              Point tmpEndB = Construct::point(1,0,0);
              int iter =0; bool bSwitch = false; 
              while( terr>err && iter<21 ){
                
                //0. deadlock? (figure 14 of "Extended Fabrik" paper) 
 
                bool bDeadlock = false;
                
                if (bSwitch) tmpEndA = mFrame[end].pos();
                else tmpEndB = mFrame[end].pos();
                bSwitch = !bSwitch;
                auto dist = Round::dist(tmpEndA, tmpEndB);

                //cout << "dist: " << dist << endl;
                if (dist < err ) {
                  cout << "deadlock at " << iter << endl;
                  bDeadlock = true;
                }
                

                //1.place end at target
                mFrame[end].pos() = target;

                mFrame[end].rot() = mFrame[end].rot() * Gen::rot( Biv::xz * xzoff * iter ); //*************
                tres.push_back( { mFrame[end].pos(), prevCircle(end), mFrame[end] } );

                //2.work backwards, constraining position AND orientation
               for (int i=end;i>begin;--i){
               
                 // to constrain position, we must know previous rotation
                 auto rota = prevRot(i);

                 auto tmpPos = mFrame[i-1].pos();
                 mFrame[i-1].pos() = Constrain::PointToCircle( mFrame[i-1].pos(), prevCircle(i) );

                 mFrame[i-1].rot() = rota;
                 
                 if (i==end) tres.push_back( { mFrame[i-1].pos(), CXY(0), mFrame[i-1] } );

               }
          
          
              //4. work forwards
              Rot ry = mBaseFrame.rot();
              if (begin>0) ry = mFrame[begin-1].rot() * mLink[begin-1].rot(); //check

              //3.place beginning at base 
              mFrame[begin].pos() = base;

              if (bDeadlock){
                mFrame[begin].rot() = ry * Gen::rot( Biv::xy * xyoff );//* iter ); //*************
                mFrame[begin+1].pos() = PAO.spin( mFrame[begin].mot() * mLink[begin].mot() );
              }
               
              for (int i=begin; i<=end; ++i){
                  
                  auto ytarget = Vec::y;
                  mFrame[i].rot() = ry;
                 
                  int next = i+1;
                  if (next<=end){
                    mFrame[next].pos() = Constrain::PointToCircle( mFrame[next].pos(), nextCircle(i) );
                    ytarget = ( mFrame[next].vec() - mFrame[i].vec() ).unit();   
                    ytarget = Op::pj( ytarget, mFrame[i].xy() ).unit();
                  }
                  else { 
                    ytarget = Op::pj( Vec::y, mFrame[i].xy() ).unit();
                  }
                  
                  ytarget = ytarget.spin(!ry);
                    
                  auto adjustedRot = Gen::ratio( Vec::y, ytarget );
                  mFrame[i].rot() = ry * adjustedRot;
                  ry = ry * adjustedRot * mLink[i].rot();
               //   tres.push_back( {mFrame[i].pos(), CXZ(1), mFrame[i]} );

                
              }
              

                terr = Round::sqd( mFrame[end].pos(), target);
                iter++;
              }

              cout << iter << endl;
              
              calcJoints(begin);

              return tres;
            }


            /// Derive Joint Rotations from Current Frame Absolute Positions
            /// Currently assumes that positions have ALREADY been calculated with any constraints in mind
            void calcJoints(int start = 0, bool bLoop=false){

                Rot ry = mBaseFrame.rot();
                if (start != 0) ry = mFrame[start-1].rot() * mLink[start-1].rot();
                
                for (int i = start; i < (bLoop ? mNum : mNum-1); ++i){
                  
                  int next = i < (mNum-1) ? i+1 : 0;    
                  
                  auto target = ( mFrame[next].vec() - mFrame[i].vec() ).unit();         //global target direction
                  target = Op::pj( target, mFrame[i].xy() ).unit();
                  target = target.spin(  !(ry) );//!(ry*linkRot) );//*linkRot) );
  
                  auto adjustedRot = Gen::ratio( Vec::y, target );

                  mJoint[i].rot() = adjustedRot;

                  ry = ry * mJoint[i].rot() * mLink[i].rot();   //* mLink[i].rot()                          //compound: last * current * next
                }
                
            }   
      
      /// Derive New Relative Link Frames from current Positions 
      /// @ param bOrientation: whether to consider current z orientation of frames when reverse engineering links
      void calcLinks(bool bOrientation =false){

        if (!bOrientation){
          for (int i = 0; i < mNum; ++i){
            int next = (i<mNum-1)?i+1:0;
            //mLink[i].rot() = Gen::ratio( mFrame[i].z(), mFrame[next].z() );
            //auto ratio = mFrame[next].rot() / mFrame[i].rot();              //relative transform
            auto yratio = Gen::ratio( mFrame[i].y(), mFrame[next].y() );      //relative rotational component

            //auto theta = Gen::iphi( Gen::ratio( mFrame[i].z(), mFrame[next].z() ) );//acos( (mFrame[i].z()<=mFrame[next].z())[0] );
            mLink[i].rot() = !mFrame[i].rot() * !yratio * mFrame[next].rot(); //SEQUENCE MATTERS!  first local rot, then undo local ys, then undo source

            auto dv = mFrame[next].vec() - mFrame[i].vec();
           // Vec project = Op::project(dv, mFrame[i].xy());
           // Vec reject = Op::reject(dv, mFrame[i].xy());
           //under local transformation 
            mLink[i].pos() = dv.spin( !mFrame[i].rot() ).null(); 
            ////project + rejectVec(0,ylength,0).null();
          } 
        } else{
          for (int i = 0; i < mNum; ++i){
            int next = (i<mNum-1)?i+1:0;
            //mLink[i].rot() = Gen::ratio( mFrame[i].z(), mFrame[next].z() );
            auto theta = acos( (mFrame[i].z()<=mFrame[next].z())[0] );
            mLink[i].rot() = Gen::rotor( Biv::xz * theta * .5 );
            auto vec = (mFrame[next].vec() - mFrame[i].vec()).spin( !mFrame[i].rot() );
            mLink[i].pos() = vec.null();
          }   
        }
      } 
        
        
            ///Satisfy Specific Angle Constraint at frame k
            void angle(int k, double theta){
                
                Rot R =  mJoint[k].rot();
                                
                Biv b = Biv( R ) * -1;
                // * ( (t > 1) ? ); // note: check Op:lg and Gen::log_rot (maybe mult by -1 there as well)

                Rot nr = Gen::rot( b.unit() * theta );
                
                mJoint[k].rot( nr );
                
                //forward kinematics to k
                fk(k, mNum);
                
            }
        
  
  };

} } //vsr::cga



//OLD TMP

//                 if (bLoop){
//                  Vec y = Vec::y.spin(ry);
//                  auto target = Vec(mFrame[0].pos()-mFrame[mNum-1].pos()).unit();
//                  auto linkRot = Gen::ratio( Vec::y, mLink[mNum-1].vec().unit() );
//                  auto dv = target.spin( !linkRot );    //current status
//                  
//                  mJoint[mNum-1].rot() = Gen::ratio( y, dv );//Gen::rot( Biv::xy * acos( ( dv <= y )[0] )/2.0  ); //set rotation
//                }
//
//                Vec t = mBaseFrame.y(); // Vec::y;
//                Rot R = mBaseFrame.rot(); // (1,0,0,0);
                
//                //Where we are in current rotation scheme
//                for (int i = 0; i < start; ++i){
//                    t = t.sp( mFrame[i].rot() );
//                }

//                //From Here forward, what rotation we need to point where we want to go (note: assumes no offset!)
//                for (int i=start;i<mNum-1;++i){
//                    
//                    Vec target = Vec(mFrame[i+1].pos() - mFrame[i].pos()).unit(); // Op::dle( Biv( linf(i) ) );  //0. Goal (Direction of Line to next joint) 
//                   // if (i>0) t = t.sp( mLink[i-1].rot() );       //1. Consider Rotary Contribution of Previous Link 
//                    Vec q = t.sp( Gen::ratio(Vec::y, mLink[i].vec().unit()) ); // And Translated Contribution of Current Link                
//                    Rot nr = Gen::ratio( q, target );        //2. What additional work it takes to turn the current integration towards Goal
//                    //R = nr * R;                         //3. Compound into R
//                   // mFrame[i].rot( R );                 //4. Apply Rotation to Frame temporarily
//                    mJoint[i].rot( nr ); //new
//                    t = t.sp( nr );                     //5. Save new integrated angle
//                }
//
//                if (bLoop){
//                  Vec target = Vec(mFrame[0].pos()-mFrame[mNum-1].pos()).unit();
//                  Vec q = t.sp( Gen::ratio( Vec::y, mLink[mNum-1].vec().unit() ) );
//                  Rot nr = Gen::ratio( q, target );
//                  mJoint[mNum-1].rot(nr);
//                }


                //new version.  
                //Current rotation
               // Vec t = mBaseFrame.y();
              //  Rot ry = mBaseFrame.rot();
                
                //Set Base Joint
//                mJoint[0].rot( !mBaseFrame.rot() * mFrame[0].rot() );
//                
//                //Reverse engineer by getting relative transformations
//                for (int i = 1; i < mNum; ++i){   
//                    Rot Rt =  (!mFrame[i-1].rot()) *  mFrame[i].rot()    ;
//                    mJoint[i].rot( Rt ); 
//                }

                //Next apply fk() and see if it all worked !. . .


#endif
