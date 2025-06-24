File Descriptions
==================

This file provides descriptions of the example files in the versor library, organized by category. Each example demonstrates specific aspects of conformal geometric algebra and its applications.

Main Examples (examples/)
========================

xBasic.cpp
----------
Basic conformal geometric algebra operations including sphere-plane intersections, circle construction through points, geometric meets, and simple rotations. Good starting point for learning CGA fundamentals.

xAlgebra.cpp  
------------
Comprehensive algebra operations demonstration including multivector operations, duality, meet/join operations, and basic geometric constructions.

xBoost.cpp
----------
Boost transformations and their applications to geometric objects. Demonstrates conformal transformations that preserve angles and map circles to circles.

xFrame.cpp
----------
Frame and motor operations for 3D transformations. Shows how to work with coordinate frames and apply transformations.

xMeet.cpp
---------
Intersection operations between geometric objects like planes, spheres, and circles using the meet operator.

xMotor.cpp
----------
Motor algebra for rigid body transformations combining rotations and translations in a single operation.

xNonEuclidean.cpp
-----------------
Non-Euclidean geometry examples demonstrating hyperbolic and spherical geometry constructions.

xPGA.cpp
--------
Projective geometric algebra examples showing how PGA relates to CGA for certain operations.

xPointToCircle.cpp
------------------
Transformations between points and circles, demonstrating the flexibility of conformal geometric algebra.

xR3.cpp
-------
Three-dimensional Euclidean space operations and their representation in conformal geometric algebra.

xRobotArm.cpp
-------------
Robotic arm kinematics using conformal geometric algebra for joint transformations and end-effector positioning.

xSwarm.cpp
----------
Swarm behavior simulation using geometric algebra for particle interactions and movement.

xTorusKnot.cpp
--------------
Generation and visualization of torus knots using conformal transformations and boost operations.

xFabrikChain.cpp
----------------
Forward and backward reaching inverse kinematics (FABRIK) algorithm implementation for chain structures.

xEmptyProject.cpp
-----------------
Template file for starting new projects with basic app structure and GUI setup.

Scratch Projects (scratch/projects/)
===================================

basics/
=======

xCGA2d_ops.cpp
--------------
Two-dimensional conformal geometric algebra operations including 2D constructions and transformations.

xConstructions.cpp
------------------
Fundamental geometric constructions like angle bisectors, perpendiculars, and basic compass-and-straightedge constructions.

xDiscCollision.cpp
------------------
Disc collision detection and response using conformal geometric algebra for efficient geometric calculations.

xDualityTest.cpp
----------------
Tests and demonstrations of duality relationships in conformal geometric algebra.

xEga3d.cpp
----------
Three-dimensional Euclidean geometric algebra operations before embedding in conformal space.

xExternalLinkage.cpp
--------------------
External linkage mechanisms and constraint systems for mechanical engineering applications.

xOrthogonalSpheres.cpp
----------------------
Construction and manipulation of orthogonal sphere systems and their properties.

xPlunge.cpp
-----------
Plunge operations and geometric transformations in conformal space.

xProjection.cpp
---------------
Various projection operations between geometric objects and spaces.

xStereoTest.cpp
---------------
Stereographic projection tests and applications in conformal geometric algebra.

xTangency.cpp
-------------
Tangency conditions and constructions between geometric objects like circles and spheres.

xTransformations.cpp
--------------------
Comprehensive demonstration of conformal transformations including inversions, reflections, and rotations.

boosts/
=======

xTrajectory.cpp
---------------
Elliptic and Hyperbolic orbits of knots based on point weighting or radius adjustment. Demonstrates how boost transformations can create various orbital trajectories.

xCyclidicVolumes.cpp
--------------------
Construction and manipulation of cyclide surfaces and volumes using conformal geometric algebra. Shows how to create complex 3D geometric structures.

xCyclidicVolumes2.cpp
---------------------
Advanced cyclide construction techniques with different parametrizations and control methods.

xAnapole.cpp / xAnapole2.cpp
----------------------------
Anapole field constructions and electromagnetic applications using conformal geometric algebra.

xBoost2D.cpp
------------
Two-dimensional boost transformations and their geometric interpretations.

xCircleBlending.cpp
-------------------
Smooth blending between circles using boost transformations and interpolation techniques.

xHopf.cpp / xHopfKnot.cpp
-------------------------
Hopf fibration and Hopf knot constructions using quaternionic and conformal representations.

xKnot.cpp / xKnotted.cpp
------------------------
Various knot constructions and transformations using boost operations and conformal mappings.

xLog.cpp / xLog2Log.cpp
-----------------------
Logarithmic operations on conformal transformations and their compositions.

xOrbits.cpp
-----------
Orbital mechanics and periodic motion using boost transformations.

xSixSphere.cpp / xSixSphere2.cpp
--------------------------------
Complex six-sphere constructions and their geometric relationships.

xTangentTrajectories.cpp
------------------------
Tangent vector fields and trajectory following using differential geometry in conformal space.

geo/
====

xSphereBlend.cpp
----------------
Sphere blending using point pair generators. Demonstrates how to smoothly transition between spheres using boost transformations with proper weighting.

xBary.cpp
---------
Barycentric coordinates and their applications in conformal geometric algebra for interpolation and blending.

xBasketball.cpp
---------------
Basketball trajectory simulation using physical constraints and conformal transformations.

xConic.cpp / xConicCurves.cpp
-----------------------------
Conic section constructions and transformations including parabolas, ellipses, and hyperbolas.

xConformalCoord.cpp
-------------------
Conformal coordinate systems and their transformations for mapping between different geometric representations.

