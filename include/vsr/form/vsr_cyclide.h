/*
 * =====================================================================================
 *
 *       Filename:  vsr_cyclide.h
 *
 *    Description:  cyclidic nets
 *
 *        Version:  1.0
 *        Created:  07/07/2015 12:48:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#ifndef  vsr_cyclide_INC
#define  vsr_cyclide_INC

#include "vsr_cga3D_frame.h"

namespace vsr { namespace cga {


/// 3D Frame of Tangent Vectors (Point Pairs) @todo inherit from cga::Frame
struct TangentFrame{

  Frame frame;        ///< perhaps eliminate this

  Sphere sphere[3];   ///< sphere through next point with tangents as below
  Circle circle[3];   ///< circle through next point (unneeded)
  Pair tan[3];
  Circle bitan[3];

  TangentFrame(){
    Frame f;
    frame.pos() = f.pos();;
    bitan[0] = f.tx().dual();
    bitan[1] = f.ty().dual();
    bitan[2] = f.tz().dual();
     
    for (int j=0;j<3;++j){
      tan[j] = bitan[j].undual(); // swap duality here
    }    
  }

  TangentFrame( const Frame& f ){
    frame.pos() = f.pos();
    bitan[0] = f.tx().dual();
    bitan[1] = f.ty().dual();
    bitan[2] = f.tz().dual();
    for (int j=0;j<3;++j){
      tan[j] = bitan[j].undual(); // ditto
    }      
  }

  /// Construct from position and relative TangentFrame
  TangentFrame( const Point&p, const TangentFrame tf) {
    set(p,tf);
  }

  // Calculate Edges as intersections of constant coordinates
  void calcCurves(){
    circle[0] = (sphere[1].dual() ^ sphere[2].dual()).dual();
    circle[1] = (sphere[0].dual() ^ sphere[2].dual()).dual();
    circle[2] = (sphere[0].dual() ^ sphere[1].dual()).dual();
  }

  /// Set from position and relative TangentFrame
  void set( const Point& p, const TangentFrame tf ){
     frame.pos() = p;
     for (int j=0;j<3;++j){
        sphere[j] = tf.bitan[j] ^ p;
        bitan[j] = Tangent::at( sphere[j], frame.pos() );
        tan[j] = bitan[j].undual();
     }
  }

  Drv xdir(){ return -Round::direction(tan[0]); } ///< get x DirectionVector
  Drv ydir(){ return -Round::direction(tan[1]); } ///< get y DirectionVector
  Drv zdir(){ return -Round::direction(tan[2]); } ///< get z DirectionVector

  /// Generate Boost Relative to x TangentVector (null Pair)
  Bst xcurve(float amt){
    return Gen::bst(tan[0]*amt);
  }
  /// Generate Boost Relative to y TangentVector (null Pair)
  Bst ycurve(float amt){
    return Gen::bst(tan[1]*amt);
  }
  /// Generate Boost Relative to z TangentVector (null Pair)
  Bst zcurve(float amt){
    return Gen::bst(tan[2]*amt);
  }
  
  /// Generate Boost Relative to x and y TangentVector (null Pair)
  Bst xycurve(float amtX, float amtY){
    return Gen::bst( tan[0]*amtX + tan[1]*amtY );// + tan[2] *amtZ);
  }
  /// Generate Boost Relative to x and z TangentVector (null Pair)
  Bst xzcurve(float amtX, float amtZ){
    return Gen::bst( tan[0]*amtX + tan[2]*amtZ );// + tan[2] *amtZ);
  }
  /// Generate Boost Relative to y and z TangentVector (null Pair)
  Bst yzcurve(float amtY, float amtZ){
    return Gen::bst( tan[1]*amtY + tan[2]*amtZ );// + tan[2] *amtZ);
  }

  /// x = Constant Coordinate Surface from Boost generator 
  DualSphere xsurface( const Bst& b ){ 
   return DualSphere( Round::carrier(bitan[0].dual() ).spin(b));
  }
  /// y = Constant Coordinate Surface from Boost generator 
  DualSphere ysurface( const Bst& b ){ 
   return DualSphere( Round::carrier(bitan[1].dual() ).spin(b));
  }
  /// z = Constant Coordinate Surface from Boost generator 
  DualSphere zsurface( const Bst& b ){ 
   return DualSphere( Round::carrier(bitan[2].dual() ).spin(b));
  }
  
  /// x = Constant Coordinate Surface from curvature scalar 
  DualSphere xsurface( float amt ) { 
    return DualSphere( Round::carrier(bitan[0]).dual() ).spin(xcurve(amt));    
  }

  /// y = Constant Coordinate Surface from curvature scalar 
  DualSphere ysurface( float amt ){
    return DualSphere( Round::carrier(bitan[1]).dual() ).spin(ycurve(amt));        
  }

  /// z = Constant Coordinate Surface from curvature scalar 
  DualSphere zsurface( float amt ){
    return DualSphere( Round::carrier(bitan[2]).dual() ).spin(zcurve(amt));        
  }

  /// Get position of TangentFrame
  Point point() const { return Round::location(tan[0]); }
  
  /// Point along x direction
  Point xpoint(float amt){ return point().translate(xdir()*amt); }
  /// Point along y direction
  Point ypoint(float amt){ return point().translate(ydir()*amt); }
  /// Point along z direction
  Point zpoint(float amt){ return point().translate(zdir()*amt); }

  /// TangentFrame yzcurved 
  TangentFrame xbend(float amtY, float amtZ,float dist =1){
    auto bst = yzcurve(amtY,amtZ); 
    auto pt = Round::location( xpoint(dist).spin(bst) );
    return TangentFrame(pt, *this);
  }

  /// TangentFrame xzcurved 
  TangentFrame ybend(float amtX, float amtZ,float dist =1){
    auto bst = xzcurve(amtX,amtZ); 
    auto pt = Round::location( ypoint(dist).spin(bst) );
    return TangentFrame(pt, *this);
  }
  
  /// TangentFrame xycurved 
  TangentFrame zbend(float amtX, float amtY,float dist =1){
    auto bst = xycurve(amtX,amtY); 
    auto pt = Round::location( zpoint(dist).spin(bst) );
    return TangentFrame(pt, *this);
  }

  TangentFrame xclose(float amt, const Point& pa, const Point& pb){
    return circleClose( xsurface(amt), pa, pb);
  }
  TangentFrame yclose(float amt, const Point& pa, const Point& pb){
    return circleClose( ysurface(amt), pa, pb);
  }
  TangentFrame zclose(float amt, const Point& pa, const Point& pb){
    return circleClose( zsurface(amt), pa, pb);
  }
  TangentFrame close(const TangentFrame& ta, const TangentFrame& tb, const TangentFrame& tc, const TangentFrame& td){
    
    auto pt = point();

    auto cir = pt ^ ta.point() ^ tb.point();
    auto cir2 = pt ^ tc.point() ^ td.point();

    auto pair = (Round::surround(cir) ^ cir2.dual()).dual();

    auto tpa = Construct::pointA(pair);
    auto tpb = Construct::pointB(pair);

    auto p1 = FERROR( (pt <= tpa).wt() ) ? tpb : tpa;

    return TangentFrame(p1, *this);

  }

  /// Given a sphere and two points, make a circle with the two points and find its intersection with sphere
  TangentFrame circleClose( const DualSphere& s, const Point& pa, const Point& pb ){
    
    auto pt = point();
    auto cir = pt ^ pa ^ pb;
    auto pair = (s ^ cir.dual()).dual();

    /// @todo speed up this extraction
    auto tpa = Construct::pointA(pair);
    auto tpb = Construct::pointB(pair);

    auto p1 = FERROR( (pt <= tpa).wt() ) ? tpb : tpa;

    return TangentFrame(p1, *this);

  }

};

/**
* @brief Contact encapsulates a normal tangent vector at a point on a sphere

  contains a DualSphere, a Point position on the sphere, 
  and a null Pair tangent vector specifying the normal on
  the sphere at that point.
*/
struct Contact{
  
