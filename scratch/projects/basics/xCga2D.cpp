/*
 * =====================================================================================
 *
 *       Filename:  xCga2D.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/23/2014 15:29:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */



#include "space/vsr_cga2D_types.h"   

#include "util/vsr_cga3D_app.h"

#include "draw/vsr_cga2D_draw.h"

using namespace vsr;

struct MyApp : App {    
   
    virtual void setup(){}
     
  	virtual void onDraw(){ 
        
        auto cA = round::dls( 1.0, 0.0, 0.0 );
        auto cB = round::dls( 2.0, 1.5, 0.0 );

        auto intersect = ( cA ^ cB).dual();
        Draw(cA); Draw(cB); Draw(intersect,1,0,0);
	}
   
	
};


int main(){
                             
  MyApp app;
  app.start();
	return 0;

}
