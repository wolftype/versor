#include "vsr_cga2D_types.h"  
#include "vsr_cga2D_draw.h" 
#include "vsr_GLVimpl.h" 

using namespace vsr;
using namespace vsr::cga2D;
     
//Q: what is a motor in 2d . . . (what does it do)
//A: it is the ratio (product) of two lines . . . 
//(and the operator which takes line a to its reflection) 


typedef decltype( Biv() + Drv() ) LogMot;

Mot mot(const LogMot& m){
    VT w = Biv(m).wt();
    if (w==0) return Mot(1,0,m[1],m[2]);

	  VT  c = ( sqrt( fabs ( w ) ) );
	  VT sc = -sin(c);
	  //if(c!=0) sc /= c;
    VT cc = cos(c);
    Drv d = Drv(m) * Math::sinc(c);
   // d.vprint();
    return Mot(cc, sc, d[0], d[1]);  .5;// 
}

LogMot log(const Motor& m){
  LogMot lm = m;

  VT ac = acos( m[0] );	//angle of rotor
  VT den = Math::sinc(ac);
  VT den2 = ac * ac * den;

  //Bivector Part
                          //drb
  auto tbiv = ( ( Ori(1) <= ( lm * Inf(1) ) ) / den * -1.0 );

  Drv drv;
  if (den2 == 0 ) drv = m;

  else {
    Drv tq =  ( tbiv * lm ) ;
   // tq.vprint();
    drv = ( tbiv * tq ) / ( den2 * -1.0 );
   // drv.vprint();
  }
  
  lm = drv + tbiv;

  return lm;
}

struct MyApp : App {
                   
	float time;   

	
	void initVariables(){  
		

	}
	
	void onDraw(){
	  
    time += .01;
    float t= sin(time);                                                      
    Point c = Round::dls(2,0,1).translate(t,0) ;
    
    Draw(c); 
	
    Line la = Flat::line(0,1).rotate( Biv(time) );
    Line lb = Flat::line(1,1).translate(0,1);
    
    Draw(lb, 0,0,1);
    
    Draw(la, 1,0,0) ; 	 

    Line lc = la.reflect(lb);
    
    Draw(lc,0,1,0);
	
    la = la.runit(); lb = lb.runit(); lc = lc.runit();

    Motor m = (lb*la);
    //what is log(m) in 2d??

   // (Ori()<=Drb()).bprint();
   auto lm = log(m);

   for (int i = 0; i < 100; ++i){
      float t= 1.0 * i/100;
    //  Draw( la.spin( mot(lm * t) ) );
      Draw( la.spin( mot(lm * t) ),0,.5,.5 );

   }

   auto par = ( la.dual() ^ c ).dual();
   Draw(par,1,0,0); 
   Draw(par.dual(),0,1,0);

   // lm.bprint();    
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
