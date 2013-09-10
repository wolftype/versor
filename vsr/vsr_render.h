//render using advanced es2.0  

#include "gfx/gfx_pipe.h"
#include "vsr_products.h"
#include "vsr_xf.h"
#include "vsr_frame.h"

namespace vsr{   
	
	void Render(const Frame& frame, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 )	{ 
		
		
		// Render( f.x(), mvm, pipe, r,g,b,a);
		// Render( f.y(), mvm, pipe, r,g,b,a);
		// Render( f.z(), mvm, pipe, r,g,b,a);  
		//static MBO fm() 
		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
		
		mat = mvm * tmp.copy( vsr::Xf::mat(frame.rot(), frame.vec()) );
		mat.fill(mv);
		pipe.program -> uniform("modelView", mv ); 
		
	   //pipe.line
		
	 }  
	
   	void Render(const Cir& cir, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 )	{
        
		static MBO circle ( Mesh::Circle(1.0), GL::DYNAMIC );
		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
		
		mat = mvm * tmp.copy( vsr::Xf::mat(cir) );
		mat.fill(mv);
		pipe.program -> uniform("modelView", mv );
		
		circle.mesh.color(r,g,b,a);
		circle.update();    
		pipe.line( circle );                    
	} 
	
	void Render(const Vec& vec, const Mat4f& mvm, Pipe& pipe,float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 ){ 
		
		static MBO cone ( Mesh::Cone(.1,.3) );
		static MBO line ( Mesh::Line( vsr::Vec(0,0,0),  vec), GL::DYNAMIC );

		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
        
		line.mesh[1].Pos = Vec3f(vec[0],vec[1], vec[2]);
		line.update(); 
		mvm.fill(mv);
		pipe.program -> uniform("modelView", mv );
	   	pipe.line( line );
   	
		mat = mvm * tmp.copy( vsr::Xf::mat(vec) );
		mat.fill( mv );
		pipe.program -> uniform("modelView", mv );    
		pipe.line( cone );

	}

	void Render(const Pnt& pnt, const Mat4f& mvm, Pipe& pipe,float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 ){

		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
		static MBO sphere ( Mesh::Sphere() );
		static MBO point ( Mesh::Point( pnt ) );

	    double ta = vsr::Ro::size( pnt, true );

	    //Draw as dual Sphere (if |radius| > 0.000001);   

	    if ( fabs(ta) >  FPERROR ) {

	        bool real = ta > 0 ? 1 : 0;	
		  //  tmp.copy( vsr::Xf::mat( pnt, sqrt( fabs(ta) ) ) ); 
			tmp = vsr::Xf::mat( pnt, sqrt( fabs(ta) ) );
			//cout << tmp << endl; 
			mat = mvm * tmp;//tmp;

			mat.fill(mv);
			pipe.program -> uniform("modelView", mv );    
			pipe.line( sphere );

	        // (real) ? Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t);	

	    } else {


			//Vertex v(pnt[0], pnt[1], pnt[2]);
            mvm.fill(mv);
			pipe.program -> uniform("modelView", mv );
			pipe.line( point );

	    }

	} 
	
	void Render(const Par& par, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0  ){
 
		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
		static MBO sphere ( Mesh::Sphere() );

		
		double size = Ro::size( par, false );
        std::vector<Pnt> pp = Ro::split( par );
		
		static MBO points ( Mesh::Points(pp) );  
		
        double ta = Ro::size( pp[0], true );   

        Pnt p1 = Ro::cen( pp[0] );
        Pnt p2 = Ro::cen( pp[1] );

        if ( fabs(ta) >  FPERROR ) {				

            double t = sqrt ( fabs ( ta ) );
            bool real = size > 0 ? 1 : 0;	
            
			mat = mvm * tmp.copy( vsr::Xf::mat(p1) );
			mat.fill(mv);       
			pipe.program -> uniform ("modelView", mv);
			real ? pipe.line( sphere.set(GL::TS) ) : pipe.line( sphere.set(GL::P) ); 

			mat = mvm * tmp.copy( vsr::Xf::mat(p2) ); 
			mat.fill(mv);       
			pipe.program  -> uniform("modelView", mv);
            real ? pipe.line( sphere.set(GL::TS) ) : pipe.line( sphere.set(GL::P) );  

        } else { 
			mvm.fill(mv);
			pipe.program  -> uniform("modelView", mv);
			points.mesh[0].Pos = Vec3f( p1 );
			points.mesh[1].Pos = Vec3f( p2 ); 
			points.mesh.color(r,g,b,a);
			points.update();
			pipe.line( points );
        }
		
	} 
	
	void Render(const Dll& dll, const Mat4f& mvm, Pipe& pipe,float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0  ){
        static float mv[16]; 
		static Mat4f mat;
		static Mat4f tmp;
		static MBO line( Mesh::Line( Vec(0,0,-10), Vec(0,0,10) ) );
		 
		mat = mvm * tmp.copy( vsr::Xf::mat(dll) );
		mat.fill(mv);
		pipe.program -> uniform("modelView", mv ); 
		pipe.line(line);
	}  
 
	void Render(const Lin& lin, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0  ){
        static float mv[16]; 
		static Mat4f mat;
		static Mat4f tmp;
		static MBO line( Mesh::Line( Vec(0,0,-10), Vec(0,0,10) ) );
		 
		mat = mvm * tmp.copy( vsr::Xf::mat(lin) );
		mat.fill(mv);
		pipe.program -> uniform("modelView", mv ); 
		pipe.line(line);
	}   
	
	
	void Render(const Dlp& dlp, const Mat4f& mvm, Pipe& pipe,float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 ){
		
		static float mv[16]; 
		static Mat4f mat;
		static Mat4f tmp;      
		
		static MBO plane( Mesh::Grid(10,10) );  
				
		mat = mvm * tmp.copy( vsr::Xf::mat( dlp ) );
		mat.fill(mv);
		pipe.program -> uniform("modelView", mv ); 
		pipe.line( plane );
	}   
	
	// void Render(const Field<Vec>& f, const Mat4f& mvm, Pipe& pipe,float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 ){
	// 	
	// }
	// 
	// void Render( Field<Pnt>& f, const Mat4f& mvm, Pipe& pipe ){
	// 
	// 	static MBO points ( Mesh::Points( f.dataPtr(), f.num() ), GL::DYNAMIC );
	// 
	// 	// points.mesh
	// 	for (int i = 0; i < f.num(); ++i){
	// 		points.mesh[i].Pos = Vec3f( f[i] );
	// 	}
	// 	points.update();
	// 	pipe.line(points);
	// 
	// }  

	// void Render( Field<Vec>& f, const Mat4f& mvm, Pipe& pipe ){
	// 
	// 	static MBO points ( Mesh::Points2( &(f.grid(0)), f.dataPtr(), f.num() ).mode(GL::L), GL::DYNAMIC );
	// 
	// 	// points.mesh
	// 	for (int i = 0; i < f.num(); ++i){  
	// 		Vec3f v( f.grid(i) );
	// 		points.mesh[i*2+1].Pos = v + Vec3f( f[i] ); 
	// 	}
	// 	points.update();
	// 	pipe.line(points);
	// 
	// } 
	
	#define DRAW(x) Render(x, mvm, pipe); 
    #define DRAW3(x,r,g,b) Render(x, mvm, pipe, r, g, b); 
}