#include "vsr_products.h"

/*
Atttempt to make spin computations faster by doing a compile-time backtrace
*/                          

namespace vsr{    
	
	template<class A, class B, class R>
	struct REProd{
		typedef typename EGP<A,B>::Type AB;     
		
		typedef typename Index< AB, R>::Type DO; 
		
		constexpr R gp(const A& a, const B& b) const{
			return DO::template Make<R>(a, b);
		} 
		  
	};
	
	//Reduced Product input a and b and known return type r
	template<class A, class B, class R, class Metric, bool SplitIt>
	struct RProd{
		typedef typename MGP<A,B,Metric>::Type AB; //A*B Type
		
		//Instructions for (type A*B) 
		typedef typename Index< AB, R>::Type DO; 
		   
		constexpr R gp(const A& a, const B& b) const{
			return DO::template Make<R>(a, b);
		}
	}; 
	
	//Reduced Product input a and b and known return type r
	template<class A, class B, class R, class Metric>
	struct RProd<A, B, R, Metric, true>   {
		typedef typename CGP<A,B,Metric>::Type AB; //A*B Type
		
		//Instructions for (type A*B) 
		typedef typename Index< AB, R>::Type DO; 
		   
		constexpr R gp(const A& a, const B& b) const{
			return DO::template Make<R>(a, b);
		}
	};
	
}