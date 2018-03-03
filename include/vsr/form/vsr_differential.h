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

#include "space/vsr_cga3D_op.h" //<-- assume cga3d for now
#include "vsr_graph.h"

namespace vsr { namespace cga {

//simplicial variable (2D facets in 3D space)
//we will generalize this . . .
struct Simplicial2 {
   
   Vec ea, eb;            ///< edges
   Biv tpss;              ///< tmp
   Biv pss;               ///< Tangent Pseudoscalar
   Vec ra, rb;            ///< reciprocal frames (a suitable basis for FEM)
   VSR_PRECISION area, la, lb;       ///< edge lengths
   
   Simplicial2(){}

   /// Constructor takes three coordinates,
   //  and makes edge vectors, their lengths, tangent pseudoscalar, area, reciprocal frames
   template<class T>
   Simplicial2(const T& a, const T& b, const T& c){
      ea = b-a; la = ea.norm();
      eb = c-a; lb = eb.norm();
      tpss=ea^eb;
      area = tpss.rnorm() * .5;
      pss = !(tpss);
      ra = (eb<=pss); ///<-- weight by length ? !
      rb = (-ea<=pss);
   }

   void print(){
      ea.print();
      eb.print();
   }

   /// Vector Derivative of some T-valued function defined on points
   //  finds difference along edges, dots that with edge vectors, 
   //  uses resultant scalar value to weigh reciprocals
   //  (sum these together?)
   template<class T>
   vector<Vec> derivative1(const T& n, const T& na, const T& nb){
     vector<Vec> v;
     //diff along edges
     auto dna = na - n;
     auto dnb = nb - n; 
     // coefficients
     auto wa = dna <= ea;      
     auto wb = dnb <= eb;
     // weighted reciprocals
     v.push_back(ra*wa);
     v.push_back(rb*wb);
     return v;
   }

   //exterior derivative / differential
   template<class T>
   Biv derivative(const T& n, const T& na, const T& nb){
     //diff along edges
     auto dna = na - n;
     auto dnb = nb - n; 
     // coefficients (project difference onto tangent space)
     auto wa = dna <= ea;      
     auto wb = dnb <= eb;
     // weighted reciprocals
     return ((ra*wa)^(rb*wb)) * area; //q: divide by area sums after
   }

   //wedged derivative / differential
   Biv derivative(const float& n, const float& na, const float& nb){
     //diff along edges
     auto dna = na - n;
     auto dnb = nb - n; 

     // coefficients
     auto wa = dna;// <= ea;      
     auto wb = dnb;// <= eb;
    // cout << wa << " " << wb << endl;
     // weighted reciprocals
     return ((ra*wa)^(rb*wb)) ; //q: divide by area sums after
   }
   
  
  // sum of weights across recipcrocals
   template<class T>
   Vec derivative2(const T& n, const T& na, const T& nb){
     //diff along edges
     auto dna = na - n;
     auto dnb = nb - n; 
     // coefficients
     auto aw1 = dna <= rb;      
     auto bw1 = dna <= ra;   
        
     auto aw2 = dnb <= rb;      
     auto bw2 = dnb <= ra;   
         
     // weighted reciprocals
     return ((ra*aw1)+(rb*bw1)) ; //q: divide by area? or after
   }   

   //Derivative of only edge A (for ccw integration around a node)
   template<class T>
   Vec derivativeA(const T& na, const T& nb){
     //diff along edge
     auto dna = nb - na;
     // coefficients of change in a direction
     auto wa = dna <= ea;      
     // weighted reciprocal
     return ra*wa ; //q: multiply recip by length or multiply by dot? or after
   }


   /// Vector Derivative of some T-valued function defined on points
   //  finds difference along edges, dots that with edge vectors, 
   //  uses resultant scalar value to weigh reciprocals
   //  SUMS these together 
   template<class T>
   Vec derivative0(const T& n, const T& na, const T& nb){
     //diff along edges
     auto dna = na - n;
     auto dnb = nb - n; 
     // coefficients (amt of change in ea and eb directions)
     auto wa = dna <= ea;      
     auto wb = dnb <= eb;
     // return sum of weighted reciprocals
     return ( (ra*wa)+(rb*wb) ); //q: divid by area? or after sum
   }

   Vec derivative0(const float& n, const float& na, const float& nb){
     //diff along edges
     auto dna = na - n;
     auto dnb = nb - n; 
     // coefficients
     auto wa = dna;// <= ea;      
     auto wb = dnb;// <= eb;
     // return sum of weighted reciprocals
     return ( (ra*wa)+(rb*wb) ); //q: divid by area? or after sum
   }


   /// Gradient of some T-valued function defined on points
   /// ignore ea and eb... 
   template<class T>
   Vec gradient(const T& n, const T& na, const T& nb) {
     //diff along edges
     auto dna = na - n;
     auto dnb = nb - n; 

     auto sa = dna <= (ra^(na-n));
     auto sb = dnb <= (rb^(nb-n));
     
     // return sum of weighted reciprocals
     return ( sa + sb ); //q: divid by area? or after sum
   }

   Vec gradient(const float& n, const float& na, const float& nb){
     //diff along edges
     auto dna = na - n;
     auto dnb = nb - n; 
     // return sum of weighted reciprocals
     return ( (ra*dna)+(rb*dnb) ); //q: divid by area? or after sum
   }

   /// Vector Derivative of some T-valued function defined on points
   //  finds direct difference along outer edge, wedges with gradient
   template<class T>
   Biv exterior_derivative(const T& n, const T& na, const T& nb){
     
     //diff along outer edge
     auto F = nb - na;
     auto deriv = derivative0(n,na,nb);

     return ( deriv ^ F );
    }


   /// Vector Derivative of some T-valued function defined on points
   //  finds direct difference along outer edge, wedges with gradient
   template<class T>
   auto full_derivative(const T& n, const T& na, const T& nb) -> decltype( Vec() * T() ){
  
     auto fa = na - n;
     auto fb = nb - n;
     
     auto ta = ra*fa;
     auto tb = rb*fb;

     return ( ta+tb );
    }


  float deficit(){
      return acos( (eb.unit()<=ea.unit())[0] );
  }

  
  Point center(){
    return Round::loc( Round::null(0,0,0) ^ Round::null(ea) ^ Round::null(eb));
  }

  //assumes s is next ccw, with shared edge eb
  Vec cotan( Simplicial2& s){
    Vec dual = s.center() - center();
    float wt = dual.norm()/lb;
    return eb * wt;
  }
    
   /*-----------------------------------------------------------------------------
    *  Questions: 
    *
    *  1. sum weighted bases or return separate?
    *  2. divide result by area?
    *  3. wedge around a node and sum . . . (exterior differential)
    *-----------------------------------------------------------------------------*/

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


} } //vsr::cga::

#endif   /* ----- #ifndef vsr_differential_INC  ----- */
