/*

Generic Group 
            
see also vsr_root.h for generating reflection groups

replaces vsr_pointGroup.h 

*/
  
#ifndef  vsr_group_INC
#define  vsr_group_INC

                     
#include "vsr_set.h" 
#include "vsr_root.h"
#include "detail/vsr_generic_op.h"
#include <vector>                 
 
using std::vector;

namespace vsr{

/// Simple Reflection Group (no translating or gliding spinors)
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
/// NOTE this is overshadowed by the pointgroup3d, which handles its own operator ()
template< class V >
struct Group {

  int numSimple; ///< number of simple roots used to generate group

  /// Trs is the translator type of whatever conformal metric we are in
  //typedef typename V::template BType< typename V::Mode::Trs > Trs; 
  using Trs = typename V::space::translator;

   vector<V> ops;                              ///< Pin Operators (Vec, etc)
   vector<decltype(V()*V())> sops;             ///< Spin Operators (Rot, etc)
   vector<decltype(V()*V()*V())> tops;         ///< Triple Reflection (abbar 3d group)
   vector<decltype(V()*Trs())> gops;           ///< Glide Operators 
   vector<decltype(V()*V()*Trs())> scrops;     ///< Screw Operators (Motor)

   Group() {}
   Group( vector<V> v ) : ops(v) {}

