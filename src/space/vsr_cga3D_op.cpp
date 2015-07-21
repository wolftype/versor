  #include "space/vsr_cga3D_op.h"

namespace vsr{  
namespace cga {


      
  /*-----------------------------------------------------------------------------
   *  OP
   *-----------------------------------------------------------------------------*/
      Rot op::AA( const Vec& s){
          Rot r = nga::gen::ratio(Vec::z, s.unit() ); 
          return nga::gen::aa(r);
      }  
      
      Rot op::AA( const Biv& s){
         Rot r = nga::gen::ratio(Vec::z, s.duale().unit() ); 
         return nga::gen::aa(r);
      }     
    
      Rot op::AA( const Dlp& s){
          Rot r = nga::gen::ratio(Vec::z, Vec(s).unit() ); 
          return nga::gen::aa(r);
        } 
         
      Rot op::AA( const Cir& s){
          Biv b = round::dir( s ).copy<Biv>();               
          Rot r = nga::gen::ratio(Vec::z, op::dle( b ).unit() ); 
          return nga::gen::aa(r);
        } 
    
      Vec op::Pos( const Dlp& s){ 
        return flat::loc(s , PAO, true );   
      }   
         
      Pnt op::Pos( const Cir& s){
        return round::loc( s );
      }  


    
      /*-----------------------------------------------------------------------------
       *  GEN
       *-----------------------------------------------------------------------------*/

      Rot gen::rot( const Biv& b) { return nga::gen::rot(b); } 
      Rot gen::rotor( const Biv& b) { return nga::gen::rot(b); } 



      Bst gen::bst(const Pair& p) { return nga::gen::bst(p); }
      Bst gen::boost(const Pair& p) { return nga::gen::bst(p); }    
      
      Tsd gen::dil(const Pnt& p, VSR_PRECISION t) { return nga::gen::dil(p,t); }
      Tsd gen::dilator(const Pnt& p, VSR_PRECISION t) { return nga::gen::dil(p,t); }


      Rot gen::ratio(const Vec& v, const Vec& v2) { return nga::gen::ratio(v,v2); }

      Biv gen::log(const Rot& r) { return nga::gen::log(r); }

   /*! Generate a Rotor (i.e quaternion) from spherical coordinates
        @param[in] theta in xz plane from (1,0,0) in range [0,PI]
        @param[in] phi in rotated xy plane in range []
    */
    Rot gen::rot(double theta, double phi){
        Rot rt = gen::rot( Biv::xz * theta / 2.0 );
        Rot rp = gen::rot( Biv::xy.sp( rt ) * phi /2.0 );
        return rp * rt;
    }                   

    /*! Generate a rotor from euler angles */
    Rot gen::rot( double y, double p, double r){
      Rot yaw = gen::rot( Biv::xz * y/2.0 );
      Rot pitch = gen::rot( Biv::yz.spin(yaw) * p/2.0);
      Rot tmp = pitch * yaw;
      Rot roll = gen::rot( Biv::xy.spin(tmp) * r/2.0);

      return roll * tmp;
    }


   /*! Generate a Motor from a Dual Line Axis
       @param Dual Line Generator (the axis of rotation, including pitch and period)
   */  
   Mot gen::mot( const Dll& dll){

      Dll b = dll;
      Biv B(b[0],b[1],b[2]); //Biv B(dll);  

      VSR_PRECISION w = B.wt();

      VSR_PRECISION  c = ( sqrt( fabs ( w ) ) );
      VSR_PRECISION sc = sin(c);
      VSR_PRECISION cc = cos(c);

      if ( ERROR(w,.00000001) ) return Mot(1,0,0,0,b[3],b[4],b[5],0); // translation only!

      B = B.unit();
      Vec t(b[3],b[4],b[5]);

      Vec tv;
      tv = op::pj(t,B) ;
      Vec tw;
      tw = op::rj(t,B) ;

      tv *= Math::sinc(c);

      Vec tt = tw * cc + tv;

      auto ts = B*tw;        //Vec_Biv

      return Mot(cc, B[0] * sc, B[1] * sc, B[2] * sc, tt[0], tt[1], tt[2], ts[3] * sc);
  }

  Mot gen::motor(const Dll& dll){ return mot(dll); }

