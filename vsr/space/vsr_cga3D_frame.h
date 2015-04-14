/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_frame.h
 *
 *    Description:  position, orientation, velocities, accelerations
 *
 *        Version:  1.0
 *        Created:  03/05/2014 20:11:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  vsr_cga3D_frame_INC
#define  vsr_cga3D_frame_INC


 
#include "vsr_cga3D_op.h"
#include "vsr_cga3D_xf.h" 

 
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
   
    VSR_PRECISION aBiv;    ///< Rotational acceleration 
    Biv dBiv;   ///< Rotational Velocity 
    VSR_PRECISION aVec;    ///< Translational acceleration 
    Vec dVec;   ///< Translational Velocity

    VSR_PRECISION mScale;  ///< Scale

    public:

    // these typedefs help Frame play nice with Field class . . .
    template< class B > using BType = CGAMV<5, B >;
    typedef CGA<5> Mode;
    
    /// Default Constructor
    Frame();
    /// Construct from x,y,z Coordinates
    Frame(VSR_PRECISION _x, VSR_PRECISION _y, VSR_PRECISION _z);
    /// Construct from Point, Rotor, and Scale
    Frame(const Point& p, const Rotor& r = Rot(1,0,0,0), VSR_PRECISION s = 1 );
    /// Construct from Dual Line (log of motor relative to origin)
    Frame(const DualLine& d );
    /// Construct from Motor (relative to origin)
    Frame(const Motor& m );

    /// Set Position from Point p
    Frame& pos( Pnt p ) { mPos = p; return *this; } 
    /// Set Position from x,y,z coordinates
    Frame& pos( VSR_PRECISION _x, VSR_PRECISION _y, VSR_PRECISION _z) { mPos = Ro::null(_x,_y,_z); return *this; } 
    //Frame& set( VSR_PRECISION _x, VSR_PRECISION _y, VSR_PRECISION _z) { mPos = Ro::null(_x,_y,_z); return *this; }
    /// Set Position and Orientation from Point and Rotor
    Frame& set( Pnt p, Rot r = Rot(1,0,0,0) ) { mPos = p; mRot = r; return *this; } 
    /// Set Scale
    Frame& scale( VSR_PRECISION s ) { mScale = s; return *this; }    
    /// Reset to Origin
    Frame& reset() { mPos = Ro::null(0,0,0); mRot = Rot(1,0,0,0); return *this; }
    
    /// Get Scale
    VSR_PRECISION scale() const { return mScale; }

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
    

    Vec x() const;    ///< Local x 
    Vec y() const;    ///< Local y
    Vec z() const;    ///< Local z

    //Positve and negative axis
    Point posX() const { return (mPos + x()).null(); }
    Point negX() const { return (mPos - x()).null(); } 
    Point posY() const { return (mPos + y()).null(); }
    Point negY() const { return (mPos - y()).null(); }
    Point posZ() const { return (mPos + z()).null(); }
    Point negZ() const { return (mPos - z()).null(); }
    
    Biv xy()  const;    ///< xz euclidean bivector
    Biv xz()  const;    ///< xy euclidean bivector
    Biv yz()  const;    ///< yz euclidean bivector
    
    Lin lx() const;     ///< x direction direct line
    Lin ly() const;     ///< y direction direct line
    Lin lz() const;     ///< z direction direct line      
    
    Dll dlx() const;    ///< x direction dual line
    Dll dly() const;    ///< y direction dual line
    Dll dlz() const;    ///< z direction dual line
    
    /* Homogenous Planes in Conformal Space */
    Dlp dxz() const;    ///< xz dual plane
    Dlp dxy() const;    ///< xy dual plane
    Dlp dyz() const;    ///< yz dual plane  
    

    /* Real Pair */
    Par px() const;       ///< x direction point pair around center
    Par py() const;       ///< y direction point pair around center
    Par pz() const;       ///< z direction point pair around center  

    /* Imaginary Pair */
    Par ipx() const;      ///< x direction imaginary point pair around center
    Par ipy() const;      ///< y direction imaginary point pair around center
    Par ipz() const;      ///< z direction imaginary point pair around center  
   
    /* Global Tangent as Null Point Pair*/
    Par tx() const;       ///< Null Point Pair in local x direction
    Par ty() const;       ///< Null Point Pair in local y direction
    Par tz() const;       ///< Null Point Pair in local z direction

    /* Real Circles */
    Cir cxy() const;      ///< xy circle (real, direct)
    Cir cxz() const;      ///< xz circle (real, direct)
    Cir cyz() const;      ///< yz circle (real, direct)

    /* Imaginary Circles */
    Cir icxy() const;     ///< xy circle (imaginary, direct)
    Cir icxz() const;     ///< xz circle (imaginary, direct)
    Cir icyz() const;     ///< yz circle (imaginary, direct)

    Vec right() const { return x(); }
    Vec up() const { return y(); }   
    Vec forward() const { return -z(); }  
  
    /// Set position and orientation by motor 
    void mot(const Mot& m);

    /// Generate Translation versor based on Position
    Trs trs() const;
          
    /// Get Absolute Motor Relative to Origin 
    Mot mot() const;

    /// Get Absolute Motor Relative to Origin 
    Motor motor() const;
    
    /// Dual Sphere Shell
    Dls bound() const;

    /// Dual Imaginary Sphere Shell
    Dls ibound() const;
    
    Dll dll() const;
    DualLine dualLine() const;
        
    
    VSR_PRECISION& ab() { return aBiv; }             ///< Set bivector accelerator (rotational)
    VSR_PRECISION ab() const { return aBiv; }        ///< Get bivector accelerator (rotational)
    VSR_PRECISION& ax() { return aVec; }             ///< Set vector accelerator
    VSR_PRECISION ax() const { return aVec; }        ///< Get vector accelerator
    Biv& db() { return dBiv; }            ///< Set bivector velocity (rotational)
    Biv db() const { return dBiv; }       ///< Get bivector velocity (rotational)
    Vec& dx() { return dVec; }            ///< Set vector velocity
    Vec dx() const { return dVec; }       ///< Get vector velocity
    
    /// Move and Spin 
    Frame& step();
  
  
    /// Translation Step (translate by velocity vector)
    Frame& move();
    
    /// Spin Step (Local Rotation) */
    Frame& spin();

    /// Dilate by t around center
    Frame& dilate(double t);

    /// Rotate around local xy and return a new frame
    Frame rotXY( VSR_PRECISION amt) const;

    /// Move by dx, dy, dz and return a new frame
    Frame move( VSR_PRECISION dx = 0.0, VSR_PRECISION dy= 0.0, VSR_PRECISION dz = 0.0) const;

    Frame moveX( VSR_PRECISION amt ) const;
    Frame moveY( VSR_PRECISION amt ) const;
    Frame moveZ( VSR_PRECISION amt ) const;


  };
   
} //vsr::


#endif   /* ----- #ifndef vsr_cga3D_frame_INC  ----- */
