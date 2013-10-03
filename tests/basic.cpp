#include "vsr_products.h"
#include "vsr_ncube.h"  
//#include <vector>

using namespace vsr;
using namespace std;

// 
// struct Edge{                                            
//   int a, b;  
//   Edge(int _a, int _b) : a(_a), b(_b){}  
// 
// 	void print() { printf("%d, %d\n", a, b);}
// };
// 
// template<int DIM>
// struct NCube{
// 	typedef EGAMV< typename Blade1<DIM>::VEC > TVec;
// 	vector<TVec> roots; 
// 	vector<Edge> edges;
// 	
// 	int NumVert = pow(2.f,DIM); 
// 	
// 	NCube() { printf("DIM: %d\n", DIM); init(); }
// 	
// 	void init(){
// 		TVec va, vb;
// 		va[DIM-1] = -.5;
// 		vb[DIM-1] = .5;
// 		
// 		int ix = 0; 
// 		
// 		auto next = NCube<DIM-1>();
// 		
// 		for (auto i : next.roots ){
// 		   
// 		 	roots.push_back( TVec(i+va) );
// 			roots.push_back( TVec(i+vb) ); 
// 	
// 		} 
// 	   
// 	 	int d = pow(2.f, DIM-1);   
// 		for (auto i : next.edges ){        
//   		
// 			edges.push_back(i); 
// 			edges.push_back( Edge(i.a + d, i.b + d));   
// 
// 		}                                                
// 		
// 		for (int i = 0; i < next.NumVert; ++i ){
// 			edges.push_back( Edge(i, i + d) );
// 		}
// 		
// 	} 
//  
// };   
// 
// 
// template<>
// struct NCube<1>{
// 	typedef EGAMV< typename Blade1<1>::VEC > TVec;   
// 	int NumVert = 2;
// 	vector<TVec> roots; 
// 	vector<Edge> edges;
// 		
// 	NCube(){ printf("DIM: %d\n", 1); init(); }
// 	
// 	void init(){
// 		roots.push_back( TVec(.5) ); 
// 		roots.push_back( TVec(-.5) ); 
// 	}
// };   

int main(){
	
    NCube<3> nc;

	nc.print();
	

	return 0;
}