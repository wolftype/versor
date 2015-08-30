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


///// Abstract Odd Operator
//struct OddSymmetryOperator{
//  
//   template<class T>
//   T apply(const T& t);
//};

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

    vector<OpIdx> opIdx; //<-- given a seed vector, store all operations here.

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

      this->ops = {a,b,c};
      this->sops = {a*b, b*c, c*a}; 
      this->tops = {a*b*c};


      if (!abar && !bbar && !abbar ){              //Case of only reflections. easy!
        //this->ops = {a,b,c};
        opIdx= {  {0,0,0}, {0,1,0}, {0,2,0} };
       } else if (abbar){
         //this->tops.push_back(a*b*c); //<--implications?
        opIdx = { {-1,0,0} };
       } else if ( abar && bbar ) { 
         opIdx = { {1,0,0}, {1,1,0} };
         //this->sops.push_back(a*b);
         //this->sops.push_back(b*c);
       } else if (abar){                                 //Case of only one or other are cyclic
          opIdx = { {0,2,0},{1,0,0} };
          //this->sops.push_back(a*b);
          //this->ops.push_back(c);              
       } else if (bbar){
          opIdx = { {0,0,0},{1,1,0} };
          //this->sops.push_back(b*c);
          //this->ops.push_back(a);
       }

        for (auto& i : opIdx){
          cout << i.type << " " << i.opIdx << " " << i.resIdx << endl;
        }
         // seed a random vector to "record" all unique reflection and spin operations
         seed();

    }

    void seed(const Vec& vec=Vec(.213,.659,1.6967).unit() ){

        //opIdx.clear();
        vector<Vec> tv;
        tv.push_back(vec);
        Vec tmp;

        for (auto& i: opIdx){
           tmp = i.type == 0 ? vec.reflect( this->ops[ i.opIdx ] )
                             : vec.spin( this->sops[i.opIdx] );                                               
           tv.push_back(tmp);
        }


        //tops? gops?
        int numBasicOps = opIdx.size();
        //REFLECTIONS RECORD
        //Keep doing it until none new, record results in reflectIdx
        bool keepgoing=true; int iter=0; int maxiter = 100;
        while(keepgoing && iter<maxiter){
          iter++;
          keepgoing=false;
          int tn = tv.size();
          
          //Any New Reflection Results

          for(int i =0; i<numBasicOps; ++i){
            for (int j=0;j<tn;++j){
              
              tmp = opIdx[i].type == 0 ? tv[j].reflect( this->ops[ opIdx[i].opIdx ] )
                                       : tv[j].spin( this->sops[opIdx[i].opIdx] );
       
              bool exists = false;
              for (auto& k : tv){
                exists = Root::Compare(tmp,k);
                if (exists) {
                  break;
                }
               }
      
              //if new, add result to tv and idx 
              if (!exists){
                tv.push_back(tmp);
                cout << tmp << endl;
                opIdx.push_back( {opIdx[i].type, opIdx[i].opIdx, j} );
                keepgoing=true;
              }
            }
          }
        }
          
        
        
        for (auto& i: opIdx){
          cout << i.type << " means " << i.resIdx  << " reflected over " << i.opIdx << endl;
        }
        cout << opIdx.size() << endl;


    }
 
    /// striated results
    template<class T>
    vector<T> apply(const vector<T>& p){
      vector<T> res( (opIdx.size()+1) * p.size() );
      int ii=0;
      for (auto& i : p){
        auto tmp = apply(i);
        int jj=0;
        for (auto& j : tmp){
          int idx = jj*p.size() + ii;
          res[idx]=j;
          jj++;
        }
        ii++;
      }
      return res;
    }
    
    template<class T>
    vector<T> apply(const T& p){
            
      vector<T> res;
      res.push_back(p);

      for (auto& i : opIdx){
        T tmp;
        T& input = res[i.resIdx];
        switch(i.type){
         case 0: //reflect
              tmp = input.reflect( this->ops[ i.opIdx] );
              break;
         case 1: //spin
              tmp = input.spin( this->sops[ i.opIdx] );
              break;
          case 2: 
              tmp = input.reflect( this->tops[0] );
              break;
          case 3: //glide
              tmp = input.reflect( this->gops[ i.opIdx] );
              break;
          case 4: //screw
              tmp = input.spin( this->scrops[ i.opIdx] );
              break;
              
        }
        res.push_back(tmp);
      }

      return res;
    }



    auto ab() RETURNS ( a * b )
    auto ac() RETURNS ( a * c )
    auto bc() RETURNS ( b * c )

};


template<class V>
struct SpaceGroup3D : PointGroup3D<V> {

    using Trs = typename V::space::translator; 
    using Vec = typename V::space::Vec;

    Vec mB_length, mC_length;       //actual (non-unit) b
    Vec mB_dir, mC_dir;  //actuall cell edge

    enum BravaisType{
      Triclinic=1, Monoclinic, Orthorhombic, Tetragonal, Trigonal, Hexagonal, Cubic
    };

