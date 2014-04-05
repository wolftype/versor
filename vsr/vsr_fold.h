/*
 *
 *
 *
 *
 *
 */


#ifndef VSR_H_FOLD_INCLUDED
#define VSR_H_FOLD_INCLUDED

#include "vsr_cga3D_op.h"
#include "vsr_cga3D_funcs.h"
#include "vsr_set.h"

namespace vsr{


//Set of points around a center
/* struct Radial : Set<Point> { */
  
/*   template< typename ... T> */
/*   Radial( T ... p ) { */
/*     add( p )...; */
/*   } */

/* }; */


struct Constrain {

    /* Pnt * target; */
    /* Dls * sphere; */
    /* Dll * line; */

    /* void crease() { */
    /*   *target */
    /* } */

    static Point Crease (const Pnt& a, const Pnt& dls, const Dll d, bool mtn){
      // Dls dls = Ro::at(b, a);
       Circle cir = a ^ d;
       Pair par = ( dls ^ cir.dual()).dual();
       return Ro::loc( Ro::split(par,mtn) );
    }

    // three distances
    static Point Triple (const Dls& da, const Dls& db, const Dls& dc, bool mtn){
       return Ro::loc( Ro::split( (da ^ db ^ dc).dual(), mtn ) ) ; 
    }
    // three distances NO SPLIT
    static Point Triple0 (const Dls& da, const Dls& db, const Dls& dc){
       return Ro::loc( (da ^ db ^ dc).dual()  ) ; 
    }
  
    //tangency constraint, two distances and an original point
    static Point Tangency(const Pnt& p, const Dls& da, const Dls& db){

      auto meet = (da ^ db).dual();
      auto tan =  Ro::loc( Ta::at( meet, p ) );
      auto sur = Ro::sur( meet );
      auto line = tan ^ sur ^ Inf(1);
      auto np = Ro::split ( ( line.dual() ^ sur).dual(), false );

      return np;
      
    }


};

//distance from point p
struct Distance {
  
    Pnt p; 
    float t;

    Distance(){}

    Distance ( const Pnt& a, const Pnt& target) :  
    p( a ), t( Ro::rad( Ro::at(a, target) ) )
    {}

    //new target
    void set( const Pnt& target ){
        t = Ro::rad( Ro::at(p,target) );
    }

    //updated source
    void src( const Pnt& s ) {
       p = s;
    }

    //new source and target
    void set(const Pnt& a, const Pnt& target){
       p = a; set( target);
    }

    Dls operator()(){ return Ro::dls(p,t); }
    Dls operator()(const Pnt& tp) { return Ro::dls(tp,t); }

};


//experimental
struct DistancePtr {
  Pnt *src;// *target; 
  float t;

  DistancePtr( Pnt& a, const Pnt& target) :
  src(&a), t( Ro::rad( Ro::at(a, target) ) )
  {}

  Dls constraint(){ return Ro::dls( *src, t ); }
};


//2 distances and an origin define a circular ORBIT

struct Rigid2 {
  
  Distance distA, distB;

  Point origin;

  bool mtn;

  Rigid2() : mtn (true) {}

  /// Input target, and two points (CLOCKWISE)
  Rigid2( const Pnt& target, const Pnt& pa, const Pnt& pb, bool m = true ) :
  distA(pa, target), distB(pb, target), origin(target), mtn(m)
  {
      //pa.vprint(); pb.vprint();    
  }

  /// (Re)Set the constraint
  void set( const Pnt& target, const Pnt& pa, const Pnt& pb) {
    distA.set( pa, target);
    distB.set( pb, target);
    origin = target;
  }

  /// Update Position of first point
  void updateA( const Pnt& p ) { distA.src(p); }
   /// Update Position of second point
  void updateB( const Pnt& p ) { distB.src(p); }


  /// Constrain some other point to lie on orbit
  Pnt operator() ( const Pnt& p ) {
      return Constrain::Tangency( p, distA(), distB() );
  }

  /// Some THETA along Orbit (from orbit's origin for now . . .);
  Pnt operator() ( float amt ) {
    return Ro::pnt_cir( circle(), amt );
  }

