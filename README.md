CSS: style0.css
	 
<script type="text/javascript"
  src="https://c328740.ssl.cf1.rackcdn.com/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
</script>
		 
<img src = "http://versor.mat.ucsb.edu/resources/images/cshape.gif" width = 80>
<img src = "http://versor.mat.ucsb.edu/resources/images/twine_0.gif" width = 100>
<img src = "http://versor.mat.ucsb.edu/resources/images/twist_04_bw.gif" width = 400>
<img src = "http://versor.mat.ucsb.edu/resources/images/lox_0.jpg" width = 100>

Versor (vsr)
===
A (Fast) C++ library for Euclidean and Conformal Geometric Algebra.  
---
### Currently tested on Linux and Mac OS X ###

[Homepage (versor.mat.ucsb.edu)](http://versor.mat.ucsb.edu) 

Versor is a C++ Library for Geometric Algebra with built-in draw routines. Version 2.0 generates optimized geometric algebra code at compile-time
through template meta-programming, and supports arbitrary dimensions and metrics (limited by your compiler...).

Developer: Pablo Colapinto  
`gmail: wolftype`  


## CONTENTS: ##
                               
* [Quickstart](#quickstart)   
* [Introduction](#introduction)
* [Generators](#generators) 
* [What the Point is](#whatthepointis)
* [Basics](#basics)
* [Gui](#gui)
* [Operators](#operators)
* [Elements](#elements)



[Download and Installation Instructions](http://versor.mat.ucsb.edu/INSTALL.html)           

[Reference Guide to the Elements](http://versor.mat.ucsb.edu/masters_appendix.pdf)

[Mailing List (for update notifications, to ask questions, discuss bugs, etc)](http://lists.create.ucsb.edu/mailman/listinfo/versor)  

[versor.js - a Javascript Port of Versor](http://github.com/weshoke/versor.js)                                                        

[My Master's Thesis on the Subject](http://wolftype.com/versor/colapinto_masters_final_02.pdf) 

[Look at the AlloSphere Research Group](http://www.allosphere.ucsb.edu/)          


	As long as algebra and geometry have been separated, their progress have been slow and their uses limited; but when these two sciences have been united, they have lent each mutual forces, and have marched together towards perfection.  
	-Joseph Louis Lagrange

	No attention should be paid to the fact that algebra and geometry are different in appearance.
	-Omar Khayyám

	L’algèbre n’est qu’une géométrie écrite; la géométrie n’est qu’une algèbre figurée.
	-Sophie Germain

	If you want to see, learn how to act
	-Heinz von Foerster 

## OTHER NICE SOFTWARE: ##

* [Cinderella](http://www.cinderella.de/tiki-index.php) Standalone GA interface
* [GAViewer](http://www.science.uva.nl/ga/viewer/content_viewer.html) Standalone GA interface
* [Gaigen](http://staff.science.uva.nl/~fontijne/g25.html) An Implementation Generator
* [CluCalc/CluViz](http://www.clucalc.info/) Standalone and Library
* [Gaalop](http://www.gaalop.de/) A precompiler to optimize CluScripts
* [Gaalet](http://sourceforge.net/apps/trac/gaalet/) An expression template library whose backend is somewhat similar to Versor's
* [versor.js](http://www.github.com/weshoke/versor.js) A javascript port of this library


QUICKSTART
---

Please see also the [INSTALL](http://versor.mat.ucsb.edu/INSTALL.html) guide.  For version 2.0 you need C++11 support (gcc 4.7 or higher or clang 3.2 or higher)

	git clone git://github.com/wolftype/vsr2.0.git
	cd vsr2.0
	git submodule init
	git submodule update

To test a graphics example

	make examples/xBasic.cpp 
     
which both builds and runs the file.  

Not Working?
---                 

- For Versor 2.0 You'll need C++11 support (See makefile notes below). 
- Alternatively Versor 1.0 is available at [github.com/wolftype/vsr.git](github.com/wolftype/vsr.git)

For C++11 you'll want clang 3.2 (mac) or above or gcc 4.7 or above (linux).  
NOT CURRENTLY TESTED ON WINDOWS.   

For clang on a snow leopard (with thanks to Karl Yerkes for this tidbit)

	brew tap home-brew/versions
	brew install --HEAD llvm34 --rtti, --disable-assertions, --with-libcxx, --with-clang
 
If you don't want to or can't compile C++11 code try an [older flavor of vsr](github.com/wolftype/vsr.git). 
This older version runs just as fast, but is strictly 3D CGA (i.e. R4,1 metric) since I generated headers ahead of time.

####MAKEFILE FLAGS

1. CLANG=path/to/clang++/ (default usr/local/bin/clang++)
    The makefile assumes clang is at usr/local/bin/clang++ -- if you want to change that set this flag  

2. GCC=1  (default 0)
    If you want to build with GCC instead set GCC=1.

3. RPI=1 (default 0)
     vsr also builds on the Raspberry Pi ! ( with a cross-compiler ) (GCC=1 RPI=1)
    (note you cannot build from the pi itself, you must use a cross-compiler)

3. GFX=0 (default 1) 
    if you want to build without graphics support you can set GFX=0
      

INTRODUCTION
---

_Versor_ provides operations and draw routines for Euclidean and Conformal Geometric Algebras, 
a relatively new spatial computing model used by physicists, engineers, and artists. _Versor_ is designed to make graphical 
experimentation of geometric algebra within a C++ environment easier. You can use this library to draw geometrical things, explore 
spherical and hyperbolic spaces, transformations, design robots, etc. 
I am using it for my PhD on bio-inspired engineering.

I first developed _Versor_ while reading "Geometric Algebra for Computer Science" by Leo Dorst, Daniel Fontijne, and Stephen Mann. 
It's a fantastic book and if you're reading this you should also consider reading that.  

Built to aid in my modelling of organic forms, the initial development was funded in large part by the Olivia Long Converse Fellowship for Botanic research, 
courtesy of the Graduate Division at the University of California in Santa Barbara.  Currently supported by the Robert W. Deutsch Foundation,
this software is under a UC Regents General Public License.  Feel free to use and distribute as long as copyrights and credits
are maintained.      

---

One quick word: clifford algebras and the spatial relationships they embody can often feel abstract and daunting.  But it's a twisty, boosty ride, full of weird discoveries.  You're bound to make some, so have fun!

---

      
####BACKGROUND
The homogenous 5D CGA model used here was initially proposed by David Hestenes, Hongbo Li, and Alan Rockwood in 2001, and given full form and weight through the excellent 
and careful work of Leo Dorst, Joan and Anthony Lasenby, and Eduardo Bayro-Corrochano, and others.  These researchers' writings have helped me quite a bit.  CGA is particular breed of _Clifford Algebras_ (also known as Geometric Algebras), 
which operate upon combinatoric hypercomplex vector spaces that emerged from William Clifford's attempt to fuse Hamilton's quaternions with Grassmans' extension algebras.  Thus 
_transformations_ were married with a system of _abstraction_.  For more information, take a look at the [links](#links) to the sites at the bottom of 
this page.  For instance, for practical applications in robotics and "Geometric Cybernetics", see Eduardo Bayro-Corrochano's work.  For some
very helpful algorithms in rigid body dynamics and gravitational physics see the variety of publications by Joan and Anthony Lasenby.  To get at the beginning of it all, read David Hestenes' _New Foundations for Classical Mechanics_. 

####LICENSE
This software is licensed under a general UC Regents General Public License.  If you're planning on using CGA inside a sellable product you should be aware that 
there is a vague patent on the use of 5D CGA which _may_ limit its _commercial_ use when encoding robotic control mechanisms, or may just limit your ability to patent
the model itself.  I hope and imagine it is the latter.  Though powerful, elegant, and brilliant, the heart of CGA is just a quadratic equation and the arguments for 
the use of 5D CGA are that it is _foundational_ and _universal_, the very two characteristics of a system which would make it un-patentable.  The Clifford Algebras on which it is based are from the 19th century.

####SPEED
Typical matrix operation libraries have templated inlined functions for Vector and Matrix multiplication.  Versor
is similar, but on steroids, where _vectors_ and sparse _matrices_ of various sizes are all just called _multivectors_ and represent geometric
elements beyond just xyz directions and transformation matrices. Circles, lines, spheres, planes, points are all algebraic elements, as are 
operators that spin, twist, dilate, and bend those variables.  Both these elements and operators are _multivectors_ which multiply together in many many many different ways.  


####What's new?

Version 2.0 compiles much faster than before, and without any silly predetermined list
of allowable operations or types.  Most notably, arbitrary metrics are now possible.  For example, 
the xRoots.cpp example calculates all the Euclidean 4D reflections of a couple of point groups
(F4 and D4, namely). So you can hypercube and polychoron away (8D cubes no problem!).  Number of dimensions
allowed are somewhat limited by your compiler infrastructure -- let me know if you have a need that is not being met!

As for CGA, all the Pnt, Vec, Dll notation remains as before, but i've started adding utility functions
since it helps people out. 

  auto pa = Ro::point( 1,0,0 ); 
  auto pb = Ro::point( 0,1,0 ); 
  auto pc = Ro::point(-1,0,0 ); 
  auto circle = pa ^ pb ^ pc;
  
  Draw(c); 
  
####How does it work?

If you like mind-numbing functional template metaprogramming, take a look at the code
and please let me know what you think.  If you don't, then I wouldn't . . .  But if you have ideas or questions please do not hesitate
to contact me.   

WHAT THE POINT IS 
---

GA combines many other maths (matrix, tensor, vector, and lie algebras). It is **holistic**. CGA uses a particular mapping (a conformal one) of 3D Euclidean space to a 
4D sphere. Operations on that hypersphere are then projected back down to 3D. That how it works in a nutshell. 

A fuller treatment of this question (er, the question of why we do this) can be found in my [Master's thesis on the subject](http://wolftype.com/versor/colapinto_masters_final_02.pdf).  But basically,
Geometic Algebra offers a particular richness of spatial expression.  Imagine needing glasses and not knowing you needed glasses.  Then, when you do get glasses, the world changes
unexpectedly.  GA is like glasses for the inside of your brain.  _Conformal_ Geometric Algebra, especially the 5D variety enlisted here, are like x-ray glasses.  One 
point of clarification that occurs are **disambiguations** of previously collapsed concepts.  

For instance, the main disambiguation, is that between a _Point_ in space and a _Vector_ in space.  
A Point has no magnitude, but a Vector does.  A Point has no direction, but a Vector does. Points are _null_ Vectors.  We can make them
by writing

	Vec( 1,0,0 ).null();

* Points are null Vectors
* Points square to 0
* The dot (inner) product of two Points returns their squared distance
* The wedge (outer) product of two Points returns a Point Pair

More on that last point later . . . there are various binary operators defined (mainly three).  We can introduce one right now, which is the **dot** or **inner** product.
In mathematics, the inner product of two points `pa` and `pb` is written \\(p_{a} \rfloor p_{b}\\).  In _Versor_ we use the `<=` operator:

	Point pa = Vec(1,0,0).null();
	Point pb = Vec(-1,0,0).null();
	Scalar squaredDist = ( pa <= pb ) * -2;
	
which in this case would return a Scalar value of `4`.  The `-2` is there since the inner product really returns **half the negative** squared distance.
We can extract the Scalar into a c++ double like so:

	double squaredDist = ( pa <= pb )[0] * -2;

Points thought of as Spheres (really, Dual Spheres, more on _Duality_ later): they are Spheres of zero radius.  As such they are a type of _Round_ element.  We can also build points this way:

	Round::null( 1,0,0 );

or you can pass in another element

	Round::null( Vec(1,0,0) );

or use the built-in method

	Point pa = Vec(1,0,0).null();

Points can also be made with the macro `PT`

	Point pa = PT(1,0,0);

which is just "syntactic sugar" for `Vec(1,0,0).null()`

Speaking of Spheres, we can also make spheres with a radius this way:

	DualSphere dls = Round::dls( Vec( 1,0,0 ).null(), 1 );

or  

	DualSphere dls = Round::dls( Vec( 1,0,0 ), 1 );
	
or, specifying the radius first and then the coordinate:  

	DualSphere dls = Round::dls( 1 /* <--radius */ , 1,0,0 )
	
all of which give a dual sphere of radius 1 at coordinate 1,0,0; 


BASICS
---

_Versor_ is named after the one of the basic category of elements of geometric algebra.  
A **versor** is a type of **multivector** which can be used to compose geometric transformations, 
namely reflections, translations, rotations, twists, dilations, and transversions (special conformal transformations).

More on all of those transformations later.  

In Versor, a `Vector` (or `Vec`) is a typical Euclidean 3D element.  It can be built in the normal way:

	Vec v(1,2,3);

Some built-in Vectors exist:
	
	Vec::x x; //<-- X Direction Unit Vector Vec(1,0,0)
	Vec::y y; //<-- Y Direction Unit Vector Vec(0,1,0)
	Vec::z z; //<-- Z Direction Unit Vector Vec(0,0,1)

A `Vector` can be spun around using a `Rotor`, which is exactly like a quaternion.  However, whereas quaternions are often built by specifying an axis
and an angle, rotors are built by specifying the **plane** of rotation.  Eventually this will make much more sense to you: in general **planes** are what we
will be using to transform things.  For instance, a reflection is a reflection in a plane.  As we will see, planes can become **hyperplanes** which will allow for more extraordinary transformations.

The first completely new element to introduce is the `Bivector`, which is the plane we will use to generate our `Rotor`.  Bivectors represent **directed areas** and are __dual__
to the cross product: the cross product of two vectors in typical vector algebra returns a vector normal to the plane they define.  So it is not completely new,
but just sort of new.

Bivectors are also just three elements long, and are built the same way Vectors are.

	Biv b(1,2,3);

Some built-in Bivectors exist:
	
	Biv::xy xy; //<-- XY Counterclockwise Unit Area Biv(1,0,0)
	Biv::xz xy; //<-- XZ Counterclockwise Unit Area Biv(0,1,0)
	Biv::yz xy; //<-- YZ Counterclockwise Unit Area Biv(0,0,1)
	
While it is perfectly valid to write `Vector`, `Bivector` and `Rotor`, you'll notice I've truncated them to their three letter nicknames, `Vec` and `Rot`.  
That's up to you: Both long-name and nick-name versions are valid in libvsr (they are typedef'ed to each other).

	Biv b = Biv::xy;
	double theta = PIOVERTWO;
	Vec v1 = Vec::x.rot( b * theta )
	
You can also generate rotors using `Gen::rot( <some bivector> )`  In fact, all transformations can be generated this way, and then later applied to arbitrary elements.
For instance, `Motors` can be generated which translate and rotate an element at the same time.  This is also called a _twist_.

	Motor m = Gen::mot(<some dual line>); 	//<-- Makes A Twisting Motor around Some Dual Line
	Point p = Vec(0,0,0).null().sp(m);		//<-- Applies above motor to a Point
		
You'll notice there are _dual_ versions of elements: as in a `DualLine` (or `Dll` for short).  That's because in the real world of abstract geometry, there are usually
two ways of defining an element.  For instance, we can build a _direct_ `Line` on the Y-axis by wedging two points together, along with infinity:
	
	Line lin = Vec(0,0,0).null() ^ Vec(0,1,0).null() ^ Inf(1);
	
Or we can define a line by the bivector plane that it is normal to, and a support vector that determines how far away the line is from the origin.  To convert the above
line into its dual representation, we just call the dual() method:
	
	Dll dll = lin.dual();
	
For those who are interested, this dual representation is isomorphic to the Plücker coordinates, which are used in screw theory to twist things around.  Here, too, we can use
dual lines to generate transformations which twist things around them.


Gui
---

The examples/*.cpp files include bindings to the GLV framework for windowing and user interface controls.  
A GLVApp class and GLVInterface class provide the necessary glue to get started quickly.
    
The interface has a built in gui, mouse info, and keyboard info stored.  
    
	static Circle circle;
	Touch(inteface, circle);
	Draw(circle);
	
Putting the above code inside your application's `onDraw()` loop will enable you to click and modify geometric elements by hitting the "G", "R" and "S" keys.  
Hit any other key to deselect all elements.   


[**BUILT-IN INTERFACE**]
|                                 |                                             | 
Key                                 | Response  
------------------------------      | ------------------------------------------  
`~`                                 | Toggle full screen.  
`SHIFT` + `Mouse` or `Arrow Keys`   | Translate the camera  in x and z directions.  
`CTRL`+ `Mouse` or `Arrow Keys` 	| Rotate the camera  
`ALT` +`Arrow Keys`              	| Rotate the model view around.  
`G`                                 | Grab an Element  
`R`                                 | Rotate an Element  
`S`                                 | Scale an Element 
Any other key						| Release all Elements 
 


OPERATORS
---

The elements of the algebra are geometric entities (circles, planes, spheres, etc) and operators (rotations, translations, twists, etc) which 
act on the elements of the algebra.  All are known as _multivectors_ since they are more than just your typical vectors.

Multivector elements are most often combined using three overloaded binary operators: 

The **Geometric** Product of elements `A` and `B`:  

	A * B

multiplies two multivector elements together.  This is most useful when multiplying one by the inverse of another (see `!` operator, below).

The **Outer** Product of elements `A` and `B`:  

	A ^ B

"wedges" two multivectors together.  Its from Grassman's algebra of extensions, and can be thought of as a way of creating higher dimensions from smaller ones.
For instance, wedging two `Vectors` (directed magnitudes) together returns a `Bivector` (a directed Area).  Wedging two `Points` together returns a `PointPair`.
Wedging three `Points` together returns a `Circle`.

The **Inner** Product of elements `A` and `B`:

	A<=B


There is also a **Commutator** product (differential)

	A%B

And a few overloaded operations, including,

The Inverse:  

	!A

returns \\(A^{-1}\\)

The Reverse:  

	~A

returns \\(\tilde{A}\\)


And finally, since I ran out of overloadable operators, some basic methods

	A.conj() 

which returns \\(\bar{A}\\)

	A.inv()  

which returns \\(\hat{A}\\)

In summary:  

| Versor          | Math                                              |                                         Description                                        |   |  
----------------- | ------------------------------------------------- | :----------------------------------------------------------------------------------------: | -  
`A * B`           | \\(AB\\)                                          | Multiplies two elements together (and, in the case of A * !B finds ratios between elements).  
`A ^ B`           | \\(A \wedge B\\)                                  |            Wedges two elements together (builds up higher dimensional elements).  
`A <= B`          | \\(A \rfloor B\\) or \\(\boldsymbol{a} \cdot B\\) |                 Contracts A out of B (returns the part of B "least like A", sort of).  
`A % B`           | \\(A \times B\\)                                  |                        Commutator, equal to \\(\frac{1}{2}(AB-BA)\\)  
`!A`              | \\(A^{-1}\\)                                      |                                       The Inverse of A.  
`~A`              | \\(\tilde{A}\\)                                   |                                       The Reverse of A.  
`A.conj()` | \\(\bar{A}\\)                                     |                                         Conjugation.  
`A.inv()`  | \\(\hat{A}\\)                                     |                                          Involution.  


ELEMENTS
---

To make the process of writing code faster, all elements of the algebra are represented by types 3 letters long. 
Alternatively, you can also use the long-form name.

[**BASIC ELEMENTS**]				  
Type  |  Long Form   		     |     Descrription                                                        |  
----- | ---------------- | :---------------------------------------------------------: |  
_Euclidean_             ||  
`Sca` | `Scalar`         |                         A real number  
`Vec` | `Vector`         |  A Directed Magnitude, or 3D Vector, typical cartesian stuff  
`Biv` | `Bivector`       | A Directed Area. Use them to make Rotors: `Gen::Rot( Biv b )`  
`Tri` | `Trivector`      |                   A Directed Volume Element  

_Round_                 ||  
`Pnt` | `Point`          |           A Null Vector: `Pnt a = Vec(1,0,0).null()`  
`Par` | `PointPair`      |    A 0-Sphere (Sphere on a Line): `Par par = Pnt a ^ Pnt b`  
`Cir` | `Circle`         |         A 1-Sphere: `Cir cir = Pnt a ^ Pnt b ^ Pnt c`  
`Sph` | `Sphere`         |     A 2-Sphere: `Sph sph = Pnt a ^ Pnt b ^ Pnt c ^ Pnt d`  
`Dls` | `DualSphere`     |            Typedef'ed as a point: `typedef Pnt Dls`  

_Flat_                  ||  
`Lin` | `Line`           |        A Direct Line: e.g. `Lin lin = Par par ^ Inf(1)`  
`Dll` | `DualLine`       |            A Dual Line: e.g. `Dll dll = lin.dual()`  
`Pln` | `Plane`          |       A Direct Plane: e.g. `Pln pln = Cir cir ^ Inf(1)`  
`Dlp` | `DualPlane`      |                A Dual Plane: e.g. `Dlp dlp = `  
`Flp` | `FlatPoint`      |  

_Versors_               ||  
`Rot` | `Rotor`          |            Spins an Element (as a Quaternion would)  
`Trs` | `Translator`     |                     Translates an Element 
`Dil` | `Dilator`        |                       Dilates an Element  
`Mot` | `Motor`          |                Twists an Element along an axis  
`Trv` | `Transversor`    |                        Bends an Element about the Origin
`Bst` | `Booster`        |                        Bends an Element around an "Orbit"

_Abstract_              ||  
`Mnk` | `MinkowskiPlane` |  
`Pss` | `Pseudoscalar`   |  
`Inf` | `Infinity`       |  


There are others as well (for instance, affine planes, lines, and points) but the above are more than sufficient to start with. 
There are also built in macros, for instance  

`EP`  			| Sphere At the Origin.   
`EM`  			| Imaginary Sphere at the Origin.    
`PT(x,y,z)`  	| A null Point at x,y,z

`EP` and `EM` can be invoked instead of `Inf` to work in non-Euclidean metrics ( Spherical and Hyperbolic, respectively)

Many Euclidean elements can be drawn by invoking Draw::Render(<element>).  Some can't (yet) either because it wasn't obvious
how to draw them (e.g the scalar) or because I just didn't figure out how to do it or because I forgot or was lazy.  If you
want something to be drawable, let me know and I'll add it in.  Or try adding it in yourself and send a pull request via github.

All elements can be dualized by invoking their `dual()` method  

All elements can be reflected over spinors with the `sp(<spinor>)` method  

All elements can be reflected over versors with the `re(<versor>)` method  

The versors are constructed by the geometric entities, typically by using the `Gen::` routines.  Operators can also be acted on by operators -- you can rotate a translation, or twist a boost.



COMMON CONFORMAL FUNCTIONS
---

`vsr_generic_op.h` and `vsr_cga3D_op.h` contain the bulk of the functions for generating elements from other elements.  Some guidelines:

* `Gen::` methods **generate** or otherwise operate on versors
* `Ro::` methods create or otherwise operate on **Round** elements (Points, Point Pairs, Circles, Spheres)
* `Fl::` methods create or otherwise operate on **Flat** elements (Lines, Dual Lines, Planes, Dual Planes, or Flat Points)
* `Ta::` methods create or otherwise operate on **Tangent** elements (Tangent Vectors, Tangent Bivectors, Tangent Trivectors)


GENERATORS 
---
  
|       |                                     |  
Returns | Function                            | Description  
------- | ----------------------------------- | -------------------------------------------------  
Rot     | Gen::rot( const Biv& b );           | //<-- Generate a Rotor from a Bivector  
Trs     | Gen::trs( const Drv& v);            | //<-- Generate a Translator from a Direction Vector  
Mot     | Gen::mot( const Dll& d);            | //<-- Generate a Motor from a Dual Line  
Dil     | Gen::dil( const Pnt& p, double amt );| //<-- Generate a Dilator from a Point and an amount  
Trv     | Gen::trv( cont Tnv& v);             | //<-- Generate a Transveror from a Tangent Vector  
Bst     | Gen::bst( const Par& p);            | //<-- Generate a Booster from a Point Pair  

   
REFLECTIONS
---

In addition to the above "even" spinors, we can also reflect.  Reflections (in a sphere, circle, or point pair, or over a line or plane ) can be calculated by writing

	Pnt p = PT(1,0,0);
	Pnt r = p.re( CXY(1) ); //Reflection of a point in a circle
	r = r / r[3]; 			//Renormalization of a point

The re() method calculates `v.re(C)` as `C*v.inv()*~C`.  With a versor `C` and an element `v` you might also try `C * v * !C`.  Inversion in a circle or a sphere may change the 
weight of the element (for at Point at x, it will change it by x^2)        


LINKS
---

* [Some Video Demos of Versor](http://vimeo.com/wolftype)
* [The Good Book: _Geometric Algebra for Computer Science_](http://www.geometricalgebra.net/)
* [GA Bookmarks on Delicious](http://www.delicious.com/tag/geometricalgebra)
* [GA Google Group](https://groups.google.com/forum/?fromgroups#!forum/geometric_algebra)
* [David Hestenes' Geometric Calculus Page](http://geocalc.clas.asu.edu/)
* [University of Amsterdam Intelligent Systems Lab](http://www.science.uva.nl/research/isla/)
* [Eduardo Bayro-Corrochano's Robotics Lab](http://www.gdl.cinvestav.mx/~edb/)
* [Cambridge University Geometric Algebra Research Group](http://www.mrao.cam.ac.uk/~clifford/)
* [Cognitive Systems at Christian-Albrechts-Universität zu Kiel](http://www.mrao.cam.ac.uk/~clifford/)

PAPERS
---

* 2011 [Versor: Spatial Computing With Conformal Geometric Algebra](http://wolftype.com/versor/colapinto_masters_final_02.pdf)
* 2012 [Boosted Surfaces: Synthesis of Meshes using Point Pair Generators in the Conformal Model](http://versor.mat.ucsb.edu/Boosted_Surfaces_submission_0113.pdf)


                                                                       
