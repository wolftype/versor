/*
 * =====================================================================================
 *
 *       Filename:  vsr_frame.h
 *
 *       Author:  Pablo Colapinto, gmail -> wolftype
 *
 * =====================================================================================
 */




#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

 
#include "vsr_cga3D_op.h"
#include "vsr_xf.h" 

 
namespace vsr {
  /*!
   *  \brief  Position and Orientation.
   *
   *  3D position and orientation along with methods for extracting local geometry.
   *  (i.e. a circle on a local xy plane, or a line in the in local y axis) 
   */
  class Frame{
    
    protected:
                         
    Point mPos;   ///< Position                                          
    Rotor mRot;   ///< Orientation
   
    VT aBiv;    ///< Rotational acceleration 
    Biv dBiv;   ///< Rotational Velocity 
    VT aVec;    ///< Translational acceleration 
    Vec dVec;   ///< Translational Velocity

    VT mScale;  ///< Scale

    public:

    // these typedefs help Frame play nice with Field class . . .
    template< class B > using BType = CGAMV<5, B >;
    typedef CGA<5> Mode;
    
    /// Default Constructor
    Frame() : mPos( Ro::null(0,0,0) ), mRot(1,0,0,0), mScale(1), aBiv(.9), aVec(.9) {}
    /// Construct from x,y,z Coordinates
    Frame(VT _x, VT _y, VT _z) : mPos( Ro::null(_x,_y,_z) ), mRot(1,0,0,0), mScale(1),  aBiv(.9), aVec(.9) {}
    /// Construct from Point, Rotor, and Scale
    Frame(const Point& p, const Rotor& r = Rot(1,0,0,0), VT s = 1 ) : mPos( p ), mRot( r ), mScale(s), aBiv(.9), aVec(.9) {}    
    /// Construct from Dual Line (log of motor relative to origin)
    Frame(const DualLine& d ) : mScale(1),  aBiv(.9), aVec(.9) {
      Motor m = Gen::mot( d );
      mPos = PAO.sp(m);
      mRot = m;
  
    }  
    /// Construct from Motor (relative to origin)
    Frame(const Motor& m ) : mPos( PAO.sp(m) ), mRot(m), mScale(1), aBiv(.9), aVec(.9) {} 

    /// Set Position from Point p
    Frame& pos( Pnt p ) { mPos = p; return *this; } 
    /// Set Position from x,y,z coordinates
    Frame& pos( VT _x, VT _y, VT _z) { mPos = Ro::null(_x,_y,_z); return *this; } 
    //Frame& set( VT _x, VT _y, VT _z) { mPos = Ro::null(_x,_y,_z); return *this; }
    /// Set Position and Orientation from Point and Rotor
    Frame& set( Pnt p, Rot r = Rot(1,0,0,0) ) { mPos = p; mRot = r; return *this; } 
    /// Set Scale
    Frame& scale( VT s ) { mScale = s; return *this; }    
    /// Reset to Origin
    Frame& reset() { mPos = Ro::null(0,0,0); mRot = Rot(1,0,0,0); return *this; }
    
    /// Get Scale
    VT scale() const { return mScale; }

    /// Get 4x4 Rotation Matrix
    gfx::Mat4f image(){ return Xf::mat(mRot); }
    /// Get Rotor
    Rotor rot() const { return mRot; }
    Rotor rotor() const { return mRot; }

    ///  Get / Set Rotor by reference 
    Rot& rot() { return mRot; } 
    // Get / Set Rotor by reference
    Rot& rotor() { return mRot; }
     
    /// Set rotor with rotor 
    Frame& rot( const Rot& r) { mRot = r; return *this; }  
    /// Set rotor with bivector generator 
    Frame& rot( const Biv& B) { mRot = Gen::rot(B); return *this; }      
    /// Transpose to quaternionic representation
    Rot quat() const { return Rot( mRot[0], -mRot[3], mRot[2], mRot[1] ); }
    /// Get Position
    Point pos() const { return mPos; }  
    /// Get / Set Position by Reference
    Point& pos() { return mPos; }  
    /// Get Euclidean Vector of position
    Vec vec() const { return mPos; }   
    

    Vec x()  const { return Vec::x.sp( mRot ); }
    Vec y()  const { return Vec::y.sp( mRot ); }
    Vec z()  const { return Vec::z.sp( mRot ); }   

    //Positve and negative axis
    Point posX() const { return (mPos + x()).null(); }
    Point negX() const { return (mPos - x()).null(); } 
    Point posY() const { return (mPos + y()).null(); }
    Point negY() const { return (mPos - y()).null(); }
    Point posZ() const { return (mPos + z()).null(); }
    Point negZ() const { return (mPos - z()).null(); }
    
    Biv xy()  const { return x() ^ y(); }    ///< xz euclidean bivector
    Biv xz()  const { return x() ^ z(); }    ///< xy euclidean bivector
    Biv yz()  const { return y() ^ z(); }    ///< yz euclidean bivector
    
    Lin lx() const { return mPos ^ x() ^ Inf(1); }  ///< x direction direct line
    Lin ly() const { return mPos ^ y() ^ Inf(1); }  ///< y direction direct line
    Lin lz() const { return mPos ^ z() ^ Inf(1); }  ///< z direction direct line      
    
    Dll dlx() const { return lx().dual(); }    ///< x direction dual line
    Dll dly() const { return ly().dual(); }    ///< y direction dual line
    Dll dlz() const { return lz().dual(); }    ///< z direction dual line
    