  DualSphere sphere;
  Point point;
  Pair tnv;

  /// Construct from Point and DualPlane
  Contact(const Point& p, const DualPlane& dlp) 
  : point(p), sphere(dlp), tnv( (p <= dlp.dual()).dual() )
  {}

  /// Construct from Point and DualSphere
  Contact(const Point& p, const DualSphere& s) 
  : point(p), sphere(s), tnv( Tangent::at(s.undual(), p).dual() )
  {}

  /// Construct from Point on some other source sphere and some target sphere
  /// (makes orthogonal plunge into target sphere)
  Contact(const Point& p, const DualSphere& source, const DualSphere& target) 
  : sphere(target) {
    auto tnvsource =  Tangent::at(source.undual(), p).dual(); // A Point Pair
    point = Construct::pointA( ( (target^tnvsource).dual() ^ target).dual() );
    tnv = Tangent::at(target.undual(), point).dual();
  }

  /// Construct from another contact and a target 
  Contact( const Contact& source, const DualSphere& target) : sphere(target) {
    point = Construct::pointA( ( ( target^source.tnv).dual() ^ target).dual() );
    tnv = Tangent::at(target.undual(), point).dual();
  }

  /// Get undualized tangentbivector (circle)
  Cir bitan() const { return tnv.dual(); } //*** dualization changed ***/
  /// Get Vector direction of TangentVector
  Vec vec() const { return -Round::direction(tnv).copy<Vec>().unit(); }
  /// Get Bivector dual of vector direction
  Biv biv() const { return vec().dual(); }  //*** dualization changed ***/

};



