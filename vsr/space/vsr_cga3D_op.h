#ifndef CGA3D_OPERATIONS_H_INCLUDED
#define CGA3D_OPERATIONS_H_INCLUDED  


// Specialized 3D CONFORMAL OPERATIONS (assumes 3D . . . )

#include <math.h> 
#include <vector>  

#include "vsr_cga3D_types.h"  

#include "detail/vsr_generic_op.h"
#include "util/vsr_constants.h" 
#include "util/vsr_math.h"

namespace vsr{
                     

namespace op{
  
  using namespace cga;//::types; 
    
  Rot AA( const Vec& s);                
  Rot AA( const Dlp& s);
  Rot AA( const Cir& s);
  Rot AA( const Biv& s);
  
  Vec Pos( const Dlp& s); 
  Pnt Pos( const Cir& s);       
  
};


namespace gen{               
  

   using namespace cga;//::types;  
  /*-----------------------------------------------------------------------------
   *  QUAT helpers
   *-----------------------------------------------------------------------------*/
  /*! Generate a Rotor (i.e quaternion) from spherical coordinates
        @param[in] theta in xz plane from (1,0,0) in range [0,PI]
        @param[in] phi in rotated xy plane in range []
    */
      Rot rot(double theta, double phi);                

  /*! Generate a Rotor (i.e. quaternion) from Euler angles
      @param[in] yaw in xz plane
      @param[in] pitch in (transformed) yz plane
      @param[in] roll in (transformed) xy plane
   */
      Rot rot( double yaw, double pitch, double roll);
      


   /*-----------------------------------------------------------------------------
    *  TWISTS (Motors, Plücker, etc)
    *-----------------------------------------------------------------------------*/
   /*! Generate a Motor from a Dual Line Axis
       @param Dual Line Generator (the axis of rotation, including pitch and period)
   */  
    Mot mot( const Dll& dll);

    /*! Dual Line Generator from a Motor 
        @param Motor m (a concatenation of rotation and translation)
    */  
    Dll log( const Mot& m);

    /*! Dual Line Generator of Motor That Twists Dual Line a to Dual Line b;

    */ 
    Dll log(const Dll& a, const Dll& b, VSR_PRECISION t = 1.0);

    /*! Generate Motor That Twists Dual Line a to Dual Line b;
    */
    Mot ratio( const Dll& a, const Dll& b, VSR_PRECISION t = 1.0);

    /*! Generate Motor That Twists Motor a to Motor b;
    */
    Mot ratio( const Mot& a, const Mot& b, VSR_PRECISION t);


    /*-----------------------------------------------------------------------------
     *  BOOSTS (Transversions, Conformal Rotors)
     *-----------------------------------------------------------------------------*/
 
      /*! Generate a Translated Transversion 
          @param Tangent Direction
          @param Position in space
          @param scalar amt (typically 0 or 1)
      */
      template <class A, class T>
      Bst bst(const A& tnv, const Vec& drv, T t){
          Par s = Par( tnv.template copy<Tnv>() ).sp( gen::trs(drv) ); 
          return gen::bst(s * t);
      }
 
      

    /*! Generate Simple Boost rotor from ratio of two dual spheres
        calculates SQUARE ROOT (normalizes 1+R)
    */
   Bst ratio( const DualSphere& a, const DualSphere& b);
   
  /*! atanh2 function for logarithm of general rotors*/
   Pair atanh2( const Pair& p, VSR_PRECISION cs);
       
  /*! Log of a simple rotor (uses atanh2) */
   Pair log( const Bst& b);


   /*!  Generate Conformal Transformation from circle a to circle b
        uses square root method of Dorst et Valkenburg, 2011
   */
   Con ratio( const Circle& a, const Circle& b);
   
   /*!  Generate Conformal Transformation from pair a to pair b
        uses square root method of Dorst et Valkenburg, 2011
   */
   Con ratio( const Pair& a, const Pair& b);//{ return ratio( a.dual(), b.dual() ); }
   
  /*! Bivector Split
        Takes a general bivector and splits  it into commuting pairs
        will give sinh(B+-)
   */
   vector<Pair> split(const Pair& par);   
   
   /*! Split Log of General Conformal Rotor */
   vector<Pair> log( const Con& rot);

   /*! Split Log from a ratio of two Circles */
   vector<Pair> log( const Circle& ca, const Circle& cb);
   
