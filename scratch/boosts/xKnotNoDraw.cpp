#include "vsr_cga3D_op.h"

#include "vsr_GLVimpl.h" 
#include "vsr_cga3D_draw.h"  
#include "vsr_cga3D_interface.h"   
//                         
//                    
#include "vsr_knot.h"

using namespace vsr;

int main(){
	
	TorusKnot tk(3,2,.001); 

	auto a = Ro::null(2,0,0);     

	tk.calc( a );

	for (int i = 0; i < tk.iter(); ++i){
		tk.cir[i].vprint();
	}
}