/// Cyclide Principal Patch (four tangent frames)
/// @todo rename to just Cyclide
struct CyclideQuad{

  /// Four TangentFrames
  TangentFrame tframe[4];

  /// Get/Set ith TangentFrame
  TangentFrame& operator [] (int idx) { return tframe[idx]; }
  /// Get ith TangentFrame
  TangentFrame operator [] (int idx) const { return tframe[idx]; }

  /// Default Constructor
  CyclideQuad(){}

  /// Construct from four Point positions
  CyclideQuad(const Point& a, const Point& b, const Point& c, const Point& d){
    pos(a,b,c,d);
  }

  /// Construct from four Contact elements, and a rotation
  CyclideQuad( const Contact& a, const Contact& b, const Contact& c, const Contact& d, float spin = 0 ){
    set(a,b,c,d,spin);
  }

  /// Construct from four TangentFrames elements
  CyclideQuad( const TangentFrame& ta, const TangentFrame& tb, const TangentFrame& tc, const TangentFrame& td){
    tframe[0] = ta; tframe[1] = tb; tframe[2]=tc, tframe[3]=td; 
    frame(ta, false, false, false, false, 0);
    log();
  }

  void set( const Contact& a, const Contact& b, const Contact& c, const Contact& d, float spin = 0 ){
    pos(a.point, b.point,c.point,d.point);

    
    Rot rot = Gen::ratio( Vec::z, a.vec() );
    Rot rot2 = Gen::rot( Biv::xy.spin(rot) * spin );
         
    frame (TangentFrame(Frame(a.point, rot2) ), false, false, false, false, 0);

    log();

  }


  void pos( const Point& a, const Point& b, const Point& c, const Point& d){
    tframe[0].frame.pos() = a;
    tframe[1].frame.pos() = b;
    tframe[2].frame.pos() = c;
    tframe[3].frame.pos() = d;
  }
  
  Pair mLogU, mLogV;      ///<-- Pair of tensors 
  Pair mLogNu, mLogNv;    ///<-- two more (normals)

  Pair mLogXu;
  Pair mLogXv;
  Pair mLogYu;
  Pair mLogYv;

