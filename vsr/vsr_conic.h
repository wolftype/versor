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



#ifndef  vsr_conic_INC
#define  vsr_conic_INC


#include "vsr_generic_op.h"


namespace vsr{

/*!
 *  \brief conic transforms 
 *
 *  conic transforms as described in Applications of Conformal Geometric Algebra
 *  in Computer Vision and Graphics, by wareham, cameron and j.lasenby
 *  see also Recent Applications of Conformal Geometric Algebra by a. lasenby
 */
class Conic {

  VT ecc;

  public:


    /// Transform a Euclidean Vec in direction v with eccentricity alpha
    template<TT DIM>
    static NPnt<DIM> Transform( const  NVec<DIM> &vec, const NVec<DIM>& v, VT alpha){
      VT talpha = ( vec <= v)[0] * alpha;
      NPnt<DIM> p = vec.null();
      NPnt<DIM>  p2 =  p + NOri< DIM >(1) * talpha; // * beta
      auto wt = p2 <= NInf< DIM >(1);
      return Ro::loc( p2 / -wt );
  }

    /// Transform a point in direction v with eccentricity alpha
    template<TT DIM>
    static NPnt<DIM> Transform( const  NPnt<DIM> & p, const NVec<DIM>& v, VT alpha){
      VT talpha = ( p <= v)[0] * alpha;
      NPnt<DIM>  p2 =  p + NOri< DIM >(1) * talpha; // * beta
      auto wt = p2 <= NInf< DIM >(1);
      return Ro::loc( p2 / -wt );
  }

    /// Inverse Transform a point in direction v with eccentricity alpha
    template<TT DIM>
    static  NPnt<DIM>  ITransform(const  NPnt<DIM> & p, const NVec<DIM>& v, VT alpha){
      VT talpha = ( p <= v)[0] * alpha;
       NPnt<DIM>  p2 = p - NOri<DIM>(1) * talpha; // / beta
      auto wt = p2 <= NInf<DIM>(1);
      return Ro::loc( p2 / -wt );
    }

    /// Transform a line in direction v with eccentricity alpha
    template<TT DIM>
    static NLin<DIM> Transform( const NLin<DIM>& line, const NVec<DIM>& v, VT alpha){
      NPnt<DIM> pa = Fl::loc( line, PAO, false ).null();
      NPnt<DIM> tpa = Transform(pa,v,alpha);
      NPnt<DIM> pb = pa.trs( -Fl::dir( line ).template copy<NVec<DIM>>().unit() );
      NPnt<DIM> tpb = Transform(pb,v,alpha);
      return tpa ^ tpb ^ Inf(1);
    }

    /// Inverse Transform a line in direction v with eccentricity alpha
   template<TT DIM>
   static NLin<DIM> ITransform( const NLin<DIM>& line, const NVec<DIM>& v, VT alpha){
    NPnt<DIM> pa = Fl::loc( line, PAO, false).null();
    NPnt<DIM> ipa = ITransform(pa,v,alpha);
    NPnt<DIM> pb = pa.trs( -Fl::dir( line ).template copy< NVec<DIM> >().unit()  );
    NPnt<DIM> ipb = ITransform(pb,v,alpha);
    return ipa ^ ipb ^ Inf(1);
  }

    /// Transform a dual line in direction v with eccentricity alpha
      template<TT DIM>
    static NDll<DIM> Transform( const NDll<DIM>& line, const NVec<DIM>& v, VT alpha){
      NPnt<DIM> pa = Fl::loc( line, PAO, true ).null();
      NPnt<DIM> tpa = Transform(pa,v,alpha);
      NPnt<DIM> pb = pa.trs( -Fl::dir( line.dual() ).template copy<NVec<DIM>>().unit() );
      NPnt<DIM> tpb = Transform(pb,v,alpha);
      return (tpa ^ tpb ^ Inf(1)).dual();
    }

    /// Inverse Transform a dual line in direction v with eccentricity alpha
   template<TT DIM>
   static NDll<DIM> ITransform( const NDll<DIM>& line, const NVec<DIM>& v, VT alpha){
    NPnt<DIM> pa = Fl::loc( line, PAO, true).null();
    NPnt<DIM> ipa = ITransform(pa,v,alpha);
    NPnt<DIM> pb = pa.trs( -Fl::dir( line.dual() ).template copy< NVec<DIM> >().unit()  );
    NPnt<DIM> ipb = ITransform(pb,v,alpha);
    return (ipa ^ ipb ^ Inf(1)).dual();
  }

