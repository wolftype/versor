CSS: style0.css

VERSOR COMPILATION and INSTALLATION Instructions
===

---

**NOTE:** 
_VERSOR_ is in active development!  `git pull` frequently for updates! And GMAIL me (wolftype)
or the list (versor@mat.ucsb.edu) with questions or problems.  

You can sign up for the list [here](#http://lists.create.ucsb.edu/mailman/listinfo/versor).

There is also a local list for my GA seminar [here](#http://lists.create.ucsb.edu/mailman/listinfo/spinor)

BUILD 
---
 
### Version 2.0 ###


	git clone git://github.com/wolftype/versor.git
	cd versor

to build the core mathematics library only run

	./build.sh

to build the graphics component, fetch the external graphics libraries

	git submodule init
	git submodule update
	./buildAll.sh 

This grabs a Graphics Library (gfx) submodule and GLV (a user-interface submodule). 

	
### Older Version 1.0 ###

	git clone git://github.com/wolftype/vsr.git
	cd vsr
	git submodule init
	git submodule update
	


	make vsr
	make examples/basics/xMeet.cpp
	
Please email me (wolftype at gmail) or the [list](#) if you have any issues!

---


If the above fails, make sure you have OpenGL, Glu, Glut, and GLEW installed. If not, see Getting GLU / GLUT / GLEW section below.
Or, email the [list](#http://lists.create.ucsb.edu/mailman/listinfo/versor). 

To use the built-in interface, gui, mouse and keyboard, etc, requires an input control listener set-up.
Bindings exist to two cross-platform windowing systems, [GLV](mat.ucsb.edu/glv/) and [AlloCore](), both made by my colleagues in the MAT program. 
It should be easy to make a binding to Open Frameworks ofxgui, for instance (also an MAT-Alum project) 

The easiest way to get started to is to use GLV.  GLV is great for quick and easy OpenGL-based interfaces.  
	
`libvsr.a` can also built in isolation with no dependencies except the standard library.  

	make vsr GFX=0

Most likely, however, you will want to be able to draw the elements to screen.

For a list of make options:

    make help

To start over:

	make clean

Feel free email me at wolftype@gmail.com with questions


Getting GLU / GLUT / GLEW UNIX / OSX:
---
###Linux (Ubuntu)

If you do not have glut and glew installed:

A) install glut:*
	sudo apt-get install freeglut3 freeglut3-dev

*note on newer versions of ubuntu (11.10 and greater) you may also have to do:
	sudo apt-get install binutils-gold

B) install glew:

On ubuntu, first do:

	sudo apt-get install libxmu-dev libxi-dev

Download tar or zip from http://glew.sourceforge.net/.  
cd into glew folder:

    cd glew
	sudo make install

or clone from current repos:

	git clone git://glew.git.sourceforge.net/gitroot/glew/glew
	cd glew
	make extensions
	sudo make install


Other Options
---
Email me with requests for ports, or to beg me to provide Windows support etc.  