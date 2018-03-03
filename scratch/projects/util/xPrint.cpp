#include "vsr_products.h"


using namespace vsr;
using namespace std;   



int main(){
    
	printf("%d\n", blade(1,2));

	printf("\n\n\n//////////////////////////////\nBIT REPRESENTATIONS OF G3\n");      
	//Euclidean GA R3
	EGA<3>::Sca s; 		//Scalar
	
	EGA<3>::e<1> e1;   //x
	EGA<3>::e<2> e2;   //y
	EGA<3>::e<3> e3;   //z  
	
	EGA<3>::e<1,2> e12;   //xy
	EGA<3>::e<1,3> e13;   //xz
	EGA<3>::e<2,3> e23;   //yz
	
	EGA<3>::e<1,2,3> e123; ///xyz                       

	printf("scalar:\t"); s.bprint();   	                             
	printf("e1:\t"); e1.bprint();
	printf("e2:\t"); e2.bprint();
	printf("e3:\t"); e3.bprint();   
	
	printf("e12:\t"); e12.bprint();
	printf("e13:\t"); e13.bprint();
	printf("e23:\t"); e23.bprint();

	printf("e123:\t"); e123.bprint(); 
		
	//Vector
	EGA<3>::Vec vec;
	
	printf("Vec:\n"); vec.bprint();
 
	//Bivectors
	EGA<3>::Biv biv;
	printf("Bivector:\n"); biv.bprint();  
	
	//Rotor (Quaternion)
	EGA<3>::Rot rot;
	printf("Rotor:\n"); rot.bprint();  
	
	
	 
	
	
	return 0;
}