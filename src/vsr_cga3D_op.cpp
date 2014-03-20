#include "vsr_cga3D_op.h"

namespace vsr{  
  
                                            
/* template Pnt Pnt::sp( const Rot& )const; */                        
/* template Pnt Pnt::sp( const Trs& )const; */                        
/* template Pnt Pnt::sp( const Mot& )const; */                      
/* template Pnt Pnt::sp( const Bst& )const; */
/* template Pnt Pnt::sp( const Tsd& )const; */
/* template Pnt Pnt::sp( const Dil& )const; */
                                                          
/* template Par Par::sp( const Rot& )const; */
/* template Par Par::sp( const Trs& )const; */
/* template Par Par::sp( const Mot& )const; */
/* template Par Par::sp( const Bst& )const; */
/* template Par Par::sp( const Tsd& )const; */
/* template Par Par::sp( const Dil& )const; */
                                        
/* template Cir Cir::sp( const Rot& )const; */
/* template Cir Cir::sp( const Trs& )const; */
/* template Cir Cir::sp( const Mot& )const; */
/* template Cir Cir::sp( const Bst& )const; */
/* template Cir Cir::sp( const Tsd& )const; */
/* template Cir Cir::sp( const Dil& )const; */
                                        
/* template Sph Sph::sp( const Rot& )const; */
/* template Sph Sph::sp( const Trs& )const; */
/* template Sph Sph::sp( const Mot& )const; */
/* template Sph Sph::sp( const Bst& )const; */
/* template Sph Sph::sp( const Tsd& )const; */
/* template Sph Sph::sp( const Dil& )const; */
                
                
/* template Dll Dll::sp( const Rot& )const; */ 
/* template Dll Dll::sp( const Trs& )const; */ 
/* template Dll Dll::sp( const Mot& )const; */ 
/* template Dll Dll::sp( const Bst& )const; */
/* template Dll Dll::sp( const Tsd& )const; */
/* template Dll Dll::sp( const Dil& )const; */
                                
/* template Lin Lin::sp( const Rot& )const; */
/* template Lin Lin::sp( const Trs& )const; */
/* template Lin Lin::sp( const Mot& )const; */
/* template Lin Lin::sp( const Bst& )const; */
/* template Lin Lin::sp( const Tsd& )const; */
/* template Lin Lin::sp( const Dil& )const; */
                              
/* template Dlp Dlp::sp( const Rot& )const; */
/* template Dlp Dlp::sp( const Trs& )const; */
/* template Dlp Dlp::sp( const Mot& )const; */
/* template Dlp Dlp::sp( const Bst& )const; */
/* template Dlp Dlp::sp( const Tsd& )const; */
/* template Dlp Dlp::sp( const Dil& )const; */
                                
/* template Pln Pln::sp( const Rot& )const; */
/* template Pln Pln::sp( const Trs& )const; */
/* template Pln Pln::sp( const Mot& )const; */
/* template Pln Pln::sp( const Bst& )const; */
/* template Pln Pln::sp( const Tsd& )const; */
/* template Pln Pln::sp( const Dil& )const; */     

/* template Rot Gen::ratio( const Vec&, const Vec& ); */                                                     
/* template Rot Gen::rot( const Biv& ); */                         
/* template Biv Gen::log( const Rot& ); */
/* template Par Gen::log( const Bst& ); */ 
/* template Trv Gen::trv( const Vec& ); */
/* template Trv Gen::trv( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ); */ 
/* template Trv Gen::trv( const Drv& ); */
/* template Trv Gen::trv( const Tnv& ); */ 
/* template Trs Gen::trs( const Vec& ); */
/* template Trs Gen::trs( const Drv& ); */ 
/* template Trs Gen::trs( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ); */ 
/* template Dil Gen::dil( VSR_PRECISION ); */ 
/* template Tsd Gen::dil( const Pnt&, VSR_PRECISION ); */ 
/* template Bst Gen::bst( const Par& ); */ 

/* template Rot Gen::aa( const Rot& r); */

/* template Par Ro::round( const Dls&, const Vec& ); */ 
/* template Cir Ro::round( const Dls&, const Biv& ); */  

/* template Pnt Ro::null( const Vec& ); */
/* template Pnt Ro::null( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z); */ 

/* template VSR_PRECISION Ro::size( const Pnt&, bool); */ 
/* template VSR_PRECISION Ro::size( const Par&, bool); */ 
/* template VSR_PRECISION Ro::size( const Cir&, bool); */ 
/* template VSR_PRECISION Ro::size( const Sph&, bool); */ 

/* template VSR_PRECISION Ro::rad( const Pnt&); */ 
/* template VSR_PRECISION Ro::rad( const Par&); */ 
/* template VSR_PRECISION Ro::rad( const Cir&); */ 
/* template VSR_PRECISION Ro::rad( const Sph&); */

/* template Pnt Ro::cen( const Pnt&); */ 
/* template Pnt Ro::cen( const Par&); */ 
/* template Pnt Ro::cen( const Cir&); */ 
/* template Pnt Ro::cen( const Sph&); */ 
         
/* template Pnt Ro::loc( const Pnt&); */ 
/* template Pnt Ro::loc( const Par&); */ 
/* template Pnt Ro::loc( const Cir&); */ 
/* template Pnt Ro::loc( const Sph&); */  


/* template Drv Ro::dir( const Par&); */ 
/* template Drb Ro::dir( const Cir&); */ 

/* template Lin Ro::car( const Par&); */ 
/* template Pln Ro::car( const Cir&); */ 

/* template Pnt Ro::sur( const Par&); */
/* template Pnt Ro::sur( const Cir&); */

/* template std::vector<Pnt> Ro::split( const Par&); */ 
/* template Pnt Ro::dls_pnt( const Pnt&, VSR_PRECISION r); */ 

/* template Pnt Fl::loc( const Dll&, const Pnt&, bool); */
/* template Pnt Fl::loc( const Lin&, const Pnt&, bool); */
/* template Pnt Fl::loc( const Dlp&, const Pnt&, bool); */  
/* template Pnt Fl::loc( const Pln&, const Pnt&, bool); */ 

/* template Drv Fl::dir( const Lin&); */
/* template Drb Fl::dir( const Pln&); */  


namespace Gen{               

