/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_frame.cpp
 *
 *    Description:  position, orientation, velocties, accelerations
 *
 *        Version:  1.0
 *        Created:  03/05/2014 20:09:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *
 * =====================================================================================
 */

#include "vsr_cga3D_frame.h"

namespace vsr{

    Frame::Frame() 
    : mPos( Ro::null(0,0,0) ), mRot(1,0,0,0), mScale(1), aBiv(.9), aVec(.9) {}
    
    Frame::Frame(VT _x, VT _y, VT _z) 
    : mPos( Ro::null(_x,_y,_z) ), mRot(1,0,0,0), mScale(1),  aBiv(.9), aVec(.9) {}

    Frame::Frame(const Vec& v, const Rotor& r, VT s ) 
    : mPos( v.null() ), mRot( r ), mScale(s), aBiv(.9), aVec(.9) {} 

    Frame::Frame(const Point& p, const Rotor& r, VT s ) 
    : mPos( p ), mRot( r ), mScale(s), aBiv(.9), aVec(.9) {} 

    Frame::Frame(const DualLine& d)
    : mScale(1),  aBiv(.9), aVec(.9) 
    {
      Motor m = Gen::mot( d );
      mPos = PAO.sp(m);
      mRot = m;
    }  

     Frame::Frame(const Motor& m ) : mPos( PAO.sp(m) ), mRot(m), mScale(1), aBiv(.9), aVec(.9) {} 

    
    /*-----------------------------------------------------------------------------
     *  Local X, Y, Z axes (calculated every time, could be stored into 3x3 image)
     *-----------------------------------------------------------------------------*/
    Vec Frame::x()  const { return Vec::x.sp( mRot ); }
    Vec Frame::y()  const { return Vec::y.sp( mRot ); }
    Vec Frame::z()  const { return Vec::z.sp( mRot ); }   

    /* Local Euclidean Planes (at origin) -- for homogenous planes see dxy(), dxz() etc */
    Biv Frame::xy()  const { return x() ^ y(); }    ///< xz euclidean bivector
    Biv Frame::xz()  const { return x() ^ z(); }    ///< xy euclidean bivector
    Biv Frame::yz()  const { return y() ^ z(); }    ///< yz euclidean bivector
    
    /* Lines along axes */
    Lin Frame::lx() const { return mPos ^ x() ^ Inf(1); }  ///< x direction direct line
    Lin Frame::ly() const { return mPos ^ y() ^ Inf(1); }  ///< y direction direct line
    Lin Frame::lz() const { return mPos ^ z() ^ Inf(1); }  ///< z direction direct line      
    
    /* Dual Lines along Axes */
    Dll Frame::dlx() const { return lx().dual(); }    ///< x direction dual line
    Dll Frame::dly() const { return ly().dual(); }    ///< y direction dual line
    Dll Frame::dlz() const { return lz().dual(); }    ///< z direction dual line
    
    /* Homogenous Planes */
    Dlp Frame::dxz() const  { return -z() <= dlx(); }    ///< xz dual plane
    Dlp Frame::dxy() const { return y() <= dlx(); }      ///< xy dual plane
    Dlp Frame::dyz() const  { return y() <= dlz(); }      ///< yz dual plane  
    
    /* Real Pair of Points around Center */
    Par Frame::px() const { return  Ro::round( ibound(), x() ); }       ///< x direction point pair around center
    Par Frame::py() const { return  Ro::round( ibound(), y() );  }      ///< y direction point pair around center
    Par Frame::pz() const { return Ro::round( ibound(),  z() );  }      ///< z direction point pair around center  

    /* Imaginary Pair */
    Par Frame::ipx() const { return  Ro::round( bound(), x() ); }       ///< x direction imaginary point pair around center
    Par Frame::ipy() const { return  Ro::round( bound(), y() );  }      ///< y direction imaginary point pair around center
    Par Frame::ipz() const { return Ro::round( bound(),  z() );  }      ///< z direction imaginary point pair around center  
   
    /* Global Tangent as Null Point Pair*/
    Par Frame::tx() const { return Par( x().copy<Tnv>() * mScale ).trs(mPos); }    ///< Null Point Pair in local x direction
    Par Frame::ty() const { return Par( y().copy<Tnv>() * mScale ).trs(mPos); }    ///< Null Point Pair in local y direction
    Par Frame::tz() const { return Par( z().copy<Tnv>() * mScale ).trs(mPos); }    ///< Null Point Pair in local z direction

    /* Real Circles */
    Cir Frame::cxy() const { return Ro::round( ibound(), xy() ); }     ///< xy circle (real, direct)
    Cir Frame::cxz() const { return Ro::round( ibound(), xz() ); }     ///< xz circle (real, direct)
    Cir Frame::cyz() const { return Ro::round( ibound(), yz() ); }     ///< yz circle (real, direct)

    /* Imaginary Circles */
    Cir Frame::icxy() const { return Ro::round( bound(), xy() ); }     ///< xy circle (imaginary, direct)
    Cir Frame::icxz() const { return Ro::round( bound(), xz() ); }     ///< xz circle (imaginary, direct)
    Cir Frame::icyz() const { return Ro::round( bound(), yz() ); }     ///< yz circle (imaginary, direct)
 
    /// Set position and orientation by motor 
    void Frame::mot(const Mot& m) { 
          mPos = PAO.sp(m); 
          mRot = m; 
      } 

    /// Generate Translation versor based on Position
    Trs Frame::trs() const { return Gen::trs(mPos); }   
          
    /// Get Absolute Motor Relative to Origin 
     Mot Frame::mot() const { 
      Mot m(trs() * rot()); 
      VT n = m.rnorm();
      return (n !=0 ) ? m / n : m ; 
    }  

