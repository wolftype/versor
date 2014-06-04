#ifndef VSR_NCUBE_H_INCLUDED
#define VSR_NCUBE_H_INCLUDED

#include "vsr_products.h"
#include "vsr_generic_op.h"
#include <vector> 

namespace vsr{    
	
using namespace std;
	
  struct CubeEdge{                                            
    int a, b;  
    CubeEdge(int _a, int _b) : a(_a), b(_b){}  

	  void print() { printf("%d, %d\n", a, b);}
  };    

/// ND Cube
template<int DIM>
struct NCube{     
  

	
	typedef NEVec<DIM> TVec;//EGAMV< DIM, typename Blade1<DIM>::VEC > TVec;
	typedef NEBiv<DIM> TBiv;//EGAMV< DIM, typename Blade1<DIM>::VEC > TVec;

	typedef NEVec<3> Vec3;// EGAMV< 3, typename Blade1<3>::VEC > Vec3;
	
	int NumVert = pow(2.f,DIM);
	
	vector<TVec> roots;
	vector<CubeEdge> edges; 
	
    void init() {
		TVec va, vb;
		va[DIM-1] = -.5;
		vb[DIM-1] = .5;
		
		auto next = NCube<DIM-1>();  
				
		for (auto i :  next.roots ){
		 	roots.push_back( TVec(i+va) );
			roots.push_back( TVec(i+vb) ); 
		}   

		int d = pow(2.f, DIM-1); 
		for (auto i : next.edges ){        
  	  edges.push_back(i); 
			edges.push_back( CubeEdge(i.a + d, i.b + d));   
		}                                                
		
		for (int i = 0; i < next.NumVert; ++i ){
			edges.push_back( CubeEdge(i, i + d) );
		}		                      
	} 
	
	vector< Vec3 > project(VT dist){ 
		vector< Vec3 > res;
		 for (auto i : roots ){
			res.push_back( Proj<DIM>::Call(dist, i) );
		}  
		return res;
	} 
  
  vector< float > val( VT dist){
    vector<float> v;
    for (auto i : roots){
      v.push_back( Proj<DIM>::Val(dist,i) );
    }
    return v;
  }
	
	NCube(){ init(); }

  /* TBiv biv( int which, float amt = 1.0){ */
  /*   return TVec */
  /* } */

  void spin( TBiv& b ){
     for (auto& i : roots){   		
		  	i = i.sp( Gen::rot( b ) ); 
      }
  }

  /* vector<Vec3> proj(float dist, bool ortho = false){ */
  /*   vector<Vec3> vec; */
  /*   for (auto& i : roots){ */
  /*     Vec3 v; */
  /*     if (ortho) v = Proj<DIM>::Ortho<3>(i); */ 
  /*     else v = Proj<DIM>::Call(dist, i); */
  /*     vec.push_back(v); */
  /*   } */
  /*   return vec; */
  /* } */
	
	void print() {
		for (auto i : roots ) { i.bprint(); i.vprint(); }
		for (auto i : edges) { i.print(); }
		
	} 


};


/// 1D Cube
template<>
struct NCube<1>{
	typedef EGAMV< 1, typename Blade1<1>::VEC > TVec; 
	int NumVert = 2;
	
	vector<TVec> roots;
	vector<CubeEdge> edges; 
	
	NCube(){  init(); }  
		
	void init(){  	
		roots.push_back( TVec(-.5) );
		roots.push_back( TVec(.5) );	
		edges.push_back( CubeEdge(0,1) );
	};    
	
	void print() {
		 for (auto i : roots) { i.bprint(); i.vprint(); }   
		 for (auto i : edges) { i.print(); } 
	}
  
};

}//vsr::  

#endif
