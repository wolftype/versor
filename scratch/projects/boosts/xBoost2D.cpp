#include "vsr_cga2D_types.h"
#include "vsr_cga2D_draw.h"
#include "vsr_app.h"

using namespace vsr;
using namespace vsr::cga2D;
using namespace vsr::nga;

struct MyApp : App {

	float time;

	Point * field;
	int width;
	int height;

	float amt;

	void setup(){
    bindGLV();
		gui(amt, "amt", -10,10);
		width = 10; height = 10;
		field = new Point[width * height];

    // Field of points
		for (int i = 0; i < width; ++i){
			float u = 1.0 * i/width;
		  	for (int j = 0; j < height; ++j){
				float v = 1.0 * j/height;

				int idx = i * width + j;
				field[idx] = Round::null( Vec(-2.0 + u*4, -2.0 + v*4) );
			}
		}
    amt = .3;
	}

	void onDraw(){

		time +=.03; 
    
    // A Point type is a "dual circle" in 2D, with a radius.  Here we translate it by sin(time)
		Point cir = Round::dls( Vec(0,0),  1.0).translate( sin(time), 0 );
    //Draw it, red
		Draw(cir,1,0,0);

    // Two points, a and b.
		Point a = Round::point(1,0);
		Point b = Round::point(-1,0);
	
    // Line through points a and b, rotated in plane by amt time*2.0 
    Line line = (a ^ b ^ Inf(1)).rotate( Biv(time) );
		//Draw it, green
    Draw(line, 0, 1, 0);

    // Make point pair from intersection of line and circle
		auto pp =  (line.dual() ^ cir ).dual();
		// Draw it (blue)
    Draw( pp,0,0,1);


    // Generate Boost from Point pair and use to warp field of points
		Bst bst = Gen::bst( pp * amt );
		for (int i = 0; i < width; ++i){
			float u = 1.0 * i/width;
		  	for (int j = 0; j < height; ++j){
				float v = 1.0 * j/height;
				int idx = i * width + j;
        // Warp points, renormalize
				Point np = Round::location( field[idx].spin(bst) );
				// Draw warped points (white)
        Draw(np);
		  }
		}
	}

};


int main(){

  MyApp app;
	app.start();

	return 0;
}
