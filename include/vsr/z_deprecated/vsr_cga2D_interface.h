/*
 * =====================================================================================
 *
 *       Filename:  vsr_cga2D_interface.h
 *
 *    Description:  keyboard and mouse interface (rough)
 *
 *        Version:  1.0
 *        Created:  02/04/2014 13:17:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef VSR_CGA2D_INTERFACE_H_INCLUDED
#define VSR_CGA2D_INTERFACE_H_INCLUDED        

#include "gfx/gfx_interface.h" 

#include "vsr_cga2D.h"  

#include <sstream>
 

namespace gfx{    
   
  using namespace vsr; 
  using namespace vsr::cga2D;
  
 
  //gfx interface (should be its own lib) templates implementation on X<A,B>
  //

  //FOR ALL MULTIVECTOR TYPES:
  template<class A, class B> 
  void Interface :: X <A,B> :: f ( A * s, const B& pos, float t ){
     
      //Address of State
      A& ts = *s;

      //Center of Defining Sphere   
      Pnt pnt = Ro::loc(pos);
  
      Vec tv = pnt;   
              
      //2D coordinates of Defining Sphere
      Vec3f sc = i->screenCoord2D(tv, i->scene -> xf );  

     // printf("selected\n");
      
      //Point in 3D space on Projection Ray closest to pos.
     // Pnt cp  = Fl::loc( vd().ray, Ro::loc(pos), 1);  


          switch(i->keyboard.code){

              case 's': //SCALE
              {
                  Vec3f tm1 = i->mouse.pos + i->mouse.drag - sc;
                  Vec3f tm2 = i->mouse.pos - sc; 

                  //Drag towards or away from element . . . 
                  int neg = (tm1.len() > tm2.len()) ? 1 : -1; 
                  auto tsd = Gen::dil( pnt, i->mouse.drag.len() * t * neg );
                  ts = ts.sp( tsd );

                  break;
              }
              case 'g': //TRANSLATE
              {    
                  Vec3f mdc = i->mouse.dragCat * t;
                  ts = ts.trs(  mdc[0], mdc[1] ); 
                  break;
              }
              case 'r': //ROTATE local line
              {    
           // cout << vd().z << mouse.dragCat << mouse.dragBivCat << endl; 
                  Dll td = pos <= ( Drv( i->mouse.dragBivCat[0], i->mouse.dragBivCat[1] ).undual()  * t );
//                  ts = ts.mot( td );
                  break;
              }

              default://case 'q': //DESELECT
              {
                  i->toggleSelect(s);
                  break;
              }
          }
  
  }  

  
}  

namespace vsr{

  /* void Touch(gfx::Interface& interface, Vec& v, float t = .5); */  
  /* void Touch(gfx::Interface& interface, Tnv& v, float t = .5); */  
  /* void Touch(gfx::Interface& interface, Drv& v, float t = .5); */  
  /* void Touch(gfx::Interface& interface, Cir& c, float t = .5 ); */
  /* void Touch(gfx::Interface& interface, Par& c, float t = .5 ); */  
  /* void Touch(gfx::Interface& interface, Pnt& c, float t = .5 ); */
  /* void Touch(gfx::Interface& interface, Lin& d,  float t = .5); */
  /* void Touch(gfx::Interface& interface, Dll& d,  float t = .5); */
  
  inline void Touch(gfx::Interface& interface, Vec& v, float t = .5 ){
	interface.touch(v, Ro::dls(v,.5), t);
   }
  inline void Touch(gfx::Interface& interface, Tnv& v, float t = .5  ){
	interface.touch(v, Ro::dls(v,.5), t);
   }
  inline void Touch(gfx::Interface& interface, Drv& v, float t = .5  ){
	interface.touch(v, Ro::dls(v,.5), t);
   }
  inline	void Touch(gfx::Interface& interface, Cir& c, float t = .5  ){
		interface.touch( c, Ro::sur(c), t );
	}
  inline	void Touch(gfx::Interface& interface, Par& c, float t = .5  ){
		interface.touch( c, Ro::sur(c), t );
	} 
  inline	void Touch(gfx::Interface& interface, Pnt& c, float t = .5  ){
		interface.touch( c, Ro::dls_pnt( Ro::loc(c), .2), t );
	}
  inline	void Touch(gfx::Interface& interface, Lin& d,  float t  = .5 ){
		interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), false) ), t );
	}	
  inline	void Touch(gfx::Interface& interface, Dll& d,  float t  = .5 ){
		interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), true) ), t );
	}	
}


#endif



// OLD

  // void Touch(gfx::Interface& interface, Frame& v, float t = .5){
  // interface.touch(v, v.bound(), t);
  //    }
  // 
  //    void Touch(gfx::Interface& interface, Vec& v, float t = .5){
  // interface.touch(v, Ro::dls(v,.5), t);
  //    }
  //    void Touch(gfx::Interface& interface, Tnv& v, float t = .5){
  // interface.touch(v, Ro::dls(v,.5), t);
  //    }
  //    void Touch(gfx::Interface& interface, Drv& v, float t = .5){
  // interface.touch(v, Ro::dls(v,.5), t);
  //    }
  // 
  // void Touch(gfx::Interface& interface, Cir& c, float t = .5){
  //   interface.touch( c, Ro::sur(c), t );
  // }
  // void Touch(gfx::Interface& interface, Par& c, float t = .5){
  //   interface.touch( c, Ro::sur(c), t );
  // } 
  // 
  // void Touch(gfx::Interface& interface, Pnt& c, float t = .5){
  //   interface.touch( c, Ro::dls_pnt( Ro::loc(c), .2), t );
  // }
  // void Touch(gfx::Interface& interface, Sph& c, float t = .5){
  //   interface.touch( c, c.dual(), t );
  // }   
  // 
  // void Touch(gfx::Interface& interface, Lin& d,  float t= .5){
  //   interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), false) ), t );
  // }  
  // void Touch(gfx::Interface& interface, Pln& d,  float t= .5){
  //   interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), false) ), t );
  // }
  // void Touch(gfx::Interface& interface, Dll& d,  float t= .5){
  //   interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), true) ), t );
  // }  
  // void Touch(gfx::Interface& interface, Dlp& d,  float t= .5){
  //   interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), true) ), t );
  // } 