    /* Homogenous Planes in Conformal Space */
    Dlp dxz() const  { return -z() <= dlx(); }    ///< xz dual plane
    Dlp dxy() const { return y() <= dlx(); }      ///< xy dual plane
    Dlp dyz() const  { return y() <= dlz(); }      ///< yz dual plane  
    

    /* Real Pair */
    Par px() const { return  Ro::round( ibound(), x() ); }       ///< x direction point pair around center
    Par py() const { return  Ro::round( ibound(), y() );  }      ///< y direction point pair around center
    Par pz() const { return Ro::round( ibound(),  z() );  }      ///< z direction point pair around center  

    /* Imaginary Pair */
    Par ipx() const { return  Ro::round( bound(), x() ); }       ///< x direction imaginary point pair around center
    Par ipy() const { return  Ro::round( bound(), y() );  }      ///< y direction imaginary point pair around center
    Par ipz() const { return Ro::round( bound(),  z() );  }      ///< z direction imaginary point pair around center  
   
    /* Global Tangent as Null Point Pair*/
    Par tx() const { return Par( x().copy<Tnv>() * mScale ).trs(mPos); }    ///< Null Point Pair in local x direction
    Par ty() const { return Par( y().copy<Tnv>() * mScale ).trs(mPos); }    ///< Null Point Pair in local y direction
    Par tz() const { return Par( z().copy<Tnv>() * mScale ).trs(mPos); }    ///< Null Point Pair in local z direction

    /* Real Circles */
    Cir cxy() const { return Ro::round( ibound(), xy() ); }     ///< xy circle (real, direct)
    Cir cxz() const { return Ro::round( ibound(), xz() ); }     ///< xz circle (real, direct)
    Cir cyz() const { return Ro::round( ibound(), yz() ); }     ///< yz circle (real, direct)

    /* Imaginary Circles */
    Cir icxy() const { return Ro::round( bound(), xy() ); }     ///< xy circle (imaginary, direct)
    Cir icxz() const { return Ro::round( bound(), xz() ); }     ///< xz circle (imaginary, direct)
    Cir icyz() const { return Ro::round( bound(), yz() ); }     ///< yz circle (imaginary, direct)

    Vec right() const { return x(); }
    Vec up() const { return y(); }   
    Vec forward() const { return -z(); }  
  
    /// Set position and orientation by motor 
    void mot(const Mot& m) { 
          mPos = PAO.sp(m); 
          mRot = m; 
      } 

    /// Generate Translation versor based on Position
    Trs trs() const { return Gen::trs(mPos); }   
          
    /// Get Absolute Motor Relative to Origin 
     Mot mot() const { 
      Mot m(trs() * rot()); 
      VT n = m.rnorm();
      return (n !=0 ) ? m / n : m ; 
    }  

    /// Get Absolute Motor Relative to Origin 
    Motor motor() const { return mot(); }
    
    /// Dual Sphere Shell
    Dls bound() const{
      return Ro::dls( mPos, mScale );
    }

    /// Dual Imaginary Sphere Shell
    Dls ibound() const{
      return Ro::dls( mPos, -mScale );
    }
    
    Dll dll() const { return Gen::log( mot() ); }                 ///< DualLine from Motor (for interpolating)  
    DualLine dualLine() const { return Gen::log( mot() ); }       ///< DualLine from Motor (for interpolating)  
        
    /// Dilate by t around center
    Frame& dilate(double t) { 
      Dls s =  bound().dil( bound(), t ) ;
      mScale = Ro::rad(s);
      return *this;
    }

    VT& ab() { return aBiv; }             ///< Set bivector accelerator (rotational)
    VT ab() const { return aBiv; }        ///< Get bivector accelerator (rotational)
    VT& ax() { return aVec; }             ///< Set vector accelerator
    VT ax() const { return aVec; }        ///< Get vector accelerator
    Biv& db() { return dBiv; }            ///< Set bivector velocity (rotational)
    Biv db() const { return dBiv; }       ///< Get bivector velocity (rotational)
    Vec& dx() { return dVec; }            ///< Set vector velocity
    Vec dx() const { return dVec; }       ///< Get vector velocity
    
    /// Move and Spin 
    Frame& step(){
      move();              
      spin(); 
      return *this;
    }  
    
    /// Translation Step (translate by velocity vector)
    Frame&  move() {
      mPos = (mPos + dVec).null();//.sp( Gen::trs(dVec) );
      dVec *= aVec; 
      return *this;
    }
    
    /// Spin Step (Local Rotation) */
    Frame&  spin() {      
      mRot = Gen::rot(dBiv) * mRot;
      dBiv *= aBiv;
      return *this; 
    }

    /// Rotate around local xy and return a new frame
    Frame rotXY( VT amt) const {
      return Frame ( mPos, Gen::rot( xy() * amt )  * mRot, mScale );
    }

    /// Move by dx, dy, dz and return a new frame
    Frame move( VT dx = 0.0, VT dy= 0.0, VT dz = 0.0) const {
      return Frame ( (mPos + Vec(dx,dy,dz) ).null(), mRot, mScale );
    }

    Frame moveX( VT amt ) const{
      return Frame( mPos.trs( x() * amt) , mRot, mScale );
    }
    Frame moveY( VT amt ) const {
      return Frame( mPos.trs( y() * amt) , mRot, mScale );
    }
    Frame moveZ( VT amt ) const{
      return Frame( mPos.trs( z() * amt) , mRot, mScale );
    }


  };
   
} //vsr::

#endif
