/*
 * =====================================================================================
 *
 *       Filename:  vsr_rigid.h
 *
 *    Description:  rigid body constraints using pointer networks
 *
 *        Version:  1.0
 *        Created:  07/10/2014 15:53:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  pretty awesome
 *
 * =====================================================================================
 */


 /*
 @file Rigid Body Sphere-based Constraint Networks

 @sa vsr_fold.h
 
 @todo major clean up
 
 */

#ifndef  vsr_rigid_INC
#define  vsr_rigid_INC

#include "vsr_graph.h"

namespace vsr { namespace cga {


/// Methods for Evaluating Constrained Points using Intersections of Sphere-based Distances
struct Constrain {

    /// Construct a Distance Constraint from a source and target
    static DualSphere Distance( const Point& src, const Point& target){
      return target <= (src ^ Inf(1));
    }

   //unused?
    static Point Crease (const Point& a, const Point& dls, const DualLine d, bool mtn){
       Circle cir = a ^ d;
       Pair par = ( dls ^ cir.dual()).dual();
       return Round::loc( Round::split(par,mtn) );
    }


    /// two distances and a theta around their meet
    static Point Double( const Dls& da, const Dls& db, double amt){
      return Round::point( (da^db).dual(), amt );
    }

    /// three distances, counter clockwise (deprecated, use Tetral)
    static Point Triple (const Dls& da, const Dls& db, const Dls& dc, bool mtn){
       return Round::loc( Round::split( (da ^ db ^ dc).dual(), mtn ) ) ; 
    }
    /// tetral constraint
    static Point Tetral (const Dls& da, const Dls& db, const Dls& dc, bool mtn){
       return Round::loc( Round::split( (da ^ db ^ dc).dual(), mtn ) ) ; 
    }
    /// planar constraint: two distances and a plane
    static Point Planar( const Dls& da, const Dls& db, const Dls& dc, bool mtn){
       Plane plane = da ^ db ^ dc ^ Inf(1);
       return Round::loc( Round::split( (da ^ dc ^ plane.dual() ).dual(), mtn ) );
    }

    /// circle tangency constraint, two distances and an original point (closest to original)
    /// see PointToCircle below
    static Point Tangency(const Pnt& p, const Dls& da, const Dls& db){

      auto meet = (da ^ db).dual();
      auto tan =  Round::loc( Tangent::at( meet, p ) );
      auto sur = Round::sur( meet );
      auto line = tan ^ sur ^ Inf(1);
      auto np = Round::split ( ( line.dual() ^ sur).dual(), false );

      return np;
      
    }

     /// spherical tangency constrain, one distance and an original point (returns point on sphere closest to p);
     static Point Tangency(const Pnt& p, const Dls& da){
       auto line =  p ^ da ^ Inf(1);
       auto np = Round::split( (line.dual() ^ da).dual(), false);
       return np;
     }

     /// Tension tangency constrain, one distance and an original point 
     /// (returns point on sphere closest to p if p is outside da, otherwise just p);
     static Point Tension(const Pnt& p, const Dls& da){
       if ( (p<=da)[0] > 0 ) return p;
       auto line =  p ^ da ^ Inf(1);
       auto np = Round::split( (line.dual() ^ da).dual(), false);
       return np;
     }

    /// tension constraint from two distances
    static Point Tension(const Pnt& p, const Dls& da, const Dls& db){
       
       if ( ( (p<=da)[0] > 0 ) && ( (p<=db)[0] > 0 ) ) return p; // returns input point if it lies within both spheres 
       
       //otherwise, constrain to lie on circle meet
       auto meet = (da ^ db).dual();
       auto tan =  Round::loc( Tangent::at( meet, p ) );
       auto sur = Round::sur( meet );
       auto line = tan ^ sur ^ Inf(1);
       auto np = Round::split ( ( line.dual() ^ sur).dual(), false );

       return np;     
    }


    /// Constrain a point p to a circle c
    static Point PointToCircle( const Point& p, const Circle& c){

      auto cen = Flat::location( Round::carrier(c), p );
      auto sur = Round::surround(c);                    // dual sphere surround of c
      auto line =  cen ^ sur ^ Infinity(1);             // line through center of circle and cen
      auto meet = (line.dual() ^ sur).dual();           // meet of line and sphere
      return Round::split( meet ,false);                // point on circle closest to p
    }

