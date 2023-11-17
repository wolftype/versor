// external linkage for c style programs
//
// consider limiting to ONLY dual space for clarity
// e.g. Lines are only duallines, Spheres are only dualspheres

//conformal 3D types and basic operations:
#include <vsr/space/vsr_cga3D_op.h>
#include <vsr/form/vsr_cga3D_frame.h>

namespace vsr { namespace cga {

// MACROS to create and destroy object(s) of type OBJ
#define CREATE_FUNC(OBJ)\
OBJ * cga_ ## OBJ ## _create () {\
  OBJ * p = new OBJ;\
  return p;\
};\
OBJ * cga_ ## OBJ ## _create_n (unsigned int n) {\
  OBJ * p = new OBJ [n];\
  return p;\
};\
void cga_ ## OBJ ## _destroy (OBJ * o) {\
  if (o) delete o;\
};\
void cga_ ## OBJ ## _destroy_n (OBJ * o) {\
  if (o) delete[] o;\
};

//MACRO for GP
#define GP_FUNC(RES, OBJA, OBJB)\
RES * cga_ ## RES ## _gp_ ## OBJA ## _ ## OBJB (RES *r, OBJA * a, OBJB * b) {\
  *r = (*a) * (*b);\
  return r;\
}

//MACRO for IP
#define IP_FUNC(RES, OBJA, OBJB)\
void cga_ ## RES ## _ip_ ## OBJA ## _ ## OBJB (RES *r, OBJA * a, OBJB * b) {\
  *r = (*a) <= (*b);\
}

//MACRO for OP
#define OP_FUNC(RES, OBJA, OBJB)\
void cga_ ## RES ## _op_ ## OBJA ## _ ## OBJB (RES *r, OBJA * a, OBJB * b) {\
  *r = (*a) ^ (*b);\
}

//MACRO to create a GENERATOR of type GEN from two types VSR
#define GEN_FUNC(GEN, VSR)\
void cga_ ## GEN ## _gen_ ## VSR ## s (GEN * g, VSR * va, VSR * vb) {\
  (*g) = Gen::log(((*vb)/(*va)).runit()) * .5;\
}

//MACRO to create a ROTOR of type ROT from Generator GEN
//NVRMIND do it manuallly below
//#define ROTOR_FUNC(ROT, GEN)\
//ROT * cga_ ## ROT (ROT * r, GEN * g, float amt) {\
//  (*r) = Gen::

//MACRO to APPLY transformation function XF on object OBJ
//may require casting of Obj first (e.g. cast Line to Circle before boosting)
#define APPLY_FUNC(OBJ, XF)\
void cga_ ## OBJ ## _apply_ ## XF (OBJ * res, OBJ * p, XF * xf) {\
  *res = (*p).spin (*xf);\
}

//MACRO to extract point Position of object OBJ
#define POS_FUNC(OBJ)\
Point * cga_Point_from_ ## OBJ (OBJ * obj){\
  return (*obj).pos();\
}

//unsafe MACRO to get nth coord of object OBJ
#define GET_COORD(OBJ)\
VSR_PRECISION cga_ ## OBJ ## _get_coord (OBJ *p, unsigned int n){\
  return (*p)[n];\
}\

//MACRO to SET position of object OBJ
#define SET_POS_FUNC(OBJ)\
OBJ * cga_ ## OBJ ## _set_pos (OBJ * o, Vec * v){\
  (*o).pos(*v);\
  return o;\
}\

#define SET_COORDS_FUNC(OBJ)\
OBJ * cga_ ## OBJ ## _set_coords (OBJ * o, VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z){\
  (*o).pos(x,y,z);\
  return o;\
}\

//Get Direction form Dual Flat element OBJ
#define DIRECTION_FROM_DUAL_FLAT_FUNC(RES, OBJ)\
void cga_Direction_from_ ## OBJ (RES * res, OBJ * obj){\
  *res = nga::Flat::direction ((*obj).undual());\
}\