  /// Constrain original point to lie on circle (at closest point on possibility circle to starting position)
  Pnt operator()() { 
      return (*this)(origin);// Constrain::Tangency( origin, distA(), distB() );
  }

  /// Constrain Tangentially by some linear amt t of origin and some other point
  Pnt operator() (const Pnt& p, float t ){
      return (*this)( origin * (1-t) + p * t );
  }

  //Constrain by some linear amt of two other points
  Pnt operator() (const Pnt& pa, const Pnt& pb, float t ){
      return (*this)( pa * (1-t) + pb * t );
  }

  Pnt distanceA(const Pnt& p, bool b ) { return Ro::split( ( distA(p) ^ circle().dual() ).dual(), b ); }
  Pnt distanceB(const Pnt& p, bool b) { return Ro::split( ( distB(p) ^ circle().dual() ).dual(), b ); }

  Cir circle() { return ( distA() ^ distB() ).dual(); }

  /// get point at theta t around constraint orbit
  Pnt orbit(VT t) { return Ro::pnt_cir( circle(), t * ( mtn?1:-1) ); }

  /// Dual Plane Facet (CCW)
  Dlp dlp(){
    return ( (*this)() ^ distB.p ^ distA.p ^ Inf(1) ).dual(); 
  }

};

//3 Distance constraints and a boolean define a point
struct Rigid3 {

    Distance a, b, c;
    bool mtn; //mountain or valley crease

   // Point target;
   //
    Rigid3(){}

    /// Clockwise Construction
    Rigid3 ( const Pnt& _target , const Pnt& pa, const Pnt& pb, const Pnt& pc, bool b)
    : a(pa, _target ), b(pb, _target), c(pc, _target), mtn(b) {}

    void update(const Pnt& pa, const Pnt& pb, const Pnt& pc) {
      a.src(pa); b.src(pb); c.src(pc);
      //return (*this)();
    }

    void updateA(const Pnt& pa ) { a.src(pa); }
    void updateB(const Pnt& pb ) { b.src(pb); }
    void updateC(const Pnt& pc ) { c.src(pc); }


    //update constraint locations
    Pnt operator()(const Pnt& pa, const Pnt& pb, const Pnt& pc, bool bSplit = true) {
      a.p = pa; b.p = pb; c.p = pc;
      return (bSplit) ? split() : tangent(); //Constrain::Triple( a(), b(), c(), mtn) : Constrain::Triple0( a(), b(), c() );
    }

    void reverse() { mtn = !mtn; } ///< Flip Mountain / Valley 

    Pnt operator()() { return Constrain::Triple( a(), b(), c(), mtn ); }

    Pnt operator()(bool morv) { mtn = morv; return Constrain::Triple( a(), b(), c(), mtn ); }

    /// Centering (not deciding between crease or valley . . .)
    Pnt split() { return Constrain::Triple( a(), b(), c(), mtn); }
    Pnt center() { return Constrain::Triple0( a(), b(), c() ); }
    Pnt tangent() { return Constrain::Tangency( a(), b(), c() ); }

    Par possible() { return ( a() ^ b() ^ c() ).dual(); }

    //targets can move
    void set(const Pnt& target) { a.set(target); b.set(target); c.set(target); }
 
    //so can sources ...
    void set(const Pnt& target, const Pnt& pa, const Pnt& pb, const Pnt& pc, bool tb ){
       mtn = tb;
       a.set(pa, target); b.set(pb,target); c.set(pc, target);
    }

    Pair orbit() { return ( a() ^ b() ^ c() ).dual(); }

    /// Dual Plane A (CCW)
    Dlp dlpA(){
      return ( (*this)() ^ b.p ^ a.p ^ Inf(1) ).dual();
    }

    /// Dual Plane B (CCW)
    Dlp dlpB(){
      return ( (*this)() ^ c.p ^ b.p ^ Inf(1) ).dual();
    }



};

//6 point Truss
struct Truss{

    struct Points{
      Pnt tl, tm, tr;//, br, bm, bl;
    };

    Points p;

    bool bUseSplit;

    Truss::Points base() const {
      return {rigidA.a.p, rigidA.b.p, rigidA.c.p};
    }

