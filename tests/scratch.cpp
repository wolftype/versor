#include <stdio.h>   
#include "products_all.h"

typedef double VT;


using namespace vsr;   

constexpr auto aa = Pnt(1,1,1,1,1); 

constexpr Pnt fun( const Pnt& p){
	return p;
}               

auto pp = fun(aa);    

// template< class A>
// constexpr auto dle(const A& a) RETURNS(
// 	a * Tri(-1)
// )  

int main(){      
	
	Tri(-1).vprint();
 
}