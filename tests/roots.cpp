#define __EUC__

#include "root.h"

using namespace vsr;   

typedef Blade1<4>::VEC Vec4;

//idositetrachoron 24 cell
auto D4 = rootSystem<4>( Vec4(1,0,0,0), Vec4(0,1,0,0), Vec4(0,0,1,0), ( Vec4(-1,-1,-1, 1) * .5 ) );
 
auto F4 = rootSystem<4>( Vec4(0,1,-1,0), Vec4(1,-1,0,0), Vec4(0,0,1,0), ( Vec4(-1,-1,-1, 1) * .5 ) );

int main(){
    
	cout << "D4: NumRoots =" << D4.size() << endl;
   for (auto i : D4 ){
		
	 i.vprint();
	
	}

	cout << "\nF4: NumRoots = " << F4.size() << endl;
   for (auto i : F4 ){

	 i.vprint();

	}

	
}