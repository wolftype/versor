  #include "vsr_cga3D_op.h"

namespace vsr{  
  
//template class CGAMV<5,   CGA<5>::Sca   >;  
//template class CGAMV<5,   CGA<5>::Vec   >;  
//template class CGAMV<5,   CGA<5>::Vec2D >;  
//template class CGAMV<5,   CGA<5>::Biv   >;  
//template class CGAMV<5,   CGA<5>::Tri   >;  
//template class CGAMV<5,   CGA<5>::Rot   >;  
//template class CGAMV<5,   CGA<5>::Mnk   >;  
//template class CGAMV<5,   CGA<5>::Pss   >;     
//template class CGAMV<5,   CGA<5>::Pnt   >;  
//template class CGAMV<5,   CGA<5>::Par   >; 
//template class CGAMV<5,   CGA<5>::Cir   >; 
//template class CGAMV<5,   CGA<5>::Sph   >;  
//template class CGAMV<5,   CGA<5>::Dll   >; 
//template class CGAMV<5,   CGA<5>::Lin   >; 
//template class CGAMV<5,   CGA<5>::Flp   >; 
//template class CGAMV<5,   CGA<5>::Pln   >; 
//template class CGAMV<5,   CGA<5>::Dlp   >; 
//template class CGAMV<5,   CGA<5>::Trs   >;   
//template class CGAMV<5,   CGA<5>::Mot   >; 
//template class CGAMV<5,   CGA<5>::Trv   >; 
//template class CGAMV<5,   CGA<5>::Bst   >; 
//template class CGAMV<5,   CGA<5>::Dil   >; 
//template class CGAMV<5,   CGA<5>::Tsd   >;


                                                
template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Rot& )const;                        
template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Trs& )const;                        
template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Mot& )const;                      
template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Bst& )const;
template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Tsd& )const;
template Pnt CGAMV<5, CGA<5>::Pnt>::sp( const Dil& )const;
                                                          
template Par CGAMV<5, CGA<5>::Par>::sp( const Rot& )const;
template Par CGAMV<5, CGA<5>::Par>::sp( const Trs& )const;
template Par CGAMV<5, CGA<5>::Par>::sp( const Mot& )const;
template Par CGAMV<5, CGA<5>::Par>::sp( const Bst& )const;
template Par CGAMV<5, CGA<5>::Par>::sp( const Tsd& )const;
template Par CGAMV<5, CGA<5>::Par>::sp( const Dil& )const;
                                                          
template Cir CGAMV<5, CGA<5>::Cir>::sp( const Rot& )const;
template Cir CGAMV<5, CGA<5>::Cir>::sp( const Trs& )const;
template Cir CGAMV<5, CGA<5>::Cir>::sp( const Mot& )const;
template Cir CGAMV<5, CGA<5>::Cir>::sp( const Bst& )const;
template Cir CGAMV<5, CGA<5>::Cir>::sp( const Tsd& )const;
template Cir CGAMV<5, CGA<5>::Cir>::sp( const Dil& )const;
                                                          
template Sph CGAMV<5, CGA<5>::Sph>::sp( const Rot& )const;
template Sph CGAMV<5, CGA<5>::Sph>::sp( const Trs& )const;
template Sph CGAMV<5, CGA<5>::Sph>::sp( const Mot& )const;
template Sph CGAMV<5, CGA<5>::Sph>::sp( const Bst& )const;
template Sph CGAMV<5, CGA<5>::Sph>::sp( const Tsd& )const;
template Sph CGAMV<5, CGA<5>::Sph>::sp( const Dil& )const;


template Dll CGAMV<5, CGA<5>::Dll>::sp( const Rot& )const; 
template Dll CGAMV<5, CGA<5>::Dll>::sp( const Trs& )const; 
template Dll CGAMV<5, CGA<5>::Dll>::sp( const Mot& )const; 
template Dll CGAMV<5, CGA<5>::Dll>::sp( const Bst& )const;
template Dll CGAMV<5, CGA<5>::Dll>::sp( const Tsd& )const;
template Dll CGAMV<5, CGA<5>::Dll>::sp( const Dil& )const;
                                                  
