/*

Generic Group 
            
see also vsr_root.h for generating reflection groups

replaces vsr_pointGroup.h 

*/
  
                     
#include "vsr_set.h" 
#include "vsr_root.h"
#include "vsr_generic_op.h"
#include <vector>                 
 
using std::vector;

namespace vsr{

/// Simple Reflection Group (no translating spinors)
template<class V>
struct SimpleGroup{
   vector<V> ops;                       ///< Pin Operators (Vec, etc)

   SimpleGroup() {}
   SimpleGroup( vector<V> v ) : ops(v) {} ///< Instantiate with unit length pin group
 
    /// Applies all operators on p motif and returns results
	  template<class T>
    vector<T> operator()(const T& p){
        vector<T> res;
        for (auto& i : ops ){
           T tp = p.reflect( i ); //assuming all i's are unit
           res.push_back( tp );
           //reflect back over first mirror plane to get original ...
           res.push_back( tp.reflect( ops[0] ) );
        }
        return res;
    }
};

/// A Group of Operations called with group( sometype t ) or group( vector<sometype> t)
/// V are  versors any dimension, etc DualLines in cga2D or DualPlanes in cga3D or Circles . . .
template< class V >
struct Group {

  /// Trs is the translator type of whatever conformal metric we are in
  typedef typename V::template BType< typename V::Mode::Trs > Trs; 

   vector<V> ops;                       ///< Pin Operators (Vec, etc)
   vector<decltype(V()*V())> sops;      ///< Spin Operators (Rot, etc)
   vector<decltype(V()*Trs())> gops;    ///< Glide Operators 

   Group() {}
   Group( vector<V> v ) : ops(v) {}

    /// Applies all operators on p motif and returns results
	  template<class T>
    vector<T> operator()(const T& p){
          vector<T> res;

          for (auto& i : ops ){
              T tp = p.reflect( i.unit() ); 
              res.push_back( tp );
              //reflect back over first mirror plane to get original ...
              res.push_back( tp.reflect( ops[0].unit() ) );
          }

          for (auto& i : sops){
            T tp = p.spin(i);
            res.push_back(tp);
          }

           //apply glides and reapply pins
           for (auto& i : gops){
              T tg = p.reflect(i);
              if (ops.empty()) {
                res.push_back(tg);
                res.push_back( tg.reflect( gops[0] ) );
              }              
              for (auto& j : ops){
                T tp = tg.reflect( j.unit() );
                res.push_back(tp);
                res.push_back( tp.reflect( ops[0].unit() ) );
              }
           }
          
          return res;
    }

    /// Applies all operations on a vector of type T
    template<class T>
    vector<T> operator()(const vector<T>& p){
      vector<T> res;
      for (auto& i : p) {
        auto tRes = (*this)(i);
        for (auto& j : tRes){
          res.push_back(j);
        }
      }    
      return res;
    }

}; 

//A 2d point group at the origin . . . 
//////V is the reflection type of the point group (e.g. Vec)
template< class V > 
struct PointGroup2D : Group<V> {
  
    typedef decltype( V() ^ V() ) Biv;

    V a, b;                 ///< Generators

    int mP;                 ///< Symmetry Group
    bool bPin;              ///< Pin or Spin

    PointGroup2D(int p, bool pin = true) : mP(p), bPin(pin) {
       a = V::x;
       b = V::x.rot( Biv::xy * PIOVERTWO/p );
       calcOps(a.unit(), b.unit());
   }

   void calcOps(const V& ta, const V& tb){
       this->ops.clear();
       this->sops.clear();
       this->gops.clear();
       if (bPin) this->ops = Root::System(ta,tb);
       else {
        auto rot = a*b;
        auto trot = rot;
        for (int i=0;i<mP;++i){
          this->sops.push_back(trot);
          trot = trot * rot;
        }
      }    
    }

};

//a pointgroup + a lattice formed by translating along the generators a and b
//we generate the point group calcOps() and then replace by glide reflections if called for
//To use Translators multiplicatively, here we assume the conformal model is in play (2D or above)
template<class V > 
struct SpaceGroup2D : PointGroup2D<V> {

  /// Trs is the translator type of whatever conformal metric we are in
  typedef typename V::template BType< typename V::Mode::Trs > Trs; 
  typedef typename V::template BType< typename V::Mode::Vec > Vec;
  typedef decltype( V() ^ V() ) Biv;

  int mDiv;  //(p)rimitive lattice = 1.0 (default); (c)entered lattice = 2; (h)exagonal lattice = 3
  float mRatio; 

  /// Constructor
  // @param mP: symmetry group (1,2,3,4,6)
  // @param mRatio: for translations on the lattice 
  // @param bPin: pin or spin generators
  // @param mDiv: number of divisions into lattice cell
  // @param ga: glide reflection replaces first mirror generator
  // @param gb: glide reflection replaces second mirror generator 
  SpaceGroup2D(int p, float ratio = 1.0, bool pin=true, int div = 1, bool ga=false, bool gb=false) 
  : PointGroup2D<V>(p,pin), mRatio(ratio), mDiv(div)  {
   
    if (p<4){ 
      //Glide Reflections
      if (ga) { //replace first mirror
        this->ops.clear();
        this->ops.push_back(this->b);
        this->gops.push_back( this->a * Gen::trs(this->b * .5) ); 
      }    
      if (gb) { //replace second mirror
        this->ops.pop_back();
        this->gops.push_back( this->b * Gen::trs(this->a * .5) ); 
      }
    } else {
      if (p==4){
        this->b = this->a + (this->a).rot( Biv::xy * PIOVERTWO/2);
        if(ga){
          this->ops.clear();
          this->ops.push_back(this->b.unit());
          this->ops.push_back(this->b.reflect(this->a).unit());
          this->gops.push_back( this->a * Gen::trs((this->b - this->a) * .5) ); 
        }
      }
      if (p==6){
        this->b = this->a + this->a.rot( Biv::xy * PIOVERTWO/3);
      }
    }
  
  }

