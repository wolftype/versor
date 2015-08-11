/*
 * =====================================================================================
 *
 *       Filename:  vsr_graph_draw.h
 *
 *    Description:  draw hegraph
 *
 *        Version:  1.0
 *        Created:  05/01/2014 16:27:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  vsr_graph_draw_INC
#define  vsr_graph_draw_INC
#include "vsr_graph.h"
#include "vsr_generic_draw.h"

namespace vsr{
  namespace generic{

    //Draw HEGraph Structure
    template<class T>
    void Draw( const HEGraph<T>& graph){
      
      for (int i = 0; i < graph.edge().size(); ++i){
      
          if ( graph.edge()[i] -> opp == NULL ) glColor3f(1,0,0);
          else glColor3f(0,1,0);
          
          glBegin(GL_LINES);
            GL::vertex( graph.edge()[i] -> next -> next -> node -> ptr -> begin() );
            glColor3f(0,0,1);
            GL::vertex( graph.edge()[i] -> node -> ptr -> begin() );
          glEnd();

      }

        for (auto& i : graph.face() ){
           ColorTriangles(i->a(), i->b(), i->c() );
        }
    }

  }

}//vsr
#endif   /* ----- #ifndef vsr_graph_draw_INC  ----- */
