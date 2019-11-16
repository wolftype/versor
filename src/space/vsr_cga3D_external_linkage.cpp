// external linkage for c style programs

//conformal 3D types and basic operations:
#include <vsr/space/vsr_cga3D_op.h>

namespace vsr { namespace cga {

#define CREATE_FUNC(OBJ)\
OBJ * cga_ ## OBJ ## _create () {\
OBJ * p = new OBJ;\
  return p;\
}\
OBJ * cga_ ## OBJ ## _create_n (unsigned int n) {\
OBJ * p = new OBJ [n];\
  return p;\
}\

//typedef


extern "C" {

  struct vec3 { VSR_PRECISION x,y,z; };
  struct quat { VSR_PRECISION w,x,y,z; };

  // MAKE
  CREATE_FUNC(Point);
  CREATE_FUNC(Pair);
  CREATE_FUNC(Circle);
  CREATE_FUNC(Sphere);
  CREATE_FUNC(Line);
  CREATE_FUNC(Plane);

  void cga_point_from_coords (Point *p, VSR_PRECISION x, VSR_PRECISION y, VSR_PRECISION z){
    *p = Round::null (x,y,z);
  }

  void cga_point_from_vec (Point *p, vec3 * v){
    *p = Round::null (v->x, v->y, v->z);
  }

  // Get Coord
  VSR_PRECISION cga_point_get_coord (unsigned int n, Point *p){
    return (*p)[n];
  }

  // Fill Vector
 // void vec_from_cga_point (vec3 *v, Point *p){
 //   (*v).x = (*p)[0];
 //   (*v).y = (*p)[1];
 //   (*v).z = (*p)[2];
 // }

  // build
  void cga_vec_from_point (Vec *v, Point *p){
    (*v) = (*p);
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

  void cga_plane_through_points (Plane *res, Point *pa, Point *pb, Point *pc)
  {
     *res = (*pa) ^ (*pb) ^ (*pc) ^ Inf(1);
  }

  void cga_sphere_of_points (Sphere *res, Point *pa, Point *pb, Point *pc, Point *pd)
  {
     *res = (*pa) ^ (*pb) ^ (*pc) ^ (*pd);
  }

  void cga_dualsphere_at_point (DualSphere *res, Point *pa, VSR_PRECISION radius)
  {
    *res = Round::dls (*pa, radius);
  }

  void cga_dualsphere_at_vec (DualSphere *res, Vec *vec, VSR_PRECISION radius)
  {
    *res = Round::dls (*vec, radius);
  }

  // generate
  void cga_rotor_gen (Rotor *r, Biv *b)
  {
    *r = Gen::rot (*b);
  }

  void cga_motor_gen (Motor *m, Dll *d)
  {
    *m = Gen::mot (*d);
  };

} // extern "C"

}} //vsr::cga::
