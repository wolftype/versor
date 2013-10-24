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

#include "vsr_frame.h"

namespace vsr {

	class Chain : public Frame {
        
		MFrame * mJoint;			///< In Socket Transformation (RDHC, etc) SET THIS directly using joint(i) 
								///  (all others follow after calling fk() method)  
								
		MFrame * mLink;			///< Relative Link to NEXT joint
		
		MFrame * mFrame;			///< Absolute frames of Joints = prevFrame  * prevLink *  joint

		int mNum;
		
		void _init(){
			for (int i = 0; i < mNum; ++i){
				Vec v(0,1.0,0);
				mLink[i].pos() = Ro::null(v);//trs( Gen::trs(v) );
				mFrame[i].scale(.2);
				
				//cout << mLink[i].pos() << endl;
			}
            
            fk();
		}
		
		public:
		
			Chain(int n = 3) : mNum(n), mFrame(NULL), mJoint(NULL), mLink(NULL) {
                
					alloc(n);
					_init();		
                  
			}       
		    
			
        ~Chain(){
            if (mFrame) delete[] mFrame;
            if (mLink) delete[] mLink;
            if (mJoint) delete[] mJoint;
        }
        
            void alloc(int n){
                mNum = n; 
				if (mNum>0){
                if (mFrame ) delete[] mFrame;
                if (mLink ) delete[] mLink;
                if (mJoint ) delete[] mJoint;   
            	mFrame = new MFrame[n];
				mLink  = new MFrame[n];
				mJoint = new MFrame[n];
				_init(); 
				}           
            }
            
            void frameSet(){
                mJoint[0].set( mPos, mRot ); fk();
            }
        
			/* GETTERS AND SETTERS */
			int num() const { return mNum; }
			MFrame& link(int k) { return mLink[k]; }						///< set k's Link To Next Joint
			MFrame& joint(int k) { return mJoint[k]; }					///< set kth joint's In Socket Transformation
			MFrame& frame(int k) { return mFrame[k]; }					///< set Absolute Displacement Motor

			MFrame link(int k) const { return mLink[k]; }				///< Get k's Link To Next joint 
			MFrame joint(int k) const { return mJoint[k]; }				///< Get kth Joint's In Socket Transformation
			MFrame frame(int k) const { return mFrame[k]; }				///< Get Absolute Displacement Motor

			
			MFrame& operator [] (int k) { return mFrame[k]; }				///< Set kth Absolute Frame
			MFrame operator [] (int k) const { return mFrame[k]; }		///< Get kth Absolute Frame
			
			/* SURROUNDS */
			/// Sphere Centered at Joint K Going Through Joint K+1 
			Dls nextDls(int k) const{
				return Ro::dls(mFrame[k].pos(), mLink[k].vec().norm() );//mFrame[k+1].pos());
			}
			/// Sphere Centered at Joint K Going Through Joint K-1
			Dls prevDls(int k) const{
				return Ro::dls(mFrame[k].pos(), mLink[k-1].vec().norm());//mFrame[k-1].pos());
			}
            /// Sphere at Point p through Joint K
            Dls goalDls(const Pnt& p, int k){
                return Ro::dls(p, mLink[k].vec().norm());
            }
			/// Sphere at point p through last link (default, or set arbitary link)
			Dls lastDls(const Pnt& p){
				return Ro::dls(p,mLink[mNum-1].vec().norm());
			}
        
            /* Possible Points */
            
            /// Pnt at position t along Link idx
            Pnt at(int idx, double t = 0.0){
                return Ro::null( Interp::linear<Vec>( mFrame[idx].vec(), mFrame[idx+1].vec(), t) );
            }
            
            MFrame& base() { return mFrame[0]; }
            MFrame& first() { return mFrame[0]; }        
            MFrame& last() { return mFrame[mNum -1]; }
			
			/// Vert xy Plane Containing Root Target Point v ( NORMALIZED )
			Dlp xy(const Pnt& p) {
				return Op::dl(frame(0).pos()^Ro::null(0,1,0)^p^Inf(1)).unit();
			}
			/// Horiz xz Plane Containing Target Point v
			Dlp xz(const Pnt& p)  {
				return Dlp(0,1,0,p[1]);
			}

