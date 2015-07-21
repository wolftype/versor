Versor (libvsr)	{#mainpage}
===
###version 3.0

A (fast) C++ template library for Euclidean, conformal, and arbitrary geometric algebras.  
---
### Currently tested on Linux and Mac OS X ###

Developer: Pablo Colapinto  

`gmail: wolftype`


[Homepage (versor.mat.ucsb.edu)](http://versor.mat.ucsb.edu) 

[Bibtex (for referencing this work)](http://versor.mat.ucsb.edu/bibtex.txt)

Versor is a C++ Library for **Geometric Algebra**, sometimes called **Clifford Algebra**, 
a mathematical system for encoding geometric concepts numerically.

@tableofcontents

@section Reference

Though versor is designed to support any geometric algebra, 
this document focuses on @ref cga. Click on the Modules tab at the top of this doxygen-generated documentation to get started.

@section Features
  
  - \e Generic
  
      The headers generate optimized code at compile-time through template metaprogramming.  
  
  - \e Lightweight
  
      The core of the library is under 150kb, and supports arbitrary dimensions and metrics (limited by your compiler...). 
  
  - \e Fast
  
      All combinatorics are sorted out by the compiler.
  
  - \e Visualized
      
      Most geometric algebra types can be drawn in either OpenGL or OpenGLES.
  
  - \e Horizontal
  
      The Math library is completely independant of the Rendering library.
  
  - \e Expressive
  
      Many key algorithms are included 
  
  - \e Documented, in Active Development, and open source
  
      Working on this always, please send feedback. 

@section Use

The library can be used for example as
  
  - header-only to operate in your preferred metric (e.g. Conformalized \\(\\mathbb{R}^{4,2}\\) )
  - a static or shared library with prebuilt @ref cga routines
  - a stand-alone application with built-in graphics for computer or mobile  

