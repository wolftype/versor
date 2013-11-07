#ifndef ROOT_SYSTEM_INCLUDED
#define ROOT_SYSTEM_INCLUDED
                      
#include <vector>
//#include <iostream>
#include "vsr_products.h"   
#include "vsr_generic_op.h"


using namespace std;

namespace vsr{
      

struct Root{
             
template<class V>
bool compare(const V& a, const V& b, VT amt =.00005){   
	//( a <= b ).vprint();
	return ( ( a - b ).wt() ) < amt;
}            
	
template<int DIM, class ... R >
vector< EGAMV<DIM, typename Blade1<DIM>::VEC >  > System ( R ... v )  {   
	
	typedef EGAMV<DIM, typename Blade1<DIM>::VEC > V;
	 
	int n = sizeof...(R);
	
	V root[] = { v... };
	
	vector< V > results;
	
	for ( int i = 0; i < n; ++i ){
		results.push_back( root[i] );
	}
	
	for (int i = 0; i < n; ++i){
	   	for (int j = 0; j < n; ++j){

			V nr = root[i].re(root[j]);//ref(root[i],root[j]);//-!root[j] * root[i] * root[j];
			 
			bool exists = 0;
			for ( int k = 0; k < results.size(); ++k){
				exists = ( compare(nr,results[k]) ); 
				// nr.vprint(); results[k].vprint(); std::cout << exists << endl;     
				if (exists) break;
			}   
			
			if (!exists) results.push_back( nr ); 
			 
		} 
	}   
	
	bool keepGoing = true; 
//	
    while (keepGoing){   
  //  for ( int ii = 0; ii < 3; ++ii){
		for (int i = 0; i < n; ++i){
		
            int cs = results.size();
			for (int j = 0; j < cs; ++j ){
			 
				bool done = 1;
			    V nr = results[j].re( root[i] );//ref(results[j],root[i]);; 
	                
                bool exists = 0; 
				for ( int k = 0; k < cs; ++k){ 
					// std::cout << k << endl;
					//nr.vprint();  
					exists = ( compare(nr,results[k]) );
					//nr.vprint(); results[k].vprint(); std::cout << exists << endl;
					if (exists) {  
					   break;
					}
				}
				
				if (!exists)  { 
					//std::cout <<"! exists" << endl;
					//nr.vprint();  
					results.push_back( nr );
					done = 0; 
				}   
			
				if (done) keepGoing = false; 
			}
		}  
   }   


    keepGoing = true; 
//	
    while (keepGoing){   
  //  for ( int ii = 0; ii < 3; ++ii){
        int cs = results.size(); 
		for (int i = 0; i < cs; ++i){  
			int ns = results.size();
			for (int j = 0; j < ns; ++j ){

				bool done = 1;
			    V nr = results[j].re( results[i] ); //ref(results[j],results[i]);; 

                bool exists = 0; 
				for ( int k = 0; k < ns; ++k){ 
					// std::cout << k << endl;
					// nr.vprint();  
					exists = ( compare(nr,results[k]) );
					//nr.vprint(); results[k].vprint(); std::cout << exists << endl;
					if (exists) {  
					   break;
					}
				}

				if (!exists)  { 
					//std::cout <<"! exists" << endl;
					//nr.vprint();  
					results.push_back( nr );
					done = 0; 
				}   

				if (done) keepGoing = false; 
			}
		}  
   }

 	return results;
}  
};
            
} //vsr::
#endif