template Lin CGAMV<5, CGA<5>::Lin>::sp( const Rot& )const;
template Lin CGAMV<5, CGA<5>::Lin>::sp( const Trs& )const;
template Lin CGAMV<5, CGA<5>::Lin>::sp( const Mot& )const;
template Lin CGAMV<5, CGA<5>::Lin>::sp( const Bst& )const;
template Lin CGAMV<5, CGA<5>::Lin>::sp( const Tsd& )const;
template Lin CGAMV<5, CGA<5>::Lin>::sp( const Dil& )const;
                                                
template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Rot& )const;
template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Trs& )const;
template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Mot& )const;
template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Bst& )const;
template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Tsd& )const;
template Dlp CGAMV<5, CGA<5>::Dlp>::sp( const Dil& )const;
                                                  
template Pln CGAMV<5, CGA<5>::Pln>::sp( const Rot& )const;
template Pln CGAMV<5, CGA<5>::Pln>::sp( const Trs& )const;
template Pln CGAMV<5, CGA<5>::Pln>::sp( const Mot& )const;
template Pln CGAMV<5, CGA<5>::Pln>::sp( const Bst& )const;
template Pln CGAMV<5, CGA<5>::Pln>::sp( const Tsd& )const;
template Pln CGAMV<5, CGA<5>::Pln>::sp( const Dil& )const;     


template Rot Gen::ratio( const Vec&, const Vec& );                                                     
template Rot Gen::rot( const Biv& );                         
//template Rot Gen::rot( VT, const Biv& ); 
template Biv Gen::log( const Rot& );
template Par Gen::log( const Bst& ); 
template Trv Gen::trv( const Vec& );
template Trv Gen::trv( VT x, VT y, VT z ); 
template Trv Gen::trv( const Drv& );
template Trv Gen::trv( const Tnv& ); 
template Trs Gen::trs( const Vec& );
template Trs Gen::trs( const Drv& ); 
template Trs Gen::trs( VT x, VT y, VT z ); 
template Dil Gen::dil( VT ); 
template Tsd Gen::dil( const Pnt&, VT ); 
template Bst Gen::bst( const Par& ); 

template Rot Gen::aa( const Rot& r);

template Par Ro::round( const Dls&, const Vec& ); 
template Cir Ro::round( const Dls&, const Biv& );  

template Pnt Ro::null( const Vec& );
template Pnt Ro::null( VT x, VT y, VT z); 

template VT Ro::size( const Pnt&, bool); 
template VT Ro::size( const Par&, bool); 
template VT Ro::size( const Cir&, bool); 
template VT Ro::size( const Sph&, bool); 

template VT Ro::rad( const Pnt&); 
template VT Ro::rad( const Par&); 
template VT Ro::rad( const Cir&); 
template VT Ro::rad( const Sph&);

template Pnt Ro::cen( const Pnt&); 
template Pnt Ro::cen( const Par&); 
template Pnt Ro::cen( const Cir&); 
template Pnt Ro::cen( const Sph&); 
         
template Pnt Ro::loc( const Pnt&); 
template Pnt Ro::loc( const Par&); 
template Pnt Ro::loc( const Cir&); 
template Pnt Ro::loc( const Sph&);  


template Drv Ro::dir( const Par&); 
template Drb Ro::dir( const Cir&); 

template Lin Ro::car( const Par&); 
template Pln Ro::car( const Cir&); 

template Pnt Ro::sur( const Par&);
template Pnt Ro::sur( const Cir&);

template std::vector<Pnt> Ro::split( const Par&); 
template Pnt Ro::dls_pnt( const Pnt&, VT r); 

template Pnt Fl::loc( const Dll&, const Pnt&, bool);
template Pnt Fl::loc( const Lin&, const Pnt&, bool);
template Pnt Fl::loc( const Dlp&, const Pnt&, bool);  
template Pnt Fl::loc( const Pln&, const Pnt&, bool); 

template Drv Fl::dir( const Lin&);
template Drb Fl::dir( const Pln&);  

// template VT Fl::wt( const Dll&, bool);
// template VT Fl::wt( const Lin&, bool);
// template VT Fl::wt( const Dlp&, bool);  
// template VT Fl::wt( const Pln&, bool);  
//
//
namespace Op{

  Rot AA( const Vec& s){
      Rot r = Gen::ratio(Vec::z, s.unit() ); 
      return Gen::aa(r);
  }  
  
  Rot AA( const Biv& s){
     Rot r = Gen::ratio(Vec::z, s.duale().unit() ); 
     return Gen::aa(r);
  }     

