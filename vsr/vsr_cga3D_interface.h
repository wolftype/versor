#ifndef VSR_CGA3D_INTERFACE_H_INCLUDED
#define VSR_CGA3D_INTERFACE_H_INCLUDED        

#include "gfx/gfx_interface.h"
#include "vsr_cga3D_types.h"  

#include <sstream>
 

namespace gfx{    
   
  using namespace vsr; 
   
  template<class A, class B>
  void Interface :: xf ( A * s, const B& pos, float t ){
	   
      //Address of State
      A& ts = *s;
      
 		//stringstream tts; tts << s;
		//std::cout << "address: " << tts.str() << std::endl;
      //Center of Defining Sphere   
	  Pnt pnt = Ro::loc(pos);
	
      Vec tv = pnt;   
              
      //2D coordinates of Defining Sphere
      Vec3f sc = screenCoord(tv, scene -> xf );  

     // printf("selected\n");
      
      //Point in 3D space on Projection Ray closest to pos.
     // Pnt cp  = Fl::loc( vd().ray, Ro::loc(pos), 1);  


	        switch(keyboard.code){

	            case 's': //SCALE
	            {
	                Vec3f tm1 = mouse.pos + mouse.drag - sc;
	                Vec3f tm2 = mouse.pos - sc; 

	                //Drag towards or away from element . . . 
	                int neg = (tm1.len() > tm2.len()) ? 1 : -1; 
	                auto tsd = Gen::dil( pnt, mouse.drag.len() * t * neg );
	                ts = ts.sp( tsd );

	                break;
	            }
	            case 'g': //TRANSLATE
	            {  
	                ts = ts.trs( mouse.dragCat * t );
	                break;
	            }
	            case 'r': //ROTATE local line
	            {    
				   // cout << vd().z << mouse.dragCat << mouse.dragBivCat << endl; 
	                Dll td = pos <= ( Drv( mouse.dragBivCat[0], mouse.dragBivCat[1], mouse.dragBivCat[2] ).dual()  * t );
	                ts = ts.mot( td );
	                break;
	            }

	//            case 'b': //boost by drag (not working)
	//            {
	//                Tnv tnv( mouse.dragCat );
	//                
	//                Bst pp = Gen::trv( Op::sp( tnv, Gen::trs( cp ) ) * t );
	//                ts = Op::sp (ts, pp);
	//                glPushMatrix();
	//				glTranslated(cp[0],cp[1],cp[2]);
	//				//tnv.draw();
	//                glPopMatrix();
	//                break;
	//            }

	            // case 't': // twist about global line (experimental)
	            // {
	            //     Dll td = Op::dl( mouse.origin ^ mouse.dragCat ^ Inf(1) );
	            //     ts = Op::sp ( ts, Gen::mot(td) );
	            //     break;
	            // }  

	            default://case 'q': //DESELECT
	            {
	                toggleSelect(s);
	                break;
	            }
	        }
	
  }
	
}  

namespace vsr{
  
   void Touch(gfx::Interface& interface, Vec& v, float t = .5){
	interface.touch(v, Ro::dls(v,.5), t);
   }
   void Touch(gfx::Interface& interface, Tnv& v, float t = .5){
	interface.touch(v, Ro::dls(v,.5), t);
   }
   void Touch(gfx::Interface& interface, Drv& v, float t = .5){
	interface.touch(v, Ro::dls(v,.5), t);
   }
	
	void Touch(gfx::Interface& interface, Cir& c, float t = .5 ){
		interface.touch( c, Ro::sur(c), t );
	}
	void Touch(gfx::Interface& interface, Par& c, float t = .5 ){
		interface.touch( c, Ro::sur(c), t );
	} 
	
	void Touch(gfx::Interface& interface, Pnt& c, float t = .5 ){
		interface.touch( c, Ro::dls_pnt( Ro::loc(c), .2), t );
	}
	void Touch(gfx::Interface& interface, Sph& c, float t = .5 ){
		interface.touch( c, c, t );
	}	

	void Touch(gfx::Interface& interface, Lin& d,  float t = .5){
		interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), false) ), t );
	}	
	void Touch(gfx::Interface& interface, Pln& d,  float t = .5){
		interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), false) ), t );
	}
	void Touch(gfx::Interface& interface, Dll& d,  float t = .5){
		interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), true) ), t );
	}	
	void Touch(gfx::Interface& interface, Dlp& d,  float t = .5){
		interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), true) ), t );
	}
}


#endif