//see also xTLattice

#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/form/vsr_field.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include <vsr/util/vsr_map.h>
#include <vsr/util/vsr_draw_util.h>

using namespace gfx;
using namespace vsr;
using namespace vsr::cga;


void DrawT (const TFrame_& tf, bool bFlip = false)
{
 Draw (tf.t[0] * (bFlip ? -1.0 : 1.0), 1,0,0);
 Draw (tf.t[1] * (bFlip ? -1.0 : 1.0), 0,1,0);
 Draw (tf.t[2] * (bFlip ? -1.0 : 1.0), 0,0,1);
}


struct MyApp : App {

  Frame frame[8];
  Field<Pnt> field;

  float vSpacing = 1.0;

  void setup(){

    field = Field<Pnt>(2,2,2, 3, 3, 3);

    for (int i = 0; i < field.num(); ++i){
       frame[i].pos(field[i]); 
    }
    
  
  }

  void onDrawGui(){
    gui(vSpacing, "spacing",0,10); 
  }

  void onDraw(){
    //calculate mouse position in world space
    if (bSetMouse) calcMouse3D();

    draw(field);

    for (auto f : frame)
      draw (f);

   }
};


int main(){

  MyApp app;
  app.start();

  return 0;
}
