//render using advanced es2.0  

#include "products_all.h"
#include "gfx/gfx_pipe.h"
#include "vsr_xf.h"

namespace vsr{     
	
   	template<> void Render(const vsr::Cir& cir, const Mat4f& mvm, Pipe& pipe )	{
        
		static MBO circle ( Mesh::Disc(.5) );
		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
		
		mat = mvm * tmp.copy( vsr::Xf::mat(cir) );
		mat.fill(mv);
		pipe.program -> uniform("modelView", mv );    
		pipe.line( circle );                    
	} 
	
}