    /// Transform a plane in direction v with eccentricity alpha
    template<TT DIM>
    static NPln<DIM> Transform( const NPln<DIM>& plane, const NVec<DIM>& v, VT alpha){
    //get points to apply aipoint to
      NPnt<DIM> pa = Fl::loc( plane, PAO, false ).null();
      auto ca = ( Ro::dls(pa, 1) ^ plane.dual() ).dual();
      auto pb = Ro::pnt_cir(ca, 0);
      auto pc = Ro::pnt_cir(ca, PIOVERFOUR);

      NPnt<DIM> tpa = Transform(pa,v,alpha);
      NPnt<DIM> tpb = Transform(pb,v,alpha); 
      NPnt<DIM> tpc = Transform(pc,v,alpha);
      return tpa ^ tpb ^ tpc ^ Inf(1);
    }

    /// Inverse Transform a plane in direction v with eccentricity alpha
    template<TT DIM>
    static NPln<DIM> ITransform( const NPln<DIM>& plane, const NVec<DIM>& v, VT alpha){
    //get points to apply aipoint to
      NPnt<DIM> pa = Fl::loc( plane, PAO, false ).null();
      auto ca = ( Ro::dls(pa, 1) ^ plane.dual() ).dual();
      auto pb = Ro::pnt_cir(ca, 0);
      auto pc = Ro::pnt_cir(ca, PIOVERFOUR);

      NPnt<DIM> tpa = ITransform(pa,v,alpha);
      NPnt<DIM> tpb = ITransform(pb,v,alpha); 
      NPnt<DIM> tpc = ITransform(pc,v,alpha);
      return tpa ^ tpb ^ tpc ^ Inf(1);
    }

  /// Transform a dual plane in direction v with eccentricity alpha
   template<TT DIM>
   static NDlp<DIM> Transform( const NDlp<DIM>& plane, const NVec<DIM>& v, VT alpha){
    NPnt<DIM> pa = Fl::loc( plane, PAO, true).null();
    auto ca = ( Ro::dls(pa, 1) ^ plane ).dual();
    auto pb = Ro::pnt_cir(ca, 0);
    auto pc = Ro::pnt_cir(ca, PIOVERFOUR);

    NPnt<DIM> ipa = Transform(pa,v,alpha);
    NPnt<DIM> ipb = Transform(pb,v,alpha);
    NPnt<DIM> ipc = Transform(pc,v,alpha);

    return (ipa ^ ipb ^ ipc ^ Inf(1)).dual();
  }

  /// Inverse Transform a dual plane in direction v with eccentricity alpha
   template<TT DIM>
   static NDlp<DIM> ITransform( const NDlp<DIM>& plane, const NVec<DIM>& v, VT alpha){
    NPnt<DIM> pa = Fl::loc( plane, PAO, true).null();
    auto ca = ( Ro::dls(pa, 1) ^ plane ).dual();
    auto pb = Ro::pnt_cir(ca, 0);
    auto pc = Ro::pnt_cir(ca, PIOVERFOUR);

    NPnt<DIM> ipa = ITransform(pa,v,alpha);
    NPnt<DIM> ipb = ITransform(pb,v,alpha);
    NPnt<DIM> ipc = ITransform(pc,v,alpha);

    return (ipa ^ ipb ^ ipc ^ Inf(1) ).dual();
  }


    /* template<TT DIM> */
    /* static NDlp<DIM> Transform( const NDlp<DIM>& plane, const NVec<DIM>& v, VT alpha){ */
    /* //get points to apply aipoint to */
    /*   NPnt<DIM> pa = Fl::loc( plane, PAO, true ).null(); */
    /*   NPnt<DIM> tpa = Transform(pa,v,alpha); */
    /*   NPnt<DIM> pb = pa.trs( -Fl::dir( plane.dual() ).dual().template copy<NVec<DIM>>().unit() );//normal */
    /*   NPnt<DIM> tpb = Vec( Transform(pb,v,alpha) ).unit(); */
    /*   return tpa <= tpb.template copy<NDrv<DIM>>(); */
    /* } */

   /* template<TT DIM> */
   /* static NDlp<DIM> ITransform( const NDlp<DIM>& plane, const NVec<DIM>& v, VT alpha){ */
    /* NPnt<DIM> pa = Fl::loc( plane, PAO, true).null(); */
    /* NPnt<DIM> ipa = ITransform(pa,v,alpha); */
    /* NPnt<DIM> pb = pa.trs( -Fl::dir( plane.dual() ).dual().template copy< NVec<DIM> >().unit() ); */
    /* NPnt<DIM> ipb = Vec( ITransform(pb,v,alpha) - ipa ).unit(); */
    /* return ipa <= ipb.template copy<NDrv<DIM>>(); */
  /* } */


};

} //vsr::

#endif   /* ----- #ifndef vsr_conic_INC  ----- */

