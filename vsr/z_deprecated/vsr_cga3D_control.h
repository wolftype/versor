/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga3D_control.h
 *
 *    Description:  bindings to gfx_control.
 *
 *
 *        Version:  1.0
 *        Created:  11/17/2014 12:22:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  pretty awesome
 *
 * =====================================================================================
 */


#ifndef VSR_CGA3D_CONTROL_H_INCLUDED
#define VSR_CGA3D_CONTROL_H_INCLUDED        

#include <sstream>

#include "gfx/gfx_objectController.h"

#include "vsr_cga3D_op.h" 
#include "vsr_cga3D_frame.h" 

namespace gfx{    
   
  using namespace vsr;
  using vsr::Vec; 

  //FOR ALL ROUND MULTIVECTOR TYPES:
  template<class A>  
  Vec3f ObjectController :: ObjectPtr<A> :: worldPosition(){
    auto p = Ro::loc(*mAddress);
    return Vec3f(p[0],p[1],p[2]);
  }

  /// FOR FRAMES
  template<>  
  Vec3f ObjectController :: ObjectPtr<Frame> :: worldPosition(){
    auto p = mAddress->pos();
    return Vec3f(p[0],p[1],p[2]);
  }

 
  //FOR ALL MULTIVECTOR TYPES:
  template<class A>  
  void ObjectController :: ObjectPtr<A> :: transform (){

      //Address of State
      A& ts = *mAddress;
      //Temp Stored State
      A& ps = tObject;

      //Center of Defining Sphere   
      Pnt pnt = Ro::loc(ts); //was pos
              
      //2D coordinates of Defining Sphere
      Vec3f sc = i->mScene->project(pos);  

          switch(i->io().keyboard.code){

              case 's': //SCALE
              {

                  Vec3f tm1 = i->io().pos() - sc; 
                  Vec3f tm2 = i->io().click() - sc; 

                  //Drag towards or away from element . . . 
                  int neg = (tm1.len() > tm2.len()) ? 1 : -1; 
                  float amt =  i->io().drag().len() * neg;
                  auto tsd = Gen::dil( pnt, amt);

                  ts = ps.sp( tsd );

                  break;
              }
              case 't': //TRANSLATE
              {    
                  Vec3f mdc = i->dragCat() * 10; 
                  ts = ps.trs(  mdc[0], mdc[1], mdc[2] ); 
                  break;
              }
              case 'r': //ROTATE 
              {    
                  auto b = i->axisCat();
                  Dll td = pnt <= (Drv(b[0], b[1], b[2]).dual() );
                  ts = ps.mot( td );
                  break;
              }

              default: // DESELECT
              {
               //   i->deselect(mAddress);
                  break;
              }
          }
  
  }  


    template<>
    void ObjectController :: ObjectPtr<Frame> :: transform() {
     
      //Address of State
      Frame& ts = *mAddress;
      //Temp Stored State
      Frame& ps = tObject;

      //Center  
      Pnt pnt = ps.pos();
              
      //2D coordinates of Defining Sphere
      Vec3f sc = i->mScene->project(pos);  
 

        switch(i->io().keyboard.code){
          case 's': //SCALE
          {
              Vec3f tm1 = i->io().pos() - sc; 
              Vec3f tm2 = i->io().click() - sc; 

              //Drag towards or away from element . . . 
              int neg = (tm1.len() > tm2.len()) ? 1 : -1; 
              float amt =  i->io().drag().len() * neg;
              ts = ps.dilate( amt );

              break;

          }
          case 't': //TRANSLATE
          {   
              Vec3f mdc = i->dragCat() * 10;
              ts.pos() = ps.pos().trs( mdc[0], mdc[1], mdc[2] );
              break;
          }
          case 'r': //ROTATE 
          {    
              auto b = i->axisCat();
              Rot tr = Gen::rot( Vec(b[0],b[1],b[2]).duale() );
              ts.rot() = tr * ps.rot();
              break;
          }

          default://case 'q': //DESELECT
          {
              i->toggleSelect(mAddress);
              break;
           }
        }

    };


}  



#endif




