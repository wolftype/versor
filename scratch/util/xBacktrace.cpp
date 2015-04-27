#include "vsr_cga3D_types.h"

     

using namespace vsr;

int main(){
	
	
	// typedef typename EGA<3>::Vec Vec;
	// typedef typename EGA<3>::Rot Rot; 
	
	// typedef typename EProd< Rot, Vec >::Type RVec;  
	// 
	// Vec::bprint(); Rot::bprint();  RVec::bprint();
	//    
	//     typedef EProd< RVec, Rot> InstruxA;   
	//  
	// typedef REProd< RVec, Rot, Vec> InstruxB; 
	 
    typedef Prod< decltype( Bst() * Pnt() ), Bst, typename CGA<5>::M, true> InstruxA; 

	typedef RProd< decltype( Bst() * Pnt() ), Bst, Pnt, typename CGA<5>::M, true> InstruxB;

	InstruxA::DO::print();
	
	InstruxB::DO::print();
	
	
}