    // Constrain a point p to a sphere s
    static Point PointToSphere( const Point& p, const DualSphere& s){
      auto dll = (p ^ s ^ Inf(1) ).dual();
      auto pair = (dll ^ s).dual();
      return Construct::pointB(pair);
    }
    /* static Point Fabrik(const Dls& base, const Dls& goal){ */

    /*       //repeat until distance is decreased to within error threshold, or give up after 20 iterations */
    /*       while (s[0] > err){ */
              
    /*           Pnt tmpGoal = goal; */
    /*           Pnt tmpBase = base; */
              
    /*           static Dls dls; //surround */
    /*           static Dll dll; //line */
    /*           static Par par; //intersection of line ^ surround */
              
    /*           //backward reaching */
    /*           for (int i = end; i > begin; --i){ */
    /*               mFrame[i].pos( tmpGoal );           //set position of ith frame */
    /*               dls = prevDls(i);                   //set boundary sphere through i-1 th frame; */
    /*               dll = linb(i);//.sp( !mLink[i].mot() );                      //get line from ith to i-1th frame */
    /*               par = (dll ^ dls).dual();           //get point pair intersection of line and boundary sphere */
    /*               tmpGoal = Round::split(par,true);      //extract point from point pair intersection */
    /*           } */
              
    /*           //forward correction */
    /*           for (int i = begin; i < end; ++i){ */
    /*               dls = nextDls(i);                   //set boundary sphere through i+1 th frame */
    /*               dll = linf(i);                      //get line to i+1th frame; */
    /*               par = (dll ^ dls).dual();           //get point pair intersection of line and boundary sphere */
    /*               tmpBase = Round::split(par,true); */
    /*               mFrame[i+1].pos(tmpBase);           //set position of i+1th frame */
    /*           } */
              
    /*           s = mFrame[ end ].pos() <= p * -2.0; */
               
    /*           n++;  if (n > 20) {  break; } */
    /*       } */

    /* } */

  
};


/// Holds a pointer to a source, and has a radius t, 
/// the operator() generates a dual sphere at source with radius t
struct DistancePtr {
  
  Point * src = NULL;      ///< Pointer to Point Source 
  double t;                ///< Distance t from Source

  DistancePtr() : src(NULL) {};

  /// Feed a source and a target
  DistancePtr( Pnt& a, const Pnt& target)  
  {
    set(a,target);  
  }

  /// Set constraint from source and target
  void set(Pnt& a, const Pnt& target){
    src = &a; t = Round::rad( Round::at(*src,target) );
    if (src==NULL) printf("null DistancePtr SET\n"); 
  }

  /// Evaluate Distance Constraint as a Dual Sphere
  Dls operator()(){ 
    if (src==NULL) { printf("null DistancePtr\n"); return Dls(); }  
    return Round::dls( *src, t ); 
  }

};

/// Mutual Distance Constraint is two DistancePtrs
struct MutualDistancePtr{

  DistancePtr da,db;

  void set(Point& a, Point& b){
    da.set(a,b);
    db.set(b,a);
  }
  
  Dls forward(){ return da(); }
  Dls backward(){ return db(); }

};


/// Stores Result and Flags for Whether to evaluate
struct RNode  {

  RNode(const Point& p) : result(p){}
  
  Point result = Point();
  bool bCalc = true;
  bool bVisited = false;
  void reset(){
    bCalc = true; bVisited = false;
  }

  void init(const Point& p) { result = p; }

 // vector<RNode*> m
};


/// Calculation of an edge-based circular constraint
struct REdge{

  bool bCalc = true;
       
  bool bConstrained;            ///< Whether constrained?
  
  double maxAngle = PI;         ///< angular constraint evaluated in range (-PI,PI) (experimental)
  double minAngle = -PI;        ///< angular constraint (experimental)

 
  //Point *ra;                  ///< pointer to first constraining point 
  //Point *rb;                  ///< pointer to second constrainig point
  Point *result;                ///< pointer to calculated result
   
  DistancePtr da, db;           ///< Distance Pointer Constraint

  REdge(Point& t, Point& a, Point& b){
    set(t,a,b);
  }

