css: style0.css

VERSOR
===
 
Various Notes (in progress)
---                      
#CONSTRUCTIONS

##Lines

`Line` | `Lin`

`DualLine` | `Dll`

We will often assume normalized lines (`.unit()`) and dualLines (`.runit()`)
  
  Dll <= Dll is a (reversed) scalar dot product.  Zero when Orthogonal. Positive when pointing in opposite directions
  Dll ^ Dll is a Direction Trivector and specfies Relative Chirality and distance.  Is ZERO when they intersect OR are parellel. 
  Increases in Magnitude as closest point gets farther away (except for parallel lines)    
  In the Case of Parallel lines, Lin <= Lin, is 1 if oriented same direction and -1 if opposite directions. 
  L1-L2 The bisector line of two normalized lines L1 and L2
  L2L1L2 reflects L1 through L2 by twisting through it
  L1 - L2L1L2 is a line orthogonal to L2


##Constructing Planes

`Point` is also `Pnt`

	Pnt <= Dll returns the shared DualPlane. 
    The dot() of the DualPlane is the squared distance betwen point and line, if the line has been normalized.

  Pnt ^ Dll returns the Direct Circle about the DualLine through the Point.
    The Ro::size(Circle,false) returns the squared radius of the Circle.
 
  Pnt ^ Inf <= Dll returns the Dual Plane through the point orthogonal to the Dual Line
    Pnt ^ Inf is a Flat Point, often used for "potentialities", e.g. a flat point is ready-to-be converted . . .

  Pnt <= Drv is Dual Plane through p with normal n

##DualLines and Lines


##Rounds

  'Point' is also 'Pnt' is also 'DualSphere' is also 'Dls'
  'PointPair' is also 'Par'
  'Circle' is also 'Cir'

  Point p <= Cir k returns the Tangent to Circle k at Point p
  Point p ^ k.dual() returns the orthogonal "plunge" of k through p (and the inversion of p in k)
  Point p <= DualSphere dls is POSITIVE if p is within dls
	relates to distance of p to dls by .... 

Extendors

  Group theory includes the notion of an Extendor (i.e. a Glide Plane or a Roto-inversion)
  A GLD (glide plane) is a Reflection followed by a Translation (or vice versa): a Line * FlatPoint  
  Not to be confused with Extensors (from tensor theory)

Distances

  Dual Line and Point: ( pnt <= dll.runit() ).wt() seems to return -dist^2/2.0 
  Dual Plane and Point: ( pnt <= dlp ) returns distance and which side of plane . .

  Circle and Point: -( pnt <= cir ).wt()  
  Sphere and point: ( pnt <= dls).wt()

TO DO
---

*   Cylinder - Cylinder intersection
Point to cylinder is okay (within surround, colinear with carrier)

*   Two Point Pairs that share a point are ___
*   Inversion in a point pair is ____ (useful for what)	