   /*! Split Log from a ratio of two Circles */
   vector<Pair> log( const Pair& ca, const Pair& cb);
   
   
  /*! General Conformal Transformation from a split log*/
   Con con( const vector<Pair>& log, VSR_PRECISION amt);


};

template<class Algebra, class B> template <class A>
MV<Algebra,B> MV<Algebra,B>::mot( const MV<Algebra,A>& t) const{
     return this -> sp ( gen::mot(t) );  
} 
template<class Algebra, class B> template <class A>
MV<Algebra,B> MV<Algebra,B>::motor( const MV<Algebra,A>& t) const{
     return this -> sp ( gen::mot(t) );  
} 
template<class Algebra, class B> template <class A>
MV<Algebra,B> MV<Algebra,B>::twist( const MV<Algebra,A>& t) const{
     return this -> sp ( gen::mot(t) );  
} 


/* namespace Ro { */

/*    template<class V> */
/*    inline Point point( const Lin& lin, const V& v){ */
/*      return round::null( Fl::loc( lin, v, false) ); */
/*    } */          

/* }; */
    

//extern template class CGAMV<5,   CGA<5>::Sca   >;  
//extern template class CGAMV<5,   CGA<5>::Vec   >;  
//extern template class CGAMV<5,   CGA<5>::Vec2D >;  
//extern template class CGAMV<5,   CGA<5>::Biv   >;  
//extern template class CGAMV<5,   CGA<5>::Tri   >;  
//extern template class CGAMV<5,   CGA<5>::Rot   >;  
//extern template class CGAMV<5,   CGA<5>::Mnk   >;  
//extern template class CGAMV<5,   CGA<5>::Pss   >;     
//extern template class CGAMV<5,   CGA<5>::Pnt   >;  
//extern template class CGAMV<5,   CGA<5>::Par   >; 
//extern template class CGAMV<5,   CGA<5>::Cir   >; 
//extern template class CGAMV<5,   CGA<5>::Sph   >;  
//extern template class CGAMV<5,   CGA<5>::Dll   >; 
//extern template class CGAMV<5,   CGA<5>::Lin   >; 
//extern template class CGAMV<5,   CGA<5>::Flp   >; 
//extern template class CGAMV<5,   CGA<5>::Pln   >; 
//extern template class CGAMV<5,   CGA<5>::Dlp   >; 
//extern template class CGAMV<5,   CGA<5>::Trs   >;   
//extern template class CGAMV<5,   CGA<5>::Mot   >; 
//extern template class CGAMV<5,   CGA<5>::Trv   >; 
//extern template class CGAMV<5,   CGA<5>::Bst   >; 
//extern template class CGAMV<5,   CGA<5>::Dil   >; 
//extern template class CGAMV<5,   CGA<5>::Tsd   >;

//extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Rot& )const;                         
//extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Trs& )const;                         
//extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Mot& )const;                       
//extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Bst& )const;
//extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Tsd& )const;
//extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Dil& )const;
//                                                         
//extern template Par CGAMV<5, CGA<5>::Par>::sp( const Rot& )const;
//extern template Par CGAMV<5, CGA<5>::Par>::sp( const Trs& )const;
//extern template Par CGAMV<5, CGA<5>::Par>::sp( const Mot& )const;
//extern template Par CGAMV<5, CGA<5>::Par>::sp( const Bst& )const;
//extern template Par CGAMV<5, CGA<5>::Par>::sp( const Tsd& )const;
//extern template Par CGAMV<5, CGA<5>::Par>::sp( const Dil& )const;
//                                                       
//extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Rot& )const;
//extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Trs& )const;
//extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Mot& )const;
//extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Bst& )const;
//extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Tsd& )const;
//extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Dil& )const;
//                                                         
//extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Rot& )const;
//extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Trs& )const;
//extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Mot& )const;
//extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Bst& )const;
//extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Tsd& )const;
//extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Dil& )const; 
//
//extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Rot& )const;                         
//extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Trs& )const;                         
//extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Mot& )const;                       
//extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Bst& )const;
//extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Tsd& )const;
//extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Dil& )const;
//                                                         
//extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Rot& )const;
//extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Trs& )const;
//extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Mot& )const;
//extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Bst& )const;
//extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Tsd& )const;
//extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Dil& )const;
//                                                       
//extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Rot& )const;
//extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Trs& )const;
//extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Mot& )const;
//extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Bst& )const;
//extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Tsd& )const;
//extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Dil& )const;
//                                                         
//extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Rot& )const;
//extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Trs& )const;
//extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Mot& )const;
//extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Bst& )const;
//extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Tsd& )const;
//extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Dil& )const;  
//                                                           
//
//                                                           
//extern template Rot gen::rot( const Biv& );                         
//
//extern template Biv gen::log( const Rot& );
//extern template Par gen::log( const Bst& ); 
//extern template Trv gen::trv( const Vec& );
//extern template Trv gen::trv( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ); 
//extern template Trv gen::trv( const Drv& );
//extern template Trv gen::trv( const Tnv& ); 
//extern template Trs gen::trs( const Vec& );
//extern template Trs gen::trs( const Drv& ); 
//extern template Trs gen::trs( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ); 
//extern template Dil gen::dil( VSR_PRECISION ); 
//extern template Tsd gen::dil( const Pnt&, VSR_PRECISION ); 
//extern template Bst gen::bst( const Par& ); 
//
//extern template Rot gen::ratio( const Vec&, const Vec& );
//extern template Rot gen::aa( const Rot& r);
//
//   
//extern template Par round::round( const Dls&, const Vec& ); 
//extern template Cir round::round( const Dls&, const Biv& );  
//
//extern template Pnt round::null( const Vec& );
//extern template Pnt round::null( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z);
//
//extern template VSR_PRECISION round::size( const Pnt&, bool); 
//extern template VSR_PRECISION round::size( const Par&, bool); 
//extern template VSR_PRECISION round::size( const Cir&, bool); 
//extern template VSR_PRECISION round::size( const Sph&, bool); 
//
//extern template VSR_PRECISION round::rad( const Pnt&); 
//extern template VSR_PRECISION round::rad( const Par&); 
//extern template VSR_PRECISION round::rad( const Cir&); 
//extern template VSR_PRECISION round::rad( const Sph&);
//
//extern template Pnt round::cen( const Pnt&); 
//extern template Pnt round::cen( const Par&); 
//extern template Pnt round::cen( const Cir&); 
//extern template Pnt round::cen( const Sph&); 
//                
//extern template Pnt round::loc( const Pnt&); 
//extern template Pnt round::loc( const Par&); 
//extern template Pnt round::loc( const Cir&); 
//extern template Pnt round::loc( const Sph&);  
//
//extern template Drv round::dir( const Par&); 
//extern template Drb round::dir( const Cir&); 
//
//extern template Lin round::car( const Par&); 
//extern template Pln round::car( const Cir&);
//
//extern template Pnt round::sur( const Par&); 
//extern template Pnt round::sur( const Cir&);
//
//extern template std::vector<Pnt> round::split( const Par&);  
//extern template Pnt round::dls_pnt( const Pnt&, VSR_PRECISION r);
// 
//
//extern template Pnt Fl::loc( const Dll&, const Pnt&, bool);
//extern template Pnt Fl::loc( const Lin&, const Pnt&, bool);
//extern template Pnt Fl::loc( const Dlp&, const Pnt&, bool); 
//extern template Pnt Fl::loc( const Pln&, const Pnt&, bool);
//       
//
//extern template Drv Fl::dir( const Lin&);
//extern template Drb Fl::dir( const Pln&);  
       
// extern template VSR_PRECISION Fl::wt( const Dll&, bool);
// extern template VSR_PRECISION Fl::wt( const Lin&, bool);
// extern template VSR_PRECISION Fl::wt( const Dlp&, bool);  
// extern template VSR_PRECISION Fl::wt( const Pln&, bool);  

 

 
#define E1 e1(1)
#define E2 e2(1)
#define E3 e3(1)


#define PT(x,y,z) vsr::round::null(vsr::cga::Vec(x,y,z))
#define DLS(r) vsr::round::dls(0,0,0,r)
//#define IPT(x,y,z) vsr::round::inull(x,y,z)
#define PV(v) vsr::round::null(v)
#define PX(f) vsr::round::null(vsr::cga::Vec(f,0,0))
#define PY(f) vsr::round::null(vsr::cga::Vec(0,f,0))
#define PZ(f) vsr::round::null(vsr::cga::Vec(0,0,f))
#define PAIR(x,y,z) (PT(x,y,z)^PT(-x,-y,-z))
//#define IPAIR(x,y,z) (IPT(x,y,z)^IPT(-x,-y,-z))
#define CXY(f) (PX(f)^PY(f)^PX(-f)).unit()
#define CXZ(f) (PX(f)^PZ(f)^PX(-f)).unit()
#define CYZ(f) (PY(f)^PY(-f)^PZ(f)).unit()
#define F2S(f) f*1000.0
#define S2F(f) f/1000.0
#define LN(x,y,z) ( vsr::cga::Point(0,0,0,1,.5)^PT(x,y,z)^vsr::cga::Inf(1) )
#define DLN(x,y,z) ( vsr::op::dl(LN(x,y,z)) )
#define PAO vsr::cga::Point(0,0,0,1,0)   /// Point At Origin
#define EP vsr::cga::Dls(0,0,0,1,-.5)  ///  Dual unit sphere at origin: swap with infinity for hyperbolic space
#define EM vsr::cga::Dls(0,0,0,1,.5)   ///  Dual imaginary unit sphere at origin: swap with infinity for spherical space
#define INFTY vsr::cga::Inf(1)
#define HYPERBOLIC_INF EP
#define SPHERICAL_INF EM
#define EUCLIDEAN_INF INFTY
#define HLN(x,y,z) (vsr::cga::Ori(1)^PT(x,y,z)^EP) //hyperbolic line (circle)
#define HDLN(x,y,z) (vsr::op::dl(HLN(x,y,z)))


}    //vsr::                                       

#endif 

