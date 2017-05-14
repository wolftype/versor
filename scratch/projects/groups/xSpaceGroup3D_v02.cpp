#include "vsr_app.h"
#include "vsr/form/vsr_group.h"
#include "vsr/form/vsr_twist.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {


  float radius = 0;
  float amt2 = 0;

  float p=2;
  float q=2;
  bool pbar=false;
  bool qbar=false; 
  bool pqbar=false;

  float bravaisType;
  float latticeType;
  
  bool bGlideA, bGlideB, bGlideC;
  float glideA = 0;
  float glideB = 0;
  float glideC = 0;
  float screwAB =0;
  float screwBC = 0;
  float screwAC = 0;
  float screwAB_trs, screwBC_trs;

  float num_width,num_height,num_depth;

  float eyeSep;
  float focal;

  TwistField twf;

  Point mouse;
  Circle cmotif;
   
  bool bDrawCircle, bDrawTwist, bDrawPoints, bTwistIt;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
  	mColor.set(0,0,0);
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(radius,"radius",-100,100)(amt2,"amt2",-100,100);

    //(bToggle,"bToggle")(bSet,"bSet");

    gui(pqbar,"pqbar");                  // abc
    gui(p,"p",0,10);                     
    gui(pbar,"pbar");                    // rotational?
    gui(q,"q",0,10);
    gui(qbar,"qbar"); 
    gui(bravaisType,"bravais",1,7);
    gui(latticeType,"lattice",1,5);
    gui(glideA, "glide a",0,5);
    gui(bGlideA, "glide a alt");
    gui(glideB, "glide b",0,5);
    gui(bGlideB, "glide b alt");
    gui(glideC, "glide c",0,5);
    gui(bGlideC, "glide c alt");
    gui(screwAB, "screw ab",0,5);
    gui(screwBC, "screw bc",0,5);
    gui(screwAC, "screw ac",0,5);
    gui(screwAB_trs, "screw ab trs",0,5);
    gui(screwBC_trs, "screw bc trs",0,5);    
    gui(num_width, "num_width",1,50);
    gui(num_height, "num_height",1,50);
    gui(num_depth, "num_depth",1,50); 
    gui(eyeSep,"eyesep",-10,10);   
    gui(focal,"focal",-100,100);
    gui(bDrawCircle, "drawCircle");  
    gui(bDrawTwist, "drawTwist"); 
    gui(bDrawPoints, "bDrawPoints"); 
    gui(bTwistIt, "twistIt"); 

    for (auto& i : twf.frame()){
      objectController.attach(&i);    
    }   

    objectController.attach(&cmotif);

    radius = .5;
    amt2 = .5;

    num_width = 3;
    num_height = 3;
    num_depth = 3;
    eyeSep = .3;
    focal = 45;

    cmotif = Construct::circle(0,0,0);
   }

   void onAnimate(){
   		scene.camera.lens.eyeSep()=eyeSep;
      scene.camera.lens.focal()=focal;

   }

	void onDraw(){

    // Mouse position makes a sphere
    if (bSetMouse) mouse = calcMouse3D();
    auto dls = Round::sphere(mouse, radius);
	  
    // Make a Box here
    vector<Pnt> motif;
	  float phi = PIOVERTWO * amt2;
	  for (int i=0;i<4;++i){
	    float t = (float)TWOPI * i/4.0;
	    auto rota = Gen::rot(t,-phi);
	    auto rotb = Gen::rot(t,phi);
	    motif.push_back( Construct::point(dls, Vec::x.spin(rota)) );
	    motif.push_back( Construct::point(dls, Vec::x.spin(rotb)) );
	  }


    // The Lattice is composed of Bravais type (monoclinic, triclinic, etc) and Lattice Type (Primitive)
    // Should be determined by p and q ...
	  SpaceGroup3D<Vec>::Lattice lattice = {(int)bravaisType,1};
    // The glide is composed of whether to glide in each direction and also the index into which input vector
	  SpaceGroup3D<Vec>::Glide glide = { {(int)glideA, bGlideA},{(int)glideB,bGlideB},{(int)glideC,bGlideC} };
    // not sure ...
	  SpaceGroup3D<Vec>::Screw screw = { (int)screwAB, (int)screwBC, (int)screwAC, (int)screwAB_trs, (int)screwBC_trs };
	  
    // Make the SpaceGroup
    SpaceGroup3D<Vec> sg(p,q,pbar,qbar,pqbar,lattice,Vec(1,1,1),glide,screw);

   //  // Make a point group by applying to motif
	  // auto bpgroup = sg.apply( motif );

   //  // Make a space group by hanging motif on (primitive) lattice
	  // auto bsgroup = sg.hang( bpgroup, num_width, num_height, num_depth);

   //  // Draw the Box Faces
   //  glBegin(GL_TRIANGLES);
   //  for (int i = 0; i< bsgroup.size(); i+=2){
   //     auto &a = bsgroup[i];
   //     auto &b = bsgroup[i+1];
   //     auto &c = bsgroup[((i+2) % 8 != 0) ? i + 2 : i-6];
       
   //     float ct = (float)i/bsgroup.size();
   //     glColor4f(1.0,1.0,1.0,1.0);//.2,1-ct,1.0);
   //     GL::vertex(a);
   //     GL::vertex(b);
   //     GL::vertex(c);
   //  }
   //  glEnd();

     auto cpgroup = sg.apply(cmotif);
     auto csgroup = sg.hang (cpgroup, num_width, num_height, num_depth);

    if (bDrawCircle) {
       Draw(cmotif,0,1,0);
       for (auto &i : csgroup) Draw(i);
    }


    vector<Pnt> dgroup (csgroup.size() * 20);
    for (auto &i : csgroup){
      for (int j = 0 ; j < 20; ++j)
        dgroup.push_back(Construct::point(i, PI*(float)j/20.0));
    }

    if (bSetMouse) twf.set(dgroup);
    // auto rgroup = dgroup;

    if (bTwistIt) dgroup = twf.apply(dgroup);

    if (bDrawTwist){
      for (auto& i : twf.frame()) DrawB(i);
    }

    if (bDrawPoints) {
      glPushMatrix();
      glBegin(GL_LINES);
      for (auto &i : dgroup)
        GL::vertex(i);
      glEnd();
      glPopMatrix();
    }
	}

};


int main(){
	MyApp app;
	app.start();
	return 0;
}
