#include "interface/vsr_cga3D_interface.h"

namespace vsr{
	
	void Touch(gfx::Interface& interface, Frame& v, float t){
	interface.touch(v, v.bound(), t);
   } 

	void Touch(gfx::Interface& interface, Frame& v, const Frame& c, float t){
	interface.touch(v, c.pos(), t);
   }
  
   void Touch(gfx::Interface& interface, Vec& v, float t ){
	interface.touch(v, Ro::dls(v,.5), t);
   }
   void Touch(gfx::Interface& interface, Tnv& v, float t ){
	interface.touch(v, Ro::dls(v,.5), t);
   }
   void Touch(gfx::Interface& interface, Drv& v, float t ){
	interface.touch(v, Ro::dls(v,.5), t);
   }
	
	void Touch(gfx::Interface& interface, Cir& c, float t ){
		interface.touch( c, Ro::sur(c), t );
	}
	void Touch(gfx::Interface& interface, Par& c, float t ){
		interface.touch( c, Ro::sur(c), t );
	} 
	
	void Touch(gfx::Interface& interface, Pnt& c, float t ){
		interface.touch( c, Ro::dls_pnt( Ro::loc(c), .2), t );
	}
	void Touch(gfx::Interface& interface, Sph& c, float t ){
		interface.touch( c, c.dual(), t );
	}   

	void Touch(gfx::Interface& interface, Lin& d,  float t){
		interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), false) ), t );
	}	
	void Touch(gfx::Interface& interface, Pln& d,  float t){
		interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), false) ), t );
	}
	void Touch(gfx::Interface& interface, Dll& d,  float t){
		interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), true) ), t );
	}	
	void Touch(gfx::Interface& interface, Dlp& d,  float t){
		interface.touch( d, Ro::dls_pnt( Fl::loc(d, Ori(1), true) ), t );
	}
}
