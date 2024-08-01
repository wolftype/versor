// test external linkage lib
// a shared bundle at src/space/vsr_cga3D_external_linkage.cpp
// e.g. for use in Unity etc.
//

#include <vsr/vsr_app.h>
#include <src/space/vsr_cga3D_external_linkage.cpp>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App {

  void setup(){

    Point * p = cga_tpoint_create();
  }

  void onDraw(){

    Point point;
    DualPlane dlp (0,0,1);
    DualLine dll;

    Line line;
    Vec vec(1,0,0);
    Vec dir (1,1,1);

    cga_line_from_vec_dir (&line, &vec, &dir);

    dll = line.dual() * 2;

    cga_meet_dualline_dualplane (&point, &dll, &dlp);

    Draw(line);
    Draw(dlp);
    Draw(point);

//    nga::Flat::direction (dll.undual());
//
    Biv biv;
    cga_cast_DualLine_Bivector (&dll, &biv);

    Rotor rotor;
    cga_Rotor_from_DualLine (&rotor, &dll);

    Point pt;
    cga_Point_from_DualLine (&pt,&dll);

    Draw(pt, 1,0,0);

    Draw (Vec::y.spin(rotor), 0,1,0);

    Frame * frameA = cga_Frame_create();
    Frame * frameB = cga_Frame_create();

    //cga_Frame_set_pose (frameA, 1,0,0,1,1,1);
    cga_Frame_set_pose (frameA, 2,-1,2,1,1,1);
    cga_Frame_set_pose (frameB, 1,-1,1,1,1,1);

    Draw(*frameA);
    Draw(*frameB);
  }

};


int main(){

  MyApp app;
  app.start();

  return 0;
}