//Get Location form Dual Flat element OBJ
#define POINT_FROM_DUAL_FLAT_FUNC(OBJ)\
void cga_Point_from_ ## OBJ (Point * res, OBJ * obj){\
  *res = nga::Flat::loc(*obj, PAO, true);\
}\

//Get Direction form Dual Flat element OBJ
#define DIRECTION_FROM_ROUND_FUNC(RES, OBJ)\
void cga_ ## RES ## _from_ ## OBJ (RES * res, OBJ * obj){\
  *res = nga::Round::direction (*obj);\
}
//Get Location form Dual Round element OBJ
#define POINT_FROM_ROUND_FUNC(OBJ)\
void cga_Point_from_ ## OBJ (Point * res, OBJ * obj){\
  *res = nga::Round::location(*obj);\
}\

// CAST
#define CAST_FUNC(FROM, TO)\
TO * cga_cast_ ## FROM ## _ ## TO (FROM * from, TO * to){\
  *to = *from;\
  return to;\
}\

// COPY
#define COPY_FUNC(FROM, TO)\
TO * cga_copy_ ## FROM ## _ ## TO (FROM * from, TO * to){\
  *to = (*from).copy<TO>();\
  return to;\
}\

extern "C" {

  struct vec3 { VSR_PRECISION x,y,z; };
  struct quat { VSR_PRECISION w,x,y,z; };

  // MAKE
  CREATE_FUNC(Vector);
  CREATE_FUNC(Bivector);
  CREATE_FUNC(Point);
  CREATE_FUNC(Pair);
  CREATE_FUNC(Circle);
  CREATE_FUNC(Sphere);
  CREATE_FUNC(DualSphere);
  CREATE_FUNC(Line);
  CREATE_FUNC(DualLine);
  CREATE_FUNC(Plane);
  CREATE_FUNC(DualPlane);
  CREATE_FUNC(FlatPoint);
  CREATE_FUNC(DualFlatPoint);
  CREATE_FUNC(Rotor);
  CREATE_FUNC(Translator);
  CREATE_FUNC(Dilator);
  CREATE_FUNC(Motor);
  CREATE_FUNC(Boost);
  CREATE_FUNC(ConformalRotor);
  CREATE_FUNC(DirectionVector);

//  Point * cga_tpoint_create() {
//    Point * p = new Point();
//    return p;
//  };
//
  // PRODUCTS
  GP_FUNC(Con, Boost, Boost);

  //CASTING AND COPYING
  CAST_FUNC(DualLine, Bivector);
  CAST_FUNC(DualLine, Pair);
  COPY_FUNC(DirectionVector, Vector);

  //FLAT_DUAL
  DIRECTION_FROM_DUAL_FLAT_FUNC(DirectionVector, DualLine);
  POINT_FROM_DUAL_FLAT_FUNC(DualLine);

  //ROUND
  DIRECTION_FROM_ROUND_FUNC(DirectionVector, Pair);
  DIRECTION_FROM_ROUND_FUNC(DirectionBivector, Circle);
  POINT_FROM_ROUND_FUNC(Pair);
  POINT_FROM_ROUND_FUNC(Circle);
  POINT_FROM_ROUND_FUNC(DualSphere);
  GET_COORD (Point);
  GET_COORD (Vec);
  GET_COORD (DualLine);
  GET_COORD (Rotor);

  // APPLY Transforms
  APPLY_FUNC(Vec, Rotor);

  APPLY_FUNC(Point, Motor);
  APPLY_FUNC(DualLine, Motor);
  APPLY_FUNC(Pair, Motor);
  APPLY_FUNC(Circle, Motor);

  APPLY_FUNC(Point, Boost);
  APPLY_FUNC(Pair, Boost);
  APPLY_FUNC(Circle, Boost);

  APPLY_FUNC(Point, Con);
  APPLY_FUNC(Pair, Con);
  APPLY_FUNC(Circle, Con);

  //Generate a Rotor from ratio of B
  GEN_FUNC(Bivector, Vector);
  GEN_FUNC(DualLine, DualLine);
  GEN_FUNC(Pair, DualSphere);

  void cga_Rotor_gen (Rotor *r, Biv *b, float amt)
  {
    *r = Gen::rot (*b * amt);
  }

  void cga_Motor_gen (Motor *m, Dll *d, float amt)
  {
    *m = Gen::mot (*d * amt);
  }

  void cga_Boost_gen (Bst *b, Pair *p, float amt)
  {
    *b = Gen::bst (*p * amt);
  }


  // ROTORS
  //
  //Rotor from Direction vector y
  Rotor * cga_Rotor_from_Direction_y (Rotor * res, DirectionVector * drv){
    *res = Gen::ratio (Vec::y, (*drv).copy<Vec>());
    return res;
  };

  //Rotor from vector y
  Rotor * cga_Rotor_from_Vec_y (Rotor * res, Vector * vec){
    *res = Gen::ratio (Vec::y, (*vec).unit());
    return res;
  };

  //Rotor from DirectionBivector xz
  Rotor * cga_Rotor_from_Direction_xz (Rotor * res, DirectionBivector * drb){
    *res = Gen::ratio (Vec::y, (*drb).copy<Biv>().duale().unit());
    return res;
  };

  //Rotor from Bivector xz
  Rotor * cga_Rotor_from_Biv_xz (Rotor * res, Bivector * biv){
    *res = Gen::ratio (Vec::y, (*biv).duale().unit());
    return res;
  };

  //Rotor from DualLine
  Rotor * cga_Rotor_from_DualLine(Rotor * res, DualLine * dll){
    *res = Gen::ratio (Vec::y, Biv(*dll).duale().unit());
    return res;
  };

  //Rotor from Circle (from Op::AA)
  Rotor * cga_Rotor_from_Circle(Rotor * res, Circle * cir){
    Biv b = Round::dir (*cir).copy<Biv> ();
    *res = Gen::ratio (Vec::z, Op::dle (b).unit ());
    return res;
  };

  //Rotor from Frame
  Rotor * cga_Rotor_from_Frame (Rotor *res, Frame *frame){
     *res = (*frame).rot();
     return res;
  };

  // ROUNDS: FEATURE EXTRACTION (See also Macro funcs above)
  // squared size of a DualSphere
  VSR_PRECISION cga_size_of_Point (DualSphere * dls) {
    return nga::Round::size(*dls, true);
  }
  // squared size of a Pair
  VSR_PRECISION cga_size_of_Pair (Pair * pair) {
    return nga::Round::size(*pair, false);
  }
  // squared size of a Circle
  VSR_PRECISION cga_size_of_Circle (Circle *cir) {
    return nga::Round::size(*cir, false);
  }
  // squared size of a Sphere
  VSR_PRECISION cga_size_of_Sphere (Sphere *sph) {
    return nga::Round::size(*sph, false);
  }
  void cga_PointA_from_Pair(Point *p, Pair*t){
    std::vector< cga::Pnt> pp = nga::Round::split( *t  );
    *p = pp[0];
  }
  void cga_PointB_from_Pair(Point *p, Pair*t){
    std::vector< cga::Pnt> pp = nga::Round::split( *t );
    *p = pp[1];
  }



  //FRAMES
  CREATE_FUNC(Frame);
  SET_POS_FUNC(Frame);
  SET_COORDS_FUNC(Frame);

  Frame * cga_Frame_set_rot (Frame * f, Rotor *r){
    (*f).rot (*r);
    return f;
  };

  Rotor * cga_Frame_get_rot (Frame * f, Rotor *r){
    *r = (*f).rot();
    return r;
  };

  Point * cga_Frame_get_point (Frame * f, Point *p){
    *p= (*f).pos();
    return p;
  };

  // orient z axis to v direcion
  Frame * cga_Frame_orient_z (Frame * f, Vec * v ){
    (*f).orient (*v);
    return f;
  };

  Frame * cga_Frame_set_pose (Frame * f,
      VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z,
      VSR_PRECISION xd, VSR_PRECISION yd, VSR_PRECISION zd){
      (*f).pos(x,y,z);
      (*f).orient(Vec(xd,yd,zd).unit());
      return f;
  };

  DualLine * cga_Frame_get_x_axis (Frame *f, DualLine *dll)
  {
    *dll = (*f).dlx();
    return dll;
  }
  DualLine * cga_Frame_get_y_axis (Frame *f, DualLine *dll)
  {
    *dll = (*f).dly();
    return dll;
  }
  DualLine * cga_Frame_get_z_axis (Frame *f, DualLine *dll)
  {
    *dll = (*f).dlz();
    return dll;
  }

  // POINTS
  Point * cga_Point_from_coords (Point *p, VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z) {
    *p = Round::null (x,y,z);
    return p;
  }

  Point * cga_DualSphere_from_coords (Point *p, VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z, VSR_PRECISION r) {
    *p = Round::dls(x,y,z,r);
    return p;
  }

  Point * cga_Point_from_Vec (Point *p, vec3 * v){
    *p = Round::null (v->x, v->y, v->z);
    return p;
  }

//  VSR_PRECISION cga_Point_get_coord (unsigned int n, Point *p){
//    return (*p)[n];
//  }

  // Basis weights
  VSR_PRECISION cga_get_coord (unsigned int n, float *p){
    return p[n];
  }

  Vec * cga_Vec_from_Point (Vec *v, Point *p){
    (*v) = (*p);
    return v;
  }

  void cga_Pair_of_Points (Pair *res, Point *pa, Point *pb)
  {
     *res = (*pa) ^ (*pb);
  }

  void cga_Circle_of_Points (Cir *res, Point *pa, Point *pb, Point *pc)
  {
     *res = (*pa) ^ (*pb) ^ (*pc);
  }

  void cga_Sphere_of_Points (Sphere *res, Point *pa, Point *pb, Point *pc, Point *pd)
  {
     *res = (*pa) ^ (*pb) ^ (*pc) ^ (*pd);
  }

  void cga_DualSphere_of_Points (DualSphere *res, Point *pa, Point *pb, Point *pc, Point *pd)
  {
     *res = ((*pa) ^ (*pb) ^ (*pc) ^ (*pd)).dual();
  }

  void cga_DualSphere_at_Point (DualSphere *res, Point *pa, VSR_PRECISION radius)
  {
    *res = Round::dls (*pa, radius);
  }

  void cga_DualSphere_at_Vec (DualSphere *res, Vec *vec, VSR_PRECISION radius)
  {
    *res = Round::dls (*vec, radius);
  }

  void cga_Line_of_Points (Line *res, Point * pa, Point *pb)
  {
    *res = (*pa) ^ (*pb) ^ Inf(1);
  }

  void cga_DualLine_of_Points (DualLine *res, Point * pa, Point *pb)
  {
    *res = ((*pa) ^ (*pb) ^ Inf(1)).dual();
  }

  void cga_Line_from_Vec_Dir (Line *res, Vec * v, Vec *dir)
  {
    *res = (*v).null() ^ (*dir).copy<Drv>();
  }

  void cga_Line_from_Point_Dir (Line *res, Point * p, Vec *dir)
  {
    *res = (*p) ^ (*dir).copy<Drv>();
  }

  void cga_Plane_through_Points (Plane *res, Point *pa, Point *pb, Point *pc)
  {
     *res = (*pa) ^ (*pb) ^ (*pc) ^ Inf(1);
  }
  void cga_DualPlane_through_Points (DualPlane *res, Point *pa, Point *pb, Point *pc)
  {
     *res = ((*pa) ^ (*pb) ^ (*pc) ^ Inf(1)).dual();
  }


  // MEET
  void cga_Point_at_DualLine_DualPlane (Point *p, DualLine *dll, DualPlane *dlp)
  {
    *p = Construct::meet (*dll, *dlp);
  }

  void cga_Point_at_Line_Plane (Point *p, Line *dll, Plane *dlp)
  {
    *p = Construct::meet ((*dll).dual(), (*dlp).dual());
  }


} // extern "C"

}} //vsr::cga::
