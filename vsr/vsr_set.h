
#ifndef VSR_SET_H_INCLUDED
#define VSR_SET_H_INCLUDED

#include <vector>

namespace vsr  {
   
    using std::vector;

	//vector of data
    template<typename T>
    class Set {

    protected:
        
        vector<T> mData;
        //vector<T> mTmp;             ///<  Stack for in-place calculations
        unsigned long mActiveId;
    
    public:


        unsigned long& activeId() { return mActiveId; }
        unsigned long activeId() const { return mActiveId; }

        
        Set& insert(T s) { mData.insert( mData.begin(), s); }
        Set& pop() { mData.pop_back(); } 
        Set& add(T s) { mData.push_back(s); mActiveId = mData.size(); return *this; }
        
        /* template<typename ... S> */
        /* Set ( S ... s ) { S ts[] = {s...}; mData = vector<T>{ts, ts + sizeof(ts) / sizeof(S) */

        Set& add(const Set& s) { for(int i = 0; i < s.size(); ++i) add(s[i]); }
        Set& erase(int idx) { mData.erase( mData.begin() + idx ); }
        Set& erase(int b, int e) { mData.erase( mData.begin() + b, mData.begin() + e ); }
       
        Set& clear() { vector<T>().swap(mData); return *this; } 
        
        T& active() { return mData[mActiveId-1]; }
        T active() const { return mData[mActiveId-1]; }
        
        T& operator [] (int i) { return mData[i]; }
        T operator []  (int i) const { return mData[i]; }
        
        unsigned long size() const { return mData.size(); }
        unsigned long num() const { return mData.size(); }    
       
        T * ptr() { return &(mData[0]); } 
        T& last() { return mData[mActiveId-1]; } 

        typename vector<T>::iterator begin() { return mData.begin(); }
        typename vector<T>::iterator end() { return mData.end(); }
         
		    vector<T>& data(){ return mData; }
        vector<T> data() const { return mData; }


 
    };
     
	//pointer to data T in container
  template<typename T>
	class Data {
		
	protected:
		
		T * mData;              
		int mNum;
    size_t mOffset; // data's memory spacing
    size_t mStride; // data's memory length
	
	public:                       
		
		T& data(int idx) { return mData[idx ]; }
		T data(int idx) const { return mData[idx ]; }
    
    T * ptr() { return mData; }
    void ptr( T * d ) { mData = d; }

    template<typename S>
    void setOffset() { mOffset = sizeof(S) - mStride; }
		
		Data(int N=0) : mNum(N), mData(NULL) {
      mStride = sizeof(T);
      setOffset<T>();
    }

    void num(int N) { mNum = N; }
    int num() const { return mNum; }  

		void alloc( int N){
      mNum = N;
			if (mData) delete[] mData;
			mData = new T[mNum];
		}
		

	};


   template<typename T>
	 class DoubleData : public Data<T> { 
	
  	protected:
	
  	T * mPrev;  
	
  	public:
	
  	void swap(){
			T * tmp = mPrev; mPrev = this->mData; this->mData = tmp;
		}

		T& prev(int idx) { return mPrev[idx]; }
		T prev(int idx) const { return mPrev[idx]; }
		
		DoubleData(int N=0) : Data<T>(N) {}
	
		void alloc(int N){             
			Data<T>::alloc(N); 
			if (mPrev) delete[] mPrev;
			mPrev = new T[this->mNum];
		} 

		void copy(){
      if (!mPrev) mPrev = new T[this->mNum];
			std::copy( this->mData, this->mData+this->mNum, mPrev);
		}
	};

  //N Copies of type T Arrays
  template<int N, typename T>
  class NData : public Data<T> {
    protected:
    NData<N-1, T> mNext;

    public:
    void resize(int n) { this->mNum = n; alloc(); }
    void alloc(){
      Data<T>::alloc();
      mNext.alloc();
    }
    void copy(){
      std::copy( this->mData, this->mData + this->mNum, mNext.ptr() );
    }
  };
  template<typename T>
  class NData<1,T> : public Data<T>{
    public:
    void resize(int N) { this->mNum = N; alloc(); }
    void alloc(){
      Data<T>::alloc();
    }

  };

} //vsr::
#endif
