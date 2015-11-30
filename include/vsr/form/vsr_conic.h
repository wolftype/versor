/*
 * * =====================================================================================
 *
 *       Filename:  vsr_conic.h
 *
 *    Description:  conic transforms as described in Applications of Conformal Geometric Algebra
 *    in Computer Vision and Graphics, by wareham, cameron and j.lasenby
 *    see also Recent Applications of Conformal Geometric Algebra by a. lasenby
 *
 *        Version:  1.0
 *        Created:  01/09/2014 12:44:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */

/** @file Conic Transforms */

#ifndef  vsr_conic_INC
#define  vsr_conic_INC


#include "detail/vsr_generic_op.h"


namespace vsr{ 

/** @defgroup cgatransforms More Transformations 
    @ingroup cgaops

    Mappings and Inverse Mappings
*/

/*! 
 *  \brief conic transforms 
 *
 *  conic transforms as described in Applications of Conformal Geometric Algebra
 *  in Computer Vision and Graphics, by wareham, cameron and j.lasenby
 *  see also Recent Applications of Conformal Geometric Algebra by a. lasenby

    @ingroup cgatransforms
*/

class Conic {

  VSR_PRECISION ecc;

  public:

    /**
    * @brief Transform a vsr::cga::Vec 
    *
    * @param veca  vsr::cga::Vec position to transform
    * @param vecb  vsr::cga::Vec direction
    * @param alpha VSR_PRECISION eccentricity
    *
    * @return transformed vsr::cga::Point 
    */
    template<class A>
    static GAPnt<A> Transform( const  GAVec<A> &vec, const GAVec<A>& v, VSR_PRECISION alpha){
      VSR_PRECISION talpha = ( vec <= v)[0] * alpha;
      GAPnt<A> p = vec.null();
      GAPnt<A>  p2 =  p + GAOri<A>(1) * talpha; // * beta
      auto wt = p2 <= GAInf<A>(1);
      return nga::Round::loc( p2 / -wt );
  }

    /**
    * @brief Transform a vsr::cga::Point  
    *
    * @param veca  vsr::cga::Vec position to transform
    * @param vecb  vsr::cga::Vec direction
    * @param alpha VSR_PRECISION eccentricity
    *
    * @return a vsr::cga::Point image
    */
    template<class A>
    static GAPnt<A> Transform( const  GAPnt<A> & p, const GAVec<A>& v, VSR_PRECISION alpha){
      VSR_PRECISION talpha = ( p <= v)[0] * alpha;
      GAPnt<A>  p2 =  p + GAOri<A>(1) * talpha; // * beta
      auto wt = p2 <= GAInf<A>(1);
      return nga::Round::loc( p2 / -wt );
  }

    /**
    * @brief Inverse Transform of a vsr::cga::Point  
    *
    * @param veca  vsr::cga::Vec position to transform
    * @param vecb  vsr::cga::Vec direction
    * @param alpha VSR_PRECISION eccentricity
    *
    * @return a vsr::cga::Point pre-image 
    */
    template<class A>
    static  GAPnt<A>  ITransform(const  GAPnt<A> & p, const GAVec<A>& v, VSR_PRECISION alpha){
      VSR_PRECISION talpha = ( p <= v)[0] * alpha;
       GAPnt<A>  p2 = p - GAOri<A>(1) * talpha; // / beta
      auto wt = p2 <= GAInf<A>(1);
      return nga::Round::loc( p2 / -wt );
    }
    /**
    * @brief Transform a vsr::cga::Point  
    *
    * @param line  vsr::cga::Line to transform
    * @param vecb  vsr::cga::Vec direction
    * @param alpha VSR_PRECISION eccentricity
    *
    * @return a vsr::cga::Circle image
    */
    template<class A>
    static GALin<A> Transform( const GALin<A>& line, const GAVec<A>& v, VSR_PRECISION alpha){
      GAPnt<A> pa = nga::Flat::loc( line, PAO, false ).null();
      GAPnt<A> tpa = Transform(pa,v,alpha);
      GAPnt<A> pb = pa.trs( -nga::Flat::dir( line ).template copy<GAVec<A>>().unit() );
      GAPnt<A> tpb = Transform(pb,v,alpha);
      return tpa ^ tpb ^ GAInf<A>(1);
    }

    /**
    * @brief Transform a vsr::cga::Point  
    *
    * @param line  vsr::cga::Line to transform
    * @param vecb  vsr::cga::Vec direction
    * @param alpha VSR_PRECISION eccentricity
    *
    * @return a vsr::cga::Circle pre-image
    */   
   template<class A>
   static GALin<A> ITransform( const GALin<A>& line, const GAVec<A>& v, VSR_PRECISION alpha){
    GAPnt<A> pa = nga::Flat::loc( line, PAO, false).null();
    GAPnt<A> ipa = ITransform(pa,v,alpha);
    GAPnt<A> pb = pa.trs( -nga::Flat::dir( line ).template copy< GAVec<A> >().unit()  );
    GAPnt<A> ipb = ITransform(pb,v,alpha);
    return ipa ^ ipb ^ GAInf<A>(1);
  }

    /// Transform a dual line in direction v with eccentricity alpha
      template<class A>
    static GADll<A> Transform( const GADll<A>& line, const GAVec<A>& v, VSR_PRECISION alpha){
      GAPnt<A> pa = nga::Flat::loc( line, PAO, true ).null();
      GAPnt<A> tpa = Transform(pa,v,alpha);
      GAPnt<A> pb = pa.trs( -nga::Flat::dir( line.dual() ).template copy<GAVec<A>>().unit() );
      GAPnt<A> tpb = Transform(pb,v,alpha);
      return (tpa ^ tpb ^ GAInf<A>(1)).dual();
    }

