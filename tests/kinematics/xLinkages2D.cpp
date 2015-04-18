#include "vsr_cga2D_types.h"  
#include "vsr_cga2D_draw.h" 
#include "vsr_GLVimpl.h" 

using namespace vsr;
                                   
//N dimensional Frame, could be conformal 2d or 3d or 4d, etc
template<TT DIM > 
struct NFrame{  
	 
	typedef NPnt<DIM> TPNT;  
 	typedef NVec<DIM> TVEC;   //TPNT::BType< TPNT::Mode::Vec >
	typedef NRot<DIM> TROT;   //TPNT::BType< TPNT::Mode::Rot >
	typedef NTrs<DIM> TTRS;   //TPNT::BType< TPNT::Mode::Trs >
	typedef NMot<DIM> TMOT;   //TPNT::BType< TPNT::Mode::Mot >
	
	TPNT mPnt;
	TROT mRot; 
	
	NFrame( const TPNT& p, const TROT& r = TROT(1)) : mPnt(p), mRot(r){} 
    template<typename ... T>
 	NFrame( T ... v ) : mPnt( Ro::null(v...) ), mRot(1) {}
    NFrame() : mPnt( Ro::null(TVEC()) ), mRot(1) {}

	TROT rot() const { return mRot; }
	TPNT pos() const { return mPnt; }

	TROT& rot() { return mRot; }
	TPNT& pos() { return mPnt; } 
	
	TTRS trs() const { return Gen::trs( TVEC(mPnt) ); }  
	
	TMOT mot() { 	
		TMOT m(trs() * rot()); 
		VT n = m.rnorm();
		return (n !=0 ) ? m / n : m ; 
	} 
	
	NFrame& mot( const TMOT& m ){
		mPnt = Ro::null(TVEC()).spin(m);
		mRot = m;  
		return *this;
	}

	
};

template<class TFRAME>
struct Chain{
	TFRAME * mFrame;
	TFRAME * mLink;
	TFRAME * mJoint;
	
	int mNum;
	Chain(int num = 4) : mNum(num) { init(); }
	int num() const { return mNum; }
	void init(){
		alloc(); 
		for (int i = 0;i  < mNum; ++i){
			mLink[i] = TFRAME(0,1);
			//mLink[i].pos().vprint(); 
		}  
		fk();
		for (int i = 0; i < mNum; ++i){
			mFrame[i].pos().vprint();
		}
	}           
	  
	TFRAME& operator[](int idx) { return mFrame[idx]; }
	TFRAME operator[](int idx) const { return mFrame[idx]; }
	
	void alloc(){
		mFrame = new TFRAME[mNum];
		mLink = new TFRAME[mNum];
		mJoint = new TFRAME[mNum];
	} 
	
	/// Forward Kinematics: Absolute Concatenations of previous frame, previous link, and current joint
	void fk() {	
		auto mot = mJoint[0].mot();
		//mot.vprint();
		mFrame[0].mot( mot );
		for (int i = 1; i < mNum; ++i){		
			auto rel =  mLink[i-1].mot() * mJoint[i].mot();
			mFrame[i].mot( mFrame[i-1].mot() * rel );
		}  
	}  
};             
     

struct MyApp : App {
                   
	float time;   

	Chain< NFrame<4> > chain;
	
	void initVariables(){     	
		//chain.fk();
	}
	
	void onDraw(){
      
		for (int i = 0; i < chain.num(); ++i){ 
			//chain[i].pos().vprint();
			Draw(chain[i].pos());
		}

	}  

};
                        
MyApp * app;

int main(){

	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"CGA2D",&glv);    
	app = new MyApp;   
	
	app -> init(win);
	app -> initVariables(); 
	       
	glv << *app;
	Application::run();

	return 0;
}