    /*! Dual Line Generator from a Motor 
        An implementation of J.Lasenby et al "Applications of Conformal Geometric Algebra in
        Computer Vision and Graphics"
        @param Motor m (a concatenation of rotation and translation)
    */  
     Dll gen::log( const Mot& m){

          Drv cperp, cpara;
          Dll rq, q, tq;
          Biv b;

          q = m;                                                  //extract grade 2 part

          VSR_PRECISION  ac = acos( m[0] );                                  //angle of rotor  
          VSR_PRECISION  den = Math::sinc(ac);
          VSR_PRECISION  den2 = ac * ac * den;

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
    Dll gen::log(const Dll& a, const Dll& b, VSR_PRECISION t ){
          Mot m = b/a; VSR_PRECISION n = m.rnorm(); if (n!=0) m /= n;
          return gen::log( m ) * (t/2.0) ;
      }

      /*! Generate Motor That Twists Dual Line a to Dual Line b;

      */
     Mot  gen::ratio( const Dll& a, const Dll& b, VSR_PRECISION t){
          //Mot m = b/a; VSR_PRECISION n = m.rnorm(); if (n!=0) m /= n; else cout << "zero mot" << endl; 
          return gen::mot( log(a,b,t) );//gen::log( m ) * (t/2.0) );   
      }

      /*! Generate Motor That Twists Motor a to motor b by amt t;
      */
     Mot gen::ratio( const Mot& a, const Mot& b, VSR_PRECISION t){
          return gen::mot( gen::log(b/a) * t );
      }


     /*-----------------------------------------------------------------------------
      *  BOOSTS
      *-----------------------------------------------------------------------------*/

    /*! Generate Simple Boost rotor from ratio of two dual spheres
    */
   Bst gen::ratio( const DualSphere& a, const DualSphere& b, bool bFlip){
    Bst tbst = (b/a).runit();
    //if (tbst[0]<0) 
    if (bFlip) tbst = -tbst; //restrict to positive <R>
    auto ss = 2 * (1+tbst[0]);     
    auto n = (ss >= 0 ? sqrt(ss) : -sqrt(-ss) );
    return FERROR(n) ? Bst() : (tbst+1)/n;
   }

  /*! atanh2 function for logarithm of general rotors*/
   Pair gen::atanh2( const Pair& p, VSR_PRECISION cs, bool bCW){
      VSR_PRECISION norm=1;

      auto tp = p.wt();
      auto sq = sqrt( fabs(tp) );
      if(tp>0) norm = asinh(sq)/sq;
      else if (tp<0) {
        if (bCW) norm = - (PI - atan2(sq,cs))/sq; //alt direction
        else norm = atan2(sq, cs) / sq;
      }
      return p * norm;
   }

    /*! Log of a simple rotor (uses atanh2, passes in boolean for direction of interpolation) */
   Pair gen::log( const Bst& b, bool bCW){
     return atanh2( Pair(b), b[0], bCW );
   }

   /*!  Generate Conformal Transformation from circle a to circle b
        uses square root method of Dorst et Valkenburg, 2011
   */
   Con gen::ratio( const Circle& a, const Circle& b, bool bFlip, float theta){
    
    Con trot = (b/a).runit();
    //planar?
  //  float planarity = (round::carrier(a).dual().unit() ^ round::carrier(b).dual().unit()).wt();
    if ( bFlip && trot[0] < 0 ) { //fabs(planarity)<=.000009 )  {
      trot = -trot;      //restrict to positive <R> only if coplanar
    }
      
    auto rotone = trot + 1;

    VSR_PRECISION sca = 1 + trot[0];
    VSR_PRECISION sca2 = sca*sca;

    Sphere sph(trot);
    auto sph2 = sph.wt();


    if ( FERROR( sca2 - sph2) ) {
     // printf("infinity of roots . . .  \n");
      auto rotneg = (-trot) + 1;

      Vec vec;

      auto sizeB = nga::round::size(b,false);
      if ( sizeB < 1000 && !FERROR(sizeB) ) vec = Vec( round::location(b) - round::location(a) ).unit();
      else vec = round::vec(a,theta).unit();    

      auto dls = sph.dual();
      
      auto biv = ( Pair( vec.copy<Tnv>() ).trs( round::location(a) ) ^ dls ).undual();//.trs(1,0,0);

      biv = biv.runit();
        
      auto test = (biv*sph - sph*biv).wt();

      if ( !FERROR( (biv<=biv)[0] +1 ) || (!FERROR(test) ) ) {
        printf("HEY NOW NOT COMMUTING\n");
      }

      auto ret = rotone/2.0 + (biv*(rotneg/2.0));
      return ret;
    }

    auto sca3 = sca2 - sph2;
    auto sqsca3 = sqrt(sca3);

 //   cout << sca2 << " " << sph2 << " " << sca << " " << sqsca3 << endl;
 //   sca = fabs(sca);  //<--* added this fabs in
    auto v1 = ( -sph + sca ) / (2*sca3);
    auto v2 = (sph+(sca+sqsca3))/sqrt( sca+sqsca3 ); 
     
    return rotone * v1 * v2;      
   }

   /*!  Generate Conformal Transformation from Pair a to Pair b
        uses square root method of Dorst et Valkenburg, 2011
   */
   Con gen::ratio( const Pair& a, const Pair& b, bool bFlip, float theta){ 
     return ratio( a.dual(), b.dual(), bFlip, theta ); 
   }
   
   /*! Bivector Split
            Takes a general bivector and splits  it into commuting pairs
            will give sinh(B+-)
            inverse method for ipar below was given by dorst in personal correspondance
   */
    vector<Pair> gen::split(const Pair& par){
   
      typedef decltype(Sphere()+1) SqDeriv;
    
      vector<Pair> res;
      
      SqDeriv h2 = par*par;
      auto hh2 = Sphere(h2).wt();
      auto ipar = (-Sphere(h2) + h2[0]) /(h2[0]*h2[0] - hh2);  
    
      //scalar ||f||^2
      auto tmp2 = ( (h2*h2) - (h2*2*h2[0]) )[0] ;
      auto ff4 = FERROR(tmp2) ? 0 : pow( -tmp2, 1.0/4);
      auto wt = ff4 * ff4;
    //  cout << par << endl;
    //  cout << h2 << endl;
    //  cout << "SPLIT WEIGHT: " << -tmp2 << " " << h2[0] << " " << wt << endl;
      if ( FERROR(wt) ) {
          if ( FERROR(h2[0]) ){
           // cout << h2 << endl;
          //  printf("no real splitting going on\n"); //<-- i.e. interpolation of null point pairs
            res.push_back(par);
           // res.push_back(Par());
            return res;
          } else {
          //  printf("(adding random value and retrying)");
            static Pair dp(.0001,.0006,.0004,.0002,.0008,.0006,.0003,.0007,.0001,.0001);
            return split( par + dp);
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


   vector<Pair> gen::split(const Con& rot){
    
     //1. Get Exterior Derivative
     Sphere quad(rot); //grade 4 part

     auto tmp = quad + (-rot[0]); //scalar + quadvector
     //half the exterior deriv is sinh(B+/-)
     auto deriv = Pair( tmp * Pair(rot) ) * 2; //quad parts are zero here.
     //find commuting split of that
     return split( deriv );
      
   }
   
   
   /*! Split Log of General Conformal Rotor */
   vector<Pair> gen::log( const Con& rot){
      
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

     VSR_PRECISION coshp = FERROR(sm) ? sqrot[0] : -(rot2 <= !v[1])[0];
     VSR_PRECISION coshm = FERROR(sp) ? sqrot[0] : -(rot2 <= !v[0])[0];

     //5.27 on p96 of Dorst ref
     res.push_back( atanh2(v[0],coshp,false) * -.5 );
     res.push_back( atanh2(v[1],coshm,false) * -.5 );

     return res;

   }

   /*! Split Log from a ratio of two Circles */
   vector<Pair> gen::log( const Circle& ca, const Circle& cb, bool bFlip, VSR_PRECISION theta){
      return log( ratio(ca,cb,bFlip,theta) ); 
   }
   /*! Split Log from a ratio of two Circles */
   vector<Pair> gen::log( const Pair& ca, const Pair& cb, bool bFlip, VSR_PRECISION theta){
      return log( ratio(ca,cb,bFlip,theta) ); 
   }


  /*! General Conformal Transformation from a split log*/
   Con gen::con( const vector<Pair>& log, VSR_PRECISION amt) {
     Con con(1);
     for (auto& i : log){
        con *= gen::bst( i * -amt);
     }
     return con;
   }

  /*! General Conformal Transformation from a split log*/
   Con gen::con( const vector<Pair>& log, VSR_PRECISION amtA, VSR_PRECISION amtB) {
     Con tmp = gen::bst(log[0]*-amtA);
     if (log.size()>1) tmp *= gen::bst(log[1]*-amtB);
     return tmp;
   }



  /*! General Conformal Transformation from two circles */
   Con gen::con(  const Circle& ca, const Circle& cb, VSR_PRECISION amt){
      return con ( log(ca,cb), amt ); 
   }
 
   /* General Conformal Transformation from two circles and two weights */
   Con gen::con(  const Circle& ca, const Circle& cb, VSR_PRECISION amtA, VSR_PRECISION amtB){
      return con(log(ca,cb), amtA, amtB);
   }



  /*-----------------------------------------------------------------------------
   *  ROTORS
   *-----------------------------------------------------------------------------*/
      /*!
     *  \brief  generate a rotor transformation from a euclidean bivector
     */
     Rotor gen::xf(const Biv& b){
      return gen::rot(b);
    }
    /*!
     *  \brief  generate a motor transformation from a dual line
     */
     Motor gen::xf(const DualLine& dll){
       return gen::mot(dll);
     }
    /*!
     *  \brief  generate a dilation transformation from a flat point
     */
     Dilator gen::xf(const FlatPoint& flp){
       return gen::dil(Pnt(flp),flp[3]);
     }

     /*!
      *  \brief  generate a boost transformation from a point pair
      */
     Bst gen::xf(const Pair& p){
       return gen::bst(p);
     }


     /*-----------------------------------------------------------------------------
      *  PAIRS
      *-----------------------------------------------------------------------------*/
      /// Pair on Sphere in v direction
      Pair construct::pair(const DualSphere& s, const Vec& v){
        return round::produce(s,v);
      }
      
      /*!
       *  \brief Point Pair at x,y,z with direction vec (default Y) and radius r (default 1)
       */
      Pair construct::pair(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, Vec vec , VSR_PRECISION r){ 
       return round::produce( round::dls(r*-1,x,y,z), vec );//a ^ b ^ c; 
      }
      


      /*-----------------------------------------------------------------------------
       *  POINTS
       *-----------------------------------------------------------------------------*/
      /*!
       *  \brief  First point of point pair pp
       */
       Point construct::pointA(const Pair& pp){
        return round::location( round::split(pp, true) );
      } 
 
       /*!
        *  \brief  Second point of point pair pp
        */
       Point construct::pointB(const Pair& pp){
        return round::location( round::split(pp, false) );
      }

      /// Point on Circle at theta t
      Point construct::point(const Circle& c, VSR_PRECISION t){
        return round::point(c,t);
      }
      /// Point on Sphere in v direction
      Point construct::point(const DualSphere& s, const Vec& v){
        return pointA( pair(s,v) ).null();
      }

      /// Point from x,y,z
      Point construct::point(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z){
        return round::null(x,y,z);
      }
      /// Point from vec
      Point construct::point(const Vec& v){
        return v.null();
      } 

      /// Point on line l closest to p
      Point construct::point(const Line& line, const Point& p){
        return round::null ( flat::location( line, p, false ) ); 
      }
      /// Point on dualline l closest to p
      Point construct::point(const DualLine& dll, const Point& p){
        return round::null ( flat::location( dll, p, true ) ); 
      }
     
      /*-----------------------------------------------------------------------------
       *  CIRCLES
       *-----------------------------------------------------------------------------*/
      /*!
       *  \brief  Circle at origin in plane of bivector B
       */
      Circle construct::circle(const Biv& B){
       return round::produce( round::dls(1,0,0,0), B);//a ^ b ^ c; 
      }         

      /*!
       *  \brief  Circle at point p with radius r, facing direction biv
      */
      Circle construct::circle(const Point& p, VSR_PRECISION r, const Biv& biv){
       return round::produce( round::dls(p,r*-1), biv );
      }   
                       
      /// Circle at origin with normal v and radius r (default r=1.0)
       Circle construct::circle(const Vec& v, VSR_PRECISION r){
        return round::produce( round::dls(r*-1,0,0,0), op::dle(v) );//a ^ b ^ c; 
      }                    
      /// Circle at x,y,z facing in biv direction
       Circle construct::circle(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, Biv biv, VSR_PRECISION r){ 
        return round::produce( round::dls(r*-1,x,y,z), biv );//a ^ b ^ c; 
      }
       /*-----------------------------------------------------------------------------
        *  HYPERBOLIC AND SPHERICAL LINES
        *-----------------------------------------------------------------------------*/
      ///Hyperbolic line through two points
       Circle construct::hline( const Point& a, const Point& b){
        return a ^ b ^ EP;
      }  
      ///Spherical line through two points
       Circle construct::sline( const Point& a, const Point& b){
        return a ^ b ^ EM;
      } 
 

       /*-----------------------------------------------------------------------------
        *  SPHERES
        *-----------------------------------------------------------------------------*/
      /// Sphere at x,y,z with radius r (default r=1.0)     
      DualSphere construct::sphere(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, VSR_PRECISION r){
        return round::dls(r, x, y, z );
      }
      /// Sphere at point p with radius r (default r=1.0)
      DualSphere construct::sphere(const Point& p, VSR_PRECISION r){
        return round::dls(p, r);
      }     
 

      /*-----------------------------------------------------------------------------
       *  PLANES
       *-----------------------------------------------------------------------------*/
                              
      ///Dual plane with normal and distance from center
       DualPlane construct::plane( VSR_PRECISION a, VSR_PRECISION b, VSR_PRECISION c, VSR_PRECISION d){
        return Dlp(a,b,c,d);
      } 
      ///Dual plane from vec and distance from center
       DualPlane construct::plane( const Vec& v, VSR_PRECISION d){
        return v + Inf(d);
      } 
      ///Direct plane through three points
       Plane construct::plane(const Pnt& a, const Pnt& b, const Pnt& c){
        return a ^ b ^ c ^ Inf(1);
      }             


       /*-----------------------------------------------------------------------------
        *  LINES
        *-----------------------------------------------------------------------------*/
      /*!
       *  \brief  DualLine axis of circle c
       */
      DualLine construct::axis(const Cir& c){
        return (Inf(-1) <= c).runit();
      }

      /// Line from two Vecs     
      Line construct::line( const Vec& a, const Vec& b){
         return point(a[0], a[1], a[2]) ^ Vec(b[0], b[1], b[2]) ^ Inf(1);
      }


      ///Direct line through origin
      Line construct::line( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ){
        return Ori(1) ^ Vec(x,y,z) ^ Inf(1);
      }
      ///Direct line through origin
      Line construct::dualLine( VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z ){
        return line(x,y,z).dual();
      }      
      
      ///Direct line through two points
       Line construct::line( const Point& a, const Point& b){
        return a ^ b ^ Inf(1);
      }
      ///Direct line through point a in direction b   
       Line construct::line( const Point& a, const Vec& b){
        return a ^ b ^ Inf(1);
      } 

      
       
      ///Squared Distance between a line and a point
       VSR_PRECISION construct::distance( const Lin& lin, const Pnt& pnt){
        return ( pnt <= lin.dual() )[0] * -2.0;
      }
    

      /* Line line(VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z){ */
      /*   return point(a[0], a[1], a[2]) ^ Vec(b[0], b[1], b[2]) ^ Inf(1); */
      /* } */

      #pragma mark COINCIDENCE_FUNCTIONS

       ///circle intersection of dual spheres
       Circle construct::meet( const Dls& s, const Dls& d){
        return (s ^ d).dual();
      }  

      ///circle intersection of dual sphere and direct plane
       Circle construct::meet( const Dls& s, const Dlp& d){
        return (s ^ d).dual();
      }  
      ///circle intersection of dual spehre and direct plane
       Circle construct::meet( const Dls& s, const Pln& d){
        return (s ^ d.dual()).dual();
      }
      //circle intersection of direct sphere and dual plane
       Circle construct::meet( const Sphere& s, const DualPlane& d){
        return (s.dual() ^ d).dual();
      }
      //circle intersection of direct sphere and direct plane
       Circle construct::meet( const Sphere& s, const Plane& d){
        return (s.dual() ^ d.dual()).dual();
      } 


      //normalized and nulled point intersection of line and dual plane
       Point construct::meet( const Line& lin, const DualPlane& dlp){ 
        Flp flp = ( (lin).dual()^ dlp).dual();
        return (flp / flp[3]).null();
      } 
      //normalized and nulled point intersection of dualline and dual plane
       Point construct::meet( const Dll& dll, const DualPlane& dlp){ 
        auto flp = ( dll ^ dlp).dual();
        return flp.null();
      } 

    
      //Point intersection of two lines
       Point construct::meet( const Line& la, const Line& lb){
          Line r = la.reflect(lb);
          Line r2 = (la - r.unit()).unit();
          Point pori = flat::loc(r2, Ori(1), false);
          Point tp = pori.re( lb ); 
          return ( ( (tp / tp[3]) + pori)/2.0 ).null(); 
        }
         /* //test for origin */
        /* auto t = Ori(1) ^ la; Sca(t.wt()).vprint(); */
        /* (t.dual() ^ lb.dual() ).dual().vprint(); */
        /* if (t.wt() != 0 ) { */
         /*  return ( t.dual() ^ lb.dual() ).dual(); */
        /* } else { */
         /*  auto t2 = (Ori(1) ^ lb ); */
         /*  if ( t2.wt() != 0 ) return ( la.dual() ^ t2.dual() ).dual(); */
         /*  else return Flp(); */
         /* } */
       //}  

      //point pair intersection of circle and Dual plane
       Par construct::meet( const Cir& cir, const Dlp& dlp){ 
        return ( (cir).dual() ^ dlp).dual();
      } 
 
      //point pair intersection of circle and Dual sphere
       Par construct::meet( const Cir& cir, const Dls& s){ 
        return ( (cir).dual() ^ s).dual();
      }  

       #pragma mark HIT_TESTS
     
       /*!
        *  \brief  hit tests between point and pair (treats pair as an "edge")
        */
      bool construct::hit(const Point& pnt, const Pair& par ){
          //if inside surround < 0
          if( ( pnt <= round::sur( par ) )[0] < 0 ) {
            if ( (pnt ^ par ^ Inf(1)).wt() == 0.0 ) {
                return true;
              }
          } 
          return false;

      }

  
      /*!
       *  \brief  hit tests between point and circle (treats circle as "disc")
       */
      bool construct::hit(const Point& p, const Circle& cir){

         if ( (p<= round::sur(cir))[0] > 0){
          if ( fabs( (p^round::car(cir)).wt()) <.00001 ) {
            return true;
          }
         }
         return false;
      }

      double construct::squaredDistance(const Point& a, const Point& b){
        return round::sqd(a,b);
      }


      #pragma mark HYPERBOLIC_FUNCTIONS
      /*-----------------------------------------------------------------------------
       *  hyperbolic functions (see alan cortzen's document on this)
       *-----------------------------------------------------------------------------*/

      /*!
       *  \brief  hyperbolic normalization of a conformal point
       */
      Point construct::hnorm(const Pnt& p){
        return -(p / (EP<=p));
      }

      
      /*!
       *  \brief  hyperbolic distance between two conformal points
       */
      double construct::hdist(const Pnt& pa, const Pnt& pb){
        return acosh(1 - (hnorm(pa)<=hnorm(pb))[0]);
      }


      /*!
       *  \brief  hyperbolic translation transformation generator between two conformal points
       */
      Pair construct::hgen(const Pnt& pa, const Pnt& pb, double amt){
        double dist = hdist(pa,pb);         //<-- h distance
        auto hline = pa ^ pb ^ EP;          //<-- h line (circle)
        auto par_versor = (EP <= hline).runit();      //<-- h trans generator (pair)
       // par_versor /= par_versor.rnorm();   //<-- normalized ...
        return par_versor * dist * amt * .5;//<-- and ready to be applied
      }


      /*!
       *  \brief  hyperbolic spin transformation from pa to pb by amt (0,1)
       */
      Point construct::hspin(const Pnt& pa, const Pnt& pb, double amt){
        return round::loc( pa.boost( hgen(pa,pb,amt) ) );
        //versor * dist * amt * .5) );
      }
      


} // cga::
}   //vsr::   


 
