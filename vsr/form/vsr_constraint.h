#include "detail/vsr_generic_op.h"
#include "vsr_set.h"

//assumes conformal

namespace vsr{  

  //generic vec
	template<typename T>
	T Diff( const T& a, const T& b ){
		return (a-b);
	}

  ///Squared Distances
  template<typename T>
	VT Dist( const T& a, const T& b ){
   // cout << "generic vec" << endl; 
		return (a-b).wt();  //vec, etc must have a wt() method ( dot() )
	}

  /* //generic vec */
	/* template<typename T> */
	/* VT SqDist( const T& a, const T& b ){ */
		/* return (a-b).dot(); */
	/* } */

  //specific
	/* template<TT DIM> */
	/* NVec<DIM> Diff( const NVec<DIM>& a, const NVec<DIM>& b ){ */
	/* 	return (a-b); */
	/* } */
		        
	/* template<TT DIM> */
	/* VT Dist( const NVec<DIM>& a, const NVec<DIM>& b ){ */
	/* 	return (a-b).wt(); */
	/* } */
	template<TT DIM>
	VT Dist( const NPnt<DIM>& a, const NPnt<DIM>& b ){
		return Ro::sqd(a,b);
	}
	
	//template<class T>               
	struct Constraint{ 
		
		bool bActive; 
		Constraint() : bActive(true) {}
		//satisfy
		virtual void operator ()(){
		   if (bActive){
			 exec();
			} 
		} 
		
		virtual void exec(){
			
		}
	};          
	
	//rigid distance                 
	template<class T>
	class Distance : public Constraint {

    static int S_ID;

    public:

    bool operator < (const Distance& d) const { return id < d.id; }
		
		Distance(T& a, T& b) : a(&a), b(&b), sqLength( Dist(a,b) ) {
      //  cout << sqLength << endl; 
        if (sqLength==0) { a.vprint(); b.vprint(); }
        id = S_ID;
        S_ID++;
      }
		
		T * a; T * b;
		
		VT sqLength;
    int id;
    		
		void verlet() {
			auto diff = Diff( *a, *b);
			float sq = sqLength;
			
			float d = diff.wt() + sq;  					  
			float r = d == 0 ? -.5 : ( sq ) / ( d ) - .5;     			
			diff *= r;
			
			*a += diff;
			*b -= diff; 
			
		}
	};   

  template<class T>
  int Distance<T> :: S_ID = 0;
	
	// template<typename T>
	// struct Solver {
	// 	T * mData; // Data to Solve  
	// 	T * data() { return mData; }
	// };  
	
	//A Set of Distances between Type Ts (i.e. bonds)
	template<typename T>
	class Verlet : public DoubleData<T> {
	
	     
		int mIter; 
		double mTimeStep;

    public:                          
	
	 	Set< Distance<T> * > constraint;      
	
		Verlet() : mIter(20), mTimeStep(.2){}   
		
		    
		//set constraints from current positions, could make a convex hull here for instance . . 
		void set(){
			
		}  
		
		double timeSq() { return mTimeStep * mTimeStep; }
		         
		
		void step(){    
			force();
			move();
			constrain();
		} 
		
		void force(){
			
		}           
		
		void move(){ 
			double s = timeSq();
			for (int i = 0; i < this->num(); ++i ){ 
				T tmp = this->data(i);
				T v = this->data(i) - this->prev(i);
				this->data(i) += v;// + mForces[i] * s;
				this->prev(i) = tmp;
			}
		}
		void constrain(){
			for (int i = 0; i < mIter; ++i){
				for (int j = 0; j < constraint.size(); ++j){ 
					constraint[j] -> verlet();
				}
			}
		}
		
	};
}