  /// unused CommonSphere 
  DualSphere commonSphere(){
      return (mLogU ^ mLogV).dual();
  }

  /// Circle foundation of principal patch
  Circle circle() const{
      return tframe[0].frame.pos() ^ tframe[1].frame.pos() ^ tframe[2].frame.pos();
  }

  /// set by attaching to TangentFrame f, 
  /// set booleans: flipx, flipy, corner or continuous, 90 or -90, attach at idx of f)
  void frame(const TangentFrame& f, bool bFlipX, bool bFlipY, bool bCorner, bool bNeg, int begin){

    if (bCorner){

      if (bFlipY) {
        tframe[begin].bitan[1] = f.bitan[2] * (bNeg?-1:1);
        tframe[begin].bitan[2] = -f.bitan[1];
        tframe[begin].bitan[0] = f.bitan[0];
      }
      if (bFlipX){
        tframe[begin].bitan[0] = f.bitan[2] * (bNeg?-1:1);
        tframe[begin].bitan[2] = -f.bitan[0];
        tframe[begin].bitan[1] = f.bitan[1];
      }

    } else {

      for (int i=0;i<3;++i){
        tframe[begin].bitan[i] = f.bitan[i];// * ( (i&1)?1:-1);//.spin(rot);
      }
    
      if (bFlipY) tframe[begin].bitan[1] *= -1;
      if (bFlipX) tframe[begin].bitan[0] *= -1;
    
    }

    for (int j=0;j<3;++j){
     int next = begin < 3 ? begin + 1 : 0;
     tframe[begin].sphere[j] = tframe[begin].bitan[j] ^ tframe[next].frame.pos();
     tframe[begin].tan[j] = tframe[begin].bitan[j].undual();
    }
    
    // Subsequent
    for (int i=1;i<4;++i){
     int cur = begin + i;
     cur = cur % 4;
     int prev = (cur > 0) ? cur -1 : 3;
     int next = ( cur < 3 ) ? cur+1 : 0;
     for (int j=0;j<3;++j){
       tframe[cur].bitan[j] = Tangent::at( tframe[prev].sphere[j], tframe[cur].frame.pos() );
       tframe[cur].sphere[j] = tframe[cur].bitan[j] ^ tframe[next].frame.pos();
       tframe[cur].tan[j] = tframe[cur].bitan[j].undual();
     }
    }
    
    // edges 
    for (int i=0;i<4;++i) tframe[i].calcCurves();

  }

  /// Calculate frame positions (calcFrame())
  void frame(){

    //FIRST ONE
    tframe[0].bitan[0] = tframe[0].frame.tx().dual();
    tframe[0].bitan[1] = tframe[0].frame.ty().dual();
    tframe[0].bitan[2] = tframe[0].frame.tz().dual();

    for (int j=0;j<3;++j){
     tframe[0].sphere[j] = tframe[0].bitan[j] ^ tframe[1].frame.pos();
     tframe[0].tan[j] = tframe[0].bitan[j].undual();
    }
    
    // Subsequent
    for (int i=1;i<4;++i){
     int next = ( i < 3 ) ? i+1 : 0;
     for (int j=0;j<3;++j){
       tframe[i].bitan[j] = Tangent::at( tframe[i-1].sphere[j], tframe[i].frame.pos() );
       tframe[i].sphere[j] = tframe[i].bitan[j] ^ tframe[next].frame.pos();
       tframe[i].tan[j] = tframe[i].bitan[j].undual();
     }
    }
    
    // edges 
    for (int i=0;i<4;++i) tframe[i].calcCurves();
  }

  // Which direction to interpolate
  bool altU(){
    auto dotU =  ( tframe[0].frame.pos() <= tframe[2].sphere[0].dual() )[0];
    return dotU < 0;
  }  

  // Which direction to interpolate
  bool altXv(){
    auto dotU =  ( tframe[0].frame.pos() <= tframe[1].sphere[0].dual() )[0];
    return dotU < 0;
  }  