    Vec vec(float x, float y){
      return this->a*x + this->b*y*mRatio; 
    }

    template<class T>
    vector<T> apply(const T& motif, int x, int y){
      
      auto tmp = (*this)(motif);  
      vector<T> res;

      for (auto& i : tmp ){
        for (int j=-x/2.0;j<x/2.0;++j){
          for (int k=-y/2.0;k<y/2.0;++k){
            for (int m =0;m<mDiv;++m){
              float t = (float)m/mDiv;
              res.push_back( i.trs( vec(j,k) + vec(t,t) ) );
            }
          }
        }
      }
      return res;
    }

    /// Apply to a vector of elements
    template<class T>
    vector<T> apply(const vector<T>& motif, int x, int y){
      vector<T> res;
      for (auto& i : motif){
        vector<T> tRes = apply(i,x,y);
        for (auto& j : tRes) res.push_back(j);
      }
      return res;
    }

    /// Don't apply operations, just hang on lattice points
    template<class T>
    vector<T> hang(const T& motif, int x, int y){      
      vector<T> res;
        for (int j=-x/2.0;j<x/2.0;++j){
          for (int k=-y/2.0;k<y/2.0;++k){
            for (int m =0;m<mDiv;++m){
              float t = (float)m/mDiv;
              res.push_back( motif.trs( vec(j,k) + vec(t,t) ) );
            }
          }
        }
      return res;
    }    
    /// Don't apply operations, just hang on lattice points
    template<class T>
    vector<T> hang(const vector<T>& motif, int x, int y){      
      vector<T> res;
      for (auto& i : motif){
        for (int j=-x/2.0;j<x/2.0;++j){
          for (int k=-y/2.0;k<y/2.0;++k){
            for (int m =0;m<mDiv;++m){
              float t = (float)m/mDiv;
              res.push_back( i.trs( vec(j,k) + vec(t,t) ) );
            }
          }
        }
      }
      return res;
    }    
};


/// ND lattice, on a metric specified by V's type.  Not a group, but a group can be made from it
template< int DIM, class V >
struct Lattice {

    static const int Dim = DIM;

    //GET VEC "B" TYPE OF TYPE V's AMBIENT METRIC ("MODE")
    typedef typename V::template BType< typename V::Mode::Vec > Vec;
    typedef typename V::template BType< typename V::Mode::Biv > Biv;

    Vec vec[ DIM ];

    Vec& operator[](int idx) { return vec[idx]; }
    Vec operator[](int idx) const { return vec[idx]; }

    Vec at() { return Vec(); }

    template<class ... T>
    Vec at( int x, T...v ){
        int idx = DIM - ( sizeof...(v) + 1 );
        return vec[idx] * x + at(v...);
    }

    /// END case
    void set(){}
    /// Recursively set each idx 
    template<class ... T>
    void set(int x, T ... xs){
      using TE = typename V::template BType< typename V::Mode::template e< ( DIM - ( sizeof...(T) )) > >;
      int idx = DIM - ( sizeof...(T) + 1);
      Biv biv = V::x ^ TE(1);
      vec[idx] = V::x.rot( biv * PIOVERTWO/x );

      set(xs...);
    }
    
    /// Feed in a list of ratios 
    template<class ... T>
     Lattice(int x, T ... xs) {
        vec[0] = V::x;
        set(x,xs...);      
    }

};
    




template<class V>
struct PointGroup3D : Group<V> {

    typedef typename V::template BType< typename V::Mode::Biv > Biv;
    //typedef typename V::template BType< typename V::Mode::Mot > Mot;

    V a, b, c;

    //must satisfy dicycle ab^p = bc^q = ac^2
    PointGroup3D(int p, int q, bool abar=false, bool bbar=false, bool abbar=false) {
      a = V::x;
      Biv biv = Biv::xy * PIOVERTWO / p;
      Biv biv2 = Biv::yz * PIOVERTWO / q;
      b = V::x.rot(biv).rot(biv2);
      c = V::y;
      if (abar == bbar == abbar == false){
        this->ops = Root::System(a,b,c);
      }
      if (abbar){
       // this->sops = a * b * c;
      }
    }

    auto ab() RETURNS ( a * b )
    auto ac() RETURNS ( a * c )
    auto bc() RETURNS ( b * c )

};

template<class V>
struct SpaceGroup3D : PointGroup3D<V> {

    typedef typename V::template BType< typename V::Mode::Trs > Trs; 

    Trs ta, tb, tc;

    SpaceGroup3D(int p, int q) : PointGroup3D<V>(p,q){
        ta = Gen::trs( this->a);
        tb = Gen::trs( this->b);
        tc = Gen::trs( this->c);
    }

    Trs trs( int x, int y, int z ) {
       return Gen::trs( this->a * x +  this->b * y + this->c*z );
    }

    Group<V> at(int x, int y, int z){
      vector<V> res;
      for (auto i : this->ops) res.push_back( i.spin( trs(x,y,z) ) );
      return res;
    }


};



} //vsr::
