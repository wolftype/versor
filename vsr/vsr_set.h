
#ifndef VSR_SET_H_INCLUDED
#define VSR_SET_H_INCLUDED



namespace vsr  {

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
        
        typename vector<T>::iterator begin() { return mData.begin(); }
         
		vector<T>& data(){ return mData; }
 
    };
    



} //vsr::
#endif