    /// Inverse Transform a dual line in direction v with eccentricity alpha
   template<class A>
   static GADll<A> ITransform( const GADll<A>& line, const GAVec<A>& v, VSR_PRECISION alpha){
    GAPnt<A> pa = nga::Flat::loc( line, PAO, true).null();
    GAPnt<A> ipa = ITransform(pa,v,alpha);
    GAPnt<A> pb = pa.trs( -nga::Flat::dir( line.dual() ).template copy< GAVec<A> >().unit()  );
    GAPnt<A> ipb = ITransform(pb,v,alpha);
    return (ipa ^ ipb ^ GAInf<A>(1)).dual();
  }

    /// Transform a plane in direction v with eccentricity alpha
    template<class A>
    static GAPln<A> Transform( const GAPln<A>& plane, const GAVec<A>& v, VSR_PRECISION alpha){
    //get points to apply aipoint to
      GAPnt<A> pa = nga::Flat::loc( plane, PAO, false ).null();
      auto ca = ( nga::Round::dls(pa, 1) ^ plane.dual() ).dual();
      auto pb = nga::Round::pnt_cir(ca, 0);
      auto pc = nga::Round::pnt_cir(ca, PIOVERFOUR);

      GAPnt<A> tpa = Transform(pa,v,alpha);
      GAPnt<A> tpb = Transform(pb,v,alpha); 
      GAPnt<A> tpc = Transform(pc,v,alpha);
      return tpa ^ tpb ^ tpc ^ GAInf<A>(1);
    }

    /// Inverse Transform a plane in direction v with eccentricity alpha
    template<class A>
    static GAPln<A> ITransform( const GAPln<A>& plane, const GAVec<A>& v, VSR_PRECISION alpha){
    //get points to apply aipoint to
      GAPnt<A> pa = nga::Flat::loc( plane, PAO, false ).null();
      auto ca = ( nga::Round::dls(pa, 1) ^ plane.dual() ).dual();
      auto pb = nga::Round::pnt_cir(ca, 0);
      auto pc = nga::Round::pnt_cir(ca, PIOVERFOUR);

      GAPnt<A> tpa = ITransform(pa,v,alpha);
      GAPnt<A> tpb = ITransform(pb,v,alpha); 
      GAPnt<A> tpc = ITransform(pc,v,alpha);
      return tpa ^ tpb ^ tpc ^ GAInf<A>(1);
    }

  /// Transform a dual plane in direction v with eccentricity alpha
   template<class A>
   static GADlp<A> Transform( const GADlp<A>& plane, const GAVec<A>& v, VSR_PRECISION alpha){
    GAPnt<A> pa = nga::Flat::loc( plane, PAO, true).null();
    auto ca = ( nga::Round::dls(pa, 1) ^ plane ).dual();
    auto pb = nga::Round::pnt_cir(ca, 0);
    auto pc = nga::Round::pnt_cir(ca, PIOVERFOUR);

    GAPnt<A> ipa = Transform(pa,v,alpha);
    GAPnt<A> ipb = Transform(pb,v,alpha);
    GAPnt<A> ipc = Transform(pc,v,alpha);

    return (ipa ^ ipb ^ ipc ^ GAInf<A>(1)).dual();
  }

  /// Inverse Transform a dual plane in direction v with eccentricity alpha
   template<class A>
   static GADlp<A> ITransform( const GADlp<A>& plane, const GAVec<A>& v, VSR_PRECISION alpha){
    GAPnt<A> pa = nga::Flat::loc( plane, PAO, true).null();
    auto ca = ( nga::Round::dls(pa, 1) ^ plane ).dual();
    auto pb = nga::Round::pnt_cir(ca, 0);
    auto pc = nga::Round::pnt_cir(ca, PIOVERFOUR);

    GAPnt<A> ipa = ITransform(pa,v,alpha);
    GAPnt<A> ipb = ITransform(pb,v,alpha);
    GAPnt<A> ipc = ITransform(pc,v,alpha);

    return (ipa ^ ipb ^ ipc ^ GAInf<A>(1) ).dual();
  }


    /* template<class A> */
    /* static GADlp<A> Transform( const GADlp<A>& plane, const GAVec<A>& v, VSR_PRECISION alpha){ */
    /* //get points to apply aipoint to */
    /*   GAPnt<A> pa = nga::Flat::loc( plane, PAO, true ).null(); */
    /*   GAPnt<A> tpa = Transform(pa,v,alpha); */
    /*   GAPnt<A> pb = pa.trs( -nga::Flat::dir( plane.dual() ).dual().template copy<GAVec<A>>().unit() );//normal */
    /*   GAPnt<A> tpb = Vec( Transform(pb,v,alpha) ).unit(); */
    /*   return tpa <= tpb.template copy<NDrv<DIM>>(); */
    /* } */

   /* template<class A> */
   /* static GADlp<A> ITransform( const GADlp<A>& plane, const GAVec<A>& v, VSR_PRECISION alpha){ */
    /* GAPnt<A> pa = nga::Flat::loc( plane, PAO, true).null(); */
    /* GAPnt<A> ipa = ITransform(pa,v,alpha); */
    /* GAPnt<A> pb = pa.trs( -nga::Flat::dir( plane.dual() ).dual().template copy< GAVec<A> >().unit() ); */
    /* GAPnt<A> ipb = Vec( ITransform(pb,v,alpha) - ipa ).unit(); */
    /* return ipa <= ipb.template copy<NDrv<DIM>>(); */
  /* } */


};


} //vsr::

#endif   /* ----- #ifndef vsr_conic_INC  ----- */

