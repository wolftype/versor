#include "vsr_products.h"

/*
Atttempt to make spin faster
*/                          

namespace vsr{
	
	//some arbitrary number of arguments (backtrace)
	template<class A, class B, class R, class Metric, bool SplitIt>
	struct Algo{
		typedef Prod<A,B,Metric,SplitIt> P;
		 
		typedef typename Index< typename P::InstList, R>::Type DO; 
		
		constexpr R gp(const A& a, const B& b) const{
			return DO::template Make<Type>(a, b);
		}
	};
	
}