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

/// Cylclidic Net (four tangent frames)
struct CyclideQuad{

  TangentFrame tframe[4]; ///<-- four frames

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



  // Calculate frame positions
  void frame(){

    //FIRST ONE
    tframe[0].bitan[0] = tframe[0].frame.tx().dual();
    tframe[0].bitan[1] = tframe[0].frame.ty().dual();
    tframe[0].bitan[2] = tframe[0].frame.tz().dual();

    for (int j=0;j<3;++j){
     tframe[0].sphere[j] = tframe[0].bitan[j] ^ tframe[1].frame.pos();
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
  Bst xfu( VSR_PRECISION u ){ return Gen::bst(mLogU * -u ); }
  
  // Simple Rotor in V direction 
  Bst xfv( VSR_PRECISION v ){ return Gen::bst(mLogV * -v ); }

  Bst xfwu( VSR_PRECISION wu ){ return Gen::bst(mLogNu * -wu ); }
  Bst xfwv( VSR_PRECISION wv ){ return Gen::bst(mLogNv * -wv ); }
  
  Pair ortho(VSR_PRECISION u, VSR_PRECISION v){
      return tframe[0].sphere[2].dual().spin( xfwu(u) ) ^ tframe[3].sphere[2].dual().spin( xfwv(v) ); 
  }

  // Evaluate conformal rotor at u,v
  Con xf( VSR_PRECISION u, VSR_PRECISION v) { return xfv(v) * xfu(u); }

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
  T apply( const T& t, VSR_PRECISION u, VSR_PRECISION v) {
    return t.spin( xf(u,v) );
  }

  Vec evalNormal( VSR_PRECISION u, VSR_PRECISION v){
    auto tan = apply( tframe[0].frame.tz(), u,v);
    return -Round::direction( tan ).copy<Vec>().unit();
  }

  
  /// Apply conformal rotor to a point at u,v
  Point eval( VSR_PRECISION u, VSR_PRECISION v) {
    return Round::loc( tframe[0].frame.pos().spin( xf(u,v) ) );
  }

  /// Apply conformal rotor to a point at u,v amt w
  Point eval( VSR_PRECISION u, VSR_PRECISION v, VSR_PRECISION w) {
    return Round::loc( tframe[0].frame.pos().spin( xf(u,v,w) ) );
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
