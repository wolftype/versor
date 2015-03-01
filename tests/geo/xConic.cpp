
#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    

  float alpha, beta;
  float time;

  Line ray;
  Point mouse;
   
	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
    time = 0;
	}

  void initGui(){
    gui(alpha,"alpha",-100,100);
    gui(beta,"beta",-100,100);
  }
  


  Point apoint( const Point & p, const Vec& v = Vec::y){
    float talpha = ( p <= v)[0] * alpha;
    Point p2 =  p + Ori(1) * talpha; // * beta
    auto wt = p2 <= Inf(1);
    return Ro::loc( p2 / -wt );
  }

  Point aipoint( const Point& p, const Vec& v = Vec::y){
    float talpha = ( p <= v)[0] * alpha;
    Point p2 = p - Ori(1) * talpha; // / beta
    auto wt = p2 <= Inf(1);
    return Ro::loc( p2 / -wt );

  }

    Line aline( const Line& line, const Vec& v = Vec::y){
    //get points to apply aipoint to
      Point pa = Fl::loc( line, PAO, false ).null();
      Point ipa = apoint(pa,v);
      Point pb = pa.trs( -Fl::dir( line ).copy<Vec>().unit() );
      Point ipb = apoint(pb,v);
      return ipa ^ ipb ^ Inf(1);
    }


  Line ailine( const Line& line, const Vec& v = Vec::y){
    //get points to apply aipoint to
    Point pa = Fl::loc( line, PAO, false).null();
   // Draw(pa,0,1,0);
    Point ipa = aipoint(pa,v);
    Point pb = pa.trs( -Fl::dir( line ).copy<Vec>().unit()  );
  //  Draw(pb,0,1,1);
    Point ipb = aipoint(pb,v);
    return ipa ^ ipb ^ Inf(1);
  }

  //attempt to apply conic transform on something other than a point
  template<class T>
  T tpoint( const T& t){
    auto talpha = ( Vec::y <= t ) * alpha;
    T tw = t +  talpha ^ Ori(1);
    //auto wt = tw <= Inf(1);
    return tw;// / -wt;
  }

    template<class T>
  T tipoint( const T& t){
    auto talpha = ( Vec::y <= t ) * alpha;
    T tw = t - ( talpha ^ Ori(1) );
   // auto wt = tw <= Inf(1);
    return tw;// / -wt;
  }



    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

  	virtual void onDraw(){ 

      getMouse();

      Vec mv = Vec(mouse).unit();

      time += .01;

      //FORWARD TRANSFORM 
      int num = 50;
      for (int i = 0; i < num; ++i){

          float tu= TWOPI * i/num;//-1 + 2.0 * i/num;
          Point p1 = Vec::x.spin( Gen::rot ( Biv::xy * tu ) ).null();

          Draw( p1, 1,1,0);


          Point ap = apoint(p1, mv );
          Draw( ap, 1,0,0);
      }

      //Three Points
      Point pa = Ro::null(-1,0,0);  
      Point pb = Ro::null(1,0,0); 
      Point pc = Ro::null(0,1,0);

     //A Spinning Line to be intersected with a conic
      Point pq = Ro::null(-1,-2,0).rot( Biv::xy * time ) ; 
      Line la = ( pq ^ pb ^ Inf(1) ).unit(); 
      Draw(la,0,0,1);

      //An originating sphere
      Sphere ca = pa ^ pb ^ pc ^ Ro::null(0,0,1); //Draw(ca);

      //Inverse transformed line made from inverse transformed points
      Line ila = ailine( la, mv).unit(); 

      //Meet of inversed transformed line and originating circle
      auto c = ( ila.dual() ^ ca.dual() ).dual();  Draw(c,0,1,0);

      auto opa =  Ro::loc( Ro::split( c, true) );
      auto opb =  Ro::loc( Ro::split( c, false) );

      Draw(opa,1,0,0);
      Draw(opb, 1,0,0);
      //Intersections, re-transformed
      auto ppa = apoint( opa, mv  );
      auto ppb = apoint( opb, mv );

      Draw(ppa); Draw(ppb);

      //tangent
//      auto tana = Tangent::at( ca, opa ) ^ Inf(1);
//      Line nl = ila.reflect( tana.unit().dual() ); 

//      Draw( aline(nl, mv), 0,1,1 );
      
      //Point tpa = Fl::loc( nl, PAO, false).null(); 
      //Point tpb = tpa.trs( -Fl::dir( nl ).copy<Vec>().unit() ); 
     
     // Draw ( tpa, 1,1,0);
      //Draw( tpb, 0,1,1);

      //Draw( apoint(tpa, mv), 1,1,0); 
     // Draw( apoint(tpb,mv), 0,1,1);

     // tpa = Fl::loc( ila2, PAO, false);
     // Draw( apoint(tpa, mv), 1,1,0); 

      //Draw(  ppa ,1,1,0); Draw(  ppb ,1,1,.5);

    //   Draw(ila,0,1,0);
	  //  Draw(ila2,0,1,1);
      //Draw(ca,0,0,1,.1);
   }
   
	
};


MyApp * app;


int main(){
                             
	GLV glv(0,0);	

	Window * win = new Window(500,500,"Versor",&glv);    
	app = new MyApp( win ); 
  app -> initGui();
	
	glv << *app;

	Application::run();

	return 0;

}

