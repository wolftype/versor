#include "vsr_cga2D_types.h"  
#include "vsr_cga2D_draw.h" 
#include "vsr_GLVimpl.h" 

using namespace vsr;
using namespace vsr::cga2D;
     

// AXIOM 5

typedef decltype( Biv() + Drv() ) LogMot;



struct MyApp : App {
                   
	float time; 
  float dil;  

	
	void initVariables(){  
		gui(dil,"dil",-10,10);

	}
	
	void onDraw(){
	  
    time +=.01;

    Line la = Flat::line(1,0).translate(0,-1);
    Line lb = Flat::line(1,.2);

    auto dla = Ro::dls( Vec(1,-.5), 1);// Flat::dline(1,0);
    auto dlb = Ro::dls( Vec(-1,.4), 1.0 );// Flat::dline(1,1);

    Point pa = Ro::point(-2,1);
    Point pb = Ro::point(4,3);
   
   //circle centered at pb, through pa
    auto c = Ro::at( pb, pa );
    auto p = ( c ^ la.dual() ).dual();

    Dll da = Ro::split(p,true) - pa;
    Dll db = Ro::split(p,false) - pa;

    //AXIOM 5 (pa to la through pb)
    //
    /* Draw(pa); Draw(pb);    //Point pa and Point pb */
    /* Draw(c);               //Circle at pb with pa on it */
    /* Draw(la,1,0,0);        //The line to fold to F(pa) -> la */
    /* Draw(p,0,1,0);         //The meet of the line and the circle */
    /* Draw(da); Draw(db);    //The two possible fold lines */


    //AXIOM 6 (pa and pb to la and lb)
    Draw(pa);
    Draw(pb);       //Point pa and Point pb
    //Draw(dla); 
    //Draw(dlb);
    //

   // auto drv = Flat::dir( la );
   // drv.bprint(); drv.vprint();
    //Draw(drv); 
    
    Draw(la);Draw(lb); 

   // Draw( pa.reflect(la.dual()), 0,0,1);  Draw( pb.reflect(la.dual()), 0,0,1);


    auto lpa = Fl::loc( la, pa, false );// Draw(lpa);
    //line inbetween [BLUE]
    auto lba = Dll(pa - lpa.null()); Draw(lba,0,0,1);

    //circle at lpa through pa [BLUE]
    //auto lca = Ro::at( pa.null(), pa );  Draw(lca, 0,0,1);
    //center of lca
    //auto pca = Ro::loc( lca ); Draw(pca);
    



    auto lpb = Fl::loc( lb, pb, false );// Draw(lpb);
    //line inbetween [BLUE]
    auto lbb = Dll(pb - lpb.null()); Draw(lbb,0,0,1);

    auto inter = lb.dual() ^ lba; //Draw(inter,0,1,0);
    
    Point ip = inter.dual().unit().null(); Draw(ip); 

    auto ra = Ro::at( ip, pa ); Draw(ra);
    auto rb = Ro::at( ip, pb ); Draw(rb);

    auto para = (la.dual() ^ ra).dual(); //par meet
    auto parb = (lb.dual() ^ rb).dual(); //par meet

    /* auto rpaa = Ro::split( para, true ); */ 
    /* auto rpab = Ro::split( para, false ); */ 
    /* auto rpba = Ro::split( parb, true ); */ 
    /* auto rpbb = Ro::split( parb, false ); */ 



   // Draw( );

    
    //circle at lpb through pb [BLUE]
   // auto lcb = Ro::at( lpb.null(), pb );  Draw(lcb, 0,0,1); //lcb.bprint();
    //center of lcb 
    //auto pcb = Ro::loc( lcb ); Draw(pcb);


    

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
