#define __CON__  
#define GLOBAL_METRIC RMetric<4,1>::Type()

#include "operations.h"
#include <iostream>

using namespace vsr;
using namespace std;
   

typedef Blade1<4>::VEC Vec4;

//constexpr int test() { return 2; }

int main(){
	
	//Mot d;
	auto a = Pss(1);
	
	(~a).vprint();
	(!a).vprint();  
	
	(~a * a ).vprint();

	return 0;
}