    Rigid3 rigidA, rigidB, rigidC;

    void reverse() {
      rigidA.reverse();
      rigidB.reverse();
      rigidC.reverse();
    }
   // Pnt a, b, c;

    Truss( const Pnt& b, const Pnt& c, const Pnt& d, 
             const Pnt& a, const Pnt& f, const Pnt& e, bool mtn)
    : rigidA( c, a, f, e, mtn ),
      rigidB( b, a, f, c, mtn ),
      rigidC( d, c, f, e, mtn ),
      bUseSplit( true )
      //a(pb), b(pc), c(pd)
      
      {
        p.tl = rigidB(); p.tm = rigidA(); p.tr = rigidC();
      }

      Truss::Points update(const Pnt& pa, const Pnt& pb, const Pnt& pc){
        p.tm = rigidA( pa, pb, pc, bUseSplit );
        p.tl = rigidB( pa, pb, p.tm, bUseSplit);
        p.tr = rigidC( p.tm, pb, pc,  bUseSplit);
        return p;
      }


      Truss::Points update(const Truss& t){
         Points tp = t.p;
         return update( tp.tl, tp.tm, tp.tr);// p;
      }

            //self update
      Truss::Points update(){
        return update(*this);
      }


};

struct Fold {

  /* static Line Bisect(const Point& a, const Point& b, const Point& c){ */
  /*     Line lab =  a ^ b ^ Inf(1); */
  /*     Line lbc = b ^ c ^ Inf(1); */
      
  /*     return (lab - lac).unit(); */
  /* } */

  /* static vector<Line> Bisect(const Point& a, const Point& b, const Point& c){ */
  /*     Line lab =  a ^ b ^ Inf(1); */
  /*     Line lbc = b ^ c ^ Inf(1); */
      
  /*     return (lab - lbc).unit(); */
  /* } */


    static vector<Line> Lines(const Point& a, const Point& b, const Point& c){
      vector<Line> vl;
      vl.push_back( (a ^ b ^ Inf(1)).runit() );
      vl.push_back( (b ^ c ^ Inf(1)).runit() );
      vl.push_back( (c ^ a ^ Inf(1)).runit() );
      return vl;
    }

    //Perimeter
    static vector<Line> Lines(const Point& a, const Point& b, const Point& c, const Point& d){
      vector<Line> vl;
      vl.push_back( (a ^ b ^ Inf(1)).runit() );
      vl.push_back( (b ^ c ^ Inf(1)).runit() );
      vl.push_back( (c ^ d ^ Inf(1)).runit() );
      vl.push_back( (d ^ a ^ Inf(1)).runit() );
      return vl;
    }

    //Perimeter
    static vector<Line> Lines( const vector<Point>& src ){
        vector<Line> vl;
        for (int i = 0; i < src.size()-1; ++i){
          vl.push_back( ( src[i] ^ src[i+1] ^ Inf(1) ).unit() );
        }
        vl.push_back( (src.back() ^ src[0] ^ Inf(1) ).unit() );
        return vl;
    }
    
    //Perimeter
    static vector<Line> Lines( Point * src, int num ){
        vector<Line> vl;
        for (int i = 0; i < num -1; ++i){
          vl.push_back( ( src[i] ^ src[i+1] ^ Inf(1) ).unit() );
        }
        vl.push_back( (src[num-1] ^ src[0] ^ Inf(1) ).unit() );
        return vl;
    }

    //Radial
    static vector<Line> Lines( const Point& center, const vector<Point>& nodes){
        
        vector<Line> vl;
        for (auto i : nodes){
          vl.push_back( ( center ^ i ^ Inf(1) ).unit() );
        }

        return vl;
    }



    static vector<Line> Bisect( const vector<Line>& lines ){
        vector<Line> vl;
        vl.push_back( (lines.back() - lines[0]).unit() );
        for (int i = 0; i < lines.size()-1; ++i){
          vl.push_back( ( lines[i] - lines[i+1] ).unit() );
        }
        return vl;
    }

    static Point Meet ( const vector<Line>& lines) {
      return meet( lines[0], lines[1] ) ;
    }

    static Point Perpendicular( const Point& c, const Line& line){
      return Fl::loc(line, c, false).null();
    }