    /// Get Absolute Motor Relative to Origin 
    Motor Frame::motor() const { return mot(); }
    
    /// Dual Sphere Shell
    Dls Frame::bound() const{
      return Ro::dls( mPos, mScale );
    }

    /// Dual Imaginary Sphere Shell
    Dls Frame::ibound() const{
      return Ro::dls( mPos, -mScale );
    }
    
    Dll Frame::dll() const { return Gen::log( mot() ); }                 ///< DualLine from Motor (for interpolating)  
    DualLine Frame::dualLine() const { return Gen::log( mot() ); }       ///< DualLine from Motor (for interpolating)  
        
    /// Dilate by t around center
    Frame& Frame::dilate(double t) { 
      Dls s =  bound().dil( bound(), t ) ;
      mScale = Ro::rad(s);
      return *this;
    }

    /// Dilate by t around some point p 
    Frame& Frame::dilate(const Pnt& p, double t) { 
      Dls s =  bound().dil( p, t ) ;
      mScale = Ro::rad(s);
      return *this;
    }

    /// Move and Spin 
    Frame& Frame::step(){
      move();              
      spin(); 
      return *this;
    }  
    
    /// Translation Step (translate by velocity vector)
    Frame&  Frame::move() {
      mPos = (mPos + dVec).null();//.sp( Gen::trs(dVec) );
      dVec *= aVec; 
      return *this;
    }
    
    /// Spin Step (Local Rotation) */
    Frame&  Frame::spin() {      
      mRot = Gen::rot(dBiv) * mRot;
      dBiv *= aBiv;
      return *this; 
    }

    /// Rotate around local xy and return a new frame
    Frame Frame::rotXY( VT amt) const {
      return Frame ( mPos, Gen::rot( xy() * amt )  * mRot, mScale );
    }

    //DEPRECATED
    /* /// Move by dx, dy, dz and return a new frame */
    /* Frame Frame::move( VT dx, VT dy, VT dz) const { */
    /*   return Frame ( (mPos + Vec(dx,dy,dz) ).null(), mRot, mScale ); */
    /* } */

    /// Move by dx, dy, dz and return this
    Frame& Frame::move( VT dx, VT dy, VT dz) {
      mPos = (mPos + Vec(dx,dy,dz) ).null();     
      return *this; 
    }

    /// Twist by dualLine and return this
    Frame& Frame::twist( const Dll& d ){
      return twist(Gen::mot(d));
    }

    /// Twist by motor and return this
    Frame& Frame::twist( const Mot& mot ){
      mPos = mPos.spin(mot);
      mRot = Rot(mot).unit() * mRot;//mRot.spin(mot);
      return *this;
    }

    /// Boost by point pair and return this
    Frame& Frame::boost( const Par& p){
      return boost( Gen::bst(p) );
    }

    /// Boost by boost, renormalize, and return this
    Frame& Frame::boost( const Bst& b){
      mPos = Ro::loc( mPos.spin(b) );
      mRot = mRot.spin(b);
      mRot = mRot.unit();
      return *this;
    }

    /// ABSOLUTE orient z to target, and keep y as vertical as possible
    Frame& Frame::orient( const Vec& v, bool neg ){
      Rot tRot = Gen::ratio( Vec::z * (neg?-1:1), (v-vec()).unit() );
      mRot = tRot;
      Vec ty = Op::pj( Vec::y, xy() ).unit();
      //auto cs = ty <= y();
      Rot yRot = Gen::ratio( y(), ty );
      mRot = (yRot * tRot).runit(); 
      return *this;
    }

    /// RELATIVE Rotor to orient -z to target, and keep y as vertical as possible
    Rot Frame::relOrientRot( const Vec& v, bool neg){
      Rot tRot = Gen::ratio( z() * (neg?-1:1) , (v-vec()).unit() );        //Transformation to take CURRENT z to look at v
      Vec ty = Op::pj( Vec::y, xy().spin(tRot) ).unit();                   //project Absolute y axis onto transformed xy plane
      Rot yRot = Gen::ratio( y().spin(tRot), ty );                         //transformation to take currnet TRANFORMED y to proj
      return (yRot*tRot).runit();//yRot * tRot;                          //Rotation sequence
    }

    /// Generator to orient z to target, and keep y as vertical as possible
    Biv Frame::relOrientBiv( const Vec& v, bool neg){
      return -Gen::log( relOrientRot(v,neg) );
    }
    
    /// Releative Orientation towards v by amt t
    Frame& Frame::relOrient( const Vec& v, float t, bool neg){
      mRot = Gen::rot( relOrientBiv(v,neg) * t ) * mRot;
      mRot = mRot.runit();
      return *this;
    }

    /// Relative Motor to another Frame f
    Motor Frame::relMotor( const Frame& target ){
      return target.mot() / mot();//Gen::ratio( mot(), target.mot() );
    }

    /// Relative Twist towards target frame by amt t
    Frame& Frame::relTwist( const Frame& target, float t){
      //DualLine dll = Gen::log ( target.mot() / mot() ) * t;
      return twist( Gen::ratio ( mot(), target.mot(), t) );
    }
    
    Frame Frame::moveX( VT amt ) const{
      return Frame( mPos.trs( x() * amt) , mRot, mScale );
    }
    Frame Frame::moveY( VT amt ) const {
      return Frame( mPos.trs( y() * amt) , mRot, mScale );
    }
    Frame Frame::moveZ( VT amt ) const{
      return Frame( mPos.trs( z() * amt) , mRot, mScale );
    }

}
