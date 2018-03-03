/*
 * =====================================================================================
 *
 *       Filename:  vsr_manifold.h
 *
 *    Description:  3d combo of smart objects and graphs (like halfedge)
 *
 *        Version:  1.0
 *        Created:  11/15/2013 13:47:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vsr_graph.h"
#include "vsr_smart.h"
#include "vsr_cga3D_op.h"

namespace vsr{


  /*
   * =====================================================================================
   *        Class:  Facet
   *  Description:  A triangular face
   * =====================================================================================
   */

  class Facet {
      
      SmartObj<Pnt, HEGraph::Node> a, b, c;

      public:

  };
 
  
  /*!
   * =====================================================================================
   *        Class:  Surface
   *  Description:  2d manifold
   * =====================================================================================
   */

  class Surface {
  
    public:

  };
}

