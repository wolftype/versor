/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_conic.h
 *
 *    Description:  conic lambdas for cga3D types
 *
 *        Version:  1.0
 *        Created:  03/11/2015 18:08:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */



#ifndef vsr_cga3D_conic_INC
#define vsr_cga3D_conic_INC

#include <vsr/form/vsr_conic.h>
#include <vsr/space/vsr_cga3D_types.h>

namespace vsr {

namespace cga {
namespace conic {
namespace transform {

//lambda function to transform point p (relative to origin)
auto point = [](const cga::Point &p, Vec &v, VSR_PRECISION amt) {
  return Conic::Transform (p, v, amt);
};

//lambda function to transform point p (relative to given center)
auto point_cen = [](const cga::Point &p, const cga::Point &c, Vec &v,
                         VSR_PRECISION amt) {
  return Conic::Transform (p, c, v, amt);
};
}
}
}  //cga3D::conic::transform
}
#endif /* ----- #ifndef vsr_cga3D_conic_INC  ----- */
