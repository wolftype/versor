/*
 * =====================================================================================
 *
 *       Filename:  vsr_tangent.h
 *
 *    Description:  tangent frames, contact methods, and coordinate surface six-spheres
                    in vsr::cga
 *
 *        Version:  1.0
 *        Created:  04/21/2016 16:18:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#ifndef  vsr_tangent_INC
#define  vsr_tangent_INC

#include "vsr_cga3D_frame.h"

namespace vsr { namespace cga {


/// 3D Frame of Tangent Vectors (Point Pairs) @todo make a six-sphere system vsr_coord
/// @todo currently inherits from Frame -- might want to be able to set parent class Frame::pos and Frame::rot from 
/// tangent info stored here.
/// possibly make more method, and less memory, oriented
struct TangentFrame : public Frame{

  /// Coordinate Surface Spheres (Sigma)
  Sphere sphere[3];   ///< sphere through next point with tangents as below (needed or put in next level?)
  
  /// \kappa and Kappa
  Pair tan[3];
  Circle bitan[3];
  /// Derivatives in every possible direction
  //float curvature[6]; 

  /// Default Construct -- Store Frame as Tangents
  TangentFrame() : Frame() {
    store();
  }

  /// Copy Construct from Frame -- Store Frame as Tangents
  TangentFrame( const Frame& f ) : Frame(f) {
    store();
  }

  /// Construct at position p w.r.t relative to TangentFrame
  TangentFrame( const Point& p, const TangentFrame tf) {
    set(p,tf);
  }

  /// Assignment to Frame
  TangentFrame& operator = (const Frame& f){
    this->rot() = f.rot();
    this->pos() = f.pos();
    return store();
  }

  /// Assignment to TangentFrame (needed?)
  TangentFrame& operator = (const TangentFrame& f){
    this->rot() = f.rot();
    this->pos() = f.pos();

    for (int j=0;j<3;++j){
      tan[j] = f.tan[j];
      bitan[j] = f.bitan[j]; // swap duality here
      sphere[j] = f.sphere[j];
    }
    
    return *this;
    
  }


  /// Save current Frame State
  TangentFrame& store(){
    
    tan[0] = tx();
    tan[1] = ty();
    tan[2] = tz();

    for (int j=0;j<3;++j){
      bitan[j] = tan[j].undual(); // swap duality here
    }
    
    return *this;
  }

  /// Set current tangents at position from relative TangentFrame
  void set( const Point& p, const TangentFrame tf ){
     this->pos() = p;
     for (int j=0;j<3;++j){
        sphere[j] = tf.bitan[j] ^ p;  //note this outer product order really matters for reflections
        bitan[j] = Tangent::at( sphere[j], p );
        tan[j] = bitan[j].dual();
     }
  }


  /// Flip direction of ith Tangent
  TangentFrame& flip(int idx){
    tan[idx] *= -1;
    bitan[idx] *= -1;
    return *this;
  }

  /// Make Unit Length (unneeded?)
  TangentFrame unit() const{
    TangentFrame tf = *this;
    for (int idx =0;idx < 3; ++idx){
      // normalize euclidean representation and reformulate
       tf.sphere[idx] = sphere[idx] / (Round::direction( sphere[idx])[0]); //normalize coordinate surface?
       tf.bitan[idx] = Circle( Round::direction( bitan[idx] ).copy<Biv>().runit().copy<Tnb>() ).translate( pos() );
       tf.tan[idx] = tf.bitan[idx].dual();
    }
    return tf;
  }
  

  /// Calculate Edges as intersections of constant coordinates (legacy)
  Circle calcCurve(int idx){
    Circle c;
    switch (idx){
      case 0: c = (sphere[1].dual() ^ sphere[2].dual()).undual();
      case 1: c = (sphere[0].dual() ^ sphere[2].dual()).undual();
      case 2: c = (sphere[0].dual() ^ sphere[1].dual()).undual();
    }
    return c;
  }


  Drv xdir(){ return -Round::direction(tan[0]); } ///< get x DirectionVector
  Drv ydir(){ return -Round::direction(tan[1]); } ///< get y DirectionVector
  Drv zdir(){ return -Round::direction(tan[2]); } ///< get z DirectionVector

  /// Generate Boost Relative to x TangentVector (null Pair)
  Bst xcurve(float amt){
    return Gen::bst(tan[0]*amt*-.5);
  }
  /// Generate Boost Relative to y TangentVector (null Pair)
  Bst ycurve(float amt){
    return Gen::bst(tan[1]*amt*-.5);
  }
  /// Generate Boost Relative to z TangentVector (null Pair)
  Bst zcurve(float amt){
    return Gen::bst(tan[2]*amt*-.5);
  }

  /// Generate Boost Relative to const x and const y TangentVector (null Pair)
  Bst xycurve(float amtX, float amtY){
    return Gen::bst( (tan[0]*amtX + tan[1]*amtY) * -.5 );// + tan[2] *amtZ);
  }
  /// Generate Boost Relative to x and z TangentVector (null Pair) (dx/dy + dz/dy)
  Bst xzcurve(float amtX, float amtZ){
    return Gen::bst( (tan[0]*amtX + tan[2]*amtZ) * -.5 );// + tan[2] *amtZ);
  }
  /// Generate Boost Relative to y and z TangentVector (null Pair)
  Bst yzcurve(float amtY, float amtZ){
    return Gen::bst( (tan[1]*amtY + tan[2]*amtZ) * -.5 );// + tan[2] *amtZ);
  }

  /// x = Constant Coordinate Surface from Boost generator
  DualSphere xsurface( const Bst& b ){
   return DualSphere( Round::carrier(bitan[0]).dual() ).spin(b);
  }
  /// y = Constant Coordinate Surface from Boost generator
  DualSphere ysurface( const Bst& b ){
   return DualSphere( Round::carrier(bitan[1]).dual() ).spin(b);
  }
  /// z = Constant Coordinate Surface from Boost generator
  DualSphere zsurface( const Bst& b ){
   return DualSphere( Round::carrier(bitan[2]).dual() ).spin(b);
  }

  /// An x = Constant Coordinate Surface from curvature scalar
  DualSphere xsurface( float amt ) {
    return DualSphere( Round::carrier(bitan[0]).dual().unit() ).spin(xcurve(amt));
  }

  /// A y = Constant Coordinate Surface from curvature scalar
  DualSphere ysurface( float amt ){
    return DualSphere( Round::carrier(bitan[1]).dual().unit() ).spin(ycurve(amt));
  }

  /// A z = Constant Coordinate Surface from curvature scalar
  DualSphere zsurface( float amt ){
    return DualSphere( Round::carrier(bitan[2]).dual().unit() ).spin(zcurve(amt));
  }

  /// Get position of TangentFrame
  Point point() const { return pos(); }

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

  /// Use Const X Surface to Find fourth TangentFrame on a circle
  TangentFrame xclose(float amt, const Point& pa, const Point& pb){
    return circleClose( xsurface(amt), pa, pb);
  }
  /// Use Const Y Surface to Find fourth TangentFrame on a circle
  TangentFrame yclose(float amt, const Point& pa, const Point& pb){
    return circleClose( ysurface(amt), pa, pb);
  }
 /// Use Const Z Surface to Find fourth TangentFrame on a circle
  TangentFrame zclose(float amt, const Point& pa, const Point& pb){
    return circleClose( zsurface(amt), pa, pb);
  }

  /// Given four tangent frames, find corner
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
* @brief Contact provides methods for calculations with a normal tangent vector at a point on a sphere

  contains a DualSphere, a Point position on the sphere,
  and a null Pair tangent vector specifying the normal on
  the sphere at that point.  Normal may be pointing inwards.
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


/// SixSphere Coordinate System curvatures in every direction (see Moon and Spencer for coordinate system diagram)
/// @todo simplify this representation -- tan and bitan should be methods?
struct SixSphere {

  TangentFrame frame;
  struct Curve{ float a=0; float b=0; } curve[3]; ///< Three Coordinate Curves, each with two curvatures

  float lengthX, lengthY, lengthZ;

  SixSphere() {}
  SixSphere( const Frame& f) : frame(f) {}
//  SixSphere& operator= (const Frame& f){
//    this->pos() = f.pos();  this->rot() = f.rot();
//    store();
//    return *this;
//  }

  /// Set curvatures in every direction
  void set(
            float YX, float ZX,       // constant X in the Y and Z directions
            float XY, float ZY,      // constant Y in the X and Z directions
            float XZ, float YZ,     // constant Z in the X and Y directions
            float lx=1, float ly=1, float lz=1 )//distances in each direction
            {
              curve[0].a = YX; curve[0].b = ZX;
              curve[1].a = XY; curve[1].b = ZY;
              curve[2].a = XZ; curve[2].b = YZ;

              lengthX = lx; lengthY = ly; lengthZ = lz;
            }

  /// Tangent Frame in x direction
  TangentFrame x(){ return frame.xbend(curve[0].a, curve[0].b, lengthX).unit(); }
  /// Tangent Frame in y direction
  TangentFrame y(){ return frame.ybend(curve[1].a, curve[1].b, lengthY).unit(); }
  /// Tangent Frame in z direction
  TangentFrame z(){ return frame.zbend(curve[2].a, curve[2].b, lengthZ).unit(); }
  /// Tangent Frame in x+y direction (with optional additional curve input)  
  TangentFrame xy(float c = 0.f){ 
    float XY = cxy();
    float X1Y = XY == 0 ? 0 : XY > 0 ? -(1.f/((1.f/XY) - lengthX)) : 1.f/((1.f/-XY)+lengthX);
    return x().xclose(X1Y+c, frame.pos(), y().pos()).unit(); 
  }
  /// Tangent Frame in x+z direction (with optional additional curve input)
  TangentFrame zx(float c = 0.f){ 
    float ZX = czx();
    float Z1X = ZX == 0 ? 0 : ZX > 0 ? -(1.f/((1.f/ZX) - lengthZ)) : 1.f/((1.f/-ZX)+lengthZ);
    return z().zclose(Z1X+c, frame.pos(), x().pos()).unit(); 
  }
  /// Tangent Frame in y+z direction (with optional additional curve input)  
  TangentFrame zy(float c = 0.f){ 
    float ZY = czy();
    float Z1Y = ZY == 0 ? 0 : ZY > 0 ? -(1.f/((1.f/ZY) - lengthZ)) : 1.f/((1.f/-ZY)+lengthZ);
    return z().zclose(Z1Y+c, frame.pos(), y().pos()).unit(); 
  }
  /// Tangent Frame in x+y+z direction  
  TangentFrame xyz(float cx=0.f, float cy=0.f){ return xy().close(x(), zx(cx), y(), zy(cy)).unit(); }
  
  float& cyx(){ return curve[0].a; }
  float& czx(){ return curve[0].b; }
  float& cxy(){ return curve[1].a; }
  float& czy(){ return curve[1].b; }
  float& cxz(){ return curve[2].a; }
  float& cyz(){ return curve[2].b; }

  //TangentFrame xy(float amtX1Y){ return x().xclose( amtX1Y, pos(), y().pos() ); }
  //TangentFrame xyz(){ return }
};




}} //vsr::cga::

#endif   /* ----- #ifndef vsr_tangent_INC  ----- */
