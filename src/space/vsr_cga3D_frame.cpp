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

#include "form/vsr_cga3D_frame.h"

namespace vsr{ namespace cga {

    Frame::Frame() 
    : mPos( round::point(0,0,0) ), mRot(1,0,0,0), mScale(1), aBiv(.9), aVec(.9) {}
    
    Frame::Frame(VSR_PRECISION  _x, VSR_PRECISION  _y, VSR_PRECISION  _z) 
    : mPos( round::point(_x,_y,_z) ), mRot(1,0,0,0), mScale(1),  aBiv(.9), aVec(.9) {}

    Frame::Frame(const Vec& v, const Rotor& r, VSR_PRECISION s ) 
    : mPos( v.null() ), mRot( r ), mScale(s), aBiv(.9), aVec(.9) {} 
   
    Frame::Frame(const Point& p, const Rotor& r, VSR_PRECISION s ) 
    : mPos( p ), mRot( r ), mScale(s), aBiv(.9), aVec(.9) {} 

    Frame::Frame(const DualLine& d)
    : mScale(1),  aBiv(.9), aVec(.9) 
    {
      Motor m = gen::mot( d );
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
    Par Frame::px() const { return  round::produce( ibound(), x() ); }       ///< x direction point pair aproduce center
    Par Frame::py() const { return  round::produce( ibound(), y() );  }      ///< y direction point pair aproduce center
    Par Frame::pz() const { return round::produce( ibound(),  z() );  }      ///< z direction point pair aproduce center  

    /* Imaginary Pair */
    Par Frame::ipx() const { return  round::produce( bound(), x() ); }       ///< x direction imaginary point pair aproduce center
    Par Frame::ipy() const { return  round::produce( bound(), y() );  }      ///< y direction imaginary point pair aproduce center
    Par Frame::ipz() const { return round::produce( bound(),  z() );  }      ///< z direction imaginary point pair aproduce center  
   
    /* Global Tangent as Null Point Pair*/
    Par Frame::tx() const { return Par( x().copy<Tnv>() * mScale ).trs(mPos); }    ///< Null Point Pair in local x direction
    Par Frame::ty() const { return Par( y().copy<Tnv>() * mScale ).trs(mPos); }    ///< Null Point Pair in local y direction
    Par Frame::tz() const { return Par( z().copy<Tnv>() * mScale ).trs(mPos); }    ///< Null Point Pair in local z direction

    /* Real Circles */
    Cir Frame::cxy() const { return round::produce( ibound(), xy() ); }     ///< xy circle (real, direct)
    Cir Frame::cxz() const { return round::produce( ibound(), xz() ); }     ///< xz circle (real, direct)
    Cir Frame::cyz() const { return round::produce( ibound(), yz() ); }     ///< yz circle (real, direct)

    /* Imaginary Circles */
    Cir Frame::icxy() const { return round::produce( bound(), xy() ); }     ///< xy circle (imaginary, direct)
    Cir Frame::icxz() const { return round::produce( bound(), xz() ); }     ///< xz circle (imaginary, direct)
    Cir Frame::icyz() const { return round::produce( bound(), yz() ); }     ///< yz circle (imaginary, direct)

    /// Set position and orientation by motor 
    void Frame::mot(const Mot& m) { 
          mPos = PAO.sp(m); 
          mRot = m; 
      } 

    /// Generate Translation versor based on Position
    Trs Frame::trs() const { return gen::trs(mPos); }   
          
    /// Get Absolute Motor Relative to Origin 
     Mot Frame::mot() const { 
      Mot m(trs() * rot()); 
      VSR_PRECISION  n = m.rnorm();
      return (n !=0 ) ? m / n : m ; 
    }  

    /// Get Absolute Motor Relative to Origin 
    Motor Frame::motor() const { return mot(); }
    
    /// Dual Sphere Shell
    Dls Frame::bound() const{
      return round::dls_pnt( mPos, mScale );
    }

    /// Dual Imaginary Sphere Shell
    Dls Frame::ibound() const{
      return round::dls_pnt( mPos, -mScale );
    }
    
    Dll Frame::dll() const { return gen::log( mot() ); }                 ///< DualLine from Motor (for interpolating)  
    DualLine Frame::dualLine() const { return gen::log( mot() ); }       ///< DualLine from Motor (for interpolating)  
        
    /// Dilate by t aproduce center
    Frame& Frame::dilate(double t) { 
      Dls s =  bound().dil( bound(), t ) ;
      mScale = round::rad(s);
      return *this;
    }

    /// Dilate by t around some point p 
    Frame& Frame::dilate(const Pnt& p, double t) { 
      Dls s =  bound().dil( p, t ) ;
      mScale = round::rad(s);
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
      mPos = (mPos + dVec).null();//.sp( gen::trs(dVec) );
      dVec *= aVec; 
      return *this;
    }
    
    /// Spin Step (Local Rotation) */
    Frame&  Frame::spin() {      
      mRot = gen::rot(dBiv) * mRot;
      dBiv *= aBiv;
      return *this; 
    }

    /// Rotate aproduce local xy and return a new frame
    Frame Frame::rotXY( VSR_PRECISION  amt) const {
      return Frame ( mPos, gen::rot( xy() * amt )  * mRot, mScale );
    }

    //DEPRECATED
    /* /// Move by dx, dy, dz and return a new frame */
    /* Frame Frame::move( VSR_PRECISION dx, VSR_PRECISION dy, VSR_PRECISION dz) const { */
    /*   return Frame ( (mPos + Vec(dx,dy,dz) ).null(), mRot, mScale ); */
    /* } */

    /// Move by dx, dy, dz and return this
    Frame& Frame::move( VSR_PRECISION dx, VSR_PRECISION dy, VSR_PRECISION dz) {
      mPos = (mPos + Vec(dx,dy,dz) ).null();     
      return *this; 
    }

    Frame& Frame::move( const Vec& v){
      mPos = (mPos + v ).null();     
      return *this; 
    }

    /// Rotate by Bivector xy, xz, yz
    Frame& Frame::rotate( const Biv& b){
      return rotate( gen::rot(b) );
    } 

    /// Rotate by Rotor R
    Frame& Frame::rotate( const Rot& r){
      mRot = mRot.spin(r);
      return *this;
    }

    /// Twist by dualLine and return this
    Frame& Frame::twist( const Dll& d ){
      return twist(gen::mot(d));
    }

    /// Twist by motor and return this
    Frame& Frame::twist( const Mot& mot ){
      mPos = mPos.spin(mot);
      mRot = Rot(mot).unit() * mRot;//mRot.spin(mot);
      return *this;
    }

    /// Boost by point pair and return this
    Frame& Frame::boost( const Par& p){
      return boost( gen::bst(p) );
    }

    /// Boost by boost, renormalize, and return this
    Frame& Frame::boost( const Bst& b){
      mPos = round::loc( mPos.spin(b) );
      mRot = mRot.spin(b);
      mRot = mRot.unit();
      return *this;
    }

    /// ABSOLUTE orient z to target, and keep y as vertical as possible
    Frame& Frame::orient( const Vec& v, bool pos ){
      Rot tRot = gen::ratio( Vec::z * (pos?1:-1), (v-vec()).unit() );
      mRot = tRot;
      Vec ty = op::pj( Vec::y, xy() ).unit();
      //auto cs = ty <= y();
      Rot yRot = gen::ratio( y(), ty );
      mRot = (yRot * tRot).runit(); 
      return *this;
    }

    // Absolute orient z towrads target by amt t
    Frame& Frame::orient(const Vec& v, float t, bool pos){
      Rot tRot = gen::ratio( Vec::z * (pos?1:-1), (v-vec()).unit() );
      Vec ty = op::pj( Vec::y, Biv::xy.spin(tRot) ).unit();
      Rot yRot = gen::ratio( Vec::y.spin(tRot), ty );
      mRot = gen::rot( -gen::log( (yRot * tRot).runit() ) * t ); 

      return *this;

    }

    // Absolute orient x towrads target by amt t
    Frame& Frame::orientX(const Vec& v, float t, bool pos){
      Rot tRot = gen::ratio( Vec::x * (pos?1:-1), (v-vec()).unit() );
      Vec ty = op::pj( Vec::y, Biv::xy.spin(tRot) ).unit();
      Rot yRot = gen::ratio( Vec::y.spin(tRot), ty );
      mRot = gen::rot( -gen::log( (yRot * tRot).runit() ) * t ); 

      return *this;

    }

    /// RELATIVE Rotor to orient -z to target, and keep y as vertical as possible
    Rot Frame::relOrientRot( const Vec& v, bool pos){
      Rot tRot = gen::ratio( z() * (pos?1:-1) , (v-vec()).unit() );        //Transformation to take CURRENT z to look at v
      Vec ty = op::pj( Vec::y, xy().spin(tRot) ).unit();                   //project Absolute y axis onto transformed xy plane
      Rot yRot = gen::ratio( y().spin(tRot), ty );                         //transformation to take currnet TRANFORMED y to proj
      return (yRot*tRot).runit();//yRot * tRot;                          //Rotation sequence
    }

    /// Generator to orient z to target, and keep y as vertical as possible
    Biv Frame::relOrientBiv( const Vec& v, bool pos){
      return -gen::log( relOrientRot(v,pos) );
    }
    
    /// Releative Orientation towards v by amt t
    Frame& Frame::relOrient( const Vec& v, float t, bool pos){
      mRot = gen::rot( relOrientBiv(v,pos) * t ) * mRot;
      mRot = mRot.runit();
      return *this;
    }

    /// Relative Motor to another Frame f
    Motor Frame::relMotor( const Frame& target ){
      return target.mot() / mot();//gen::ratio( mot(), target.mot() );
    }

    /// Relative Twist towards target frame by amt t
    Frame& Frame::relTwist( const Frame& target, float t){
      //DualLine dll = gen::log ( target.mot() / mot() ) * t;
      return twist( gen::ratio ( mot(), target.mot(), t) );
    }
    
    Frame Frame::moveX( VSR_PRECISION amt ) const{
      return Frame( mPos.trs( x() * amt) , mRot, mScale );
    }
    Frame Frame::moveY( VSR_PRECISION  amt ) const {
      return Frame( mPos.trs( y() * amt) , mRot, mScale );
    }
    Frame Frame::moveZ( VSR_PRECISION  amt ) const{
      return Frame( mPos.trs( z() * amt) , mRot, mScale );
    }

} } //vsr::cga::
