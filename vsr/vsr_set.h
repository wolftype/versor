
#ifndef VSR_SET_H_INCLUDED
#define VSR_SET_H_INCLUDED



namespace vsr  {
    
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
        Set& add(const Set& s) { for(int i = 0; i < s.size(); ++i) add(s[i]); }
        Set& erase(int idx) { mData.erase( mData.begin() + idx ); }
        Set& erase(int b, int e) { mData.erase( mData.begin() + b, mData.begin() + e ); }
        
        
        T& active() { return mData[mActiveId-1]; }
        T active() const { return mData[mActiveId-1]; }
        
        T& operator [] (int i) { return mData[i]; }
        T operator []  (int i) const { return mData[i]; }
        
        unsigned long size() const { return mData.size(); }
        unsigned long num() const { return mData.size(); }    
        
        typename vector<T>::iterator begin() { return mData.begin(); }
         
		vector<T>& data(){ return mData; }
 
    };
     
	//pointer of data
    template<typename T>
	class Data {
		
	protected:
		
		T * mData;              
		int mNum;
	
	public:                       
		
		//T * ptr() { return mData; }
		T& data(int idx) { return mData[idx]; }
		T data(int idx) const { return mData[idx]; }
    
    T * ptr() { return mData; }
		
		Data(int N=1) : mNum(N), mData(NULL){ alloc(); }
		void resize(int N) { mNum = N; alloc(); }
		void alloc(){
			if (mData) delete[] mData;
			mData = new T[mNum];
		}
		
		int num() { return mNum; }  
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
		//T& prev() { return *mPrev; }                               
		
		DoubleData(int N=1) : Data<T>(N) { alloc(); }
		void resize(int N) { this->mNum = N; alloc(); }  
		void alloc(){              
			Data<T>::alloc(); 
			if (mPrev) delete[] mPrev;
			mPrev = new T[this->mNum];
		} 
		void copy(){
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