  /// set rigid constraint based on target point and two other points
  void set(Point& target, Point& a, Point&b){
    //ra = &a; rb = &b; 
    result = &target;
    da.set(a, target);
    db.set(b, target);
  }

  /// the circular constraint
  Circle meet(){
    return (da() ^ db()).dual();
  }

  /// measure error -- a value of 0 means we result is on constraining circle and we are good
  double error(){
    return fabs( (*result <= meet()).wt() );
  }

  bool ok(){
      return error() < .001;
  }

  void eval(double amt){
    if (bCalc) *result = Round::point( (da() ^ db()).dual(), amt); 
  }

  void eval(){
    if (bCalc) *result = Constrain::PointToCircle( *result, meet() );
  }


  
};

/// Feed in three points
struct RSimplex{
  
  RNode *ra,*rb,*rc;
  MutualDistancePtr ab, bc, ca;
  
  void set(RNode * a, RNode *b, RNode *c){
    ra = a; rb = b; rc = c;
    ab.set(a->result, b->result);
    bc.set(b->result, c->result);
    ca.set(c->result, a->result);
  }

  /// Circle constraint around an edge
  Circle ea(){
    return (bc.backward() ^ ca.forward()).dual();
  }

  Circle eb(){
    return (ca.backward() ^ ab.forward()).dual();
  }

  Circle ec(){
    return (ab.backward() ^ bc.forward()).dual();
  }


  void onEval(){
    
  }
  
};

struct RNodeN{

  virtual void onEval(){

  }
  
};


/// Generic Rigid Constraint Node
struct RigidNode{

  /// Type of Fold
//  enum Fold{
//    Mountain, Valley, Planar
//  };

  /// Point original = Point();
  Point result = Point();                   ///< stored evaluated result
  
  bool bCalc = true;                        ///< whether to Evaluate
  bool bVisited = false;                    ///< whether node has been visited already

  void reset(){
    bCalc = true;
    bVisited = false;
  }
  
  virtual void onEval(double amt) = 0;      ///< evaluation implementation to be implemented by subclass
  virtual void onEvalAt(const Point& p)=0;  ///< evaluation dependendent on some point p (i.e. closest to p)
 
  vector <RigidNode*> mParent;              ///< list of dependencies which might need to be evaluated before this one
  vector <RigidNode*> mChild;               ///< list of dependents which will need to be evaluated after this one
  vector <DistancePtr> mDistance;           ///< list of distance constraints i should satisfy w.r.t parents
  vector <DistancePtr> mChildDistance;      ///< list of distance constraints children should satisfy w.r.t me

  /// Evaluate (with optional input range)
  Point eval(double amt=0){
    for (auto& i : mDistance) if (i.src==NULL) bCalc = false; // don't calculate if distance ptrs have NOT been set
    if (bCalc) onEval(amt);
    return result;
  }

  /// Evaluate (with optional point)
  Point eval(const Point& p){
    if (bCalc) onEvalAt(p);
    return result;
  }

  /// Evaluate by traversing tree upwards (backwards)
  Point up(){
    if (!bVisited){
      bVisited =true;
      for (auto& i : mParent){
        i->up();
      }
    }
    return eval();
  }

  /// Evaluate by traversing tree downwards (forwards)
  void down(){
    eval();
    if (!bVisited){
      bVisited = true;
      for (auto& i : mChild){
        i->down();
      }
    }
  }

  /// Set result (must do this initialization step first)
  void initResult(const Point& p){         
    result = p;
  }

  /// Add a rigid distance constraint
  RigidNode& add(RigidNode * ra){
    if (ra==NULL) printf("null ra set\n");
    mParent.push_back(ra);
    mDistance.push_back( DistancePtr(ra->result,result) );
    ra->mChild.push_back(this);
    ra->mChildDistance.push_back( DistancePtr(result, ra->result) );
    return *this;
  }

};


/**
* @brief A rig constrained by N Edges (circles)

        Specialize evaluation strategy
*/
struct RigN : RigidNode {

    virtual void onEval(double amt){
      
    }

    virtual void onEvalAt(const Point& p){}
};


/// A Rigid Constraint with closest point evaluation strategy


/// A Rigid Constraint Node set by Two Distance Pointers
struct Rig2 : RigidNode {
  