  /*! Generate a Rotor (i.e quaternion) from spherical coordinates
        @param[in] theta in xz plane from (1,0,0) in range [0,PI]
        @param[in] phi in rotated xy plane in range []
    */
    Rot rot(double theta, double phi){

       // double ptheta = (PIOVERTWO * theta);
       // double pphi = (PIOVERFOUR * phi);

        Rot rt = Gen::rot( Biv::xz * theta );
        Rot rp = Gen::rot( Biv::xy.sp( rt ) * phi  );

        return rp * rt;
    }                   

    /*! Generate a rotor from euler angles */
    Rot rot( double y, double p, double r){
      Rot yaw = Gen::rot( Biv::xz * y/2.0 );
      Rot pitch = Gen::rot( Biv::yz.spin(yaw) * p/2.0);
      Rot tmp = pitch * yaw;
      Rot roll = Gen::rot( Biv::xy.spin(tmp) * r/2.0);

      return roll * tmp;
    }


   /*! Generate a Motor from a Dual Line Axis
       @param Dual Line Generator (the axis of rotation, including pitch and period)
   */  
   Mot mot( const Dll& dll){

      Dll b = dll;
      Biv B(b[0],b[1],b[2]); //Biv B(dll);  

      VSR_PRECISION w = B.wt();

      VSR_PRECISION  c = ( sqrt( fabs ( w ) ) );
      VSR_PRECISION sc = sin(c);
      VSR_PRECISION cc = cos(c);

      if ( w == 0 ) return Mot(1,0,0,0,b[3],b[4],b[5],0); // translation only!

      B = B.unit();
      Vec t(b[3],b[4],b[5]);

      Vec tv;
      tv = Op::pj(t,B) ;
      Vec tw;
      tw = Op::rj(t,B) ;

      tv *= Math::sinc(c);

      Vec tt = tw * cc + tv;

      auto ts = B*tw;        //Vec_Biv

      return Mot(cc, B[0] * sc, B[1] * sc, B[2] * sc, tt[0], tt[1], tt[2], ts[3] * sc);
  }

    /*! Dual Line Generator from a Motor 
        @param Motor m (a concatenation of rotation and translation)
    */  
     Dll log( const Mot& m){

          Drv cperp, cpara;
          Dll rq, q, tq;
          Biv b;

          q = m;                                                  //extract grade 2 part

          VSR_PRECISION  ac = acos( m[0] );                                  //angle of rotor  
          VSR_PRECISION  den = Math::sinc(ac);
          VSR_PRECISION  den2 = ac * ac * den;

          b =  ( ( Ori(1) <= ( q * Inf(1) ) ) / den * -1.0 );     //bivector part - negative necessary . dll? . . 
          tq = (b * q);                                           //Make motor and extract Grade 2 part

          if (den2 == 0 ) {                                       // Pure Rotation (no slide along screw)
               printf("%f, %f, %f\n", ac, den, den2 );
               printf("den2 = 0 in motor log\n"); 
              //cperp = b * -1.0;
              cpara = b * tq * -1.0;// * -1.0; or q
          } else {
              cperp = ( b * Drt(m[7]) ) / ( ( den2 )  * -1.0 );   //perpendicular (along line of axis)
              cpara = ( b * tq ) / ( ( den2 )  * -1.0 );          //parallel      (in plane of rotation)
          }

          Drv c = cperp + cpara; 

          rq += b;
          rq += c;          

          return rq;
      }

    /*! Dual Line Generator of Motor That Twists Dual Line a to Dual Line b;

    */ 
    Dll log(const Dll& a, const Dll& b, VSR_PRECISION t ){
          Mot m = b/a; VSR_PRECISION n = m.rnorm(); if (n!=0) m /= n;
          return Gen::log( m ) * (t/2.0) ;
      }

      /*! Generate Motor That Twists Dual Line a to Dual Line b;

      */
     Mot ratio( const Dll& a, const Dll& b, VSR_PRECISION t){
          //Mot m = b/a; VSR_PRECISION n = m.rnorm(); if (n!=0) m /= n; else cout << "zero mot" << endl; 
          return Gen::mot( log(a,b,t) );//Gen::log( m ) * (t/2.0) );   
      }



      // /*! Generate a Translated Transversion 
      //     @param Tangent Direction
      //     @param Position in space
      //     @param scalar amt (typically 0 or 1)
      // */
      // template <class A, class T>
      // Bst bst(const A& tnv, const Vec& drv, T t){
      //     Par s = tnv.template copy<Tnv>().template cast<Par>().sp( Gen::trs(drv) ); 
      //          // s.vprint();
      //     return Gen::bst(s * t);
      // }   

}

}   //vsr   


 
