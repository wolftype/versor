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


/// Frame Tangent to Surface
struct TangentFrame{

  Frame frame;
  Sphere sphere[3];   ///< sphere through next point
  Circle circle[3];   ///< circle through next point
  Pair tan[3];
  Circle bitan[3];

  // Calculate Edges as intersections of constant coordinates
  void calcCurves(){
    circle[0] = (sphere[1].dual() ^ sphere[2].dual()).dual();
    circle[1] = (sphere[0].dual() ^ sphere[2].dual()).dual();
    circle[2] = (sphere[0].dual() ^ sphere[1].dual()).dual();
  }
};

/**
* @brief normal tangent vector at point of sphere
*/
struct Contact{
  
  Point point;
  Pair tnv;
  DualSphere sphere;

  Contact(const Point& p, const DualSphere& s) 
  : point(p), sphere(s), tnv( Tangent::at(s.undual(), p).dual() )
  {}

  Contact(const Point& p, const DualSphere& source, const DualSphere& target) 
  : sphere(target) {
    auto tnvsource =  Tangent::at(source.undual(), p).dual();
    point = Construct::pointA( ( (target^tnvsource).dual() ^ target).dual() );
    tnv = Tangent::at(target.undual(), point).dual();
  }

  Contact( const Contact& source, const DualSphere& target) : sphere(target) {
    point = Construct::pointA( ( ( target^source.tnv).dual() ^ target).dual() );
    tnv = Tangent::at(target.undual(), point).dual();
  }


};


/// Cylclidic Net (four tangent frames)
struct CyclideQuad{

  TangentFrame tframe[4]; ///<-- four frames

  TangentFrame& operator [] (int idx) { return tframe[idx]; }
  TangentFrame operator [] (int idx) const { return tframe[idx]; }

  CyclideQuad(){}
  CyclideQuad(const Point& a, const Point& b, const Point& c, const Point& d){
    pos(a,b,c,d);
  }

  void pos( const Point& a, const Point& b, const Point& c, const Point& d){
    tframe[0].frame.pos() = a;
    tframe[1].frame.pos() = b;
    tframe[2].frame.pos() = c;
    tframe[3].frame.pos() = d;
  }
  
  Pair mLogU, mLogV;      ///<-- two bivectors
  Pair mLogNu, mLogNv;    ///<-- two more (normals)

  DualSphere commonSphere(){
      return (mLogU ^ mLogV).dual();
  }


  //from some input, either flipx or y, either on corner or continuous, 90 or -90, attach at ..)
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

  // Calculate frame positions
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
  bool altV(){
    auto dotV =  ( tframe[0].frame.pos() <= tframe[1].sphere[1].dual() )[0];
    return dotV < 0;
  }  

  bool altWU(){
    auto dotW =  ( tframe[0].frame.pos() <= tframe[2].sphere[2].dual() )[0];
    return dotW < 0;

  }

  bool altWV(){
    auto dotW =  ( tframe[0].frame.pos() <= tframe[1].sphere[2].dual() )[0];
    return dotW < 0;

  }
      
  // Calculate logs
  void log(){
      mLogU = Gen::log( Gen::ratio(tframe[0].sphere[0].dual(), tframe[2].sphere[0].dual(), true ), altU() );
      mLogV = Gen::log( Gen::ratio(tframe[3].sphere[1].dual(), tframe[1].sphere[1].dual(), true ), altV() );
  }

  void logZ(){
      mLogNu = Gen::log( Gen::ratio( tframe[0].sphere[2].dual(), tframe[2].sphere[2].dual(),true ), altWU() );
      mLogNv = Gen::log( Gen::ratio( tframe[3].sphere[2].dual(), tframe[1].sphere[2].dual(),true ), altWV() );
  }
  
  // Simple Rotor in U direction 
  Bst xfu( VSR_PRECISION u ) const { return Gen::bst(mLogU * -u ); }
  
  // Simple Rotor in V direction 
  Bst xfv( VSR_PRECISION v ) const { return Gen::bst(mLogV * -v ); }

  Bst xfwu( VSR_PRECISION wu ){ return Gen::bst(mLogNu * -wu ); }
  Bst xfwv( VSR_PRECISION wv ){ return Gen::bst(mLogNv * -wv ); }
  
  Pair ortho(VSR_PRECISION u, VSR_PRECISION v){
      return tframe[0].sphere[2].dual().spin( xfwu(u) ) ^ tframe[3].sphere[2].dual().spin( xfwv(v) ); 
  }

  // Evaluate conformal rotor at u,v
  Con xf( VSR_PRECISION u, VSR_PRECISION v) const { return xfv(v) * xfu(u); }

  // Evaluate conformal rotor at u,v, w
  Con xf( VSR_PRECISION u, VSR_PRECISION v, VSR_PRECISION w) { return xfwv(u) * xfwu(v) * xf(u,v); }


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

  Vec evalNormal( VSR_PRECISION u, VSR_PRECISION v){
    auto tan = apply( tframe[0].tan[2], u,v);
    return -Round::direction( tan ).copy<Vec>().unit();
  }

  
  /// Apply conformal rotor to a point at u,v
  Point eval( VSR_PRECISION u, VSR_PRECISION v) const {
    return Round::loc( tframe[0].frame.pos().spin( xf(u,v) ) );
  }

  /// Apply conformal rotor to a point at u,v amt w
  Point eval( VSR_PRECISION u, VSR_PRECISION v, VSR_PRECISION w) {
    return Round::loc( tframe[0].frame.pos().spin( xf(u,v,w) ) );
  }
 
 
  

};

struct CircularNet {

  //CyclideDraw cyclideDraw;
  vector<CyclideQuad> mCyclide;

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
        bool bFlipX = false; bool bFlipY = false;
        bFlipX = true;
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
        bool bFlipX = false; bool bFlipY = false;
        bFlipY = true;
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
        bool bFlipX = false; bool bFlipY = false;
        bFlipX = true;
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

//  void draw(){
//    for (auto& i : mCyclide){
//      i.log();
//      cyclideDraw.draw(i);
//    }
//  }

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