  Rig2() : RigidNode() {}
  
  Rig2(const Point& target, RigidNode * ra, RigidNode * rb) : RigidNode() {
    set(target,ra,rb); 
  }

  void set(const Point& target, RigidNode * ra, RigidNode * rb){

    initResult(target);
    set(ra,rb);
  }

  void set(RigidNode * ra, RigidNode * rb){

    add(ra).add(rb);
    bCalc=true;
  }

  Circle meet(){
    return (mDistance[0]() ^ mDistance[1]()).dual();
  }


  virtual void onEval(double amt){
    result = Round::point( (mDistance[0]() ^ mDistance[1]()).dual(), amt); 
  }


  void onEvalAt(const Point& p){
    result = Constrain::PointToCircle( p, meet() );
  }

};


/// Three Distances constraint, with optional co-planarity constraint
struct Rig3 : RigidNode {
  
  bool bMtn;               ///< fold is a mountain fold
  bool bCoplanar = false;  ///< is result coplanar with three constraints

  Rig3() : RigidNode() {}
  
  /**
  * @brief Constructor
  *
  * @param target cga::Point
  * @param ra RigidNode constraint
  * @param rb RigidNode constraint
  * @param rc RigidNode constraint
  * @param m boolean to specify Mountain or Valley Fold
  * @param p boolean to specify Coplanar
  */
  Rig3( const Pnt& target, RigidNode * ra, RigidNode * rb, RigidNode * rc, bool m, bool p) : RigidNode() {
    set(target,ra,rb,rc,m,p);
  }

  /// set from target and constraints Counter Clockwise, bool mtn, bool coplanar
  void set( const Point& target, RigidNode * ra, RigidNode *rb, RigidNode *rc, bool m, bool p){
     result = target;
     set(ra,rb,rc,m,p);
  }

  /// set from target and constraints Counter Clockwise
  void set( RigidNode * ra, RigidNode *rb, RigidNode *rc, bool m, bool p){
   // mParent.push_back(ra); mParent.push_back(rb); mParent.push_back(rc);
    add(ra).add(rb).add(rc);
    bMtn = m;
    bCoplanar=p;

    bCalc=true;
  }

  Circle meet(){
    return (mDistance[0]() ^ mDistance[2]()).dual();
  }


  virtual void onEval(double amt){
    result = bCoplanar ? Constrain::Planar( mDistance[0](), mDistance[1](), mDistance[2](),bMtn) : 
                         Constrain::Triple( mDistance[0](), mDistance[1](), mDistance[2](),bMtn);
  }

  //pass through
  virtual void onEvalAt(const Point& p){
    //result = onEval(0);
  }

};


/// A Rigid Constraint node in quad formation
struct Rig4 : RigidNode {

  Rig4( const Point& target, RigidNode * ra, RigidNode * rb, RigidNode *rc, RigidNode *rd) : RigidNode(){

  }

  virtual void onEval(double amt){

  }

  virtual void onEvalAt(const Point& p){

  }

  /// feed current result back in
  virtual void onEvalRecursive(){
    
  }
};

/// A Rigid Constraint Node set by Three Distance Pointers
struct Rigid{
  
  /// default calc is false until ra parents are set
  bool bCalc;
  
  /// coplanar or not
  bool bTriple;

  /// stored evaluated result of (*this)()
  Pnt result;
  
  /// mountain or valley
  bool mtn;

  /// three dual sphere distances
  DistancePtr da,db,dc;
  
  /// linked to three parents
  Rigid *ra, *rb, *rc;

  /// Has n children which depend on it
  vector<Rigid*> child;

  Rigid() : bCalc(false), bTriple(true), ra(NULL), rb(NULL), rc(NULL) {}

  Rigid(const Pnt& res ) : bTriple(true),  ra(NULL), rb(NULL), rc(NULL) {
    set(res);
  }

  /// set result
  void set(const Pnt& res){
    bCalc = false;
    result = res;
  }

  /// set result
  void setResult(const Pnt& res){
    bCalc = false;
    result = res;
  }

  Rigid( const Pnt& target,  Rigid * pa,  Rigid * pb,  Rigid * pc, bool m) : bTriple(true) 
  {
    set(target,pa,pb,pc,m);
  }

