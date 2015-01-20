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
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  vsr_rigid_INC
#define  vsr_rigid_INC


struct Constrain {

   //unused?
    static Point Crease (const Pnt& a, const Pnt& dls, const Dll d, bool mtn){
      // Dls dls = Ro::at(b, a);
       Circle cir = a ^ d;
       Pair par = ( dls ^ cir.dual()).dual();
       return Ro::loc( Ro::split(par,mtn) );
    }


    /* static Pair Triple(const Dls& da, const Dls& db, const Dls & dc){ */
    /*   return (da ^ db ^ dc).dual(); */
    /* } */

    // three distances, counter clockwise (deprecated, use Tetral)
    static Point Triple (const Dls& da, const Dls& db, const Dls& dc, bool mtn){
       return Ro::loc( Ro::split( (da ^ db ^ dc).dual(), mtn ) ) ; 
    }
    // tetral constraint
    static Point Tetral (const Dls& da, const Dls& db, const Dls& dc, bool mtn){
       return Ro::loc( Ro::split( (da ^ db ^ dc).dual(), mtn ) ) ; 
    }
    //planar constraint: two distances and a plane
    static Point Planar( const Dls& da, const Dls& db, const Dls& dc, bool mtn){
       Plane plane = da ^ db ^ dc ^ Inf(1);
       return Ro::loc( Ro::split( (da ^ dc ^ plane.dual() ).dual(), mtn ) );
    }

    //tangency constraint, two distances and an original point (closest to original)
    static Point Tangency(const Pnt& p, const Dls& da, const Dls& db){

      auto meet = (da ^ db).dual();
      auto tan =  Ro::loc( Ta::at( meet, p ) );
      auto sur = Ro::sur( meet );
      auto line = tan ^ sur ^ Inf(1);
      auto np = Ro::split ( ( line.dual() ^ sur).dual(), false );

      return np;
      
    }


    // three distances, center of meet pre-SPLIT (experimental)
    static Point Triple0 (const Dls& da, const Dls& db, const Dls& dc){
       return Ro::loc( (da ^ db ^ dc).dual()  ) ; 
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
    /*               tmpGoal = Ro::split(par,true);      //extract point from point pair intersection */
    /*           } */
              
    /*           //forward correction */
    /*           for (int i = begin; i < end; ++i){ */
    /*               dls = nextDls(i);                   //set boundary sphere through i+1 th frame */
    /*               dll = linf(i);                      //get line to i+1th frame; */
    /*               par = (dll ^ dls).dual();           //get point pair intersection of line and boundary sphere */
    /*               tmpBase = Ro::split(par,true); */
    /*               mFrame[i+1].pos(tmpBase);           //set position of i+1th frame */
    /*           } */
              
    /*           s = mFrame[ end ].pos() <= p * -2.0; */
               
    /*           n++;  if (n > 20) {  break; } */
    /*       } */

    /* } */

  
};

//experimental
struct DistancePtr {
  Pnt * src; // center 
  float t;

  DistancePtr(){};

  DistancePtr( Pnt& a, const Pnt& target)  
  {
    set(a,target);  
  }

  void set(Pnt& a, const Pnt& target){
    src = &a; t = Ro::rad( Ro::at(*src,target) );
  }

  Dls operator()(){ return Ro::dls( *src, t ); }

};

struct Rigid{
  //default calc is false until ra parents are set
  bool bCalc, bTriple;
  Pnt result;

  DistancePtr da,db,dc;
  
  //Dependent on three parents
  Rigid *ra, *rb, *rc;

  //Has n children which depend on it
  vector<Rigid*> child;

  bool mtn;

  Rigid() : bCalc(false), bTriple(true), ra(NULL), rb(NULL), rc(NULL) {}

  Rigid(const Pnt& res ) : bTriple(true),  ra(NULL), rb(NULL), rc(NULL) {
    set(res);
  }
  void set(const Pnt& res){
    bCalc = false;
    result = res;
  }

  Rigid( const Pnt& target,  Rigid * pa,  Rigid * pb,  Rigid * pc, bool m) : bTriple(true) 
  {
    set(target,pa,pb,pc,m);
  }

  //Counter Clockwise
  void set( const Pnt& target, Rigid * pa, Rigid * pb, Rigid * pc, bool m){
    mtn = m; bCalc=true;
    ra = pa; rb = pb; rc = pc;
    result = target; 
    da.set(ra->result,target);
    db.set(rb->result,target);
    dc.set(rc->result,target); 
  }