    /// Applies all operators on p motif and returns results
    // Note this is overloaded in PointGroups, which use a "seed-based recording" approach
	  template<class T>
    vector<T> operator()(const T& p){
          vector<T> res;

          for (auto& i : ops ){
              T tp = p.reflect( i.unit() ); 
              res.push_back( tp );
              //reflect EACH back over first mirror plane to get original ...
              res.push_back( tp.reflect( ops[0].unit() ) );
          }

          for (auto& i : sops){
            T tp = p.spin(i);
            res.push_back(tp);
          }

          //unsure about this (abc)..
          for (auto& i : tops){
            T tp = p.reflect(i);
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
  
  using Trs = typename V::space::translator;
  using Vec = typename V::space::vector;

  typedef decltype( V() ^ V() ) Biv;

  int mDiv;  //(p)rimitive lattice = 1.0 (default); (c)entered lattice = 2; (h)exagonal lattice = 3
  float mRatioY=1;
  float mRatioX=1; 

  /// Constructor
  // @param mP: symmetry group (1,2,3,4,6)
  // @param mRatio: for translations on the lattice 
  // @param bPin: pin or spin generators
  // @param mDiv: number of divisions into lattice cell
  // @param ga: whether a glide reflection replaces first mirror generator
  // @param gb: whether a glide reflection replaces second mirror generator 
  SpaceGroup2D(int p, float ratio = 1.0, bool pin=true, int div = 1, bool ga=false, bool gb=false) 
  : PointGroup2D<V>(p,pin), mRatioY(ratio), mDiv(div)  {
  
  
    if (p==1){
      //Glide Reflections
      if (ga) { //replace first mirror
        this->ops.clear();
       // this->ops.push_back(this->b);
        this->gops.push_back( this->a * Gen::trs( Vec::y * .5) ); 
      }        
    } else if (p<4){ 
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
      if (this->mP!=1) return this->a*x*mRatioX + this->b*y*mRatioY; 
      else return this->a*x + Vec::y*y;
    }


    /*-----------------------------------------------------------------------------
     *  Apply translations to a single element
     *-----------------------------------------------------------------------------*/
    template<class T>
    vector<T> apply(const T& motif, int x, int y){
      
      auto tmp = (*this)(motif);  
      vector<T> res;

      for (auto& i : tmp ){
        for (int j=-x/2.0;j<x/2.0;++j){
          for (int k=-y/2.0;k<y/2.0;++k){
            for (int m =0;m<mDiv;++m){
              float t = (float)m/mDiv;
              //Vec v = vec(j
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

    struct OpIdx{
      int type, opIdx, resIdx;
    };

    using Biv = typename V::space::Biv;
    using Vec = typename V::space::Vec;

    V a, b, c;          //<-- basis generators

    vector<OpIdx> opIdx; //<-- given a seed vector, store all reflection operations here.
  //  vector<OpIdx> spinIdx; //<-- "" spin operations

    //must satisfy dicycle ab^p = bc^q = ac^2
    PointGroup3D(int p, int q, bool abar=false, bool bbar=false, bool abbar=false) {
      
      //0. a and c are at 90 degrees, must find b...
      a = V::x;
      c = V::y;
      
      //1. employ the good old spherical trig cosine rule ...
      double tb = PIOVERTWO;
      double ta = PI/(int)p;
      double tc = PI/(int)q;

      double ca = cos(ta);
      double sa = sin(ta);
      double cb = cos(tb);
      double sb = sin(tb);
      double cc = cos(tc);
      double sc = sin(tc);

      double tA = acos( (ca-(cb*cc))/(sb*sc) );
      double tC = acos( (cc-(ca*cb))/(sa*sb) );

      //2. ... to rotate the yx plane ...
      auto bivA = (a ^ c).rot( a.duale() * tC / 2.0 );
      auto bivC = (a ^ c).rot( c.duale() * tA / 2.0 );

      //3. ... and find b via coincidence of planes ...
      b = (bivA.duale() ^ bivC.duale()).duale().unit();


      if (!abar && !bbar && !abbar ){              //Case of only reflections. easy!
        this->ops = {a,b,c};
       } else if (abbar){
         this->tops.push_back(a*b*c); //<--implications?
       } else if ( abar && bbar ) { 
         this->sops.push_back(a*b);
         this->sops.push_back(b*c);
       } else if (abar){                                 //Case of only one or other are cyclic
          this->sops.push_back(a*b);
          this->ops.push_back(c);              
       } else if (bbar){
          this->sops.push_back(b*c);
          this->ops.push_back(a);
       }

         // seed a random vector to "record" all unique reflection and spin operations
         seed();

    }


    template<class T>
    vector<T> operator() (const T& p){
      return apply(p);
    }

    template<class T>
    vector<T> operator() (const vector<T>& p){
      vector<T> res;
      for (auto& i : p){
        auto tmp = apply(i);
        for (auto& j : tmp){
          res.push_back(j);
        }
      }
      return res;
    }

    void seed(const Vec& vec=Vec(.213,.659,1.6967).unit() ){

        opIdx.clear();
        vector<Vec> tv;

        //FIRST PASS (ops and sops)
        for(auto& i : this->ops){
          auto tvec = vec.reflect(i);
          tv.push_back(tvec);
        }

        for(auto& i : this->sops){
          auto tvec = vec.spin(i);
          tv.push_back(tvec);
        }

        //tops? gops?
                 
        //REFLECTIONS RECORD
        //Keep doing it until none new, record results in reflectIdx
        bool keepgoing=true; int iter=0; int maxiter = 100;
        while(keepgoing && iter<maxiter){
          iter++;
          keepgoing=false;
          int tn = tv.size();
          
          //Any New Reflections?
          for(int i =0; i<this->ops.size(); ++i){
            for (int j=0;j<tn;++j){
          
              auto tvec = tv[j].reflect( this->ops[i] );
       
              bool exists = false;
              for (auto& k : tv){
                exists = Root::Compare(tvec,k);
                if (exists) {
                  break;
                }
               }
       
              if (!exists){
                tv.push_back(tvec);
                opIdx.push_back( {1,i,j} );
                keepgoing=true;
              }
            }
          }

          //Any New Spins?
          for(int i =0; i<this->sops.size(); ++i){
            for (int j=0;j<tn;++j){
          
              auto tvec = tv[j].spin( this->sops[i] );
       
              bool exists = false;
              for (auto& k : tv){
                exists = Root::Compare(tvec,k);
                if (exists) {
                  break;
                }
               }
       
              if (!exists){
                tv.push_back(tvec);
                opIdx.push_back( {0,i,j} );
                keepgoing=true;
              }
            }
          }
        }      


    }
    
    template<class T>
    vector<T> apply(const T& p){
            
      vector<T> res;
      //pin first
      for(auto& i : this->ops){
        res.push_back(p.reflect(i));
      }

      //spin second
      for(auto& i : this->sops){
        res.push_back(p.spin(i));
      }
      
      for (auto& i : opIdx){
        auto tmp = i.type ? res[ i.resIdx ].reflect( this->ops[ i.opIdx] )
                          : res[ i.resIdx ].spin( this->sops[ i.opIdx] );
        res.push_back(tmp);
      }

      return res;
    }



    auto ab() RETURNS ( a * b )
    auto ac() RETURNS ( a * c )
    auto bc() RETURNS ( b * c )

};

      //Biv biv = Biv::xy * PIOVERTWO / p;
      //Biv biv2 = Biv::yz * PIOVERTWO / q;
      //b = V::x.rot(biv).rot(biv2);
      //c = V::y;


template<class V>
struct SpaceGroup3D : PointGroup3D<V> {

   // typedef typename V::template BType< typename V::Mode::Trs >
    using Trs = typename V::space::translator; 
    using Vec = typename V::space::Vec;

//    Trs ta, tb, tc;
                                                    //.5
    //Primitive, Body, A-Face, C-Face, Hexagonal, Face2, Face3, Rhombo
    enum LatticeType{
      Primitive, Body, AFace, CFace, HFace, Face2, Face3, Rhombo
    };
    
    //A axial (a), B axial (b), C axial (c), Diagonal (n), Diamond (d)
    enum GlideType{
      None, AxialA, AxialB, AxialC, Diagonal, Diamond
    };
    struct GlideParameter{
      GlideType type; //
      bool bInvert; //boolean switch for b or a-b, b+c or a-b+c
    };

    struct Glide{
      Glide( GlideParameter _a = {None,false}, GlideParameter _b = {None,false}, GlideParameter _c = {None,false}) : a(_a), b(_b), c(_c) {}
      GlideParameter a, b, c;
    };
      
    LatticeType mLatticeType;
    Glide mGlide;

    Vec mRatio; //<-- ratio of width to height
    int mScrew;

    SpaceGroup3D(
      int p, int q, 
      bool abar=false, bool bbar=false, bool abbar=false,
      LatticeType lattice = Primitive,
      Vec ratio= Vec(1,1,1),
      Glide glide = Glide(),
      int screw=0
      ) 
      : 
      PointGroup3D<V>(p,q,abar,bbar,abbar),
      mLatticeType(lattice),
      mRatio(ratio),
      mGlide(glide),
      mScrew(screw)
      {
        init();
      }


    //Q: how to seed non-symmorphic space groups
    void init(){

        //GLIDE replacements for non-symmorphic groups
        vector<int> replace; //keep track of indices into this->ops to replace
        vector<V> nops;     //new operations list
        switch(mGlide.a.type){
          case AxialB: //< replace a reflection with a glide
          {
            replace.push_back(0);
            auto trs = Gen::trs( ( (mGlide.a.bInvert) ? (this->a - this->b) : (this->b) ) * .5 );
            this->gops.push_back( this->a * trs );
            break;
          }
          case AxialC:
          {
            replace.push_back(0);
            auto trs = Gen::trs( ( (mGlide.a.bInvert) ? (this->a - this->c) : (this->c) ) * .5 );
            this->gops.push_back( this->a * trs );
            break;
          }
          case Diagonal:
          {
            replace.push_back(0); //we will replace 0 idx
            auto trs = Gen::trs( ( (mGlide.a.bInvert) ? (this->a - (this->b+this->c)) : (this->b+this->c) ) * .5 );
            this->gops.push_back( this->a * trs ); 
            break;
          }
          
        }

        switch(mGlide.b.type){

        }

        switch(mGlide.c.type){

        }

        for (int i = 0; i < this->ops.size(); ++i){
          //bool add=true;
          //for (auto& j : replace) if (i==j) add = false;
          //if (bool) nops.push_back(i);
        }

        //SCREW replacements

        //Reseed opIdx
        this->seed();

    }
    
    template<class T>
    vector<T> operator() (const T& p){
      return apply(p);
    }

    template<class T>
    vector<T> apply( const T& p ){
      auto pg = PointGroup3D<V>::apply(p);

      for (int i=0;i<pg.size();++i){
        for (auto& j : this->gops ){
          pg.push_back( pg[i].reflect(j) );
        }
        for (auto& j : this->scrops ){
          pg.push_back( pg[i].spin(j) );
        }
      }

      return pg;
    }

    template<class T>
    vector<T> apply(const vector<T>& p){
      vector<T> res;
      for(auto& i : p){
        auto tmp = apply(i);
        for (auto& j : tmp){
          res.push_back(j);
        }
      }
      return res;
    }

  
    /*!-----------------------------------------------------------------------------
     * Calculate a vector transformation basice on generators and ratio 
     *-----------------------------------------------------------------------------*/
    Vec vec(float x, float y, float z){
      return this->a*x * mRatio[0] + this->b*y * mRatio[1] + this->c*z * mRatio[2]; 
    }


    /*!-----------------------------------------------------------------------------
     *  Hang a motif on a Lattice of dimensions x,y,z
     *-----------------------------------------------------------------------------*/
    template<class T>
    vector<T> hang(const T& motif, int x, int y, int z){      
      vector<T> res;

        switch( mLatticeType ){
          case Primitive: //Primitive 
          {
              for (int j=-x/2.0;j<x/2.0;++j){
                for (int k=-y/2.0;k<y/2.0;++k){
                  for (int l=-z/2.0;l<z/2.0;++l){
                    res.push_back( motif.trs( vec(j,k,l) ) );                
                  }
                }
              }
            break;
          } 
          case Body://Body
          {
             for (int j=-x/2.0;j<x/2.0;++j){
               for (int k=-y/2.0;k<y/2.0;++k){
                 for (int l=-z/2.0;l<z/2.0;++l){
                  res.push_back( motif.trs( vec(j,k,l) ) );   
                  res.push_back( motif.trs( vec(j,k,l) + vec(.5,.5,.5) ) );    
                 }
               }
             }
            break;
          }
          case AFace:
          {
             for (int j=-x/2.0;j<x/2.0;++j){
               for (int k=-y/2.0;k<y/2.0;++k){
                 for (int l=-z/2.0;l<z/2.0;++l){
                   res.push_back( motif.trs( vec(j,k,l)  ) );  
                   res.push_back( motif.trs( vec(j,k,l) + vec(0,.5,.5) ) );    
                 }
               }
             }
            break;
          }
          case CFace:
          {
             for (int j=-x/2.0;j<x/2.0;++j){
               for (int k=-y/2.0;k<y/2.0;++k){
                 for (int l=-z/2.0;l<z/2.0;++l){
                   res.push_back( motif.trs( vec(j,k,l)  ) );  
                   res.push_back( motif.trs( vec(j,k,l) + vec(.5,.5,0) ) );    
                 }
               }
             }
            break;
          }          
           case HFace:
          {
             for (int j=-x/2.0;j<x/2.0;++j){
               for (int k=-y/2.0;k<y/2.0;++k){
                 for (int l=-z/2.0;l<z/2.0;++l){
                   res.push_back( motif.trs( vec(j,k,l)  ) );  
                   res.push_back( motif.trs( vec(j,k,l) + vec(.3333333,.3333333,0) ) );    
                   res.push_back( motif.trs( vec(j,k,l) + vec(.6666666,.6666666,0) ) );   
                 }
               }
             }
            break;
          }  
           case Rhombo:
          {
             for (int j=-x/2.0;j<x/2.0;++j){
               for (int k=-y/2.0;k<y/2.0;++k){
                 for (int l=-z/2.0;l<z/2.0;++l){
                   res.push_back( motif.trs( vec(j,k,l)  ) );  
                   res.push_back( motif.trs( vec(j,k,l) + vec(.3333333,.3333333,.3333333) ) );    
                   res.push_back( motif.trs( vec(j,k,l) + vec(.6666666,.6666666,.6666666) ) );   
                 }
               }
             }
            break;
          }            
          case Face2:
          {
             for (int j=-x/2.0;j<x/2.0;++j){
               for (int k=-y/2.0;k<y/2.0;++k){
                 for (int l=-z/2.0;l<z/2.0;++l){
                   res.push_back( motif.trs( vec(j,k,l)  ) );  
                   for (int m =1;m<2;++m){
                    float t = (float)m/2;
                    res.push_back( motif.trs( vec(j,k,l) + vec(t,t,0) ) );    
                    res.push_back( motif.trs( vec(j,k,l) + vec(0,t,t) ) );  
                    res.push_back( motif.trs( vec(j,k,l) + vec(-t,0,t) ) );  
                   }
                 }
               }
             }
            break;
          }
          case Face3:
          {
             for (int j=-x/2.0;j<x/2.0;++j){
               for (int k=-y/2.0;k<y/2.0;++k){
                 for (int l=-z/2.0;l<z/2.0;++l){
                   res.push_back( motif.trs( vec(j,k,l)  ) );  
                   for (int m =1;m<3;++m){
                    float t = (float)m/3;
                    res.push_back( motif.trs( vec(j,k,l) + vec(t,t,0) ) );    
                    res.push_back( motif.trs( vec(j,k,l) + vec(0,t,t) ) );  
                    res.push_back( motif.trs( vec(j,k,l) + vec(-t,0,t) ) );  
                   }
                 }
               }
             }
            break;
          }

        }
      return res;
    }   
    
    template<class T>
    vector<T> hang(const vector<T>& motif, int x, int y, int z ){
      vector<T> res;
      for (auto& i : motif){
        auto tmp = hang(i,x,y,z);
        for (auto& j : tmp) res.push_back(j);
      }
      return res;
    }

};


} //vsr::
#endif   /* ----- #ifndef vsr_group_INC  ----- */
