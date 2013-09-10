#ifndef TETRA_H_INCLUDED
#define TETRA_H_INCLUDED  

#include "products_all.h" 

namespace vsr {

template<int DIM>
struct Proj{
   	typedef typename Blade1<DIM>::VEC TVec;
	typedef typename Blade1<DIM-1>::VEC OneDown; //Projection Down
  	 
	static auto Call( VT dist, const TVec& v ) RETURNS (
		( Proj<DIM-1>::Call( dist, v.template cast<OneDown>() * ( dist / (dist - v[DIM-1] ) ) ) )
	) 
	
	// static VT Val( VT dist, const Vec& v) {
	// 	return Proj< DIM, 4, DIM==TARGET >::Call(dist, v);
	// }     
};         
template<>
struct Proj<3>{  
    typedef typename Blade1<3>::VEC TVec;
	static TVec Call(VT dist, const TVec& v) { return v; }
}; 

 
struct Edge{                                            
  int a, b;  
  Edge(int _a, int _b) : a(_a), b(_b){}  

	void print() { printf("%d, %d\n", a, b);}
};

template<int DIM>
struct NCube{
	typedef typename Blade1<DIM>::VEC TVec;
   // typedef typename Blade1<DIM-1>::VEC Proj; //Projection Down  
	typedef typename Blade1<3>::VEC Vec3;
	
	int NumVert = pow(2.f,DIM);
	
	vector<TVec> roots;
	vector<Edge> edges; 
	
    void init() {
		TVec va, vb;
		va[DIM-1] = -.5;
		vb[DIM-1] = .5;
		
		int ix = 0;
		for (auto i : lower() ){
		   
		 	roots.push_back( i + va );
			roots.push_back( i + vb ); 
			
			//edges.push_back( Edge(ix, ix + 1) );
			
			
		}   
		 
		int d = pow(2.f, DIM-1); 
		for (auto i : lowerEdges() ){        
  		
			edges.push_back(i); 
			edges.push_back( Edge(i.a + d, i.b + d));  
			// edges.push_back( Edge(i.a, i.a + d));
			// //edges.push_back( Edge(i.b, i.b + d));    

		}                                                
		
		for (int i = 0; i < NCube<DIM-1>().NumVert; ++i ){
			edges.push_back( Edge(i, i + d) );
		}
		                      
		// for ( auto i : lowerEdges() ) {
		// 	edges.push_back( Edge( i.a, ) )   
		// }  
	} 
	
	
   auto lower() RETURNS(
	   NCube<DIM-1>().roots 
	) 
	
	auto lowerEdges() RETURNS(
	   NCube<DIM-1>().edges 
	)
	
	vector< Vec3 > project(VT dist){ 
		vector< Vec3 > res;
		 for (auto i : roots ){
			res.push_back( Proj<DIM>::Call(dist, i) );//i.cast<Proj>() * ( dist / (dist - i[DIM-1] ) ) ); 
		}  
		return res;
	}
	
	NCube(){ init(); }
	
	void print() {
		for (auto i : roots ) { i.bprint(); i.vprint(); }
		for (auto i : edges) { i.print(); }
		
	} 
};

template<>
struct NCube<1>{
	typedef typename Blade1<1>::VEC TVec; 
	int NumVert = 2;
	
	vector<TVec> roots;
	vector<Edge> edges; 
	
	NCube(){ init(); }  
		
	void init(){  	
		roots.push_back( TVec(-.5) );
		roots.push_back( TVec(.5) );  
		
		edges.push_back( Edge(0,1) );
	};    
	
	void print() {
		for (auto i : roots) { i.bprint(); i.vprint(); }   
		 for (auto i : edges) { i.print(); } 
	}
  
};

} //vsr::    

#endif