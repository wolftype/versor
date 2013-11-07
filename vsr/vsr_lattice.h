//
//  vsr_lattice.h
//  Versor
//           

// Should not assume a metric . . .
//
//  Created by Pablo Colapinto on 3/7/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef Versor_vsr_lattice_h
#define Versor_vsr_lattice_h


#include <vector>

#include "vsr_generic_op.h" 

#include "vsr_interp.h"  

#include "gfx/gfx_data.h"


using namespace std;
using namespace gfx;   

namespace vsr {

    #define ITER \
        for (int i = 0; i < mWidth; ++i){\
            for (int j = 0; j < mHeight; ++j){\
                for (int k = 0; k < mDepth; ++k){\
                    int tidx = idx(i,j,k);
    
    #define ITEND }}}

    #define ITERV \
        for (int i = 0; i < mWidth; ++i){\
            double u = 1.0 * i/mWidth;\
            for (int j = 0; j < mHeight; ++j){\
                double v = 1.0 * j/mHeight;\
                for (int k = 0; k < mDepth; ++k){\
                    double w = 1.0 * k/mDepth; \
                    int tidx = idx(i,j,k);

	#define BOUNDITER0\
				for (int i = 0; i < mWidth-1; ++i){ \
				for (int j = 0; j < mHeight-1; ++j){ \
				for (int k = 0; k < mDepth-1; ++k){        
    
	#define BOUNDITER\
				for (int i = 1; i < mWidth-1; ++i){ \
				for (int j = 1; j < mHeight-1; ++j){ \
				for (int k = 1; k < mDepth-1; ++k){

	#define BOUNDEND \
				}}}

    /*! A Lattice of Points */                                        
    class Lattice {
    
        public:
            
			using LPnt = NPnt<5>;  //Lattice Point

            Lattice (int n ) : mNum(n) {
        
                mPoint = new LPnt[mNum];

            }

            int num() const { return mNum; }
            void num( int n ) {
                resize(n);
            }

            void resize(int n){
                if(mPoint) delete[] mPoint;
                mNum = n;
                mPoint = new LPnt[mNum];
            }
            
            LPnt& pnt( int ix ) { return mPoint[ix]; }    
            LPnt pnt( int ix ) const { return mPoint[ix]; }    

            LPnt* pnt() { return mPoint; }
                        
        protected:
        
            int mNum;

            LPnt * mPoint;
        
    };
    
        /*! Discretized Volume Indexing (Isometric Cubic Lattice w/o data) */
    class CubicLattice  {
    
        public:
         
		using LPnt = NPnt<5>;  //Lattice Point

        CubicLattice(int _w = 1, int _h = 1, int _d = 1, double _s = 1.0)
        : mWidth(_w), mHeight(_h), mDepth(_d), mSpacing(_s), 
         mNum( mWidth * mHeight * mDepth),
         mNumVxl( (mWidth-1) * (mHeight-1) * (mDepth-1) ),
		 mPoint(NULL), mVxl(NULL), mNbr(NULL), mNbrVxl(NULL)
         {
            alloc(); init();
        }
        
        ~CubicLattice() {
            onDestroy();
        }
        
        void onDestroy(){   
			if (mPoint) delete[] mPoint;
            if (mVxl) delete[] mVxl;
            if (mNbr) delete[] mNbr;
            if (mNbrVxl) delete[] mNbrVxl;
            
            if (!mFace.empty() ) mFace.clear();
            if (!mEdge.empty() ) mEdge.clear();
            if (!mCorner.empty()) mCorner.clear();
            if (!mFaceVxl.empty()) mFaceVxl.clear();
            if (!mEdgeVxl.empty()) mEdgeVxl.clear();
            if (!mCornerVxl.empty()) mCornerVxl.clear();
            
            mWidth = mHeight = mDepth = mNum = mNumVxl = 0;
        }
        
        CubicLattice& resize( int _w, int _h, int _d, int _s = 1.0){
            onDestroy();
            mWidth = _w; mHeight = _h; mDepth = _d; mSpacing = _s;
			mNum = _w * _h * _d; 
			mNumVxl = (_w-1) * (_h-1) * (_d -1);
            alloc(); init();
            return *this;
        }
                
        int w() const { return mWidth; }
        int h() const { return mHeight; }
        int d() const { return mDepth; }
        double spacing() const { return mSpacing; }
        void spacing( double amt ) { mSpacing = amt; init(); }
        
        int num() const { return mNum; }
        int numVxl() const { return mNumVxl; }    
        /*! Get Index In Array */
        int idx(int i, int j, int k) const { 
            if (i > w() - 1) i = w()  -1;
            if (j > h() - 1) j = h()  -1;
            if (k > d() - 1) k = d()  -1;

            if (i < 0 ) i = 0;
            if (j < 0 ) j = 0;
            if (k < 0 ) k = 0;
            
            return i * mHeight * mDepth + j * mDepth + k; 
        }
        
        
	   
        //VOXEL IDX
        Vxl vxl(int ix) const { return mVxl[ix]; }
        Vxl& vxl(int ix)  { return mVxl[ix]; }
        
        Nbr nbr(int ix) const { return mNbr[ix]; }
        Nbr& nbr(int ix)  { return mNbr[ix]; }
        Nbr nbrVxl(int ix) const { return mNbrVxl[ix]; }
        Nbr& nbrVxl(int ix)  { return mNbrVxl[ix]; }
        
        /* Totals and Offsets From Center */
        /*! Total Width */
        double tw() const { return (mWidth-1) * mSpacing; }
        /*! Offset Width */
        double ow() const { return tw() / 2.0 ; }
        /*! Total Height */
        double th() const { return (mHeight-1) * mSpacing; }
        /*! Offset Height */
        double oh() const { return th() / 2.0 ; }
        /*! Total Depth */
        double td() const { return (mDepth-1) * mSpacing; }
        /*! Offset Depth */
        double od() const { return td() / 2.0 ; }//0;}//
        /*! Spatial Positions of ith element in x direction  */
        double px(int i) const { return -ow() + (mSpacing * i); }
        /*! Spatial Positions of jth element in y direction  */
        double py(int j) const { return -oh() + (mSpacing * j); }
        /*! Spatial Positions of kth element in z direction  */
        double pz(int k) const { return  od() - (mSpacing * k); }

        void alloc(){
            mPoint = new LPnt[mNum];
            mNbr = new Nbr[mNum];   
            mVxl = new Vxl[mNumVxl];
            mNbrVxl = new Nbr[mNumVxl];
        }
        
        void init(){
            
            
            ITER
                mPoint[ tidx ]  = Ro::null( px(i),  py(j),  pz(k) ); 
                        
                int type = 0;
                type |= (k==0) ? FRONT : 0;
                type |= (k==mDepth-1) ? BACK: 0;					
                type |= (j==0) ? BOTTOM : 0;
                type |= (j==mHeight-1) ? TOP: 0;					
                type |= (i==0) ? LEFT : 0;
                type |= (i==mWidth-1) ? RIGHT: 0;					
            
                mNbr[tidx] = Nbr(tidx, mWidth, mHeight, mDepth, type);

            ITEND

        
            //VXLS
            BOUNDITER0
            
                int ix = i * (mHeight-1) * (mDepth-1) + j * (mDepth-1) + k;
                Vec3f v( px(i) + mSpacing/2.0, py(j) + mSpacing/2.0, pz(k) - mSpacing/2.0);
               
                 mVxl[ix] = vxlAt(v);
                
                //assign face information
                int type = 0;
                type |= (k==0) ? FRONT : 0;
                type |= (k==mDepth-2) ? BACK: 0;					
                type |= (j==0) ? BOTTOM : 0;
                type |= (j==mHeight-2) ? TOP: 0;					
                type |= (i==0) ? LEFT : 0;
                type |= (i==mWidth-2) ? RIGHT: 0;	
                
                mVxl[ix].type = type;					
                mNbrVxl[ix] = Nbr(ix, mWidth, mHeight, mDepth, type);
            BOUNDEND
            
            //Assign Edges and Faces idx
            for (int i = 0; i < mNum; ++i){
                FE( mNbr[i] );
            }
    		//Assign EdgeVxl and FaceVxl idx
            for (int i = 0; i < mNumVxl; ++i){
                vxlFE( mNbrVxl[i] );
            }
            
        }
        			
        /*! Voxel of Vector v */ 
   		template<class V>
        Vxl vxlAt( const V& tv ) const { 
		
            Vxl vxl;
            
            V v = bound(tv);
            
            //Bottom - Left corner Voxel	
            int lw = floor( ( v[0] + ow() ) / spacing() );
            int lh = floor((v[1] + oh() ) / spacing() );
            int ld = floor((-v[2] + od() ) / spacing() );
            
            if (lw > w()-2) lw = w() -2;
            if (lh > h()-2) lh = h() -2;
            if (ld > d() -2) ld = d() -2;

            vxl.a = idx(lw,lh,ld);
            vxl.b = idx(lw+1,lh,ld);
            vxl.c = idx(lw+1,lh+1,ld);
            vxl.d = idx(lw,lh+1,ld);

            vxl.e = idx(lw,lh,ld+1);
            vxl.f = idx(lw+1,lh,ld+1);
            vxl.g = idx(lw+1,lh+1,ld+1);
            vxl.h = idx(lw,lh+1,ld+1);
            
            return vxl;
	}
     
	 template<class V> 
     V bound ( const V& p) const {
		V v = p;        
        
		if ( v[0] < px(0) ) v[0] = px(0);// + f.spacing()/2.0;
        else if  ( v[0] > px( w()-1) ) v[0] = px( w()-1);// - f.spacing()/2.0;

		if ( v[1] < py(0) ) v[1] = py(0);// + f.spacing()/2.0;	
        else if ( v[1] > py( h()-1) ) v[1] = py( h()-1);// - f.spacing()/2.0;	
        
		if ( v[2] > pz(0) ) v[2] = pz(0);// - f.spacing()/2.0;
		else if ( v[2] < pz( d()-1) ) v[2] = pz( d()-1);// + f.spacing()/2.0;	

		return v;
    }	

    //bound and modify to range [0,1]  
	template<class V>
    V range2D( const V& v) const{
        V t = v;

        double minx = px(0);
        double maxx = px(mWidth-1);
        double miny = py(0);
        double maxy = py(mHeight-1);
        
        if (t[0] < minx) t[0] = minx;
        else if (t[0] > maxx) t[0] = maxx;
        if (t[1] < miny) t[1] = miny;
        else if (t[1] > maxy) t[1] = maxy;
        
        double dx = (t[0] - minx)/tw();
        double dy = (t[1] - miny)/th();
        
        return V(dx,dy);
    }

    //bound and modify to range [0,1]  
	template<class V>
    V range( const V& v) const{
        V t = v;

        double minx = px(0);
        double maxx = px(mWidth-1);
        double miny = py(0);
        double maxy = py(mHeight-1);
        double maxz = pz(0);
        double minz = pz(mDepth-1);
        
        if (t[0] < minx) t[0] = minx;
        else if (t[0] > maxx) t[0] = maxx;
        if (t[1] < miny) t[1] = miny;
        else if (t[1] > maxy) t[1] = maxy;
        if (t[2] < minz) t[2] = minz;
        else if (t[2] > maxz) t[2] = maxz;
        
        double dx = (t[0] - minx)/tw();
        double dy = (t[1] - miny)/th();
        double dz = -(t[2] - maxz)/td();
        
        return V(dx,dy,dz);
    }
        
        
        ///  Routines to Find Face and Edge Boundary 
        void FE( Nbr nb){

        //one undefined neighbor is a face
		int i = 0;
		while (i < 7){
			if (nb[i] == -1) { 
				mFace.push_back( nb.idx ); 
				break; 
			}
			i++;
		}
				
        //two undefined neighbors is an edge
		i = 0; int n = 0;		
		while (	i < 7 ){
			if (nb[i] == -1) {
				n++;
				if (n==2){
					mEdge.push_back( nb.idx );
					break;
				}
			}
			i++;
		}
		
        //two undefined neighbors is a corner
		i = 0; n = 0;				
		while (	i < 7 ){
			if (nb[i] == -1) {
				n++;
				if (n==3){
					mCorner.push_back( nb.idx );
					break;
				}
			}
			i++;
		}

	}	
    
    //see above
    void vxlFE( Nbr nb){
		int i = 0;
		while (i < 7){
			if (nb[i] == -1) { 
				mFaceVxl.push_back( nb.idx ); 
				break; 
			}
			i++;
		}
				
		i = 0; int n = 0;		
		while (	i < 7 ){
			if (nb[i] == -1) {
				n++;
				if (n==2){
					mEdgeVxl.push_back( nb.idx );
					break;
				}
			}
			i++;
		}
		i = 0; n = 0;
		while (	i < 7 ){
			if (nb[i] == -1) {
				n++;
				if (n==3){
					mCornerVxl.push_back( nb.idx );
					break;
				}
			}
			i++;
		}
	}
    
            /*! Indicex of surface at u, v [0, 1]*/
        Patch surfIdx(double u, double v){
        
            double fw = u * (mWidth - 1);
            double fh = v * (mHeight - 1);
            
            int iw = floor ( fw );
            int ih = floor ( fh );
            
            double rw = fw - iw;
            double rh = fh - ih;
            
            if (iw == mWidth -1) { iw = mWidth -2; rw = 1.0; }
            if (ih == mHeight -1) { ih = mHeight -2; rh = 1.0; }
            
            int a= ( idx ( iw, ih, 0 ) );
            int b= ( idx ( iw + 1, ih, 0 ) );
            int c= ( idx ( iw + 1, ih + 1, 0 ) );
            int d= ( idx ( iw, ih + 1, 0 ) );
            
            return Patch( a, b, c, d, rw, rh);
        }
        
        /*! Volume Index at u,v,w [0-1] */
        VPatch vidx(double u, double v, double w) const{
            double fw = u * (mWidth - 1);
            double fh = v * (mHeight - 1);
            double fd = w * (mDepth-1);
            
            int iw = floor ( fw );
            int ih = floor ( fh );
            int id = floor ( fd );
            
            double rw = fw - iw;
            double rh = fh - ih;
            double rd = fd - id;
            
            if (iw == mWidth -1) { iw = mWidth -2; rw = 1.0; }
            if (ih == mHeight -1) { ih = mHeight -2; rh = 1.0; }
            if (id == mDepth -1) { id = mDepth -2; rd = 1.0; }
            
            int a= ( idx ( iw, ih, id ) );
            int b= ( idx ( iw + 1, ih, id ) );
            int c= ( idx ( iw + 1, ih + 1, id ) );
            int d= ( idx ( iw, ih + 1, id ) );
            int e= ( idx ( iw, ih, id +1) );
            int f= ( idx ( iw + 1, ih, id +1) );
            int g= ( idx ( iw + 1, ih + 1, id +1) );
            int h= ( idx ( iw, ih + 1, id +1) );
            
            return VPatch( a, b, c, d, e, f, g, h, rw, rh, rd);
        }
        
        /*! Indices of Line at T */
        Patch idxU(double t){
            double fw = t * (mWidth - 1);
            
            int iw = floor ( fw );
            double rw = fw - iw;
            if (iw == mWidth -1) { iw = mWidth -2; rw = 1.0; }
            
            int a = idx(iw, 0, 0);
            int b = idx(iw+1, 0, 0);
            int c = idx(iw+2, 0, 0);
            
            return Patch(a, b, 0, 0, rw, 0);
        }
        
    
        //backwards compatibilty
        
        /*! Set grid data by Coordinate */
        LPnt&  gridAt(int w = 0, int h = 0, int d = 0) { return mPoint[ idx(w, h, d)  ]; }   
        /*! Get grid data by Coordinate */
        LPnt	gridAt(int w = 0, int h = 0, int d = 0) const { return mPoint[ idx(w, h, d)  ]; }     
        /*! Set Grid (position) Data*/ 
        LPnt& grid(int i) { return mPoint[i]; }  
        /*! Get Grid (position)  Data */      
        LPnt grid(int i) const { return mPoint[i]; }     
        
        LPnt surf(double u, double v){
            
            Patch p =  surfIdx(u,v);
            
            LPnt a = mPoint[ p.a ];//gridAt ( iw, ih, 0 );
            LPnt b = mPoint[ p.b ];//gridAt ( iw + 1, ih, 0 );
            LPnt c = mPoint[ p.c ];//gridAt ( iw + 1, ih + 1, 0 );
            LPnt d = mPoint[ p.d ];//gridAt ( iw, ih + 1, 0 );
            
            return Ro::null( Interp::surface<LPnt>( a,b,c,d, p.rw, p.rh) ) ;       
        }   

        LPnt surfGrid(double u, double v) { return surfPnt(u,v); }
        LPnt surfPnt(double u, double v){
             
            double pw = 1.0 / ( mWidth-1);
            double ph = 1.0/ ( mHeight-1);
            //double pd = 1.0 / mDepth;
            
            double fw = u / pw;
            double fh = v / ph;
            //double fd = w / pd;
            
            int iw = floor ( fw );
            int ih = floor ( fh );
            //int id = floor ( fd );
           // cout << iw << " " << ih << endl; 
            
            double rw = fw - iw;
            double rh = fh - ih;
            //double rd = fd - id;
           // cout << rw << " " << rh << endl; 
            
            LPnt a = grid( idx ( iw, ih, 0 ) );
            LPnt d =grid( idx ( iw, ih + 1, 0 ));
            LPnt b = grid( idx (  iw + 1, ih, 0 ));
            LPnt c = grid( idx (  iw + 1, ih + 1, 0 ));
            
            return Ro::null( Interp::surface<LPnt> (a,b,c,d, rw, rh) );       
        }

        vector<int>& face() { return mFace; }
        int face(int ix) const { return mFace[ix]; }

		 vector<int>& faceVxl() { return mFaceVxl; } 
		 Vxl faceVxl(int ix) { return mVxl[ mFaceVxl[ix] ]; }    
		
	   // Vxl vxl(int ix) const { return mVxl[ix]; }


     protected:
        
        int mWidth, mHeight, mDepth;
        int mNum, mNumVxl;
        double mSpacing;
        
        //Points in Space
        LPnt * mPoint;
        
        //vxl access
        Vxl *mVxl;
        
        //nbr access
        Nbr *mNbr, *mNbrVxl;
        
        //INDICES
        vector <int> mFace;
        vector <int> mEdge;
        vector <int> mCorner;
        vector <int> mFaceVxl;
        vector <int> mEdgeVxl;
        vector <int> mCornerVxl; 
                



    };
    
    #undef ITER
    #undef ITEND
    #undef BOUNDITER
    #undef BOUNDEND
}
#endif
