#ifndef VSR_CGA3D_TYPES_INCLUDED
#define VSR_CGA3D_TYPES_INCLUDED

#include "detail/vsr_mv.h"

namespace vsr{ namespace cga{ // namespace types{


using Sca = NSca<5>;
using Vec = NVec<5>;
using Biv = NBiv<5>;
using Rot = NRot<5>;
using Tri = NTri<5>;

using Ori = NOri<5>;
using Inf = NInf<5>;
using Mnk = NMnk<5>;
using Pss = NPss<5>;

using Pnt = NPnt<5>;
using Par = NPar<5>;
using Cir = NCir<5>;
using Sph = NSph<5>;
using Dls = NDls<5>;

using Flp = NFlp<5>;
using Dll = NDll<5>;
using Lin = NLin<5>;
using Dlp = NDlp<5>;
using Pln = NPln<5>;

using Drv = NDrv<5>;
using Tnv = NTnv<5>;
using Drb = NDrb<5>;
using Tnb = NTnb<5>;
using Drt = NDrt<5>;
using Tnt = NTnt<5>;

using Trs = NTrs<5>;
using Mot = NMot<5>;
using Trv = NTrv<5>;
using Bst = NBst<5>;
using Con = NCon<5>; 
using Dil = NDil<5>;
using Tsd = NTsd<5>;


//FULL NAMES
typedef  Sca Scalar;
typedef  Biv Bivector;
typedef  Tri Trivector ;
typedef  Rot Rotor;
typedef  Ori Origin;
typedef  Inf Infinity;
typedef  Mnk Minkowski;
typedef  Pss Pseudoscalar;
typedef  Pnt Point;
typedef  Par Pair;
typedef  Cir Circle;
typedef  Sph Sphere;
typedef  Pnt DualSphere;
typedef  Drv DirectionVector;
typedef  Drb DirectionBivector;
typedef  Drt DirectionTrivector;
typedef  Tnv TangentVector;
typedef  Tnb TangentBivector;
typedef  Tnt TangentTrivector;
typedef  Dll DualLine;
typedef  Lin Line;
typedef  Flp FlatPoint;
typedef  Pln Plane;
typedef  Dlp DualPlane;
typedef  Trs Translator;
typedef  Mot Motor;
typedef  Trv Transversor;
typedef  Bst Boost;
typedef  Dil Dilator;
typedef  Tsd TranslatedDilator;


/* extern template Pnt Pnt::sp( const Rot& )const; */                         
/* extern template Pnt Pnt::sp( const Trs& )const; */                         
/* extern template Pnt Pnt::sp( const Mot& )const; */                       
/* extern template Pnt Pnt::sp( const Bst& )const; */
/* extern template Pnt Pnt::sp( const Tsd& )const; */
/* extern template Pnt Pnt::sp( const Dil& )const; */
                                                         
/* extern template Par Par::sp( const Rot& )const; */
/* extern template Par Par::sp( const Trs& )const; */
/* extern template Par Par::sp( const Mot& )const; */
/* extern template Par Par::sp( const Bst& )const; */
/* extern template Par Par::sp( const Tsd& )const; */
/* extern template Par Par::sp( const Dil& )const; */
                                                       
/* extern template Cir Cir::sp( const Rot& )const; */
/* extern template Cir Cir::sp( const Trs& )const; */
/* extern template Cir Cir::sp( const Mot& )const; */
/* extern template Cir Cir::sp( const Bst& )const; */
/* extern template Cir Cir::sp( const Tsd& )const; */
/* extern template Cir Cir::sp( const Dil& )const; */

} } // } //vsr::cga::types::




#endif