			/// Dual Line Forward: Line from kth frame to kth Link
			Dll linkf(int k) { return Op::dl( mFrame[k].pos() ^ mFrame[k+1].pos() ^ Inf(1) ).runit() ; }			
			/// Dual Line Forward: Line from kth frame to kth+1 joint
			Dll linf(int k) { return Op::dl( mFrame[k].pos() ^ mFrame[k+1].pos() ^ Inf(1) ).runit() ; }
			/// Dual Line Backward: Line from kth frame to kth-1 joint
			Dll linb(int k ) { return Op::dl( mFrame[k].pos() ^ mFrame[k-1].pos() ^ Inf(1) ).runit() ; }
			/// Dual Line From Kth Joint to Input Target (Default is From Last joint)
			Dll lin(const Pnt& p ) { return Op::dl( mFrame[mNum-1].pos() ^ p ^ Inf(1) ).runit() ; }
			
			/// Forward Kinematics: Absolute Concatenations of previous frame, previous link, and current joint
            void fk() {	
                Mot mot = mJoint[0].mot();
                mFrame[0].mot( mot );
                for (int i = 1; i < mNum; ++i){		
					Mot rel =  mLink[i-1].mot() * mJoint[i].mot();//mLink[i-1].mot() * mJoint[i].mot();
					mFrame[i].mot( mFrame[i-1].mot() * rel );// * mFrame[i-1].mot() );// mFrame[i-1].mot() * rel ) ;
                }
            }	      
        
            /// Forward Kinematics: Selective From begin joint to end joint
			void fk(int begin, int end){
//                Mot mot = mJoint[0].mot();
				for (int i = begin; i < end; ++i){
					Mot m =  mFrame[i-1].mot()  * mLink[i-1].mot() * mJoint[i].mot();
					mFrame[i].mot( m );				
				}
			}

            void ik(int end, int begin){
                
            }
        
