#include "vsr_root.h"
#include <iostream>  

using namespace vsr;  
using namespace std;  
 

typedef EGAMV< 4, Blade1<4>::VEC > V4;

auto H4 = Root::System( V4(0,-1,0,0), V4(0,1.618,1,-.618) * .5, V4(0,0,-1,0), ( V4(-.618,0,1, 1.618) * .5 ) );
auto D4 = Root::System( V4(1,0,0,0), V4(0,1,0,0), V4(0,0,1,0), ( V4(-1,-1,-1, 1) * .5 ) );

int main(){
	 
	cout << "\nD4: NumRoots = " << D4.size() << endl;
	cout << "\nH4: NumRoots = " << H4.size() << endl;

	for (auto i : D4 ){
//		i.vprint();
	}

  for (auto i : H4 ){
//		i.vprint();
	}
}
