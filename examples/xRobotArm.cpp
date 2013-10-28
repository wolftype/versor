#include "vsr_cga3D.h"
#include "vsr_GLVimpl.h" 
#include "vsr_chain.h"


using namespace vsr;
using namespace glv;  

struct MyApp : App {
	
	float amt;
	Chain k;
   // Frame baseFrame, targetFrame, secondFrame, finalFrame;
	Pnt targetPos;
	float distA;

	
	MyApp(Window * win) : App(win),
	k(5)
	{}
	
	virtual void initGui(){
       gui(distA, "LinkLength", 1,10);
     	distA = 1.0;
	}

	void onDraw(){
        
		MFrame baseFrame;
		
  		//Target: Mouse Position 
		
		targetPos = Point( Line( interface.mouse.projectNear, interface.vd().ray ),  Ori(1) );  
		
		MFrame targetFrame ( targetPos ); 

		Draw(targetPos, 1,0,0); 
		
		
         MFrame secondFrame( 0, distA, 0 );

		 auto firstSphere = Sphere( secondFrame.pos(), distA ); 	// Make a sphere from a point and a radius, calls Ro::dls( Pnt, float )
         auto targetSphere = Sphere( targetPos, distA ); 
        
		 //Plane of Rotation formed by yaxis of base and target point
		 auto rotationPlane = baseFrame.ly() ^ targetPos;
		
		 Draw(rotationPlane,0,1,0);   
          
 		//XZ plane of Target
		 Dlp targetXZ = targetFrame.dxz();
		 Draw(targetXZ,0,.5,1);
 
		 //Line of Target
		 Dll tline = targetXZ ^ rotationPlane.dual();
		 Draw(tline,1,1,0);
 
		 //Point Pairs of Final joint
		 Par fjoint = ( tline ^ targetSphere ).dual();
		 Draw(fjoint);  
		
 	   	 //Pick the one closest to the base frame
		 MFrame finalFrame ( Ro::split(fjoint,false), Rot(1,0,0,0) );

		 //Sphere around fframe
		 auto ffsphere = Sphere( finalFrame.pos(), distA);

		 //Circle of Possibilities
		 Cir cir = ( ffsphere ^ firstSphere).dual();
		 Draw(cir,.5,1,1);

		 //TWo points where the middle joint could be
		 Par fpair = ( rotationPlane.dual() ^ cir.dual() ).dual();
		 Draw(fpair, 1,.5,.5);

		 //Pick One and put the middle frame there
		 MFrame middleFrame( Ro::split(fpair,true) );


		 //We can store the `positions in a chain class which will sort out relative orientations for us
		 k[0] = baseFrame;
		 k[1] = secondFrame;
		 k[2] = middleFrame;
		 k[3] = finalFrame;
		 k[4] = targetFrame;

		 //Base Frame will rotate to plane defined by its yaxis and target point
		 Rot r1 =  Gen::ratio( Vec::z, rotationPlane.dual().runit() );
		 k[0].rot( r1 );

		 //for all the other frames, calculate joint rotations and link lengths from current positions
		 k.joints(1); 
		 k.links();
         


		 for (int i = 0; i < 4; ++i){

			 glColor3f(0,1,0);
			 gfx::Glyph::Line( k[i].pos(), k[i+1].pos() );
			
		     Draw(k[i]);
		 }


		 Draw(ffsphere,1,0,0,.2);
		 Draw(firstSphere,1,0,0,.2);
	}
};
                        
MyApp * myApp;

int main(){
                          
	
	GLV glv(0,0);	
    		        
	Window * win = new Window(500,500,"Versor",&glv);    
                          
	myApp = new MyApp(win);
	myApp -> initGui();
	
	glv << *myApp;

	Application::run();
	
	return 0;
	
}


         


 
 




 

 
