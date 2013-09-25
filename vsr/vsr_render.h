//render using advanced es2.0  

#include "gfx/gfx_pipe.h"
#include "vsr_products.h"
#include "vsr_xf.h"
#include "vsr_frame.h"  
#include "vsr_field.h"

namespace vsr{   
	
	void Render(const Frame& frame, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 )	{ 
		
		
		// Render( f.x(), mvm, pipe, r,g,b,a);
		// Render( f.y(), mvm, pipe, r,g,b,a);
		// Render( f.z(), mvm, pipe, r,g,b,a);  
		static MBO fm ( Mesh::Frame() ); 
		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
		
		mat = mvm * tmp.copy( vsr::Xf::mat( frame.rot(), frame.vec(), frame.scale() ) );
		mat.fill(mv);
		pipe.program -> uniform("modelView", mv ); 
		
		pipe.line( fm );
		
	 }  
	
   	void Render(const Cir& cir, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 )	{
        
		static MBO circle ( Mesh::Disc(1.0), GL::DYNAMIC );
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
		
		static MBO cone ( Mesh::Cone(.3) );
		static MBO line ( Mesh::Line( vsr::Vec(0,0,0),  vec), GL::DYNAMIC );
		static MBO dir ( Mesh::Dir(), GL::DYNAMIC );

		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
        
		line.mesh[1].Pos = Vec3f(vec[0],vec[1], vec[2]);
		line.mesh.color(r,g,b,a);
		line.update(); 
		mvm.fill(mv);
		pipe.program -> uniform("modelView", mv );
		pipe.line( line );
		   	
		mat = mvm * tmp.copy( vsr::Xf::mat(vec) );
		mat.fill( mv );
		pipe.program -> uniform("modelView", mv ); 
		cone.mesh.color(r,g,b,a);
		cone.update();   
		pipe.line( cone );   
		// Rot r = Gen::ratio( Vec::z, vec )
		// mat = mvm * tmp.copy( vsr::Xf::mat(r, ) );
		// mat.fill( mv );
		// pipe.program -> uniform("modelView", mv );
		// dir.mesh.color(r,g,b,a);
		// dir.update();
		// pipe.line( dir );  

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
			sphere.mesh.color(r,g,b,a);
			sphere.update();   
			pipe.line( sphere );

	        // (real) ? Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t);	

	    } else {


			//Vertex v(pnt[0], pnt[1], pnt[2]);
            mvm.fill(mv);
			pipe.program -> uniform("modelView", mv ); 
			point.mesh[0].Pos = Vec3f(pnt[0],pnt[1],pnt[2]);
			point.mesh.color(r,g,b,a);
			point.update();
			pipe.line( point );

	    }

	} 
	
	void Render(const Par& par, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0  ){
 
		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
		static MBO sphere ( Mesh::Sphere(), GL::DYNAMIC );

		
		double size = Ro::size( par, false );
        std::vector<Pnt> pp = Ro::split( par );
		
		static MBO points ( Mesh::Points(pp) );  
		
        double ta = Ro::size( pp[0], true );   

        Pnt p1 = Ro::cen( pp[0] );
        Pnt p2 = Ro::cen( pp[1] );

        if ( fabs(ta) >  FPERROR ) {				
            
			sphere.mesh.color(r,g,b,a);
			sphere.update();
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
		static MBO line( Mesh::Line( Vec(0,0,-50), Vec(0,0,50) ), GL::DYNAMIC );
		
		// Vec d = (Fl::dir( dll.undual() ) * 10).copy<Vec>();
		// 	    Vec v = Fl::loc( dll , PAO, true);
		// Vec aa = v + d;
		// Vec bb = v - d;
		mat = mvm * tmp.copy( vsr::Xf::mat(dll) );
		mat.fill(mv);
		pipe.program -> uniform("modelView", mv ); 
		// line.mesh[0].Pos = Vec3f( aa[0], aa[1], aa[2]); 
		//  line.mesh[1].Pos = Vec3f( bb[0], bb[1], bb[2]);  
		line.mesh.color(r,g,b,a);
		line.update();  
		pipe.line(line);
	}  
 
	void Render(const Lin& lin, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0  ){
		//         static float mv[16]; 
		// static Mat4f mat;
		// static Mat4f tmp;
		// static MBO line( Mesh::Line( Vec(0,0,-10), Vec(0,0,10) ), GL::DYNAMIC );
		//  
		// mat = mvm * tmp.copy( vsr::Xf::mat(lin) );
		// mat.fill(mv);
		// pipe.program -> uniform("modelView", mv ); 
		// line.mesh.color(r,g,b,a);
		// line.update();
		// pipe.line(line); 
		Render( lin.dual(), mvm, pipe, r,g,b,a);
	}   
	
	
	void Render(const Dlp& dlp, const Mat4f& mvm, Pipe& pipe,float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 ){
		
		static float mv[16]; 
		static Mat4f mat;
		static Mat4f tmp;      
		
		static MBO plane( Mesh::Grid(10,10) );  
				
		mat = mvm * tmp.copy( vsr::Xf::mat( dlp ) );
		mat.fill(mv);
		pipe.program -> uniform("modelView", mv );
		plane.mesh.color(r,g,b,a);
		plane.update(); 
		pipe.line( plane );
	}   
	
	void Render( const Biv& biv, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 ){
		
	   	static float mv[16]; 
		static Mat4f mat;
		static Mat4f tmp;
		
		static MBO cir( Mesh::Circle(), GL::DYNAMIC );
		
		mat = mvm * tmp.copy( vsr::Xf::mat( biv ) );
		mat.fill(mv);
		pipe.program -> uniform("modelView", mv );
		cir.mesh.color(r,g,b,a);
		cir.update(); 
		pipe.line( cir );
		 
	}
	
	// void Render(Field<Vec>& f, const Mat4f& mvm, Pipe& pipe,float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 ){
	// 	
	// }   
	
	void Render(Field<Pnt>& f, const Mat4f& mvm, Pipe& pipe,float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0 ){
		static float mv[16]; 
		static MBO points ( Mesh::Points( f.dataPtr(), f.num() ), GL::DYNAMIC );
		
		mvm.fill(mv);
	    pipe.program -> uniform("modelView", mv); 
		
 		for (int i = 0; i < f.num(); ++i){
 			points.mesh[i].Pos = Vec3f( f[i] );
 		}
	 	points.update();
	 	pipe.line(points); 
		
	} 
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

	void Render( Field<Vec>& f, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0  ){
	
		static MBO points ( Mesh::Points2( &(f.grid(0)), f.dataPtr(), f.num() ).mode(GL::L), GL::DYNAMIC );
	
		// points.mesh
		for (int i = 0; i < f.num(); ++i){  
			Vec3f v( f.grid(i) );
			int idx = i*2+1;
			points.mesh[idx].Pos = v + Vec3f( f[i] ); 
			points.mesh[idx].Col = Vec4f(r,g,b,a);
		}
		points.update();
		pipe.line(points);
	
	}  
	
	void Render( Field<Vec2D>& f, const Mat4f& mvm, Pipe& pipe, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0  ){
	
		static MBO points ( Mesh::Points2D( &(f.grid(0)), f.dataPtr(), f.num() ).mode(GL::L), GL::DYNAMIC );
	
		// points.mesh
		for (int i = 0; i < f.num(); ++i){  
			Vec3f v( f.grid(i) );
			int idx = i*2+1;
			points.mesh[idx].Pos = v + Vec3f( f[i][0], f[i][1], 0 ); 
			points.mesh[idx].Col = Vec4f(r,g,b,a);
		}
		points.update();
		pipe.line(points);
	
	}                   
	
	//with a color field
	void Render( Field<Vec2D>& f, const Mat4f& mvm, Pipe& pipe, Field< Sca >& r, Field< Sca >& g, Field< Sca >& b ){
	
		static MBO points ( Mesh::Points2D( &(f.grid(0)), f.dataPtr(), f.num() ).mode(GL::L), GL::DYNAMIC );
	
		// points.mesh
		for (int i = 0; i < f.num(); ++i){  
			Vec3f v( f.grid(i) );
			int idx = i*2+1;
			points.mesh[idx].Pos = v + Vec3f( f[i][0], f[i][1], 0 ); 
			points.mesh[idx].Col = Vec4f( r[i][0], g[i][0], b[i][0], 1.0 );
		}
		points.update();
		pipe.line(points);
	
	}
	
	#define DRAW(x) Render(x, mvm, pipe); 
    #define DRAWCOLOR(x,r,g,b) Render(x, mvm, pipe, r, g, b); 
}