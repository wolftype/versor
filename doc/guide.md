CSS: style0.css
<script type="text/javascript"
  src="https://c328740.ssl.cf1.rackcdn.com/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
</script>

Rot
---

 or Rotor
  3D rotation -- quaternionic.  Mapped to quaternions. 
  Generated through `Gen::rot(Biv b)`

Trs 
---   

Translator

#Mot or Motor
#Dil or Dilator
#Trv or Transversor

#Boost [bst]
  A general transversor, so not fixed at the origin.
  Typically fed a point pair

#Point [pnt]


Versor uses short and full names. In the guide that follows, the *or* keyword is used to indicate that both words are defined. 

Namespaces are used to organize functions based on their type.

[Gen](#gen) or Generator
[Ro](#ro) or Round
[Fl](#fl) or Flat

namespace: Gen or Generator

  desc: 
      Generates rotors from exponential of geometric elements
      Deduces geometric elements from logarithm of rotors

  use: Gen::rot(Biv b) or Generator::rotor(Bivector b)     

[Gen or Generators]
|                  |                         |                         |  
function           | input                   | output  
------------------ | ----------------------- | -----------------------  
motor or mot       | DualLine ([Dll](#dll))  | Motor ([Mot](#mot))  
rotor or rot       | Bivector  ([Biv](#biv)) | Rotor ([Rot](#rot))  
translator or trs  | Vector ([Vec](#vec))    | Translator ([Trs](#trs))  
dilator or dil     | float                   | Dilator ([Dil](#dil))  
transversor or trv | Tangent ([Tnv](#tnv))   | Transversor ([Trv](#trv))  
boost or bst       | Pair ([Par](#par))      | Boost ([Bst](#bst))  

                                                   

namespace: Round

  null
  point
  cir 

namespace: Flat

namespace: Tangent

namespace: Op

  dl    (or dual)
  udl   (or undual)
  dle   (or duale)
  udle  (or unduale)
  pj    (or project)
  rj    (or reject)


