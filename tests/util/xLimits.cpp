#include "vsr_products.h"



 
namespace vsr{

extern template class CGAMV<5, typename CGA<5>::Pnt>; 
              
}   

using namespace vsr;

typedef NPnt<5> Pnt; 

typedef NBst<5> Bst;

int main(){
	 
	// static const TT DIM =14;
	// 
	//     typedef typename Blade1< DIM >::VEC Vec;
	// 
	//     typedef typename EProd< Vec, Vec > ::Type Rot;
	// 
	//     Rot::bprint();
	// Vec::bprint();  
	
	Pnt p = Pnt().spin( Bst() );
	
	Pnt().bprint(); 
	( Pnt() * Pnt() ).bprint();
}