    enum LatticeType{
      Primitive=1, Body, SingleFace, Face, Rhombo
    };

    struct Lattice{
      int system; //bravais system (Triclinic, Tetragonal, etc)
      int type;   //latticetype (primitive, Body, single face, etc)
    };
    
    //A axial (a), B axial (b), C axial (c), Diagonal (n), Diamond (d)
    enum GlideType{
      None=0, AxialA, AxialB, AxialC, Diagonal, Diamond
    };
    struct GlideParameter{
      int type; //
      bool bInvert;   //boolean switch for b or a-b, b+c or a-b+c
    };
    struct Glide{
      GlideParameter a, b, c;
    };
      
    Lattice mLattice;
    Glide mGlide;

    Vec mRatio; //<-- ratio of width to height
    int mScrew;

    SpaceGroup3D(
      int p, int q, 
      bool abar=false, bool bbar=false, bool abbar=false,
      Lattice lattice = {Triclinic, Primitive},
      Vec ratio= Vec(1,1,1),
      Glide glide = Glide(),
      int screw=0
      ) 
      : 
      PointGroup3D<V>(p,q,abar,bbar,abbar),
      mLattice(lattice),
      mRatio(ratio),
      mGlide(glide),
      mScrew(screw)
      {
        init();
      }


    //Q: how to seed non-symmorphic space groups
    void init(){

         mB_dir = bdir();
         mC_dir = cdir();
         mB_length = blength();
         mC_length = clength();

         switch(mLattice.system){
            case Triclinic:
              printf("Triclinic\n"); break;
            case Monoclinic:
              printf("Monoclinic\n"); break;
            case Orthorhombic:
              printf("Orthorhombic\n"); break;
            case Tetragonal:
              printf("Tetragonal\n"); break;
            case Trigonal:
              printf("Trigonal\n"); break;
            case Hexagonal:
              printf("Hexagonal\n"); break;
            case Cubic:
              printf("Cubic\n"); break;
         }
         switch(mLattice.type){
            case Primitive:
              printf("Primitive\n"); break;
            case Body:
              printf("Body\n"); break;
            case SingleFace:
              printf("SingleFace\n"); break;
            case Face:
              printf("Face\n"); break;
            case Rhombo:
              printf("Rhombo\n"); break;
         }

        //GLIDE replacements for non-symmorphic groups
        vector<int> replaceReflection; //keep track of indices into this->ops to replace
        switch(mGlide.a.type){
          case AxialB: //< replace a reflection with a glide
          {
            replaceReflection.push_back(0);
            auto trs = Gen::trs( ( (mGlide.a.bInvert) ? ( this->a - mB_length ) : ( mB_length ) ) * .5 );
            this->gops.push_back( this->a * trs );
            break;
          }
          case AxialC:
          {
            replaceReflection.push_back(0);
            auto trs = Gen::trs( ( (mGlide.a.bInvert) ? (this->a - this->c) : (this->c) ) * .5 );
            this->gops.push_back( this->a * trs );
            break;
          }
          case Diagonal:
          {
            replaceReflection.push_back(0); //we will replace 0 idx
            auto trs = Gen::trs( ( (mGlide.a.bInvert) ? (this->a - ( mB_length+this->c)) : ( mB_length+this->c) ) * .5 );
            this->gops.push_back( this->a * trs ); 
            break;
          }
          case Diamond:
          {
            replaceReflection.push_back(0);
            break;
          }
          
        }

        switch(mGlide.b.type){
          case AxialA: //< replace a reflection with a glide
          {
            replaceReflection.push_back(1);
            auto trs = Gen::trs( ( (mGlide.b.bInvert) ? (this->a) : (this->a) ) * .5 ); //any alt?
            this->gops.push_back( this->a * trs );
            break;
          }
          case AxialC:
          {
            replaceReflection.push_back(1);
            auto trs = Gen::trs( ( (mGlide.b.bInvert) ? (this->a -  mB_length + this->c) : (this->c) ) * .5 );
            this->gops.push_back( this->a * trs );
            break;
          }
          case Diagonal:
          {
            replaceReflection.push_back(1); //we will replace 0 idx
            auto trs = Gen::trs( ( (mGlide.b.bInvert) ? (this->a +this->c) : (this->a+this->c) ) * .5 );
            this->gops.push_back( this->a * trs ); 
            break;
          }
          case Diamond:
          {
            replaceReflection.push_back(1);
            auto trs = Gen::trs( ( (mGlide.b.bInvert) ? ( mB_length*2 - this->c - this->a*3) : (this->c - this->a) ) * .25 );
            break;
          }
        }

        switch(mGlide.c.type){
          case AxialA: //< replace a reflection with a glide
          {
            replaceReflection.push_back(2);
            auto trs = Gen::trs( ( (mGlide.b.bInvert) ? (this->a) : (this->a) ) * .5 ); //any alt?
            this->gops.push_back( this->a * trs );
            break;
          }
          case AxialB:
          {
            replaceReflection.push_back(2);
            auto trs = Gen::trs( ( (mGlide.b.bInvert) ? (this->a - this->b + this->c) : (this->c) ) * .5 );
            this->gops.push_back( this->a * trs );
            break;
          }
          case Diagonal:
          {
            replaceReflection.push_back(2); //we will replace 0 idx
            auto trs = Gen::trs( ( (mGlide.b.bInvert) ? (this->a +this->b) : (this->a+this->c) ) * .5 );
            this->gops.push_back( this->a * trs ); 
            break;
          }
          case Diamond:
          {
            replaceReflection.push_back(2);
            auto trs = Gen::trs( ( (mGlide.b.bInvert) ? (this->b*2 - this->c - this->a*3) : (this->a + this->c) ) * .25 );
            break;
          }

        }


        //replace instructions
        for (auto& i : replaceReflection){
          for (auto& j : this->opIdx){
            if (j.opIdx==i) j.type = 3; 
         }
        }


        //replace rotation with screw
        vector<int> replaceRotation;


        
    }