  // Which direction to interpolate
  bool altV(){
    auto dotV =  ( tframe[0].frame.pos() <= tframe[1].sphere[1].dual() )[0];
    return dotV < 0;
  }  

  // Which direction to interpolate
  bool altYu(){
    auto dotU =  ( tframe[0].frame.pos() <= tframe[2].sphere[1].dual() )[0];
    return dotU < 0;
  }  

  bool altWU(){
    auto dotW =  ( tframe[0].frame.pos() <= tframe[2].sphere[2].dual() )[0];
    return dotW < 0;

  }

  bool altWV(){
    auto dotW =  ( tframe[0].frame.pos() <= tframe[1].sphere[2].dual() )[0];
    return dotW < 0;

  }
      
  /// Calculate logs based on ratios of constant coordinate surface spheres
  void log(){
      mLogU = Gen::log( Gen::ratio(tframe[0].sphere[0].dual(), tframe[2].sphere[0].dual(), true ), altU() );
      mLogV = Gen::log( Gen::ratio(tframe[3].sphere[1].dual(), tframe[1].sphere[1].dual(), true ), altV() );
      
      mLogXv = Gen::log( Gen::ratio(tframe[3].sphere[0].dual(), tframe[1].sphere[0].dual(), true ), altXv() );
      mLogYu =Gen::log( Gen::ratio(tframe[0].sphere[1].dual(), tframe[2].sphere[1].dual(), true ), altYu() );

      logZ();
  }

  void logZ(){
      
      mLogNu = Gen::log( Gen::ratio( tframe[0].sphere[2].dual(), tframe[2].sphere[2].dual(),true ), altWU() );
      mLogNv = Gen::log( Gen::ratio( tframe[3].sphere[2].dual(), tframe[1].sphere[2].dual(),true ), altWV() );
  }
  
  /// Generate simple Rotor in U direction 
  Bst xfu( VSR_PRECISION u ) const { return Gen::bst(mLogU * -u ); }
  
  /// Generate simple Rotor in V direction 
  Bst xfv( VSR_PRECISION v ) const { return Gen::bst(mLogV * -v ); }

  Bst xfwu( VSR_PRECISION wu ) const { return Gen::bst(mLogNu * -wu ); }
  Bst xfwv( VSR_PRECISION wv ) const { return Gen::bst(mLogNv * -wv ); }
  Bst xfxv( VSR_PRECISION amt) const { return Gen::bst(mLogXv * -amt); }
  Bst xfyu( VSR_PRECISION amt) const { return Gen::bst(mLogYu * -amt); }
 
  /// @todo unknown use, possibly eliminate 
  Pair ortho(VSR_PRECISION u, VSR_PRECISION v){
      return tframe[0].sphere[2].dual().spin( xfwu(u) ) ^ tframe[3].sphere[2].dual().spin( xfwv(v) ); 
  }

  /// Generate compound conformal rotor at u,v
  Con xf( VSR_PRECISION u, VSR_PRECISION v) const { return xfv(v) * xfu(u); }

  /// Generate compound conformal rotor at u,v, w @todo eliminate
  //Con xf( VSR_PRECISION u, VSR_PRECISION v, VSR_PRECISION w) const { return xfwv(u) * xfwu(v) * xf(u,v); }

  /// Generate compound conformal rotor 
  //Con xfxz( VSR_PRECISION u, VSR_PRECISION v ) const { return xfwv(v) * xfu(u); }
  //Con xfyz( VSR_PRECISION u, VSR_PRECISION v ) const { return xfwv(v) * xfyu(u); }


  Circle cirU( VSR_PRECISION u) { return tframe[0].circle[1].spin( xfu(u) ); }
  Circle cirV( VSR_PRECISION v) { return tframe[3].circle[0].spin( xfv(v) ); }  
 
