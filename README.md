CSS: style0.css
<script type="text/javascript"
  src="https://c328740.ssl.cf1.rackcdn.com/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
</script>

Versor (vsr) Version 2.0
===
A (Fast) C++ library for Euclidean and Conformal Geometric Algebra.  
---
### Currently tested on Linux and Mac OS X ###

[Homepage (versor.mat.ucsb.edu)](http://versor.mat.ucsb.edu) 

Versor is a C++ Library for Geometric Algebra. Version 2.0 generates optimized geometric algebra code at compile-time
through template meta-programming, and supports arbitrary dimensions and metrics (limited by your compiler...).  

Developer: Pablo Colapinto  
`gmail: wolftype`


## CONTENTS: ##

* [Quickstart](#quickstart)                        
* [Introduction](#introduction)
* [Generators](#generators) 
* [Build Notes](#build)


[Reference Guide to the Elements](http://versor.mat.ucsb.edu/masters_appendix.pdf)

[Mailing List (for update notifications, to ask questions, discuss bugs, etc)](http://lists.create.ucsb.edu/mailman/listinfo/versor)  

[My Master's Thesis on the Subject](http://wolftype.com/versor/colapinto_masters_final_02.pdf)

[versor.js - a Javascript Port of Versor](http://github.com/weshoke/versor.js)   
      

Quickstart
---

You need C++11 support (gcc 4.7 or higher or clang 3.2 or higher)

  git clone git://github.com/wolftype/vsr2.0.git
  cd vsr2.0
  git submodule init
  git submodule update

To test a graphics example

  make tests/xBasics.cpp 
     
which both builds and runs the file.  


[**BUILT-IN INTERFACE**]
|                                 |                                             | 
Key                                 | Response  
------------------------------      | ------------------------------------------  
`~`                                 | Toggle full screen.  
`SHIFT` + `Mouse` or `Arrow Keys`   | translates the camera  in x and z directions.  
`CTRL`+ `Mouse` or `Arrow Keys` | rotates the camera  
`ALT` +`Arrow Keys`              | spins the model view around.  
`G`                                 | Grab an Element  
`R`                                 | Rotate an Element  
`S`                                 | Scale an Element  
 




INTRODUCTION
---

This package provides operations and draw routines for conformal geometric algebra, 
a relatively new spatial computing model used by physicists, engineers, and artists. _Versor_ is designed to make graphical 
experimentation of geometric algebra within a C++ environment easier. 
You can use this library to draw geometrical things, explore spherical and hyperbolic spaces, transformations, design robots, etc. 
I am using it for my PhD on bio-inspired engineering.

I first developed _Versor_ while reading "Geometric Algebra for Computer Science" by Leo Dorst, Daniel Fontijne, and Stephen Mann. 
It's a fantastic book and if you're reading this you should also consider reading that.  

Built to aid in my modelling of organic forms, the initial development was funded in large part by the Olivia Long Converse Fellowship for Botanic research, 
courtesy of the Graduate Division at the University of California in Santa Barbara.  Currently supported by the Deutsch Foundation,
this software is under a UC Regents General Public License.  Feel free to use and distribute as long as copyrights and credits
are maintained. 

Homepage at versor.mat.ucsb.edu
                                                                      
To build with the 5D Conformal Model

  #include "vsr_cga3D.h"

To bind with glut, GLV interface, and draw to screen
                             
  #include "vsr_cga3D_draw.h"

To bind with Glut, GLV interface

  #include "vsr_GLVimpl.h"                           

To inferface with elements using "G", "R", "S" keys to Grab, Rotate, or Scale:

  #include "vsr_cga3D_interface.h"
  
   
What's new?
--- 

This compiles much faster than before, and without any silly predetermined list
of allowable operations or types.  Most notably, arbitrary metrics are now possible.  For example, 
the xRoots.cpp example calculates all the Euclidean 4D reflections of a couple of point groups
(F4 and D4, namely). So you can hypercube and polychoron away (8D cubes no problem!).  

As for CGA, all the Pnt, Vec, Dll notation remains as before, but i've started adding utility functions
since it helps people out. 

  auto pa = Point( 1,0,0 ); 
  auto pb = Point( 0,1,0 ); 
  auto pc = Point(-1,0,0 ); 
  auto c = Circle(pa, pb, pc);
  
  Draw(c); 
  
How does it work?
---

If you like mind-numbing functional template metaprogramming, take a look at the code
and please let me know what you think.  If you don't, then I wouldn't . . .  But if you have ideas or questions please do not hesitate
to contact me.

METHODS
---

`vsr_op.h` contains the bulk of the functions for generating elements from other elements.  Some guidelines:

* `Gen::` methods **generate** or otherwise operate on versors
* `Ro::` methods create or otherwise operate on **Round** elements (Points, Point Pairs, Circles, Spheres)
* `Fl::` methods create or otherwise operate on **Flat** elements (Lines, Dual Lines, Planes, Dual Planes, or Flat Points)
* `Ta::` methods create or otherwise operate on **Tangent** elements (Tangent Vectors, Tangent Bivectors, Tangent Trivectors)


GENERATORS 
---
          
  Rot Gen::rot( const Biv& b );                 //<-- Generate a Rotor from a Bivector
  Trs Gen::trs( const Drv& v);                  //<-- Generate a Translator from a Direction Vector
  Mot Gen::mot( const Dll& d);                  //<-- Generate a Motor from a Dual Line
  Dil Gen::dil( const Pnt& p, double amt );     //<-- Generate a Dilator from a Point and an amount
  Trv Gen::trv( cont Tnv& v);                   //<-- Generate a Transveror from a Tangent Vector
  Bst Gen::bst( const Par& p);                  //<-- Generate a Booster from a Point Pair
   

BUILD
---                 

- For Versor 2.0 You'll need C++11 support (See important makefile notes below). 
- Alternatively Versor 1.0 is available at github.com/wolftype/vsr.git

For C++11 you'll want clang 3.2 (mac) or above or gcc 4.7 or above (linux).  
NOT tested on windows.

	brew tap home-brew/versions
	brew install --HEAD llvm34 --rtti, --disable-assertions, --with-libcxx, --with-clang
 
If you don't want to or can't compile C++11 code try an [older flavor of vsr](github.com/wolftype/vsr.git). 
This older version runs just as fast, but is strictly 3D CGA (i.e. R4,1 metric) since I generated headers ahead of time.

IMPORTANT NOTE ON MAKEFILE FLAGS

1. CLANG=path/to/clang/ (default usr/local/bin)
    The makefile assumes clang is at usr/local/bin/ -- if you want to change that set this flag  

2. GCC=1  (default 0)
    If you want to build with GCC set GCC=1.

3. RPI=1 (default 0)
        vsr also builds on the Raspberry Pi with a cross-compiler (GCC=1 RPI=1)
    (note you cannot build from the pi itself, you must use a cross-compiler)

3. GFX=0 (default 1) 
    if you want to build without graphics support you can set GFX=0
                                                                       
