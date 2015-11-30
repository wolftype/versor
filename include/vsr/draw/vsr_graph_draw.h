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


   /// Checkboard mesh (fed Shape::Sphere or Shape::Cylinder )
   template<> 
   void Renderable<vsr::HEGraph<Vertex>>::DrawImmediate( const vsr::HEGraph<Vertex>& graph){
     glBegin(GL_TRIANGLES);
     int iter =0;
     bool bChecker = false;
     for (auto& i : graph.face()){
          iter++;
          float t = (float)iter/graph.face().size(); 
          auto& a = i->a();
          auto& b = i->b();
          auto& c = i->c(); 
          glColor3f(bChecker,bChecker,bChecker);
          GL::normal( a.Norm[0], a.Norm[1],a.Norm[2]);//.begin() );
          GL::vertex( a.Pos[0], a.Pos[1], a.Pos[2] );
          GL::normal( b.Norm[0], b.Norm[1], b.Norm[2] );
          GL::vertex( b.Pos[0], b.Pos[1], b.Pos[2] );
          GL::normal( c.Norm[0], c.Norm[1], c.Norm[2] );
          GL::vertex( c.Pos[0], c.Pos[1], c.Pos[2] );
          if (!(iter&1)) bChecker = !bChecker;
     }
     glEnd();
     
  }   

} //gfx::