    /// get corrected direction of b vec along bravais lattice
    /// is stored in mB_dir on init();
    Vec bdir(){
      Vec tmp;
      switch(mLattice.system){
       case Triclinic:
       case Monoclinic:
       case Orthorhombic:
       case Trigonal:
        tmp = this->b;
        break;
       case Tetragonal:
       case Hexagonal:
        tmp = this->a.reflect( (this->b^this->c).duale().unit() );
        break;
       case Cubic:
        if ( FERROR( fabs((this->b<=this->a)[0]) - fabs((this->b<=this->c)[0]) ) )
             tmp=this->b;
        else tmp = -this->a.reflect(this->b);        
        break;
      }
      return tmp;
    }

    Vec cdir(){
      Vec tmp;
      switch(mLattice.system){
       case Cubic:
        if ( FERROR( fabs((this->b<=this->a)[0]) - fabs((this->b<=this->c)[0]) ) )
             tmp=this->c;
        else tmp = (this->a ^ this->b).duale().unit(); 
        break;

       default:
        tmp = this->c;
        break;
      }     
      return tmp;
    }

 
     /// corrected length of b
    /// is stored as mB_length on init();
    Vec blength(){
     Vec tmp;
     switch(mLattice.system){
       case Triclinic:
       case Monoclinic:
       case Orthorhombic:
       case Trigonal:
        tmp = this->b;
        break;
       case Tetragonal:
       case Hexagonal:
        tmp = this->a + mB_dir;
        break;
       case Cubic:
         if ( FERROR( fabs((this->b<=this->a)[0]) - fabs((this->b<=this->c)[0]) ) )
           tmp = this->b;
         else 
           tmp = this->a + mB_dir; 
          break;
       
      }
      return tmp;
    }

     /// corrected length of c
    /// is stored as mC_length on init();
    Vec clength(){
     Vec tmp;
     switch(mLattice.system){
       case Cubic:
        if ( FERROR( fabs((this->b<=this->a)[0]) - fabs((this->b<=this->c)[0]) ) )
             tmp = this->c;
        else tmp = mB_dir + mC_dir; 
        break;
       default:
        tmp = this->c;
        break;
      }
      return tmp;
    }

  
    /*!-----------------------------------------------------------------------------
     * Calculate a vector transformation basice on generators and ratio 
     *-----------------------------------------------------------------------------*/
    Vec vec(float x, float y, float z){
        return this->a*x * mRatio[0] + mB_dir*y * mRatio[1] + mC_dir*z * mRatio[2]; 
    }


    /*!-----------------------------------------------------------------------------
     *  Hang a motif on a Lattice of dimensions x,y,z
     *-----------------------------------------------------------------------------*/
    template<class T>
    vector<T> hang(const T& motif, int x, int y, int z){      
      
        vector<T> res;

        switch( mLattice.type ){
            case Primitive: 
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
            case Body:
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
          case SingleFace:
          {
            switch(mLattice.system){
              case Monoclinic:
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
             case Orthorhombic:
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
              case Trigonal:
              case Hexagonal:
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
            }
            break;
          }         
          case Face:
          {
            switch(mLattice.system){
              case Orthorhombic:
              case Cubic:
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
              case Trigonal:
              case Hexagonal:
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
        }
      return res;
    }   
    
    template<class T>
    vector<T> hang(const vector<T>& motif, int x, int y, int z ){
      vector<T> res( motif.size() * x * y * z );
      int ii=0;
      for (auto& i : motif){
        auto tmp = hang(i,x,y,z);
        int jj=0;
        for (auto& j : tmp) {
          int idx = jj*motif.size()+ii;
          res[idx]=j;
          jj++;
        }
        ii++;
      }
      return res;
    }

};


} //vsr::
#endif   /* ----- #ifndef vsr_group_INC  ----- */