  /// set from target and constraints Counter Clockwise
  void set( const Pnt& target, Rigid * pa, Rigid * pb, Rigid * pc, bool m){
    mtn = m; bCalc=true;
    ra = pa; rb = pb; rc = pc;
    result = target; 
    da.set(ra->result,target);
    db.set(rb->result,target);
    dc.set(rc->result,target); 
  }

  /// set from constraints Counter Clockwise
  void set( Rigid * pa, Rigid * pb, Rigid * pc, bool m){
    mtn = m; bCalc=true;
    ra = pa; rb = pb; rc = pc;
    da.set(ra->result,result);
    db.set(rb->result,result);
    dc.set(rc->result,result); 

    //add this to children of three others
    ra->child.push_back(this); rb->child.push_back(this); rc->child.push_back(this);
  }


  //hmmm?
  void set( Rigid * pa, Rigid * pb, bool m){
    mtn =m; bCalc=true; bTriple=false;
    ra=pa; rb=pb;rc=this;
    da.set(ra->result,result);
    db.set(rb->result,result);
    dc.set(rc->result,result);

    //add this to children of three others
    ra->child.push_back(this); rb->child.push_back(this); rc->child.push_back(this);
  }

  void reset(){ 
    if (ra!=NULL && rb!=NULL && rc!=NULL) bCalc = true; 
  }

  Circle circleA(){
    return (da()^db()).dual();
  }
  Circle circleB(){
    return (db()^dc()).dual();
  }  
  void orbitA(float amt){
    result = Round::point( circleA(), amt * ( mtn?1:-1) ); 
  }
  void orbitB(float amt){
    result = Round::point( circleB(), amt * ( mtn?1:-1) ); 
  } 


  Pnt up(){
    if (bCalc) {
      bCalc=false;                             // lock in case network graph is looped
      for (auto& i : child) i -> down();       // cascade children
      (*ra).up(); (*rb).up(); (*rc).up();
      // satisfy();
      result = bTriple ? Constrain::Triple(da(),db(),dc(),mtn) : Constrain::Planar(da(),db(),dc(),mtn);
    }
    return result;
  }

  //calculate current position based on parents
  void update(){
    if (bCalc){
      result = bTriple ? Constrain::Triple(da(),db(),dc(),mtn) : Constrain::Planar(da(),db(),dc(),mtn);
    }
  }

  //broadcast position to child
  void down(){
      if (bCalc){
        bCalc=false;
        for (auto& i : child) {
          i -> update();
         // i -> satisfy();
          i -> down();
        }             
     //   up();  
      } 
  }

  Pair meet(){
    return ( da() ^ db() ^ dc() ).dual();
  }

  // experimental: bring three spheres closer together towards mutual center until meet is legit.
  void satisfy(int max=20){
    auto& pa = *da.src; auto& pb = *db.src; auto& pc = *dc.src;
    auto rs = Round::size(meet(),false);
    int iter=0;
    while ( rs < -.0001 && iter < max ){
      auto center = Round::loc(pa^pb^pc);
      pa = Round::null( pa+(Vec(center-pa)*fabs(rs)) );
      pb = Round::null( pb+(Vec(center-pb)*fabs(rs)) );
      pc = Round::null( pc+(Vec(center-pc)*fabs(rs)) );
      rs = Round::size( meet(), false );
      iter++;
    }
  }
};





struct Rigid2{
  
  /// Meet of parents
  Point result;

  /// iteration
  int iter=0;

  /*-----------------------------------------------------------------------------
   *  Parents Generate Circles to Control This Result
   *-----------------------------------------------------------------------------*/
  struct Parents{
    DistancePtr da,db;
    Rigid2 *ra, *rb;          //parents

    Circle meet(){
      return (da()^db()).dual();
    }

    bool inside(const Point& p){
      return ( (p<=da())[0] > 0 ) && ( (p<=db())[0] > 0 );
    }

    void operator()(){
      (*ra)(); (*rb)();
    }
  };

  bool bMtn,bCalc,bReCalc;

  /// possibly many couples (one per valence)
  vector<Parents> parents;
  vector<Rigid2*> child;    //children


