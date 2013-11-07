/*

	space time algebra
    
	R1,3
	
 SEE Hestenes Space Time Algebra
*/
#include "vsr_products.h"
 
namespace vsr{
	
	namespace sta {
	struct STA : MGA< typename RMetric<1,3>::Type >{
         
		/*STANDARD FRAME of Orthonormal vectors decribes an Event ( time + location )
	      An event, "proper vector" can be a proper momentum ( energy-momentum vector ) of a particle 
		  or a proper velocity
		*/        
		                         
		
		//"Proper Vector"
		using Event = Vec;
		
		//Dirac Matrices are the Y . . . 
	     using Y0 = e<1>; 								//timelike observer (squares to 1) 
	  													 //forward light cone
	                
         using Y1 = e<2>; 								//spacelike    
		 using Y2 = e<3>; 								//spacelike    
		 using Y3 = e<4>; 								//spacelike   
		
		//Right-handed unit pseudoscalar
		 using I = MV< pss(4) >; 
       
		//individual position space vectors are made from spacetime bivectors yk^y0 
		// (considered "relative vectors..." -- relative to y0 . . . 
		//These generate G3 as G(P3)
		using O1 = typename OProd< Y1, Y0, Metric, false >::Type;       //yk^y0...
		using O2 = typename OProd< Y2, Y0, Metric, false >::Type;
		using O3 = typename OProd< Y3, Y0, Metric, false >::Type;  
		
		//Position space of observer y0
		using P3 = typename OProd< Event, Y0, Metric, false >::Type; 
		
		// SPLIT
		
		//a y0 spacetime split (time + position) is made from an observer's time axis Event * Y0 
		using Split = typename Prod< Event, Y0, Metric, false>::Type;      
		//lorentz-invariant means: independent of a chosen spacetime split

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

	typedef MGAMV< STA::Metric, STA::Event > Event;
	
	typedef MGAMV< STA::Metric, STA::Split > Split;  	  
   
}

}


