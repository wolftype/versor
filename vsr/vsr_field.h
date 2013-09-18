//
//  vsr_field.h
//  Versor
//
//
//  Created by Pablo Colapinto on 11/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Versor_vsr_field_h
#define Versor_vsr_field_h

#include "vsr_products.h"
#include "vsr_op.h"
#include "vsr_frame.h"
#include "vsr_lattice.h"

#include "gfx/gfx_data.h"

namespace vsr{

    #define ITN \
        for (int i = 0; i < mNum; ++i) {
        
    #define END }

    #define ITER \
        for (int i = 0; i < mWidth; ++i){\
            for (int j = 0; j < mHeight; ++j){\
                for (int k = 0; k < mDepth; ++k){\
                    int tidx = idx(i,j,k);
    
    #define ITEND }}}

    #define ITERED(x) \
            for (int i = 0; i < x.w(); ++i){\
            for (int j = 0; j < x.h(); ++j){\
                for (int k = 0; k < x.d(); ++k){\
                    int tidx = idx(i,j,k);

    #define ITERV \
        for (int i = 0; i < mWidth; ++i){\
            double u = 1.0 * i/mWidth;\
            for (int j = 0; j < mHeight; ++j){\
                double v = 1.0 * j/mHeight;\
                for (int k = 0; k < mDepth; ++k){\
                    double w = 1.0 * k/mDepth; \
                    int tidx = idx(i,j,k);
        
    
	#define BOUNDITER\
				for (int i = 1; i < mWidth-1; ++i){ \
				for (int j = 1; j < mHeight-1; ++j){ \
				for (int k = 1; k < mDepth-1; ++k){ \

	#define BOUNDED(x)\
				for (int i = 1; i < x.w()-1; ++i){ \
				for (int j = 1; j < x.h()-1; ++j){ \
				for (int k = 1; k < x.d()-1; ++k){ \

	#define BOUNDEND \
				}}}

    ///  A Basic 3D Field (slowly porting this over from the now defunct vsr_lattice class)
    /// Use to Evaluate Neighbors, Tensors, etc.
    
    template < class T>
    class Field : public CubicLattice {
        
        
        protected:
        
        T * mData;
        
        public:

        //Vector Derivative in Euclidean Metric
        //typedef typename ProductN<VEC,T::idx>::GP VecDeriv;
        
        T * dataPtr() { return mData; }
        void dataPtr( T* d ) { mData = d; }
        
        /// Zero Out All Data
        void zero() { ITER mData[tidx] = T(0); ITEND }
        
        Field( int w, int h, int d, double spacing = 1.0) :
        CubicLattice(w,h,d,spacing), mData( NULL )

        {
            alloc();
            init();
        }
        
        Field& resize( int w, int h, int d, double spacing = 1.0){
            
            resize(w,h,d,spacing);
            alloc(); init();
            
            return *this;
        }
        
        Field& respace( double s ){
            spacing(s);
            init();
            return *this;
        }
        
        void onDestroy(){
            if (mData) delete[] mData;
        }


        
        /*! Allocate Memory */
        void alloc(){
            
            if (mData) delete[] mData;

            mNum = mWidth * mHeight * mDepth;
            
            mData = new T[mNum]; 
        }
        
        /*! Set Data by Index*/
        T&	operator [] (int i)	{ return ( mData[i] ); }
        /*! Get Data by Index*/						
        T	operator [] (int i) const { return mData[i]; }        
        /*! Set Data by Coordinate */
        T&  at(int w = 0, int h = 0, int d = 0) { return mData[ idx(w, h, d)  ]; }   
        /*! Get Data by Coordinate */
        T	at(int w = 0, int h = 0, int d = 0) const { return mData[ idx(w, h, d)  ]; }  

        //INITIALIZE
        void basicInit(){
            ITER
                mData[ tidx ] = T( mPoint[ tidx ] );
            ITEND
        }
        //SPECIALIZE HERE
        void init(){
            basicInit();
        }
        
        void reset() { init(); }
        
        /*! Random Vector in Field*/
//        Vec rand(){ 
//            return Vec( px(Rand::Int(mWidth)) , py(Rand::Int(mHeight)), pz(Rand::Int(mDepth))  );         
//        }
//

            //MOVED TO vsr_draw
//        void draw(float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0){
//            for (int i = 0; i < mNum; ++i){
//                GL::Draw::Render( mData[i], r, g, b, a );
//            }
//        }
//
//        void drawGrid(float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0){
//            for (int i = 0; i < mNum; ++i){
//                GL::Draw::Render( grid(i), r, g, b, a );
//            }
//        }
//
//
//        void drawPush(float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0){
//            for (int i = 0; i < mNum; ++i){
//                GL::push();
//                GL::translate( mPoint[i].w() );
//                GL::Draw::Render( mData[i],r,g,b,a );
//                GL::pop();
//            }
//        }
        
        //data at bottom front corner of vxl at p
        template<class B>
        T& dataAt( const B& p ){
            int idx = vxlAt(p).a;
            return mData[idx];
        }

        //2d Euler (bounded)
        T euler2d( const Vec2D& v){
            Vec2D t = range(v);   //bind to range of field         
            return surf(t[0], t[1]);
        }
        
        //2d Euler (bounded)
        T euler2d( const Vec& v){
            Vec t = range(v);   //bind to range of field         
            return surf(t[0], t[1]);
        }
        
        //3d Euler (bounded) v an arbitrary point in space
        T euler3d( const Vec& v) const{
            Vec t= range(v);
            return vol(t[0], t[1], t[2]);
        }


        /*! Get BILINEAR Interpolated Data at eval u,v [0-1.0] */
        T surf(double u, double v){
            
            Patch p = surfIdx(u,v);
            
            T a = mData[ p.a ];//at ( iw, ih, 0 ) ;
            T b = mData[ p.b ];//at ( iw + 1, ih, 0 );
            T c = mData[ p.c ];//at ( iw + 1, ih + 1, 0 );
            T d = mData[ p.d ];//at ( iw, ih + 1, 0 );
            
            return Interp::surface<T> (a,b,c,d, p.rw, p.rh);
        }
        
        T vol(double u, double v, double w) const {
            VPatch p = vidx(u,v,w);
            
            T a = mData[ p.a ]; T b = mData[ p.b ]; T c = mData[ p.c ]; T d = mData[ p.d ];
            T e = mData[ p.e ]; T f = mData[ p.f ]; T g = mData[ p.g ]; T h = mData[ p.h ];
            return Interp::volume<T> (a,b,c,d,e,f,g,h, p.rw, p.rh, p.rd );
        }
        
        //Contour Integral
        vector<Vec> contour(const Vec& v, int num, double force){
            vector<Vec> vp;
            Vec tv = v;
            for (int i = 0; i < num; ++i){
                vp.push_back(tv);
                tv += euler3d(tv) * force;
            }
            return vp;
        }

 
        /*! Get QUADRIC Interpolated Data at eval u,v [0-1] */
//        T quadSurf(double u, double v){
//            Patch p =  surfIdx(u,v);
//            
//        }


    //a 6-faced Kernel (3D 'plus" sign)
    
	T sumNbrs (int idx) const{
	 T tdx; 
	 for (int i = 1; i < 7; ++i){ 
		if (nbr(idx)[i] != -1 ) tdx += mData[ nbr(idx)[i] ]; 
	 }  
	 return tdx; 	
	}
    
    T diffNbrs (int idx) const {
		T tdx; 
		tdx += mData[ nbr(idx).xr ] - mData [ nbr(idx).xl ]; //xr - xl
		tdx += mData[ nbr(idx).yt ] - mData [ nbr(idx).yb ]; //yt - yb
		tdx += mData[ nbr(idx).zb ] - mData [ nbr(idx).zf ]; //zb - zf
		return tdx; 
	}

	
	T diffXNbrs (int ix) const{
		return mData[ nbr(ix).xr ] - mData [ nbr(ix).xl ];; 
	}	

    T diffYNbrs ( int ix) const{
		return mData[ nbr(ix).yt ] - mData [ nbr(ix).yb ]; 
	}	
	
	T diffZNbrs (int ix) const{
		return mData[ nbr(ix).zb ] - mData [ nbr(ix).zf ]; //xr - xl
	}		
	
    T dx(int ix) const{ return diffXNbrs(ix); } //or . . .
    T dy(int ix) const{ return diffYNbrs(ix); }
    T dz(int ix) const{ return diffZNbrs(ix); }
    
    //Create Vector Derivative field of another Field f. . . e1 * dx + e2 * dy + e3 * dz . . . in other dimensions use reciprocal frame
    // template<class B>
    // Field& der(const Field<B>& f){
    //     BOUNDITER
    //         int ix = idx(i,j,k);
    //         typename  decltype(Vec() * T()) tdx;
    //         tdx += Vec::x * f.diffXNbrs(ix) * (mWidth-2);
    //         tdx += Vec::y * f.diffYNbrs(ix) * (mHeight-2);
    //         tdx += Vec::z * f.diffZNbrs(ix) * (mDepth-2);
    //         mData[ix] = tdx;
    //     BOUNDEND
    //     return *this;
    // } 
    
    
    //Scalar Tensor 
	double tensNbrs (int idx) const {
		double tdx; 
		tdx += mData[ nbr(idx).xr ][0] - mData [ nbr(idx).xl ][0]; //xr - xl
		tdx += mData[ nbr(idx).yt ][1] - mData [ nbr(idx).yb ][1]; //yt - yb
		tdx += mData[ nbr(idx).zb ][2] - mData [ nbr(idx).zf ][2]; //zb - zf
		return tdx; 
	}
    
    //Scalar Tensor Weighted By 1.0/dim (? spacing *  ?) vec for now . . .
	double tensNbrsWt(int idx) const {
		double tdx; double ww = 1.0 / w(); double wh = 1.0/h(); double wd = 1.0/d();
		tdx += ( mData[ nbr(idx).xr ][0] - mData [ nbr(idx).xl ][0] ) * ww; //xr - xl
		tdx += ( mData[ nbr(idx).yt ][1] - mData [ nbr(idx).yb ][1] ) * wh; //yt - yb
		tdx += ( mData[ nbr(idx).zb ][2] - mData [ nbr(idx).zf ][2] ) * wd; //zb - zf
		return tdx; 
	}    
    

    //Scalar Tensor "reverse weighted" By dim (? spacing *  ?)
	double tensNbrsRwt(int idx) const {
		double tdx;
		tdx += ( mData[ nbr(idx).xr ][0] - mData [ nbr(idx).xl ][0] ) * w(); //xr - xl
		tdx += ( mData[ nbr(idx).yt ][1] - mData [ nbr(idx).yb ][1] ) * h(); //yt - yb
		tdx += ( mData[ nbr(idx).zb ][2] - mData [ nbr(idx).zf ][2] ) * d(); //zb - zf
		return tdx; 
	}  
    
    /*!Guass Siedel Relaxation Solver Using a Previous Field State */
    void gsSolver(const Field& prev){
            //Iterative Pressure Solver substracts pressure tensor out
            int it = 20;
            for (int m = 0; m < it; ++m){
                BOUNDITER
                    int ix = idx(i,j,k);
                    T td = sumNbrs(ix);
                    mData[ix] = (prev[ix] + td ) / 6.0;                            
                BOUNDEND
                
                boundaryConditions(0);
            }
    }
    
    /*! Backwards Diffusion Using a Previous Field State */
    void diffuse(const Field& prev, double diffRate, bool bounded, bool ref){

        		static int it = 20;

                double rate = diffRate * .001 * mNum;
                if  (bounded) {
                    //field is padded ? bounded
                    //iterate
                    for (int k = 0; k < it; ++k){
                        //for each data member
                        for (int i = 0; i < this->num(); ++i){
                        
                            //sum up neighboring values
                            T td = sumNbrs(i);
                            //multiply by (rate / 1 + 6*rate ) (or multiply afterwards?
                            td *= rate ;
                            //add to old value
                            mData[i] = ( prev[i] + td ) / (1 + 6*rate);
                        }
                    }
                    //break;
                } else {       
                    //unbounded so set bounds in loop
                    //iterate
                    for (int n = 0; n < it; ++n){
                        //cout << "BOUNDED:";
                        BOUNDITER
                            int tdx = idx(i,j,k);
                            T td = sumNbrs(tdx);
                            //multiply by (rate )
                            td *= rate;					
                            //add to old value and divide new result by (1 + 6 * rate)
                            mData[tdx] = ( prev[tdx] + td ) / (1 + 6*rate);                            
                        BOUNDEND
                        
                        boundaryConditions(ref);
                    }
                    //break;
                }
        }
        
        /*! Backwards Advection Using a Previous Field State prev and Based on a Velocity Frame f */
        template<class B>
        void advect(const Field& prev, const Field<B>& f, double dt, bool ref){
                    
            double dt0 = dt;// * mWidth;
            BOUNDITER
                int tidx = idx(i,j,k);
                Pnt p = mPoint[ tidx ].trs( f.euler3d( mPoint[tidx] ) * -dt0 );//f[tidx] * -dt0 ); //Lattice Point 
                mData[tidx] = prev.euler3d( p );
            BOUNDEND
            
            boundaryConditions(ref);
        }
        
        //backwards Twist Advection?
     
     // Sets values of a scalar field to divergence of another Field b
     template <class B>
     Field& div(const Field<B>& f){
        //Sum Differences of each Vxl Face (= DIVERGENCE TENSOR)
		BOUNDITER
            int ix = idx(i,j,k);
			mData[ix] = Sca( f.tensNbrsWt(ix) * ( -.5 ) );			
		BOUNDEND   
        boundaryConditions(0);
        return *this;
    }
    
    //swap ptrs to data with another field f of same type
    Field& swap( Field& f ){
        T * tmp = dataPtr(); dataPtr( f.dataPtr() ); f.dataPtr( tmp );
        return *this;
    }
    
    //Operators
    template<class N> 
    Field& operator * ( N val ) { ITN mData[i] *= val; END  return *this; }

    //Operators Pairwise substraction
    template< TT ... XS > 
    Field& operator -= ( const Field< MV<XS...> >& f ) { ITN mData[i] -= f[i]; END  return *this; }
    
    
    //here written fro a scalar fiedl: specialize below for vectors etcs
    void boundaryConditions(bool ref){

        for (int i = 0; i < mFace.size(); ++i){
            int ix = mFace[i];
            static Nbr n;
            n = mNbr[ ix ];
            int type = n.type;
            
            mData[ix] = T(0);
            
            //negation for now treat as vectors
            if (type & LEFT) mData[ix] += ref ? -mData[n.xr] : mData[n.xr]; 
            if (type & RIGHT) mData[ix] += ref ? -mData[n.xl] : mData[n.xl]; 
            if (type & TOP) mData[ix] += ref ? -mData[n.yb] : mData[n.yb]; 
            if (type & BOTTOM) mData[ix] += ref ? -mData[n.yt] : mData[n.yt]; 
            if (type & BACK) mData[ix] += ref ? -mData[n.zf] : mData[n.zf]; 
            if (type & FRONT) mData[ix] += ref ? -mData[n.zb] : mData[n.zb]; 
            
            mData[ix] /= Math::bitcount(type);
        }

    
    }



    };
    
    template<> void Field<Vec>::boundaryConditions(bool ref){  
	
        for (int i = 0; i < mFace.size(); ++i){
            int ix = mFace[i];
            static Nbr n;
            n = mNbr[ ix ];
            int type = n.type;
            
            //negation for now treat as vectors
            if (type & LEFT) mData[ix][0] = ref ? -mData[n.xr][0] : mData[n.xr][0]; 
            if (type & RIGHT) mData[ix][0] = ref ? -mData[n.xl][0] : mData[n.xl][0]; 
            if (type & TOP) mData[ix][1] = ref ? -mData[n.yb][1] : mData[n.yb][1]; 
            if (type & BOTTOM) mData[ix][1] = ref ? -mData[n.yt][1] : mData[n.yt][1]; 
            if (type & BACK) mData[ix][2] = ref ? -mData[n.zf][2] : mData[n.zf][2]; 
            if (type & FRONT) mData[ix][2] = ref ? -mData[n.zb][2] : mData[n.zb][2];            
        }    
    }
    

//    //SPECIALIZATIONS

    template<> void Field< Vec > :: init() {
        ITER  mData[tidx] = Vec(mPoint[tidx]).unit(); ITEND
    }
    
    template<> void Field< Sca > :: init() {
        ITN  mData[i] = Sca(0); END
    }
    
    template<> void Field< Frame > :: init(){
        ITN mData[i].pos() = mPoint[i]; END
    }


//    template<> void Field< Vec > :: draw(float r, float g, float b, float a) {
//        //cout <<"vecdraw"<<endl;
//        drawPush(r,g,b,a);
//    }
//
//    
//    template<> void Field< Sca > :: draw(float r, float g, float b, float a) {
//       ITN  GL::push(); glColor4f(r,g,b,mData[i][0] * a);  GL::translate(mPoint[i].w()); GL::Glyph::Cube( mSpacing ); GL::pop(); END
//    }
//    template<> Frame Field< Frame > :: surf(double u, double v){
//        
//        Patch p =  surfIdx(u,v);
//
//        Dll a = mData[p.a].dll();
//        Dll b = mData[p.b].dll();
//        Dll c = mData[p.c].dll();
//        Dll d = mData[p.d].dll();
//         
//        return Gen::mot( Interp::surface<Dll> (a,b,c,d, p.rw, p.rh) );
//    }
    
    
//    template <>
//	double Field < Frame > :: tensNbrs (int idx) {
//		double tdx; 
//		tdx += mData[ nbr(idx).xr ].pos()[0] - mData [ nbr(idx).xl ].pos()[0]; //xr - xl
//		tdx += mData[ nbr(idx).yt ].pos()[1] - mData [ nbr(idx).yb ].pos()[1]; //yt - yb
//		tdx += mData[ nbr(idx).zb ].pos()[2] - mData [ nbr(idx).zf ].pos()[2]; //zb - zf
//		return tdx; 
//	}
    
    
    
    
    
//    template<>
//    void Field<Dll> :: init(){
//        ITER
//            mGrid[ tidx ]  = Ro::null( px(i),  py(j),  pz(k) ); 
//            mData[ tidx ] = T( mGrid[ tidx ] );
//        ITEND
//    }
    
//    template< class T >
//    void Field< T > :: 

    
    
   //still working on this 
 /*   
    template <class T> 
    class Fluid {

        Field<T> mField;            // Current Vector Veloctiy field
        Field<T> mPrev;             // Previously evaluated Vector field
        
        Field<Sca> mDensity;        // Current Density Field
        Field<Sca> mDensityPrev;    // Previously Evaluated Density Field
        
        Field<Sca> mPressure;       
        Field<Sca> mDivergence;
        
        //Vector Derivative of Scalar Pressure Field is a Vector Field
        Field< typename Sca::VecDeriv > mDerivative;
        
        public:
        
        Fluid(int _w, int _h, int _d)
        : mField(_w,_h,_d), mPrev(_w,_h,_d), 
        mPressure(_w,_h,_d), mDivergence(_w,_h,_d), 
        mDensity(_w*2,_h*2,_d*2,.5), mDensityPrev(_w*2,_h*2,_d*2,.5),
        mDerivative(_w,_h,_d)
        {
            
        }
        
        void reset() {
            
            mField.reset(); mPrev.reset();
            mDensity.reset(); mDensityPrev.reset();
            mPressure.reset();       
            mDivergence.reset();
        }
        

        Field<T>& velocity() { return mField; }
        Field<Sca>& density() { return mDensity; }
        Field<Sca>& pressure() { return mPressure; }
        
        //incompressibility of fluids
        void project(){
            
            //Reset Pressure field
            mPressure.zero();
            mDerivative.zero();
            mDivergence.zero();
            
            //Calculate Divergence tensor
            mDivergence.div( mField );
            
            //Iterative Gauss-Seidel Solver using Divergence Field
            mPressure.gsSolver( mDivergence );

            //Substract out Vector Derivative Gradient of Scalar Pressure
            mField -= mDerivative.der( mPressure ) * .5;
            
            //Satisfy Boundary Conditions
            mField.boundaryConditions(1);
            
        }
        
        void step(double visc, double diff, double advectRate){
            
            //VECTOR VELOCITY FIELD
            mField.swap(mPrev);
            
            mField.diffuse( mPrev, visc, 0, 1);             

            project();
            
            mField.swap(mPrev);
            
           //self-advection 
            mField.advect( mPrev, mPrev, advectRate, 1);
            
            project();
            
            //DENSITY FIELD
            mDensity.swap(mDensityPrev);
            mDensity.diffuse(mDensityPrev, diff, 0, 0);
            mDensity.swap(mDensityPrev);
            
            mDensity.advect(mDensityPrev, mField, advectRate, 0);                
        }

    };

 */
    #undef ITER
    #undef ITEND
    #undef BOUNDITER
    #undef BOUNDEND
    #undef ITN
    #undef END

} //vsr::

#endif

//deprecated

//   //Work on this and clean it up . . .
//	void boundaryConditions(bool ref){
//		int i,j, ix, ix2, ixo, ixo2;
//
//		//WALLS repeat inner settings OR reflect them back
//		//z component
//		for ( i = 1; i < mWidth-1; ++i){
//			for ( j = 1; j < mHeight-1; ++j ){
//				ix = idx(i,j,0);  ix2 = idx(i,j,mDepth-1);
//				
//				mData[ix] =  (ref) ?  Op::re( mData[ix+1], Dlp(0,0,1,0) ) : mData[ix+1];
//				mData[ix2] = (ref) ?  Op::re( mData[ix2-1], Dlp(0,0,1,0) ) : mData[ix2-1];
//
//			}
//		}
//		//x component
//		for ( i = 1; i < mWidth-1; ++i){
//			for ( j = 1; j < mDepth-1; ++j ){
//				 ix = idx(i,0,j);  ixo = idx(i,1,j); 
//				 ix2 = idx(i,mHeight-1,j);  ixo2 = idx(i,mHeight-2,j);
//				mData[ix] = ref ? Op::re( mData[ixo], Dlp(0,1,0,0) ): mData[ixo];//pdata[ixo] * -1 : pdata[ixo]; //
//				mData[ix2] = ref ?  Op::re( mData[ixo2], Dlp(0,1,0,0) ): mData[ixo2];//pdata[ixo2] * -1 : pdata[ixo2];//			//RIGHT
//			}
//		}
//		//y component
//		for ( i = 1; i < mHeight-1; ++i){
//			for ( j = 1; j < mDepth-1; ++j ){
//				 ix = idx(0,i,j);  ixo = idx(1,i,j); 
//				 ix2 = idx(mWidth-1,i,j);  ixo2 = idx(mWidth-2,i,j);		
//				mData[ix] = ref ?  Op::re( mData[ixo], Dlp(1,0,0,0) ) : mData[ixo];	//pdata[ixo] * -1 : pdata[ixo];//			//TOP
//				mData[ix2] = ref ? Op::re( mData[ixo2], Dlp(1,0,0,0) ) : mData[ixo2];//pdata[ixo2] * -1 : pdata[ixo2];//			//BOTTOM
//			}
//		}
//
//		
//		// EDGES Average Neighbors
//		for ( i = 1; i < mWidth-1; ++i){
//			 ix = idx(i,0,0);
//			mData[ ix ] = ( mData[ mNbr[ ix ].yt ] +  mData[ mNbr[ ix ].zb ] ) / 2.0;	//BOTTOM		
//
//			 ix = idx(i,mHeight-1,0);
//			mData[ ix ] = ( mData[ mNbr[ ix ].yb ] +  mData[ mNbr[ ix ].zb ]  ) / 2.0;   //TOP
//
//			 ix = idx(i, mHeight-1, mDepth -1);
//			mData[ ix ] =  ( mData[ mNbr[ ix ].yb ] +  mData[ mNbr[ ix ].zf ] ) / 2.0;   //
//
//			 ix = idx(i,0,mDepth-1);
//			mData[ ix ] = ( mData[ mNbr[ ix ].yt ] +  mData[ mNbr[ ix ].zf ] ) / 2.0;
//		}
//		
//		for ( i = 1; i < mHeight-1; ++i){
//			 ix = idx(0,i,0);
//			mData[ ix ] = ( mData[ mNbr[ ix ].xr ] +  mData[ mNbr[ ix ].zb ] ) / 2.0;
//
//			 ix = idx(mWidth-1,i,0);
//			mData[ ix ] = ( mData[ mNbr[ ix ].xl ] +  mData[ mNbr[ ix ].zb ] ) / 2.0;
//
//			 ix = idx(mWidth-1,i,mDepth-1);
//			mData[ ix ] = ( mData[ mNbr[ ix ].xl] +  mData[ mNbr[ ix ].zf ] ) / 2.0;
//
//			 ix = idx(0,i,mDepth-1);
//			mData[ ix ] = ( mData[ mNbr[ ix ].xr ] +  mData[ mNbr[ ix ].zf ] ) / 2.0;
//		}
//
//		for ( i = 1; i < mDepth-1; ++i){
//			 ix = idx(0,0,i);
//			mData[ ix ] = ( mData[ mNbr[ ix ].yt ] +  mData[ mNbr[ ix ].xr ] ) / 2.0;
//
//			 ix = idx(mWidth-1,0,i);
//			mData[ ix ] = ( mData[ mNbr[ ix ].yt ] +  mData[ mNbr[ ix ].xl ] ) / 2.0;
//
//			 ix = idx(mWidth-1,mHeight-1,i);
//			mData[ ix ] = ( mData[ mNbr[ ix ].yb ] +  mData[ mNbr[ ix ].xl ] ) / 2.0;
//
//			 ix = idx(0,mHeight-1,i);
//			mData[ ix ] = ( mData[ mNbr[ ix ].yb ] +  mData[ mNbr[ ix ].xr ] ) / 2.0;
//		}
//
//		//corners average neighbors		
//		for ( i = 0; i < 8; ++i){
//			mData [ mCorner[i] ] =  (ref) ? sumNbrs( mCorner[i] ) / -1.0 : sumNbrs( mCorner[i] ) / 3.0;
//		}
//
//		
//	}

//alt bound
//        Vec range( const Vec& v){
//            Vec t = v;
//
//            double minx = px(0);
//            double maxx = px(mWidth-1);
//            double miny = py(0);
//            double maxy = py(mHeight-1);
//            double maxz = pz(0);
//            double minz = pz(mDepth-1);
//            
//            if (t[0] < minx) t[0] = minx;
//            if (t[0] > maxx) t[0] = maxx;
//            if (t[1] < miny) t[1] = miny;
//            if (t[1] > maxy) t[1] = maxy;
//            if (t[2] < minz) t[2] = minz;
//            if (t[2] > maxz) t[2] = maxz;
//            
//            double dx = (t[0] - minx)/tw();
//            double dy = (t[1] - miny)/th();
//            double dz = -(t[2] - maxz)/td();
//            
//            return Vec(dx,dy,dz);
            
            
//        /* Totals and Offsets From Center */
//        /*! Total Width */
//        double tw() const { return (mWidth-1) * mSpacing; }
//        /*! Offset Width */
//        double ow() const { return tw() / 2.0 ; }
//        /*! Total Height */
//        double th() const { return (mHeight-1) * mSpacing; }
//        /*! Offset Height */
//        double oh() const { return th() / 2.0 ; }
//        /*! Total Depth */
//        double td() const { return (mDepth-1) * mSpacing; }
//        /*! Offset Depth */
//        double od() const { return td() / 2.0 ; }//0;}//
//        /*! Spatial Positions of ith element in x direction  */
//        double px(int i) const { return -ow() + (mSpacing * i); }
//        /*! Spatial Positions of jth element in y direction  */
//        double py(int j) const { return -oh() + (mSpacing * j); }
//        /*! Spatial Positions of kth element in z direction  */
//        double pz(int k) const { return  od() - (mSpacing * k); }

