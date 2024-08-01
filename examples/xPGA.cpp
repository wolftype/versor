// R3,0.1, and it's motor algebra
//
//
//This is self-contained encoding of "PGA" aka plane-based geometric algebra
//for reference (sometimes referred to as "projective" thought the operations
//are not projective, rather the space is. "Plane-based" is a better name)
//
//Note that you can also use this encoding directly in CGA aka conformal, without
//any reduction in speed, or loss of expressivity or geometric meaning,
//by avoiding (un)dualization and restricting your CGA toolset to:
//
//cga::Sca or cga::Scalar
//cga::Vec or cga::Vector
//cga::Biv or cga::Bivector
//cga::Euc or cga::EuclideanPseudoscalar
//cga::Inf or cga::Infinity
//cga::Dlp or cga::DualPlane
//cga::Dll or cga::DualLine
//cga::Dfp or cga::DualFlatPoint
//cga::Rot or cga::Rotor
//cga::Mot or cga::Motor
//cga::Trs or cga::Translator
//cga::Drv or cga::DirectionVector
//cga::Drb or cga::DirectionBivector
//cga::Drt or cga::DirectionTrivector
//
//
//And restricting your generators to
//
//Gen::rot (Bivector)
//Gen::mot (DualLine)
//Gen::trs (Vector or DirectionVector)
//
//Gen::log (Rotor)
//Gen::log (Motor)
//Gen::log (Translator)
//
//
//

#include <vsr/vsr_app.h>
#include <vsr/space/vsr_pga3D.h>
#include <vsr/draw/vsr_pga3D_draw.h>


using namespace vsr::pga;
using namespace gfx;

struct MyApp : App {

  void setup(){

//    cout << "Plane" << endl;
//    Plane().print();
//    cout << "Line" << endl;
//    Line().print();
//    cout << "Point" << endl;
//    Point().print();
//    cout << "MOTOR" << endl;
//    Motor().print();
//    cout << "ROTOR" << endl;
//    Rotor().print();
//    cout << "Translator" << endl;
//    Translator().print();
//    cout << "Motor" << endl;
//    Motor().print();


  }

  void onDraw(){

    // plane with normal 0,1,0 at 1 unit towards infinity
    Plane pa (0,1,0,1);
    // plane with normal 1,0,0 at .5 units towards infinity
    Plane pb (1,0,0,.5);
    // plane with normal 1,1,0 at 1.5 units towards infinity
    Plane pc (1,1,0,.5);
    //meet of pa and pb is a line
    Line line = pa ^ pb;
    //meet of line and pc is a point
    Point point = line ^ pc;


  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
