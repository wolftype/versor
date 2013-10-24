#ifndef VSR_CGA3D_INTERFACE_H_INCLUDED
#define VSR_CGA3D_INTERFACE_H_INCLUDED        

#include "gfx/gfx_interface.h" 

#include "vsr_cga3D_op.h" 
#include "vsr_frame.h" 

#include <sstream>
 

namespace gfx{    
   
  using namespace vsr; 
  

 
  template<class A, class B> 
  void Interface :: 
  X <A,B> :: f ( A * s, const B& pos, float t ){
	   
      //Address of State
      A& ts = *s;

      //Center of Defining Sphere   
	  Pnt pnt = Ro::loc(pos);
	
      Vec tv = pnt;   
              
      //2D coordinates of Defining Sphere
      Vec3f sc = i->screenCoord(tv, i->scene -> xf );  

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
	                ts = ts.trs(  mdc[0], mdc[1], mdc[2] ); 
	                break;
	            }
	            case 'r': //ROTATE local line
	            {    
				   // cout << vd().z << mouse.dragCat << mouse.dragBivCat << endl; 
	                Dll td = pos <= ( Drv( i->mouse.dragBivCat[0], i->mouse.dragBivCat[1], i->mouse.dragBivCat[2] ).undual()  * t );
	                ts = ts.mot( td );
	                break;
	            }

	            default://case 'q': //DESELECT
	            {
	                i->toggleSelect(s);
	                break;
	            }
	        }
	
  }  

	  template< class B >
	  struct Interface :: X< Frame, B> {
		Interface * i;
		X( Interface * _i ) : i(_i) {}
		void  f( Frame * s, const B& pos, float t ){

		   //Address of State
	      Frame& ts = *s;

	      //Center of Defining Sphere   
		  Pnt pnt = Ro::loc(pos);

	      Vec tv = pnt;   

	      //2D coordinates of Defining Sphere
	      Vec3f sc = i->screenCoord(tv, i->scene -> xf );  


	        switch(i->keyboard.code){

	            case 's': //SCALE
	            {
	                Vec3f tm1 = i->mouse.pos + i->mouse.drag - sc;
	                Vec3f tm2 = i->mouse.pos - sc; 

	                //Drag towards or away from element . . . 
	                int neg = (tm1.len() > tm2.len()) ? 1 : -1;                  
					ts.dilate( i->mouse.drag.len() * t * neg  );

	                break;
	            }
	            case 'g': //TRANSLATE
	            {   
					Vec3f mdc = i->mouse.dragCat * t;
	                ts.pos() = ts.pos().trs( mdc[0], mdc[1], mdc[2] );
	                break;
	            }
	            case 'r': //ROTATE local line
	            {    
				   // cout << vd().z << mouse.dragCat << mouse.dragBivCat << endl; 
	                //Dll td = pos <= ( Drv( i->mouse.dragBivCat[0], i->mouse.dragBivCat[1], i->mouse.dragBivCat[2] ).undual()  * t );
	                //ts.rot() = Rot( Gen::mot(td) ) * ts.rot();
					Rot tr = Gen::rot( Vec( i->mouse.dragBivCat[0],i->mouse.dragBivCat[1],i->mouse.dragBivCat[2] ).duale() );//( Vec::z ^ Vec( i->mouse.drag[0], i->mouse.drag[1], 0 ) ) );
				    ts.rot() = tr * ts.rot();
					break;
	            }

	            default://case 'q': //DESELECT
	            {
	                i->toggleSelect(s);
	                break;
	            }
	        } 
			
		}
	  };
	
	//   template< class B> 
	//   void Interface :: 
	//   X <Frame, B> :: f( Frame * s, const B& pos, float t ){
	// 	printf("frame\n");
	// }     
	
}  

namespace vsr{

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
	// 	interface.touch( c, Ro::sur(c), t );
	// }
	// void Touch(gfx::Interface& interface, Par& c, float t = .5){
	// 	interface.touch( c, Ro::sur(c), t );
	// } 
	// 
	// void Touch(gfx::Interface& interface, Pnt& c, float t = .5){
	// 	interface.touch( c, Ro::dls_pnt( Ro::loc(c), .2), t );
	// }
	// void Touch(gfx::Interface& interface, Sph& c, float t = .5){
	// 	interface.touch( c, c.dual(), t );
	// }   
	// 
	// void Touch(gfx::Interface& interface, Lin& d,  float t= .5){
	// 	interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), false) ), t );
	// }	
	// void Touch(gfx::Interface& interface, Pln& d,  float t= .5){
	// 	interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), false) ), t );
	// }
	// void Touch(gfx::Interface& interface, Dll& d,  float t= .5){
	// 	interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), true) ), t );
	// }	
	// void Touch(gfx::Interface& interface, Dlp& d,  float t= .5){
	// 	interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), true) ), t );
	// }   
	void Touch(gfx::Interface& interface, Frame& v, float t = .5);  
	void Touch(gfx::Interface& interface, Frame& v, const Frame& e, float t = .5); 
	void Touch(gfx::Interface& interface, Vec& v, float t = .5);	
	void Touch(gfx::Interface& interface, Tnv& v, float t = .5);	
	void Touch(gfx::Interface& interface, Drv& v, float t = .5);	
	void Touch(gfx::Interface& interface, Cir& c, float t = .5 );
	void Touch(gfx::Interface& interface, Par& c, float t = .5 );	
	void Touch(gfx::Interface& interface, Pnt& c, float t = .5 );
	void Touch(gfx::Interface& interface, Sph& c, float t = .5 );
	void Touch(gfx::Interface& interface, Lin& d,  float t = .5);
	void Touch(gfx::Interface& interface, Pln& d,  float t = .5);
	void Touch(gfx::Interface& interface, Dll& d,  float t = .5);
	void Touch(gfx::Interface& interface, Dlp& d,  float t = .5);   
}


#endif