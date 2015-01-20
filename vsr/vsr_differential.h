/*
 * =====================================================================================
 *
 *       Filename:  vsr_differential.h
 *
 *    Description:  differential geometry routines
 *
 *        Version:  1.0
 *        Created:  11/04/2014 14:58:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *
 * =====================================================================================
 */


#ifndef  vsr_differential_INC
#define  vsr_differential_INC


#include "vsr_cga3D_op.h"
#include "vsr_graph.h"

namespace vsr {

//simplicial variable (2D facets in 3D space)
struct Simplicial2 {
   Vec ea, eb;            ///< edges
   Biv pss;               ///< Tangent Psuedoscalar
   Vec ra, rb;            ///< reciprocal frames
   VT area, la, lb;             ///< edge lengths
   
   Simplicial2(){}

   /// Constructor takes three coordinates
   template<class T>
   Simplicial2(const T& a, const T& b, const T& c){
      ea = b-a; la = ea.norm();
      eb = c-a; lb = eb.norm();
      auto tpss=ea^eb;
      area = tpss.rnorm() * .5;
      pss = !(tpss);
      ra = eb<=pss;
      rb = -ea<=pss;
   }

   /// Vector Derivative of function defined on points
   template<class T>
   vector<Vec> derivative(const T& n, const T& na, const T& nb){
     vector<Vec> v;
     //diff along edges
     auto dna = na - n;
     auto dnb = nb - n; 
     // coefficients
     auto wa = dna <= ea;      
     auto wb = dnb <= eb;
     // weighted reciprocals
     v.push_back( ra * wa);
     v.push_back( rb * wb);
     return v;
   }


   //weighted reciprocals
   /* Vec wra() { return ra * la; } */
   /* Vec wrb() { return rb * lb; } */

   //given a node, get edge vectors
  /* template<class T> */
  /* static vector<Simplicial2> Domain(const typename HEGraph<T>::Node& n){ */
  /*   vector<Simplicial2> v; */
  /*   const auto& ta = n.data(); */
  /*   for (auto& i : n.valence() ){ */
  /*     auto& tb = i->a(); */
  /*     auto& tc = i->next->a(); */
  /*     v.push_back( Simplicial2(ta,tb,tc) ); */
  /*   } */
  /*   return v; */
  /* } */

};

//generic simplicial variable (in progress)
template<int DIM>
struct Simplicial {
  using TVEC = NEVec<DIM>;
  //using TPSS = NEPSS<DIM>;
};


//given a facet, get reciprocal



//given a half edge graph, get normals


} //vsr::

#endif   /* ----- #ifndef vsr_differential_INC  ----- */