  Rot AA( const Dlp& s){
      Rot r = Gen::ratio(Vec::z, Vec(s).unit() ); 
      return Gen::aa(r);
    } 
     
  Rot AA( const Cir& s){
      Biv b = Ro::dir( s ).copy<Biv>();               
      Rot r = Gen::ratio(Vec::z, Op::dle( b ).unit() ); 
      return Gen::aa(r);
    } 


  Vec Pos( const Dlp& s){ 
    return Fl::loc(s , Ori(1), true );   
  }   
     
  Pnt Pos( const Cir& s){
    return Ro::loc( s );
  }  

}

namespace Gen{               

  /*! Generate a Rotor (i.e quaternion) from spherical coordinates
        @param[in] theta in xz plane from (1,0,0) in range [0,PI]
        @param[in] phi in rotated xy plane in range []
    */
    Rot rot(double theta, double phi){

        Rot rt = Gen::rot( Biv::xz * theta / 2.0 );
        Rot rp = Gen::rot( Biv::xy.sp( rt ) * phi /2.0 );

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

      VT w = B.wt();

      VT  c = ( sqrt( fabs ( w ) ) );
      VT sc = sin(c);
      VT cc = cos(c);

      if ( ERROR(w,.00000001) ) return Mot(1,0,0,0,b[3],b[4],b[5],0); // translation only!

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
        An implementation of J.Lasenby et al "Applications of Conformal Geometric Algebra in
Computer Vision and Graphics"
        @param Motor m (a concatenation of rotation and translation)
    */  
     Dll log( const Mot& m){

          Drv cperp, cpara;
          Dll rq, q, tq;
          Biv b;

          q = m;                                                  //extract grade 2 part

          VT  ac = acos( m[0] );                                  //angle of rotor  
          VT  den = Math::sinc(ac);
          VT  den2 = ac * ac * den;

          //m.vprint();
          //printf("den: %f\t%f\t%f\n",ac,den,den2);
          b =  ( ( Ori(1) <= ( q * Inf(1) ) ) / den * -1.0 );     //bivector part - negative necessary . dll? . . 
          tq = (b * q);                                           //Make motor and extract Grade 2 part

          if ( FERROR(den2) ) {                                       // Pure translation 
             //  printf("%f, %f, %f\n", ac, den, den2 );
             //  printf("den2 = 0 in motor log\n"); 
              //cperp = b * -1.0;
              cperp = q;//b * tq * -1.0;// * -1.0; or q //note this used to be cpara... (but was inaccurate)
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
    Dll log(const Dll& a, const Dll& b, VT t ){
          Mot m = b/a; VT n = m.rnorm(); if (n!=0) m /= n;
          return Gen::log( m ) * (t/2.0) ;
      }

      /*! Generate Motor That Twists Dual Line a to Dual Line b;

      */
     Mot  ratio( const Dll& a, const Dll& b, VT t){
          //Mot m = b/a; VT n = m.rnorm(); if (n!=0) m /= n; else cout << "zero mot" << endl; 
          return Gen::mot( log(a,b,t) );//Gen::log( m ) * (t/2.0) );   
      }

      /*! Generate Motor That Twists Motor a to motor b by amt t;
      */
     Mot ratio( const Mot& a, const Mot& b, VT t){
          return Gen::mot( Gen::log(b/a) * t );
      }


     /*-----------------------------------------------------------------------------
      *  BOOSTS
      *-----------------------------------------------------------------------------*/

    /*! Generate Simple Boost rotor from ratio of two dual spheres
    */
   Bst ratio( const DualSphere& a, const DualSphere& b){
    Bst tbst = (b/a).runit();
    if (tbst[0]<0) tbst = -tbst; //restrict to positive <R>
    auto ss = 2 * (1+tbst[0]);     
    auto n = (ss >= 0 ? sqrt(ss) : -sqrt(-ss) );
    return FERROR(n) ? Bst() : (tbst+1)/n;
   }

  /*! atanh2 function for logarithm of general rotors*/
   Pair atanh2( const Pair& p, VT cs){
      VT norm=1;

      auto tp = p.wt();
      auto sq = sqrt( fabs(tp) );
      if(tp>0) norm = asinh(sq)/sq;
      else if (tp<0) norm = atan2(sq, cs) / sq;
      return p * norm;
   }

    /*! Log of a simple rotor (uses atanh2) */
   Pair log( const Bst& b){
     return atanh2( Pair(b), b[0] );
   }

   /*!  Generate Conformal Transformation from circle a to circle b
        uses square root method of Dorst et Valkenburg, 2011
   */
   Con ratio( const Circle& a, const Circle& b){
    
    Con trot = (b/a).runit();
   // if (trot[0]<0) trot = -trot; //restrict to positive <R>?
      
    auto rotone = trot + 1;

    VT sca = 1 + trot[0];
    VT sca2 = sca*sca;

    Sphere sph(trot);
    auto sph2 = sph.wt();

    if (sca2 == sph2) {
      printf("infinity of roots -- need to program this...\n");
      auto rotneg = -trot + 1;
      //random tangent vector
      auto biv = sph.dual() ^ PAO.trs(1);

      printf("f\n");
      //std::cout << "A" << endl;
      (biv*biv).print();
      (biv*sph).print();
      (sph*biv).print();
      return (rotone + (biv*rotneg))/2.0;
    }

    auto sca3 = sca2 - sph2;
    auto sqsca3 = sqrt(sca3);

    auto v1 = ( -sph + sca ) / (2*sca3);
    auto v2 = (sph+(sca+sqsca3))/sqrt(sca+sqsca3);
    
    return rotone * v1 * v2;      
   }

   /*!  Generate Conformal Transformation from Pair a to Pair b
        uses square root method of Dorst et Valkenburg, 2011
   */
   Con ratio( const Pair& a, const Pair& b){ return ratio( a.dual(), b.dual() ); }
   
   /*! Bivector Split
            Takes a general bivector and splits  it into commuting pairs
            will give sinh(B+-)
            inverse method for ipar below was given by dorst in personal correspondance
   */
    vector<Pair> split(const Pair& par){
   
      typedef decltype(Sphere()+1) SqDeriv;
    
      vector<Pair> res;
      
      SqDeriv h2 = par*par;
      auto hh2 = Sphere(h2).wt();
      auto ipar = (-Sphere(h2) + h2[0]) /(h2[0]*h2[0] - hh2);  
    
      //scalar ||f||^2
      auto tmp2 = ( (h2*h2) - (h2*2*h2[0]) )[0] ;
      auto ff4 = pow( -tmp2, 1.0/4);
      auto wt = ff4 * ff4;
     
      if ( FERROR(wt) ) {
          if ( FERROR(h2[0]) ){
            printf("no real splitting going on\n");
            res.push_back(par);
            res.push_back(Par());
            return res;
          } else {
           printf("bivector split norm implemenation needed\n (add random value and retry)");
          }
      }
      
      auto iha = ipar * wt;
      auto fplus = iha + 1;
      auto fminus = -iha + 1;
    
      Pair pa = par * fplus * .5;
      Pair pb = par * fminus * .5;
    
      res.push_back(pa);
      res.push_back(pb);
      
      return res;
       
    }
   
   
   /*! Split Log of General Conformal Rotor */
   vector<Pair> log( const Con& rot){
      
     vector<Pair> res;
      
     //0. Some Terms for later on
     //R^2
     auto sqrot = rot*rot;
     //<R>2
     Pair rot2(sqrot);
    
     //1. Get Exterior Derivative
     Sphere quad(rot); //grade 4 part

     auto tmp = quad + (-rot[0]); //scalar + quadvector
     //half the exterior deriv is sinh(B+/-)
     auto deriv = Pair( tmp * Pair(rot) ) * 2; //quad parts are zero here.
     //find commuting split of that
     auto v = split( deriv );
  
     //get cosh (see p96 of ref)
     auto sp = v[0].wt();//(v[0]<=v[0])[0];
     auto sm = v[1].wt();//(v[1]<=v[1])[0];

     VT coshp = FERROR(sm) ? sqrot[0] : -(rot2 <= !v[1])[0];
     VT coshm = FERROR(sp) ? sqrot[0] : -(rot2 <= !v[0])[0];

     //5.27 on p96 of Dorst ref
     res.push_back( atanh2(v[0],coshp) * -.5 );
     res.push_back( atanh2(v[1],coshm) * -.5 );

     return res;

   }

  /*! General Conformal Transformation from a split log*/
   Con con( const vector<Pair>& log, VT amt) {
     return Gen::bst(log[0]*-amt) * Gen::bst(log[1]*-amt);
   }


    


} // Gen::

}   //vsr   


 