  Rigid2() : bCalc(false), bReCalc(false) {}
  Rigid2(const Pnt& res) { set(res); }

  void set(const Pnt& res){
    bCalc=false; bReCalc=false;
    result=res;
  }

  //add parents
  void add(Rigid2 *pa, Rigid2 *pb, bool m){
    
    bCalc=true; bReCalc=true;
    
    //make new parents
    Parents p;
    p.ra=pa; p.rb=pb;
    
    //set distance contraints based on pa and pb
    p.da.set(pa->result,result);
    p.db.set(pb->result,result);

    //add this to list of children of just pa 
    pa->child.push_back(this);
   // pb->child.push_back(this);
    parents.push_back(p); 
  }


  /* Rigid2(const Pnt& target, Rigid2* pa, Rigid2*pb, bool m) */
  /* : result(target), ra(pa), rb(pb), bCalc(true), bMtn(m) */
  /* { */
  /*   da.set(ra->result,target); */
  /*   db.set(rb->result,target); */

  /*   ra->child.push_back(this); rb->child.push_back(this); */
  /* } */
 
  void reset(){ bCalc=true; bReCalc=true; iter=0; }

  void operator()(){
    if (bCalc){
     // Pnt np = result;
      bCalc=false;
      for(auto& i : parents){
        i();
        //np = Constrain::Tangency(np, i.da(), i.db());
      }
      update();
      //result = np;
    }
    //return result;
  }

  void update(){
    if(bReCalc){
      for(auto& i : parents){
        result = Constrain::Tangency(result, i.da(), i.db());
      }
    }
  }

  //update only relative to parents with r
  void satisfy_old(Rigid2 * r){
    if(bReCalc){
      bReCalc=false;
      bool bRepeat = true;
      int iter=0;
      while(bRepeat && (iter<10) ){
        bRepeat=false;
        iter++;
        for(auto& i : parents){
          if ( (r==i.ra) || (r==i.rb) ){
            bool bCocircular = ( fabs( (result <= i.meet()).wt() ) < .001);
           // bool bInside = i.inside(result);
            if (!bCocircular) {
              result = Constrain::Tangency(result, i.da(), i.db());
              bRepeat = true;
             }           
           }
          }
        }
      }
  }

  bool hasA(Rigid2 * r){
    for (auto& i : parents){
      if (r==i.ra) return true;
    }
    return false;
  }

  bool hasB(Rigid2 * r){
    for (auto& i : parents){
      if (r==i.rb) return true;
    }
    return false;
  }



  void satisfy_forward(Rigid2 * r){
    if (bReCalc){
      for(auto& i : parents){
        if (r==i.ra){
            result = Constrain::Tangency(result, i.da(), i.db());
         }
      }
    }
  }
  void satisfy_backward(Rigid2 * r){
     if (bReCalc){
      for(auto& i : parents){
        if (r==i.rb){
            result = Constrain::Tangency(result, i.da(), i.db());
         }
      }
     }
  }

  float error(Rigid2 * r){
    float tf=0; bool bFound=false;
    for (auto& i : parents){
      if (r==i.ra){
         bFound=true;
         tf = fabs((result <= i.meet()).wt());
       }
    }
    if (!bFound){
      for (auto& i : parents){
        if (r==i.rb){
          tf = fabs((result <= i.meet()).wt());
        }
      }
    }
    return tf;

  }


  /* void fabrik(Rigid2 * r){ */
  /*   if (bReCalc){ */
  /*     bool bRepeat = true; */
  /*     int iter =0; */
  /*     while (bRepeat && (iter<100)){ */
  /*       bRepeat=false; */
  /*       iter++; */
  /*       for (auto& i : parents){ */
  /*         if (r==i.ra) */
  /*       } */
  /*     } */
  /*   } */
  /* } */

  //set bReCalc to true
  bool checkRecalc(){
    for (auto& i : parents){
      if (i.ra->error(this)>.001){
        bCalc=true;
        bReCalc=true;
        i.ra->bReCalc=true;
        i.ra->bCalc=true;
      }
    }
    return bReCalc;
  }

