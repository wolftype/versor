/*
 * =====================================================================================
 *
 *       Filename:  vsr_draw.h
 *
 *    Description: instantiations of Drawable for Mulvector types (calls Immediate(...)) and Field<T>
 *
 *        Version:  1.0
 *        Created:  04/17/2015 12:11:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "gfx/gfx_render.h"

#include "detail/vsr_multivector.h"
#include "form/vsr_field.h"
 
#ifndef  vsr_draw_INC
#define  vsr_draw_INC

namespace gfx{


// template<class A, class B> 
// void Renderable< vsr::MV<A,B> >::DrawImmediate( const vsr::MV<A,B>& t){
//    vsr::cga::Immediate(t);
// }

 
 //Draw Routine (Immediate Mode)
 template<class A>
 void Draw( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
   render::begin(r,g,b,a);
   render::draw(s); //defined in gfx_render.h
 }   

 //Draw Routine (Immediate Mode)
 template<class A>
 void DrawB( const A& s, float r = 1, float g = 1, float b = 1, float a = 1){
   render::begin(r,g,b,a);
   render::drawB(s); //defined in gfx_render.h
 }   

 //Draw Routine (Immediate Mode)
 template<class A, class B>
 void DrawAt( const A& s, const B& p, float r = 1, float g = 1, float b = 1, float a = 1){
   render::begin(r,g,b,a);
   render::drawAt(s,p);
 }  

  //Draw Routine (Immediate Mode)
 template<class A, class B>
 void DrawBAt( const A& s, const B& p, float r = 1, float g = 1, float b = 1, float a = 1){
   render::begin(r,g,b,a);
   render::drawBAt(s,p);
 }  
  /*-----------------------------------------------------------------------------
   *  General Drawing strategy for a type T calls "Advanced" method from RenderCall<T>
   *-----------------------------------------------------------------------------*/
// template<class A, class B>
// struct Renderable< vsr::Multivector<A,B> >{
//   static inline void Draw(const vsr::Multivector<A,B>& t, GFXSceneNode * s){
//     vsr::RenderCall<vsr::MV<A,B>>::Piped(t,s);
//   }
// };
  

  /*-----------------------------------------------------------------------------
   *  General drawing strategy for Fields of type T
   *-----------------------------------------------------------------------------*/

  template<class T> 
  struct Renderable<vsr::Field<T>,0> : RenderableBase< vsr::Field<T> > {
    
    static void DrawImmediate( const vsr::Field<T>& f){
      for (int i = 0; i < f.num(); ++i){ 
        glPushMatrix();
        Renderable<T>::DrawImmediate( f[i] );
        glPopMatrix(); 
      }
    }

    static void Draw( const vsr::Field<T>& f, GFXSceneNode*){}
  };

//  template<class T> 
//  struct Renderable< vsr::Field<T> >{
//    static inline void DrawImmediate( const vsr::Field<T>& f ){
//       for (int i = 0; i < f.num(); ++i){ 
//          render::draw( f[i] );
//      }
//    }
//  };



}

#endif   /* ----- #ifndef vsr_draw_INC  ----- */
