/*

	space time algebra
    
	R1,3
	

*/
#include "vsr_products.h"
 
namespace vsr{
	
	struct STA : MGA< typename RMetric<1,3>::Type >{
         
		/*STANDARD FRAME of Orthonormal vectors decribes an event
	      A vec can be a proper momentum ( energy-momentum vector ) of a particle 
		  or
		  proper velocity
		*/
		
		//Dirac Matrices
		 
	     using Y0 = e<1>; 								//timelike observer (squares to 1) 
	  													 //forward light cone
	                
         using Y1 = e<2>; 								//spacelike observer   
		 using Y2 = e<3>; 								//spacelike observer   
		 using Y3 = e<4>; 								//spacelike observer  
		
		//Right-handed unit pseudoscalar
		 using I = MV< pss(4) >; 
       
		//position space vectors from spacetime bivectors yk^y0 
		using O1 = typename OProd< Y1, Y0, Metric, false >::Type;       //yk^y0...
		using O2 = typename OProd< Y2, Y0, Metric, false >::Type;
		using O3 = typename OProd< Y3, Y0, Metric, false >::Type;  
		
		using P3 = typename OProd< Vec, Y0, Metric, false >::Type;  	

	}; 
	                                            
	
	typedef MGAMV< STA::Metric, STA::Vec > Vec; 
		
	typedef MGAMV< STA::Metric, STA::Sca > Sca; 
	typedef MGAMV< STA::Metric, STA::Y0 > Y0; 
	typedef MGAMV< STA::Metric, STA::Y1 > Y1;
	typedef MGAMV< STA::Metric, STA::Y2 > Y2;
	typedef MGAMV< STA::Metric, STA::Y3 > Y3;

	
	typedef MGAMV< STA::Metric, STA::O1 > O1;
	typedef MGAMV< STA::Metric, STA::O2 > O2;
	typedef MGAMV< STA::Metric, STA::O3 > O3;  
	
	typedef MGAMV< STA::Metric, STA::I > I;
	
	typedef MGAMV< STA::Metric, STA::P3 > P3;
	  

}