    static vector<Point> Perpendiculars( const Point& c, const vector<Line>& lines){
      vector<Point> vp;

      for (auto i : lines){
        vp.push_back( Fl::loc(i, c, false).null() ); //location of point closest to line
      }

      return vp;
    }


    static vector<Circle> Circles( const vector<Point>& src, const vector<Line>& crease ){
       vector<Circle> vc;
       for (int i = 0; i < src.size(); ++i) {
           vc.push_back( src[i] ^ crease[i].dual()  );
       }

       return vc;
    }


};

struct Waterbomb {

  //Point mA, mB, mC, mD, mCenter;
  struct Data{
     Point a, b, c, d, e, f, g;
     Point& operator[](int idx){ return ((&a)[idx]); }
     Point * begin() { return &a; }
  } V;

  Rigid2 rg, rc, rb;
  Rigid3 rf;

  Waterbomb( const Point& pa, const Point& pb, const Point& pc, const Point& pd)
  : V{pa,pb,pc,pd} {
    build();
  }

  void build(){
    
      auto edges = Fold::Lines( V.begin(), 4 );     //fold lines

      auto valleys = Fold::Bisect(edges);           //bisectors

      V.e = Fold::Meet( valleys );                  //center

      auto nodes = Fold::Perpendiculars( V.e, edges ); //new nodes
      
      V.f = nodes[0];
      V.g = nodes[2];

      rg.set( V.g, V.e, V.d );
      rc.set( V.c, V.e, V.g );
      rb.set( V.b, V.f, V.e );
      // rb.set( V.b, V.e, V.c );
      rf.set( V.f, V.b, V.e, V.a,false);
       
  }

  Waterbomb::Data eval( VT amt){
      Pnt tg = rg(amt*PIOVERTWO);
     // rc.updateB(tg);
      Pnt tc = rc(amt*PIOVERTWO);// V.b, V.d, amt);
     // rb.updateB(tc);
      Pnt tb = rb(amt*PIOVERTWO);// V.c, V.f, amt);     
      rf.updateA(tb);
      Pnt tf = rf();
      return {V.a, tb, tc, V.d, V.e, tf, tg};
  }

};

struct Gusset {

  /* static Gusset Fold(const Pnt& a, const Pnt& b, const Pnt& c, const Pnt& d, VT amt){ */
      
  /* } */

};

/*!
 * =====================================================================================
 *        Class:  RabbitEar
 *  Description:  Folds a triangle molecule (aka spherical crank)
 * =====================================================================================
 */

struct RabbitEar{

  struct Data{
     Point a, b, c, d, e;
     Point& operator[](int idx){ return ((&a)[idx]); }
  } V;

  Rigid2 ra;
  Rigid3 rb;

  RabbitEar(const Point& a, const Point& b, const Point& c)
  : V{a,b,c}
  {
    build();
  }
  
  void build(){
    auto edges = Fold::Lines( V.a, V.b, V.c);
    auto bisectors = Fold::Bisect( edges );
    
    V.e = meet( bisectors[0], bisectors[1] );    
    V.d = Fold::Perpendicular( V.e, edges[1] );

    ra.set( V.d, V.e, V.c );
    rb.set( V.b, V.d, V.e, V.a, true);
  }

  RabbitEar::Data eval( double amt ){
    Pnt td = ra(amt);
    rb.updateA( td );
    Pnt tb = rb();
    return { V.a, tb, V.c, td, V.e };
  }


};

struct Petal {

    Point nodeA, nodeB, flapA, flapB, tip, base, stamen;
  
