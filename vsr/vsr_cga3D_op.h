#ifndef CGA3D_OPERATIONS_H_INCLUDED
#define CGA3D_OPERATIONS_H_INCLUDED  



// Specialized 3D CONFORMAL OPERATIONS (assumes 3D . . . )

#include <math.h> 
#include <vector>  
#include "vsr_constants.h" 
#include "vsr_math.h"
#include "vsr_generic_op.h"
#include "vsr_cga3D_types.h"  

namespace vsr{
                     

using namespace cga3D;


namespace Gen{               
  
  /*! Generate a Rotor (i.e quaternion) from spherical coordinates
        @param[in] theta in xz plane from (1,0,0) in range [0,PI]
        @param[in] phi in rotated xy plane in range []
    */
      Rot rot(double theta, double phi);                
      

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
     Dll log(const Dll& a, const Dll& b, VT t = 1.0);

      /*! Generate Motor That Twists Dual Line a to Dual Line b;

      */
    Mot ratio( const Dll& a, const Dll& b, VT t = 1.0);


      /*! Generate a Translated Transversion 
          @param Tangent Direction
          @param Position in space
          @param scalar amt (typically 0 or 1)
      */
      template <class A, class T>
      Bst bst(const A& tnv, const Vec& drv, T t){
          Par s = tnv.template copy<Tnv>().template cast<Par>().sp( Gen::trs(drv) ); 
       // s.vprint();
          return Gen::bst(s * t);
      }
   
}; 
    

extern template class CGAMV<5,   CGA<5>::Sca   >;  
extern template class CGAMV<5,   CGA<5>::Vec   >;  
extern template class CGAMV<5,   CGA<5>::Vec2D >;  
extern template class CGAMV<5,   CGA<5>::Biv   >;  
extern template class CGAMV<5,   CGA<5>::Tri   >;  
extern template class CGAMV<5,   CGA<5>::Rot   >;  
extern template class CGAMV<5,   CGA<5>::Mnk   >;  
extern template class CGAMV<5,   CGA<5>::Pss   >;     
extern template class CGAMV<5,   CGA<5>::Pnt   >;  
extern template class CGAMV<5,   CGA<5>::Par   >; 
extern template class CGAMV<5,   CGA<5>::Cir   >; 
extern template class CGAMV<5,   CGA<5>::Sph   >;  
extern template class CGAMV<5,   CGA<5>::Dll   >; 
extern template class CGAMV<5,   CGA<5>::Lin   >; 
extern template class CGAMV<5,   CGA<5>::Flp   >; 
extern template class CGAMV<5,   CGA<5>::Pln   >; 
extern template class CGAMV<5,   CGA<5>::Dlp   >; 
extern template class CGAMV<5,   CGA<5>::Trs   >;   
extern template class CGAMV<5,   CGA<5>::Mot   >; 
extern template class CGAMV<5,   CGA<5>::Trv   >; 
extern template class CGAMV<5,   CGA<5>::Bst   >; 
extern template class CGAMV<5,   CGA<5>::Dil   >; 
extern template class CGAMV<5,   CGA<5>::Tsd   >;

extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Rot& )const;                         
extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Trs& )const;                         
extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Mot& )const;                       
extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Bst& )const;
extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Tsd& )const;
extern template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Dil& )const;
                                                         
extern template Par CGAMV<5, CGA<5>::Par>::sp( const Rot& )const;
extern template Par CGAMV<5, CGA<5>::Par>::sp( const Trs& )const;
extern template Par CGAMV<5, CGA<5>::Par>::sp( const Mot& )const;
extern template Par CGAMV<5, CGA<5>::Par>::sp( const Bst& )const;
extern template Par CGAMV<5, CGA<5>::Par>::sp( const Tsd& )const;
extern template Par CGAMV<5, CGA<5>::Par>::sp( const Dil& )const;
                                                       
extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Rot& )const;
extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Trs& )const;
extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Mot& )const;
extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Bst& )const;
extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Tsd& )const;
extern template Cir CGAMV<5, CGA<5>::Cir>::sp( const Dil& )const;
                                                         
extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Rot& )const;
extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Trs& )const;
extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Mot& )const;
extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Bst& )const;
extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Tsd& )const;
extern template Sph CGAMV<5, CGA<5>::Sph>::sp( const Dil& )const; 

extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Rot& )const;                         
extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Trs& )const;                         
extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Mot& )const;                       
extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Bst& )const;
extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Tsd& )const;
extern template Dll CGAMV<5, CGA<5>::Dll>::sp( const Dil& )const;
                                                         
extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Rot& )const;
extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Trs& )const;
extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Mot& )const;
extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Bst& )const;
extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Tsd& )const;
extern template Lin CGAMV<5, CGA<5>::Lin>::sp( const Dil& )const;
                                                       
extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Rot& )const;
extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Trs& )const;
extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Mot& )const;
extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Bst& )const;
extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Tsd& )const;
extern template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Dil& )const;
                                                         
extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Rot& )const;
extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Trs& )const;
extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Mot& )const;
extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Bst& )const;
extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Tsd& )const;
extern template Pln CGAMV<5, CGA<5>::Pln>::sp( const Dil& )const;  
                                                           

                                                           
extern template Rot Gen::rot( const Biv& );                         
extern template Rot Gen::rot( VT, const Biv& ); 
extern template Biv Gen::log( const Rot& );
extern template Par Gen::log( const Bst& ); 
extern template Trv Gen::trv( const Vec& );
extern template Trv Gen::trv( VT x, VT y, VT z ); 
extern template Trv Gen::trv( const Drv& );
extern template Trv Gen::trv( const Tnv& ); 
extern template Trs Gen::trs( const Vec& );
extern template Trs Gen::trs( const Drv& ); 
extern template Trs Gen::trs( VT x, VT y, VT z ); 
extern template Dil Gen::dil( VT ); 
extern template Tsd Gen::dil( const Pnt&, VT ); 
extern template Bst Gen::bst( const Par& ); 

extern template Rot Gen::ratio( const Vec&, const Vec& );
extern template Rot Gen::aa( const Rot& r);

   
extern template Par Ro::round( const Dls&, const Vec& ); 
extern template Cir Ro::round( const Dls&, const Biv& );  

extern template Pnt Ro::null( const Vec& );
extern template Pnt Ro::null( VT x, VT y, VT z);

extern template VT Ro::size( const Pnt&, bool); 
extern template VT Ro::size( const Par&, bool); 
extern template VT Ro::size( const Cir&, bool); 
extern template VT Ro::size( const Sph&, bool); 

extern template VT Ro::rad( const Pnt&); 
extern template VT Ro::rad( const Par&); 
extern template VT Ro::rad( const Cir&); 
extern template VT Ro::rad( const Sph&);

extern template Pnt Ro::cen( const Pnt&); 
extern template Pnt Ro::cen( const Par&); 
extern template Pnt Ro::cen( const Cir&); 
extern template Pnt Ro::cen( const Sph&); 
                
extern template Pnt Ro::loc( const Pnt&); 
extern template Pnt Ro::loc( const Par&); 
extern template Pnt Ro::loc( const Cir&); 
extern template Pnt Ro::loc( const Sph&);  

extern template Drv Ro::dir( const Par&); 
extern template Drb Ro::dir( const Cir&); 

extern template Lin Ro::car( const Par&); 
extern template Pln Ro::car( const Cir&);

extern template Pnt Ro::sur( const Par&); 
extern template Pnt Ro::sur( const Cir&);

extern template std::vector<Pnt> Ro::split( const Par&);  
extern template Pnt Ro::dls_pnt( const Pnt&, VT r);
 

extern template Pnt Fl::loc( const Dll&, const Pnt&, bool);
extern template Pnt Fl::loc( const Lin&, const Pnt&, bool);
extern template Pnt Fl::loc( const Dlp&, const Pnt&, bool); 
extern template Pnt Fl::loc( const Pln&, const Pnt&, bool);
       

extern template Drv Fl::dir( const Lin&);
extern template Drb Fl::dir( const Pln&);  
       
// extern template VT Fl::wt( const Dll&, bool);
// extern template VT Fl::wt( const Lin&, bool);
// extern template VT Fl::wt( const Dlp&, bool);  
// extern template VT Fl::wt( const Pln&, bool);  

 
template<TT DIM, typename A> template<typename T>
CGAMV<DIM,A> CGAMV<DIM,A>::mot( const T& t){
     return this -> sp ( Gen::mot(t) );  
} 
template<TT DIM, typename A> template<typename T>
CGAMV<DIM,A> CGAMV<DIM,A>::motor( const T& t){
     return this -> mot(t);  
}
template<TT DIM, typename A> template<typename T>
CGAMV<DIM,A> CGAMV<DIM,A>::twist( const T& t){
     return this -> mot(t);  
}


 
#define E1 e1(1)
#define E2 e2(1)
#define E3 e3(1)


#define PT(x,y,z) vsr::Ro::null(vsr::Vec(x,y,z))
#define DLS(r) vsr::Ro::dls(0,0,0,r)
//#define IPT(x,y,z) vsr::Ro::inull(x,y,z)
#define PV(v) vsr::Ro::null(v)
#define PX(f) vsr::Ro::null(vsr::Vec(f,0,0))
#define PY(f) vsr::Ro::null(vsr::Vec(0,f,0))
#define PZ(f) vsr::Ro::null(vsr::Vec(0,0,f))
#define PAIR(x,y,z) (PT(x,y,z)^PT(-x,-y,-z))
//#define IPAIR(x,y,z) (IPT(x,y,z)^IPT(-x,-y,-z))
#define CXY(f) (PX(f)^PX(-f)^PY(f))
#define CXZ(f) (PX(f)^PX(-f)^PZ(f))
#define CYZ(f) (PY(f)^PY(-f)^PZ(f))
#define F2S(f) f*1000.0
#define S2F(f) f/1000.0
#define LN(x,y,z) ( vsr::Pnt(0,0,0,1,.5)^PT(x,y,z)^vsr::Inf(1) )
#define DLN(x,y,z) ( vsr::Op::dl(LN(x,y,z)) )
#define PAO vsr::Pnt(0,0,0,1,0)   /// Point At Origin
#define EP vsr::Dls(0,0,0,1,-.5)  ///  Dual unit sphere at origin: swap with infinity for hyperbolic space
#define EM vsr::Dls(0,0,0,1,.5)   ///  Dual imaginary unit sphere at origin: swap with infinity for spherical space
#define INFTY vsr::Inf(1)
#define HYPERBOLIC EP
#define SPHERICAL EM
#define EUCLIDEAN INFTY
#define HLN(x,y,z) (vsr::Ori(1)^PT(x,y,z)^EP) //hyperbolic line (circle)
#define HDLN(x,y,z) (vsr::Op::dl(HLN(x,y,z)))

}    //vsr::                                       

#endif 