xConvex.cpp
-----------
Convex hull algorithms and convex geometry operations using conformal geometric algebra.

xCSG.cpp
--------
Constructive solid geometry operations including Boolean operations on geometric objects.

xDarboux.cpp
------------
Darboux frames and differential geometry applications for surface analysis.

xHoroball.cpp / xHyperbolic.cpp
-------------------------------
Horoball constructions and hyperbolic geometry demonstrations in conformal space.

xVoronoi.cpp
------------
Voronoi diagram generation using conformal geometric algebra for efficient proximity calculations.

kinematics/
===========

xBricard.cpp
------------
Bricard six-bar linkage mechanism (six-R mechanism) implementation. Demonstrates complex spatial linkage systems with constraint satisfaction.

xBennett.cpp / xBennettVariation.cpp
------------------------------------
Bennett linkage mechanisms and their variations for spatial four-bar linkages.

xFabrik.cpp
-----------
Forward and backward reaching inverse kinematics (FABRIK) algorithm for solving complex kinematic chains with multiple constraints.

xConstrainedFabrik.cpp
----------------------
FABRIK with additional geometric constraints for more complex robotic applications.

xDHParam.cpp
------------
Denavit-Hartenberg parameter implementations for standard robotic kinematic modeling.

xMotors.cpp
-----------
Motor algebra applications in kinematics for efficient rigid body transformations.

xPantograph.cpp
---------------
Pantograph mechanism simulation showing parallel motion generation.

xWatt.cpp / xWatt2.cpp
---------------------
Watt linkage mechanisms and their geometric properties for mechanical engineering.

folds/
======

xMiuriOri.cpp (+ variants 2-6)
------------------------------
Miura origami folding patterns and their geometric constraints. Demonstrates how conformal geometric algebra can model origami mathematics.

xAxioms.cpp
-----------
Origami axioms (Huzita's axioms) implemented in conformal geometric algebra for fold line constructions.

xConicFolds.cpp
---------------
Conic section folding patterns and their applications in origami design.

xCrimp.cpp
----------
Crimp folding techniques and their geometric analysis.

xWaterbomb.cpp (+ variants)
---------------------------
Waterbomb origami pattern and its kinematics for deployable structures.

xReverseFold.cpp
----------------
Reverse fold operations and their geometric constraints in origami.

xHypar.cpp
----------
Hyperbolic paraboloid (hypar) folding patterns for architectural applications.

fields/
=======

xCurl.cpp
---------
Vector field curl calculations using simplicial relations and conformal geometric algebra. Demonstrates divergence and curl operations on discrete meshes.

xDifferential.cpp (+ variants 2-4)
----------------------------------
Differential geometry operations including gradients, Laplacians, and curvature calculations on discrete surfaces.

xDipole.cpp
-----------
Electromagnetic dipole field constructions and visualizations using conformal geometric algebra.

xTwistField.cpp / xTwistFieldCrystal.cpp
----------------------------------------
Twist field constructions and their applications in crystal structure modeling.

xGradient.cpp
-------------
Gradient field calculations and visualizations on geometric structures.

groups/
=======

xSpaceGroup3D.cpp (+ variants)
------------------------------
Three-dimensional space group constructions including all 230 crystallographic space groups. Demonstrates lattice systems, symmetry operations, and crystal structure generation.

xSpaceGroup2D.cpp
-----------------
Two-dimensional space group implementations for wallpaper patterns and planar symmetries.

xRoots.cpp
----------
Root system constructions and their applications in group theory and crystal structures.

xTensegrity.cpp
---------------
Tensegrity structure modeling using group theoretical constraints and geometric algebra.

util/
=====

xBasis.cpp / xBasisRedux.cpp
----------------------------
Basis operations and multivector component analysis for understanding geometric algebra foundations.

xCommand.cpp
------------
Command-line interface implementations and parsing for geometric algebra applications.

xMetricTensor.cpp
-----------------
Metric tensor operations and their applications in differential geometry.

xRuntime.cpp
------------
Runtime analysis and performance testing utilities for geometric algebra operations.

inversions/
===========

xConicInversion.cpp
-------------------
Conic inversion operations and their geometric properties.

xReflectionSurfaces.cpp (+ 2D variant)
--------------------------------------
Reflection surface constructions and their applications in optical design.

agents/
=======

xJellies.cpp
------------
Agent-based jellyfish simulation using geometric algebra for natural movement patterns.

games/
======

rollball.cpp
------------
Ball rolling physics simulation using conformal geometric algebra for collision detection.

xInterface.cpp
--------------
User interface implementations for interactive geometric algebra applications.

mesh/
=====

xImport.cpp
-----------
Mesh importing and processing utilities for geometric algebra applications.

xMesh.cpp
---------
Mesh data structures and operations using conformal geometric algebra.

xSkin.cpp
---------
Mesh skinning and deformation techniques for animation applications.

io/
===

xMeshImport.cpp
---------------
Advanced mesh importing with geometric algebra preprocessing.

xRenderToFile.cpp
-----------------
Rendering output to files for documentation and analysis purposes.

frames/
=======

xControl.cpp
------------
Frame control systems for camera and object manipulation.

xLinkages.cpp
-------------
Frame-based linkage systems and their constraints.

xNormals.cpp
------------
Normal vector calculations and manipulations for surface geometry.

templates/
==========

xSpaceTime.cpp
--------------
Spacetime algebra implementations extending conformal geometric algebra to 4D.

xIntro.cpp
----------
Introductory example demonstrating basic sphere-plane intersections, reflections, and fundamental conformal geometric algebra operations. Good starting point for newcomers.