  //Counter Clockwise
  void set( Rigid * pa, Rigid * pb, Rigid * pc, bool m){
    mtn = m; bCalc=true;
    ra = pa; rb = pb; rc = pc;
    da.set(ra->result,result);
    db.set(rb->result,result);
    dc.set(rc->result,result); 

    ra->child.push_back(this); rb->child.push_back(this); rc->child.push_back(this);
  }

  void set( Rigid * pa, Rigid * pb, bool m){
    mtn =m; bCalc=true; bTriple=false;
    ra=pa; rb=pb;rc=this;
    da.set(ra->result,result);
    db.set(rb->result,result);
    dc.set(rc->result,result);

    ra->child.push_back(this); rb->child.push_back(this); rc->child.push_back(this);
  }

  void reset(){ 
    if (ra!=NULL && rb!=NULL && rc!=NULL) bCalc = true; 
  }

  Pnt up(){
    if (bCalc) {
      bCalc=false; // lock in case network graph is looped
      for (auto& i : child) i -> down(); 
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

  //broadcast position to child and fix any errors
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

  //bring three spheres closer together towards mutual center until meet is legit.
  void satisfy(int max=20){
    auto& pa = *da.src; auto& pb = *db.src; auto& pc = *dc.src;
    auto rs = Ro::size(meet(),false);
    int iter=0;
    while ( rs < -.0001 && iter < max ){
      auto center = Ro::loc(pa^pb^pc);
      pa = Ro::null( pa+(Vec(center-pa)*fabs(rs)) );
      pb = Ro::null( pb+(Vec(center-pb)*fabs(rs)) );
      pc = Ro::null( pc+(Vec(center-pc)*fabs(rs)) );
      rs = Ro::size( meet(), false );
      iter++;
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
/*     if ( Ro::size(meet,false) < -.0001 ){ */
      
/*     } */
/*      return bTriple ? Constrain::Triple(da(),db(),dc(),mtn) : Constrain::Planar(da(),db(),dc(),mtn); */

/*   } */
/* }; */


struct Rigid2{
  Pnt result;

 // Rigid2 *ra, *rb;          //parents

  struct Parent{
    DistancePtr da,db;
    Rigid2 *ra, *rb;          //parents
    void operator()(){
      (*ra)(); (*rb)();
    }
  };

  bool bMtn,bCalc,bReCalc;

  vector<Rigid2*> child;    //children
  vector<Parent> parent;

  Rigid2() : bCalc(false), bReCalc(false) {}
  Rigid2(const Pnt& res) { set(res); }

  void set(const Pnt& res){
    bCalc=false; bReCalc=false;
    result=res;
  }


  /* void set(Rigid2 *pa, Rigid2 *pb, bool m){ */
    
  /*   ra=pa; rb=pb; */

  /*   bCalc=true; bMtn = m; */
    
  /*   da.set(ra->result,result); */
  /*   db.set(rb->result,result); */
    
  /*   ra->child.push_back(this); rb->child.push_back(this); */
  /* } */

  //add parent
  void add(Rigid2 *pa, Rigid2 *pb, bool m){
    bCalc=true; bReCalc=true;
    Parent p;
    p.ra=pa; p.rb=pb;
    p.da.set(pa->result,result);
    p.db.set(pb->result,result);
    pa->child.push_back(this);
    pb->child.push_back(this);
    parent.push_back(p); 
  }


  /* Rigid2(const Pnt& target, Rigid2* pa, Rigid2*pb, bool m) */
  /* : result(target), ra(pa), rb(pb), bCalc(true), bMtn(m) */
  /* { */
  /*   da.set(ra->result,target); */
  /*   db.set(rb->result,target); */

  /*   ra->child.push_back(this); rb->child.push_back(this); */
  /* } */
 
  void reset(){ bCalc=true; bReCalc=true; }

  void operator()(){
    if (bCalc){
      Pnt np = result;
      bCalc=false;
      for(auto& i : parent){
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
      for(auto& i : parent){
        result = Constrain::Tangency(result, i.da(), i.db());
      }
    }
  }

  void cascade(){
    if (bCalc){
      bCalc=false;
      for(auto& i : child) {
        i->update();
        //(*i)();
        i->cascade();
      }
    }
  }

  Cir circle(int idx =0) { return ( parent[idx].da() ^  parent[idx].db() ).dual(); }

  /// get point at theta t around constraint orbit
  Pnt orbit(VT t) { return Ro::pnt_cir( circle(), t * ( bMtn?1:-1) ); }
};


#endif   /* ----- #ifndef vsr_rigid_INC  ----- */