  vector<Rigid2*> satisfy(int begin, int end){
    
      if (!parents.empty()){
        int tIter=0;
        float tf;
        //find first not already locked
        Rigid2 * tr;
        for (auto& i : parents){
          if (i.ra->bReCalc){
            tr = i.ra;
            break; 
          }
        }
        //cout << parents[0].ra->hasA(this) << endl;
        bool bLoop = parents[0].ra->hasA(this);
        if (bLoop){
            do{
              tIter++;         
              for (int i=begin; i<=end;++i){
                parents[i].ra->satisfy_forward(this);
              }
              for(int i =end; i >= begin; i-- ) {
                parents[i].ra->satisfy_backward(this);
              }
              tf = tr->error(this);//parents[begin].ra->error(this);
            }while( (tf>.001) && (tIter < 10));
        } else {
            do{
              tIter++;         
              for (int i=begin; i<=end;++i){
                parents[i].ra->satisfy_forward(this); 
                parents[i].rb->satisfy_forward(this); //only last one
              }
              for(int i =end; i >= begin; i-- ) {
                parents[i].rb->satisfy_backward(this);
                parents[i].ra->satisfy_backward(this); //only first one
              }
              //this should test for first non-already bound ...
              tf = tr->error(this);//parents[begin].ra->error(this);
            }while( (tf>.001) && (tIter < 10));
            //cout << "iter " << tIter << endl;
        }
      }
      
       vector<Rigid2*> temp;

       //mark parents as satisfied
       for (auto& i : parents){
         if(i.ra->bReCalc==true) temp.push_back(i.ra);
         i.ra->bReCalc=false;
        // i.rb->bReCalc=false;
       }
       bool bLoop = parents[0].ra->hasA(this);
       if (!bLoop){
         if(parents.back().rb->bReCalc==true) temp.push_back(parents.back().rb);
         parents.back().rb->bReCalc=false;
       }

       return temp;
  }

  
  // go through all parents satisfy relationship to this
  void cascade(int begin, int end){
   
      //do this forward and backward reach how many times?
      //until first node is within error of first forward meet
      auto rp = satisfy(begin,end);
      cout << "parents to solve: " <<  rp.size() << endl;
      while (!rp.empty()){
        //cascade connections 
        vector<Rigid2*> temp;
        for (auto& i : rp){
            auto r = i->satisfy(0, i->parents.size()-1);
            for (auto& j : r){
              temp.push_back(j);
            }
            //i.ra->cascade(0,i.ra->parents.size()-1);
         }
         //cout << temp.size() << endl;
         rp = temp;
      }

  }

  Cir circle(int idx =0) { return ( parents[idx].da() ^  parents[idx].db() ).dual(); }

  /// get point at theta t around constraint orbit
  Pnt orbit(VSR_PRECISION t, int idx=0) { return Round::point( circle(idx), t * ( bMtn?1:-1) ); }
};



/*!
 *  \brief  Rig has n distance constraints to satisfy (try using fabrik solver here)
 */
struct Rig {

    Point result; ///< result of constraint computation
    bool bCalc,bReCalc;
    int iter=0;

    void set(const Pnt& res){
      bCalc=false; bReCalc=false;
      result=res;
    }

    void reset(){
      bCalc=true; bReCalc=true; iter=0;
    }

    struct Parent{
      DistancePtr da;
      Rig * rig;
      bool bStrut =true;

      //set distance constraint to rig r
      void set(const Point& p){
        da.set(rig->result, p);
      }

      //constrain a point p to spherical distance via strut or cable
      Point constrain(const Point& p){//, bool strut){
        return bStrut ? Constrain::Tangency(p,da()) : Constrain::Tension(p, da()); 
      }

      float distance(const Point& p){
        return (p<=da())[0];
      }
    };

    vector<Parent> parent;
    vector<Rig*> child;

    //add a constraint
    void add(Rig * r, bool bStrut=true){
       Parent p;
       p.bStrut = bStrut;
       p.rig = r;
       p.set(result);
       parent.push_back(p);
       p.rig->child.push_back(this);
    }

    void modify(Rig * r, bool bStrut){
      for (auto& i : parent){
        if (r==i.rig) i.bStrut=bStrut;
      }
    }