    static Petal Fold(const Point& a, const Point& b, const Point& c, const Point& d, float amt){

      Petal petal;

      Line bd =  b ^ d ^ Inf(1); bd = bd.runit();
      Line da =  d ^ a ^ Inf(1); da = da.runit();

      //Bisectors
      Line petalA = bd - da; petalA = petalA.runit(); 
      Line petalB = petalA.reflect( bd );  //Draw(petalA); Draw(petalB);
     
      //New nodes
      Point petalNodeA = meet(petalA, a ^ b ^ Inf(1) ); //Draw(petalNodeA,0,1,0);
      Point petalNodeB = meet(petalB, b ^ c ^ Inf(1) ); //Draw(petalNodeB,0,1,0);

      //Line between them
      Line petalLine = petalNodeA ^ petalNodeB ^ Inf(1); petalLine = petalLine.runit();
      //new point
      Point dmd = Ro::at(d, a).mot( petalLine.dual() * amt );// Draw(mmb,0,1,0);

      //Sphere about new point
      //Dls dmd = Ro::dls_pnt(md);
      //Circle of rotation
      Circle petalCa = a ^ petalA.dual();// Draw(petalCa);
      Circle petalCb = c ^ petalB.dual();// Draw(petalCb);

      Pair petalPairA = ( dmd ^ petalCa.dual() ).dual(); 
      Pair petalPairB = ( dmd ^ petalCb.dual() ).dual(); 

      Point ppa = Ro::loc( Ro::split( petalPairA, true) );
      Point ppb = Ro::loc( Ro::split( petalPairB, true) );

      petal.nodeA = petalNodeA;
      petal.nodeB = petalNodeB;
      petal.flapA = ppa;
      petal.flapB = ppb;
      petal.tip = Ro::loc(dmd);
      petal.base = d;
      petal.stamen = b;

      return petal;
   }
};

struct Preliminary {
    Point mA, mB, mC, mD, mE, mAB, mBC, mCD, mDA;

   // Preliminary( const& Pnt a, const& Pnt b, const& Pnt c, const& p

    static Preliminary Fold (const Point& a, const Point& b, const Point& c, const Point& d, VT amt){


        Line ab = a ^ b ^ Inf(1); ab = ab.runit(); //Draw(ab);
        Line bc = b ^ c ^ Inf(1); bc = bc.runit();// Draw(bc); 
        Line cd = c ^ d ^ Inf(1); cd = cd.runit();// Draw(cd);
        Line da = d ^ a ^ Inf(1); da = da.runit();// Draw(da);


        //Parallel, midlines
        Line sa = (bc - da).runit(); // Draw(sa);
        Line sb = (ab - cd ).runit();// Draw(sb);

        //Diagonal
        Line diag = ( a ^ c ^ Inf(1)); // a was e

        // mid point
        Point e = meet(sa, diag); 

        //Midpoints along Edges
        Point mdc = ((d+c)/2.0).null();
        Point mda = ((d+a)/2.0).null();

        //Primary Mover
        Point mc = c.mot( sa.dual() * amt); 

        //Constraints for midpoint bc
        Dls dmc = Ro::dls_pnt( mc ); 
        Line lmc = a ^ mc ^ Inf(1); lmc = lmc.runit();      // a was e    
        Circle cmc = mdc ^ ( diag.dual() );
        Pair pmc = (cmc.dual() ^ dmc ).dual();
        Point mbc = Ro::loc(Ro::split(pmc,false));          //  Notice Alternating booleans . . .
        //Draw( mbc );

        //Constraints for point mb
        Dls dmbc = Ro::dls_pnt( mbc );                       // Take a sphere for Distance edge
        Circle cmb = (b ^ ( ( a ^ c ^ Inf(1) ).dual() ) );   // And  Circle for Rotational crease (a was e)
        Pair pmb = ( cmb.dual() ^ dmbc ).dual();             // And intersect them
        Point mb = Ro::loc( Ro::split(pmb,true) );
        //Draw(mb); 
        
        //Constraints for midpoint mab
        Dls dmb = Ro::dls_pnt( mb );                         // Repeat
        Circle cmab = ((a+b)/2.0).null() ^ diag.dual();
        Pair pmab = ( cmab.dual() ^ dmb ).dual();
        Point mab = Ro::loc( Ro::split(pmab, false) );
        //Draw(mab);

        Dls dmab  = Ro::dls_pnt( mab );
        Circle cma = a ^ sb.dual();
        Pair pma = ( cma.dual() ^ dmab ).dual();
        Point ma = Ro::loc( Ro::split(pma,true) );
        //Draw(ma);

        return { ma, mb, mc, d, e, mab, mbc, mdc, mda };
    }

};


} //vsr::

#endif

