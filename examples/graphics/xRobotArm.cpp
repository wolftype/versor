#include <vsr/vsr_app.h>
#include <vsr/form/vsr_chain.h>


using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {

	float amt,linewidth;
	Chain k = Chain(5);
	Pnt targetPos;
	float distA;

	void setup(){
      bindGLV();
      gui(distA, "LinkLength", 1,10);
      gui(linewidth,"linewidth",0,10);
     	distA = 5.0;
      linewidth=3;
      //immediate(false);
	}

	void onDraw(){

    glLineWidth(linewidth);

		Frame baseFrame;

    auto mouse = calcMouse3D();

    auto v = io().viewdata.ray;
    auto line =  mouse ^ Vec(v[0],v[1],v[2]) ^ Inf(1);//Fl::line( mouse, io().viewdata.ray );

		targetPos = Construct::point( line, Ori(1) );

		Frame targetFrame ( targetPos );

		draw(targetPos, 1,0,0);

    Frame secondFrame( 0, distA, 0 );

    // Make a sphere from a point and a radius, calls Round::dls( Pnt, float )
	  auto firstSphere = Round::sphere( secondFrame.pos(), distA );
    auto targetSphere = Round::sphere( targetPos, distA );

		 //Plane of Rotation formed by yaxis of base and target point
		 auto rotationPlane = baseFrame.ly() ^ targetPos;

		 draw(rotationPlane,0,1,0);

 		//XZ plane of Target
		 DualPlane targetXZ = targetFrame.dxz();
		 draw(targetXZ,0,.5,1);

		 //Line of Target
		 Dll tline = targetXZ ^ rotationPlane.dual();
		 draw(tline,1,1,0);

		 //Point Pairs of Final joint
		 Pair fjoint = ( tline ^ targetSphere ).dual();
		 draw(fjoint);

 	   	 //Pick the one closest to the base frame
		 Frame finalFrame ( Round::split(fjoint,false), Rot(1,0,0,0) );

		 //Sphere around fframe
		 auto ffsphere = Round::sphere( finalFrame.pos(), distA);

		 //Circle of Possibilities
		 Circle cir = ( ffsphere ^ firstSphere).dual();
		 draw(cir,.5,1,1);

		 //TWo points where the middle joint could be
		 Pair fpair = ( rotationPlane.dual() ^ cir.dual() ).dual();
		 draw(fpair, 1,.5,.5);

		 //Pick One and put the middle frame there
		 Frame middleFrame( Round::split(fpair,true) );


		 //We can store the `positions in a chain class which will sort out relative orientations for us
		 k[0] = baseFrame;
		 k[1] = secondFrame;
		 k[2] = middleFrame;
		 k[3] = finalFrame;
		 k[4] = targetFrame;

		 //Base Frame will rotate to plane defined by its yaxis and target point
		 Rot r1 =  Gen::ratio( Vec::z, Vec( rotationPlane.dual().unit() ) );
		 k[0].rot( r1 );

		 //for all the other frames, calculate joint rotations and link lengths from current positions
		 k.calcJoints(1);
		 k.links();



		 for (int i = 0; i < 4; ++i){

			 glColor3f(0,1,0);
			 gfx::Glyph::Line( k[i].pos(), k[i+1].pos() );

		     draw(k[i]);
		 }


		 draw(ffsphere,1,0,0,.4,true);
		 draw(firstSphere,1,0,0,.4,true);
	}
};


int main(){

  MyApp app;
  app.start();

	return 0;

}














