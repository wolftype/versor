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


#define APPLY_FUNC(OBJ, XF)\
OBJ * cga_ ## OBJ ## _apply_ ## XF (OBJ * p, XF * xf) {\
  *p = (*p).spin (*xf);\
  return p;\
}\

#define GENERATOR_FUNC(GEN, VSR)\
GEN * cga_ ## GEN ## _from_ ## VSR ## s (GEN * g, VSR * va, VSR * vb) {\
  (*g) = Gen::log(((*vb)/(*va)).runit()) * .5;\
  return g;\
}\

#define POS_FUNC(OBJ)\
Point * cga_Point_from_ ## OBJ (OBJ * obj){\
  return (*obj).pos();\
}

//unsafe
#define GET_COORD(OBJ)\
VSR_PRECISION cga_ ## OBJ ## _get_coord (OBJ *p, unsigned int n){\
  return (*p)[n];\
}\

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

#define DIRECTION_FROM_DUAL_FUNC(OBJ)\
Drv * cga_Direction_from_ ## OBJ (Drv * res, OBJ * obj){\
  *res = nga::Flat::direction ((*obj).undual());\
  return res;\
}\

#define POINT_FROM_FLAT_DUAL_FUNC(OBJ)\
Point * cga_Point_from_ ## OBJ (Point * res, OBJ * obj){\
  *res = nga::Flat::loc(*obj, PAO, true);\
  return res;\
}\


#define CAST_FUNC(FROM, TO)\
TO * cga_cast_ ## FROM ## _ ## TO (FROM * from, TO * to){\
  *to = *from;\
  return to;\
}\

#define COPY_FUNC(FROM, TO)\
TO * cga_copy_ ## FROM ## _ ## TO (FROM * from, TO * to){\
  *to = (*from).copy<TO>();\
  return to;\
}\

extern "C" {

  Point * cga_tpoint_create() {
    Point * p = new Point();
    return p;
  };

  struct vec3 { VSR_PRECISION x,y,z; };
  struct quat { VSR_PRECISION w,x,y,z; };

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

  //Rotor from Frame
  Rotor * cga_Rotor_from_Frame (Rotor *res, Frame *frame){
     *res = (*frame).rot();
     return res;
  };

  CAST_FUNC(DualLine, Bivector);
  COPY_FUNC(DirectionVector, Vector);

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

  // Gen from ratio
  GENERATOR_FUNC(DualLine, DualLine);
  GENERATOR_FUNC(Pair, DualSphere);

  //FLAT_DUAL
  DIRECTION_FROM_DUAL_FUNC(DualLine);
  POINT_FROM_FLAT_DUAL_FUNC(DualLine);

  // Transforms
  APPLY_FUNC(Point, Motor);
  APPLY_FUNC(Circle, Motor);
  APPLY_FUNC(Vec, Rotor);

  GET_COORD (Point);
  GET_COORD (Vec);
  GET_COORD (DualLine);
  GET_COORD (Rotor);

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
  Point * cga_point_from_coords (Point *p, VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z) {
    *p = Round::null (x,y,z);
    return p;
  }

  Point * cga_point_from_vec (Point *p, vec3 * v){
    *p = Round::null (v->x, v->y, v->z);
    return p;
  }

  VSR_PRECISION cga_point_get_coord (unsigned int n, Point *p){
    return (*p)[n];
  }

  // Basis weights
  VSR_PRECISION cga_get_coord (unsigned int n, float *p){
    return p[n];
  }

  Vec * cga_vec_from_point (Vec *v, Point *p){
    (*v) = (*p);
    return v;
  }

  void cga_pair_of_points (Pair *res, Point *pa, Point *pb)
  {
     *res = (*pa) ^ (*pb);
  }

  void cga_circle_of_points (Cir *res, Point *pa, Point *pb, Point *pc)
  {
     *res = (*pa) ^ (*pb) ^ (*pc);
  }

  void cga_line_through_points (Line *res, Point * pa, Point *pb)
  {
    *res = (*pa) ^ (*pb) ^ Inf(1);
  }

  void cga_dualline_through_points (DualLine *res, Point * pa, Point *pb)
  {
    *res = ((*pa) ^ (*pb) ^ Inf(1)).dual();
  }

  void cga_line_from_vec_dir (Line *res, Vec * v, Vec *dir)
  {
    *res = (*v).null() ^ (*dir).copy<Drv>();
  }

  void cga_plane_through_points (Plane *res, Point *pa, Point *pb, Point *pc)
  {
     *res = (*pa) ^ (*pb) ^ (*pc) ^ Inf(1);
  }

  Sphere * cga_sphere_of_points (Sphere *res, Point *pa, Point *pb, Point *pc, Point *pd)
  {
     *res = (*pa) ^ (*pb) ^ (*pc) ^ (*pd);
     return res;
  }

  DualSphere * cga_dualsphere_at_point (DualSphere *res, Point *pa, VSR_PRECISION radius)
  {
    *res = Round::dls (*pa, radius);
    return res;
  }

  DualSphere * cga_dualsphere_at_vec (DualSphere *res, Vec *vec, VSR_PRECISION radius)
  {
    *res = Round::dls (*vec, radius);
    return res;
  }

  // meet
  Point * cga_meet_dualline_dualplane (Point *p, DualLine *dll, DualPlane *dlp)
  {
    *p = Construct::meet (*dll, *dlp);
    return p;
  }

  Point * cga_meet_line_plane (Point *p, Line *dll, Plane *dlp)
  {
    *p = Construct::meet ((*dll).dual(), (*dlp).dual());
    return p;
  }

  // generate
  Rotor * cga_rotor_gen (Rotor *r, Biv *b)
  {
    *r = Gen::rot (*b);
    return r;
  }

  Motor * cga_motor_gen (Motor *m, Dll *d)
  {
    *m = Gen::mot (*d);
    return m;
  }

} // extern "C"

}} //vsr::cga::
