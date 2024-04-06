
/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/

#include <vsr/vsr_app.h>
#include <vsr/form/vsr_knot.h>


using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {

	float amt,P, Q;

  Point a = Construct::point(2,0,0);

  bool bDrawMode, bAnimate;

  void onDrawGui(){
		gui(P,"P",0,10);
    gui(Q,"Q",0,10);
    gui(amt,"speed",0,1);
    gui(bAnimate, "animate");
    gui(bDrawMode,"draw mode");
  }

	virtual void setup(){
    //starting knot parameters
		amt = .01;
		P = 5;
		Q = 3;

    bAnimate=true;

    //Attach Point to Controller
    objectController.attach(&a);

	}

  void onAnimate(){
    if (bAnimate){
      TorusKnot tk(0,1,amt);
      a = Round::loc( a.sp( tk.bst() ) );
    }
  }

	void onDraw(){

    //A p,q torus knot
		TorusKnot tk(P,Q,amt);

    //Draw the point (with an added radius)
		draw( Round::dls(a,.1), 1, 1, 0 );

    //Calculate knot orbit starting at point p
		tk.calc( a );

    //draw the many circles thus generated (tk.cir is a vector<Circle>)
    for (auto& i : tk.cir)
       draw(i,0,1,0);

    //Toggle between immediate (fixed function) and programmable pipelines
  //  mRenderGraph.immediate(bDrawMode);

	}
};


int main(){

  MyApp app;
  app.start();

	return 0;

}