    //satisfy constraint to rig r
    void satisfy(Rig * r){
      if (bReCalc){       
        for (auto& i : parent){
          bool tCalc=false;
          //if parent is r
          if (r==i.rig) tCalc=true;
          //...or is connected to r
          for (auto& j : i.rig->parent){
            if (r==j.rig) tCalc=true;
          }
          //..calc
          if (tCalc){
            result = i.constrain(result);
          }
        }
      }
    }

    void cascade(){
      if (bCalc){
        iter++;
        if (iter>1000) {
          bCalc=false;  
        }   
        for (auto& i : child){
          i->satisfy(this);
        }
        for (auto& i : child){
        //  i->cascade();
        }
      }
    }

};

/* ----------------------------------------------------------------------------- */
/*  *  SIMPLICIAL CONSTRAINT . . . (n+1)-simplices to constrain (n)-chains connected by (n-1)-simplex edges */ 
/*  *-----------------------------------------------------------------------------*/ 
/* struct Rigid_{ */
/*   bool bCalc, bTriple; */
/*   Pnt result; */

/*   DistancePtr da,db,dc; */
/*   Rigid_ *ra, *rb, *rc; */
/*   bool mtn; */

/*   Rigid_() : bCalc(false), bTriple(true), ra(NULL), rb(NULL), rc(NULL) {} */

/*   Rigid_(const Pnt& res ) : bTriple(true),  ra(NULL), rb(NULL), rc(NULL) { */
/*     set(res); */
/*   } */
/*   void set(const Pnt& res){ */
/*     bCalc = false; */
/*     result = res; */
/*   } */

/*   Rigid_( const Pnt& target,  Rigid * pa,  Rigid * pb,  Rigid * pc, bool m) : bTriple(true) */ 
/*   { */
/*     set(target,pa,pb,pc,m); */
/*   } */

/*   //Counter Clockwise */
/*   void set( const Pnt& target, Rigid * pa, Rigid * pb, Rigid * pc, bool m){ */
/*     mtn = m; bCalc=true; */
/*     ra = pa; rb = pb; rc = pc; */
/*     result = target; */ 
/*     da.set(ra->result,target); */
/*     db.set(rb->result,target); */
/*     dc.set(rc->result,target); */ 
/*   } */

/*   //Counter Clockwise */
/*   void set( Rigid * ra, Rigid * rb, Rigid * rc, bool m){ */
    
/*     mtn = m; bCalc=true; */
/*     da.set(ra->result,result); */
/*     db.set(rb->result,result); */
/*     dc.set(rc->result,result); */ 

/*     //fasten cycle */
/*     ra->set(rb, rc, this, m); */
/*     rb->set(rc, this, ra, m); */
/*     rc->set(this, ra, rb, m); */
    
/*   } */

/*   /1* void set( Rigid * pa, Rigid * pb, bool m){ *1/ */
/*   /1*   mtn =m; bCalc=true; bTriple=false; *1/ */
/*   /1*   ra=pa; rb=pb;rc=this; *1/ */
/*   /1*   da.set(ra->result,result); *1/ */
/*   /1*   db.set(rb->result,result); *1/ */
/*   /1*   dc.set(rc->result,result); *1/ */
/*   /1* } *1/ */

/*   void reset(){ */ 
/*     if (ra!=NULL && rb!=NULL && rc!=NULL) bCalc = true; */ 
/*   } */

/*   Pnt operator()(){ */
/*     if (bCalc) { */
/*       bCalc=false; //lock because network graph is looped */
/*       (*ra)(); (*rb)(); (*rc)(); */
/*       result = satisfy(); //satisfying requires unlocking */
/*     } */
/*     return result; */
/*   } */

/*   Pair meet(){ */
/*     return Constrain::Triple( da(), db(), dc() ); */
/*   } */

/*   //bring three spheres closer together towards center of meet until meet is legit. */
/*   Pnt satisfy(){ */
/*     auto meet = Constrain::Triple( da(), db(), dc() ); */
/*     if ( Round::size(meet,false) < -.0001 ){ */
      
/*     } */
/*      return bTriple ? Constrain::Triple(da(),db(),dc(),mtn) : Constrain::Planar(da(),db(),dc(),mtn); */

/*   } */
/* }; */


} } //vsr::cga:;
#endif   /* ----- #ifndef vsr_rigid_INC  ----- */