  Circle cirW( VSR_PRECISION u, VSR_PRECISION v){
    auto p = eval(u,v); 
    auto parA = p <= mLogU; // dual sphere
    auto parB = p <= mLogV; // dual sphere

    //return (parA ^ parB).dual();
    auto pair = p <= (parA^parB).dual(); //pair
    return pair ^ commonSphere();
  } 

  Circle cirWB( VSR_PRECISION u, VSR_PRECISION v){
    auto p = eval(u,v); 
    auto parA = p <= mLogU; // dual sphere
    auto parB = p <= mLogV; // dual sphere

    return (parA ^ parB).dual();
    //auto pair = p <= (parA^parB).dual(); //pair
    //return pair ^ commonSphere();
  } 


  /// Apply conformal rotor to a type T at u,v
  template<class T>
  T apply( const T& t, VSR_PRECISION u, VSR_PRECISION v) const {
    return t.spin( xf(u,v) );
  }
  /// Apply conformal rotor to a type T at u,v
  template<class T>
  T applyXZ( const T& t, VSR_PRECISION u, VSR_PRECISION v) const {
    return t.spin( xfxz(u,v) );
  }
  /// Apply conformal rotor to a type T at u,v
  template<class T>
  T applyYZ( const T& t, VSR_PRECISION u, VSR_PRECISION v) const {
    return t.spin( xfyz(u,v) );
  }

  /// Apply conformal rotor to a point at u,v
  Point evalXZ( VSR_PRECISION u, VSR_PRECISION v) const {
    return Round::loc( tframe[0].frame.pos().spin( xfxz(u,v) ) );
  }
  /// Apply conformal rotor to a point at u,v
  Point evalYZ( VSR_PRECISION u, VSR_PRECISION v) const {
    return Round::loc( tframe[0].frame.pos().spin( xfyz(u,v) ) );
  }

  // Get normal at u,v 
  Vec evalNormal( VSR_PRECISION u, VSR_PRECISION v){
    auto tan = apply( tframe[0].tan[2], u,v);
    return -Round::direction( tan ).copy<Vec>().unit();
  }
  
  /// Apply conformal rotor to a point at u,v
  Point eval( VSR_PRECISION u, VSR_PRECISION v) const {
    return Round::loc( tframe[0].frame.pos().spin( xf(u,v) ) );
  }

  /// Apply conformal rotor to a point at u,v amt w
  Point eval( VSR_PRECISION u, VSR_PRECISION v, VSR_PRECISION w) const {
    return Round::loc( tframe[0].frame.pos().spin( xf(u,v,w) ) );
  }


};


/**
* @brief A net of cyclide principal patches
*/
struct CircularNet {

  /// std::vector of CyclideQuads;
  vector<CyclideQuad> mCyclide;

  /// Number of Cyclides in Net
  int size() { return mCyclide.size(); }

  CircularNet( const CyclideQuad& c ){
    mCyclide.push_back(c);
  }

  CyclideQuad& operator[] (int idx) { return mCyclide[idx]; }
  CyclideQuad operator[] (int idx) const { return mCyclide[idx]; }

