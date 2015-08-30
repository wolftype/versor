/*
 * =====================================================================================
 *
 *       Filename:  vsr_graph_draw.h
 *
 *    Description:  draw half-edge graph
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

#include "gfx/gfx_render.h"

#include "form/vsr_graph.h"
#include "vsr_generic_draw.h"

namespace gfx{

//Draw HEGraph Structure
//template<class T>
//void Immediate( const HEGraph<T>& graph){
//  
//  for (int i = 0; i < graph.edge().size(); ++i){
//  
//      if ( graph.edge()[i] -> opp == NULL ) glColor3f(1,0,0);
//      else glColor3f(0,1,0);
//      
//      glBegin(GL_LINES);
//        GL::vertex( graph.edge()[i] -> next -> next -> node -> ptr -> begin() );
//        glColor3f(0,0,1);
//        GL::vertex( graph.edge()[i] -> node -> ptr -> begin() );
//      glEnd();
//
//  }
//
//    for (auto& i : graph.face() ){
//       ColorTriangles(i->a(), i->b(), i->c() );
//    }
//}

//  template<class T> 
//  struct Renderable<vsr::HEGraph<T>> : RenderableBase< vsr::HEGraph<T> > {
//    
//    static void DrawImmediate( const vsr::HEGraph<T>& graph){
//     glBegin(GL_TRIANGLES);
//     for (auto& i : graph.face()){
//          auto& a = i->a();
//          auto& b = i->b();
//          auto& c = i->c(); 
//          //glColor4f(.2,1,.2,.7);
//          GL::normal( a.normal.begin() );
//          GL::vertex( a.pos.begin() );
//          GL::normal( b.normal.begin() );
//          GL::vertex( b.pos.begin() );
//          GL::normal( c.normal.begin() );
//          GL::vertex( c.pos.begin() );
//     }
//     glEnd();
//    }
//
//    static void Draw( const vsr::HEGraph<T>& f, GFXSceneNode*){}
//  };

/*-----------------------------------------------------------------------------
 *  Surface DRAW METHOD (immediate mode)
 *-----------------------------------------------------------------------------*/
   template<class A, class T> 
   struct Renderable<vsr::HEGraph< Multivector<A,T> >> : RenderableBase<vsr::HEGraph< Multivector<A,T>>>{
     
     using TVec = typename A::types::Vec;
     static void DrawImmediate( const vsr::HEGraph< Multivector<A,T> >& graph){
     
      glBegin(GL_TRIANGLES);
      for (auto& i : graph.face()){
          auto& a = i->a();
          auto& b = i->b();
          auto& c = i->c(); 
          //glColor4f(.2,1,.2,.7);
          TVec normal = (TVec(b-a).unit() ^ TVec(c-a).unit()).duale();
          GL::normal( normal.begin() );
          glColor3f(1,.2,1);
          GL::vertex( a.begin() );
       //   GL::normal( b.normal.begin() );
          glColor3f(1,1,.2);
          GL::vertex( b.begin() );
       //   GL::normal( c.normal.begin() );
          glColor3f(.2,1,1);
          GL::vertex( c.begin() );
      }
      glEnd();
     }

   };

} //gfx::
