#include "vsr_cga2D_types.h"  


using namespace vsr;

int main(){
	
	
	auto a = Ro::point(1,0);
	         
	auto b = Ro::point(0,1);
	         
	auto c = Ro::point(-1,0);  


	(a ^ b ^ c).vprint();
	auto cir = a^b^c;
	
	 cir.vprint();  
   
 	
	printf("direction\n");
	Drv::bprint();   
	printf("tangent\n");
	Tnv::bprint();
	printf("bivector\n");  
	Biv::bprint();  
	                  
 
	printf("line (grade 3)\n");
	Lin::bprint();  
	printf("dualLine (grade 1)\n");
	Dll::bprint();
    printf("circle (3-blade)\n");
	Cir::bprint();  	
	printf("pair (2-blade)\n");
	Par::bprint();
	printf("pseudocalar (4-blade)\n");
	Sph::bprint();  

	printf("rotor (s + biv)\n");
	auto r = Gen::rot( Biv(1) ); r.bprint();
	
	printf("translator (s + drv)\n");
	auto t = Gen::trs( Drv(1,1) ); t.bprint();    
	
	printf("motor (s + biv + drv )\n");
	( t * r).bprint();
		
	printf("transversor (s + tnv)\n");
	Gen::trv( Tnv(1,1) ).bprint();  
	
	printf("boost (s + par)\n");
	Gen::bst( Par(1,1) ).bprint();
	                                  
	( Lin() * Sph() ).bprint();  
	
	Par( Tnv()).sp( Gen::trs( Drv(1,0) ) ).bprint();

  printf("dual pair\n");
  (Op::dual( Par() ).bprint() );
	
	// ( Pnt() * Sph() ).bprint();
	// 
	// Par tp = b ^ c; 
	// //(tp+.1).vprint();
	// auto bst = Gen::bst( tp ); 
	// bst.vprint();
	// bst.bprint();             
	// 
	// // Sca s = (Vec(1,0)*Vec(.5,1)).get<0>();
	// // s.vprint(); 
	// 
	// auto ra =  Gen::ratio( Vec(1,0), Vec(0,1) );
	//     ra.vprint(); 
	// ra.bprint();   
	
	// //( Tnv(1,0) + 1 ).bprint(); 
	// for (int i = 0; i < 100; ++i){
	// 	float t = 1.0 * i/100;
	//     Pnt np = a.sp( Gen::bst( Pair( b, c ) * t ) ); 
	// 	
	// 	np /= np.get< 4 >();
	//    // np.vprint();
	// }  
	
}