  //idx specifies cyclide, edge specifies which edge
  void addAt(const DualSphere& sphere, int idx, int edge, bool bCorner=false, bool bNeg=false){
    CyclideQuad& cyc = mCyclide[idx];
    switch( edge ){
      case 0: 
      {
        bool bFlipX = true; bool bFlipY = false;
        auto cd = Contact( cyc.tframe[0].frame.pos(), cyc.tframe[0].sphere[2].undual() );
        if (bNeg) cd.tnv *= -1;
        auto ca = Contact( cd, sphere );
        auto cc = Contact( cyc.tframe[1].frame.pos(), cyc.tframe[0].sphere[2].undual() );
        if (bNeg) cc.tnv *= -1;
        auto cb = Contact( cc, sphere );
        CyclideQuad c(ca.point,cb.point,cc.point,cd.point);
        c.frame( cyc.tframe[0], bFlipX, bFlipY, bCorner, bNeg,3);
        c.log();
        mCyclide.push_back(c);
        break;
      }
      case 1: 
      {
        bool bFlipX = false; bool bFlipY = true;
        auto ca = Contact( cyc.tframe[1].frame.pos(), cyc.tframe[0].sphere[2].undual() );
        auto cb = Contact( ca, sphere );
        auto cd = Contact( cyc.tframe[2].frame.pos(), cyc.tframe[2].sphere[2].undual() );
        auto cc = Contact( cd, sphere );
        CyclideQuad c(ca.point,cb.point,cc.point,cd.point);
        c.frame( cyc.tframe[1],  bFlipX,bFlipY,bCorner,bNeg,0);
        c.log();
        mCyclide.push_back(c);
        break;
      }
      case 2: 
      {
        bool bFlipX = true; bool bFlipY = false;
        auto ca = Contact( cyc.tframe[3].frame.pos(), cyc.tframe[2].sphere[2].undual() );
        auto cd = Contact( ca, sphere );
        auto cb = Contact( cyc.tframe[2].frame.pos(), cyc.tframe[2].sphere[2].undual() );
        auto cc = Contact( cb, sphere );
        CyclideQuad c(ca.point,cb.point,cc.point,cd.point);
        c.frame( cyc.tframe[3], bFlipX, bFlipY,bCorner,bNeg,0);
        c.log();
        mCyclide.push_back(c);
        break;
      }
      case 3: 
      {
        bool bFlipX = false; bool bFlipY = false;
        bFlipY = true;
        auto cb = Contact( cyc.tframe[0].frame.pos(), cyc.tframe[0].sphere[2].undual() );
        auto ca = Contact( cb, sphere );
        auto cc = Contact( cyc.tframe[3].frame.pos(), cyc.tframe[2].sphere[2].undual() );
        auto cd = Contact( cc, sphere );
        CyclideQuad c(ca.point,cb.point,cc.point,cd.point);
        c.frame( cyc.tframe[0], bFlipX, bFlipY, bCorner,bNeg, 1);
        c.log();
        mCyclide.push_back(c);
        break;
      }

    }
  } 

  void update(){
    for (auto& i : mCyclide){
      i.log();
    }
  }


};


//experiments with other numbers of frames (six and 3...)
struct NCyclide{

//    tframe[0].frame = control;
//
//    //SET FRAME POSITIONS ALONG CIRCLE
//    for (int i=0;i<num;++i){
//      tframe[i].frame.pos() = point( net.cxy(), (float)i/num*TWOPI);
//    }
//    
//    //SET FRAME POSITIONS ALONG CIRCLE
//    for (int i=0;i<4;++i){
//      tframe[i].frame.pos() = point( net.cxy(), ((float)i/num + pos[i] )*TWOPI);
//    }
//
//    //FIRST ONE
//    tframe[0].bitan[0] = tframe[0].frame.tx().dual();
//    tframe[0].bitan[1] = tframe[0].frame.ty().dual();
//    tframe[0].bitan[2] = tframe[0].frame.tz().dual();
//
//    for (int j=0;j<3;++j){
//     tframe[0].sphere[j] = tframe[0].bitan[j] ^ tframe[1].frame.pos();
//    }
//
//
//    //SUBSEQUENT
//    for (int i=1;i<num;++i){
//     int next = ( i < (num-1) ) ? i+1 : 0;
//     for (int j=0;j<3;++j){
//       tframe[i].bitan[j] = Tangent::at( tframe[i-1].sphere[j], tframe[i].frame.pos() );
//       tframe[i].sphere[j] = tframe[i].bitan[j] ^ tframe[next].frame.pos();
//       tframe[i].tan[j] = tframe[i].bitan[j].undual();
//     }
//    }
//
//    for (int i=0;i<num;++i) tframe[i].calcCurves();


};


}} //vsr::cga::

#endif   /* ----- #ifndef vsr_cyclide_INC  ----- */