            void ifabrik(const Pnt& p, int end, int begin, double err = .01){
                //squared distance between last frame and goal p
                Sca s = mFrame[end].pos() <= p;
                
                //Temporary Goal
                Pnt goal = p;
                Pnt base = mFrame[begin].pos();
                
                int n = 0;
                
                //repeat until distance is decreased below threshold, or give up after 20 iterations
                while (s[0] > err){
                    
                    Pnt tmpGoal = goal;
                    Pnt tmpBase = base;
                    
                    //some objects
                    static Dls dls; //surround
                    static Dll dll; //line
                    static Par par; //intersection of line ^ surround
                    
                    //forward reaching
                    for (int i = end; i < begin; ++i){
                        mFrame[i].pos( tmpGoal );          //set position of ith frame
                        dls = nextDls(i);               //set boundary sphere through i-1 th frame;
                        dll = linf(i);                  //get line from tmp to i-1th frame
                        par = (dll ^ dls).dual();       //get point pair intersection of line and boundary sphere
                        tmpGoal = Ro::split(par,true);         //extract point from point pair intersection
                    }
                    
                    //backward correction
                    for (int i = begin; i > end; --i){
                        dls = prevDls(i);                   //set boundary sphere through i+1 th frame
                        dll = linb(i);                      //get line to i+1th frame;
                        par = (dll ^ dls).dual();           //get point pair intersection of line and boundary sphere
                        tmpBase = Ro::split(par,true);
                        mFrame[i-1].pos(tmpBase);             //set position of i+1th frame
                    }
                    
                    //squared distance between end frame and goal p
                    s = mFrame[ end ].pos() <= p;
                    
                    n++;  if (n > 20) {  break; }
                }
                
                //calculate joint angles
                joints();               
            }
            /// "FABRIK" Iterative Solver [see paper "Inverse Kinematic Solutions using Conformal Geometric Algebra", by Aristodou and Lasenby] feed target point, end frame and beginning frame,
            void fabrik(const Pnt& p, int end, int begin, double err = .01){
                
                //squared distance between last frame and goal p
                Sca s = mFrame[end].pos() <= p;
                
                //Temporary Goal
                Pnt goal = p;
                Pnt base = mFrame[begin].pos();
                
                int n = 0;
                
                //repeat until distance is decreased below threshold, or give up after 20 iterations
                while (s[0] > err){
                    
                    Pnt tmpGoal = goal;
                    Pnt tmpBase = base;
                    
                    static Dls dls; //surround
                    static Dll dll; //line
                    static Par par; //intersection of line ^ surround
                    
                    //backward reaching
                    for (int i = end; i > begin; --i){
                        mFrame[i].pos( tmpGoal );          //set position of ith frame
                        dls = prevDls(i);               //set boundary sphere through i-1 th frame;
                        dll = linb(i);                  //get line from tmp to i-1th frame
                        par = (dll ^ dls).dual();       //get point pair intersection of line and boundary sphere
                        tmpGoal = Ro::split(par,true);         //extract point from point pair intersection
                    }
                    
                    //forward correction
                    for (int i = begin; i < end; ++i){
                        dls = nextDls(i);                   //set boundary sphere through i+1 th frame
                        dll = linf(i);                      //get line to i+1th frame;
                        par = (dll ^ dls).dual();           //get point pair intersection of line and boundary sphere
                        tmpBase = Ro::split(par,true);
                        mFrame[i+1].pos(tmpBase);             //set position of i+1th frame
                    }
                    
                    s = mFrame[ end ].pos() <= p;
                     
                    n++;  if (n > 20) {  break; }
                }

                //calculate joint angles
                joints();

            }
        

        
            /// Derive Joint Rotations from Current Positions
            void joints(int start = 0){


                Vec t = Vec::y;
                Rot R(1,0,0,0);
                
                //Where we are in current rotation scheme
                for (int i = 0; i < start; ++i){
                    //cout << "update" << endl; 
                    t = t.sp( mFrame[i].rot() );
                }

                //From Here forward, what we need to get where we want to go
                for (int i = start; i < mNum-1; ++i){
                    //DRV of LINK
                    Vec b = Op::dle( Biv( linf(i) ) );                    
                    Rot nr = Gen::ratio( t, b ); //What it takes to turn the current integration there
                    R = nr * R;
                    mFrame[i].rot( R );
                    t = t.sp( nr ); //angle is integrated
                }
                
                //Set Base Joint
                mJoint[0].rot( mFrame[0].rot() );
                
                for (int i = 1; i < mNum; ++i){                    
                    //reverse engineer
                    Rot Rt = (!mFrame[i-1].rot()) * mFrame[i].rot();
                    mJoint[i].rot( Rt ); 
                }

            }
        
            ///Satisfy Specific Angle Constraint at frame k
            void angle(int k, double theta){
                
                Rot R =  mJoint[k].rot();
                //double t = Gen::iphi ( R );
                                
                Biv b = Biv( R ) * -1;// * ( (t > 1) ? ); // note: check Op:lg and Gen::log_rot (maybe mult by -1 there as well)

//                cout << b << endl; 
//                cout << Op::lg( R ) << endl; 
//                cout << Op :: pl( R ) << endl; 
                
                Rot nr = Gen::rot( b.unit() * theta );
                
                mJoint[k].rot( nr );
                
                //forward kinematics
                fk(k,mNum);
                
            }
    
			
			/// Derive New Relative Link Frames from current Positions
			void links(){
				for (int i = 0; i < mNum-1; ++i){
					mLink[i].pos() = Ro::null( mFrame[i+1].vec() - mFrame[i].vec() ); 
                    //mLink[i].mot( mFrame[i+1].mot() / mFrame[i].mot() ); 
				}
			} 
        
			//         virtual void drawLinkages(bool dashed = true){
			//             for (int i = 0; i < mNum-1; ++i){
			//                 (dashed) ? Glyph::DashedLine(mFrame[i].pos(), mFrame[i+1].pos() ) : Glyph::Line(mFrame[i].pos(), mFrame[i+1].pos() );
			//             }            
			//         }
			//         
			//         virtual void drawJoints(){
			//             for (int i = 0; i < mNum; ++i){
			//                 DRAW( mFrame[i] );
			//                 DRAW( mFrame[i].cxy() );
			//             }           
			//         }
			//         
			// virtual void draw(){
			// 	drawLinkages(); 
			//                 drawJoints();
			// 
			// }
	
	};

} //con::

#endif