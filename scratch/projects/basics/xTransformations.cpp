  /*
 * =====================================================================================
 *
 *       Filename:  xTransformations.cpp
 *
 *    Description:  all conformal transformations
 *
 *        Version:  1.0
 *        Created:  11/24/2015 12:08:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_twist.h"
#include "form/vsr_conic.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  
  float amt1 = 0;
  float amt2 = 0;


 // bool bTranslate, bRotate, bScale, bTwist, bBoost;

  float switcher;

  float radius;

  Point point;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt1,"amt1",-100,100)(amt2,"amt2",-100,100)(bToggle,"bToggle")(bSet,"bSet");
    
    gui(switcher,"switcher",0,10);
    
    //objectController.attach(&c);

    objectController.attach(&point);

    mColor.set(.8,.8,.8);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
  
    //mouse = calcMouse3D();
    Circle c = CXY(1);
    gl2psPointSize(5);

    int num =30;
    for (int i =0;i<num;++i){
     float t= (float)i/num;

     Circle tcir;
     
     auto rot = Gen::rot( Biv::xz * PI * -t * amt1);
     auto trs = Gen::trs( Vec::y * t * amt1 );
     
     
     auto dil = Gen::dil( point, -t * amt1);

     auto dll = Twist::Along(DLN(0,1,0), amt1, amt2);
     auto mot = Gen::mot( dll * t ); 

     auto cir = Round::produce( Construct::sphere(2,0,0,amt2), Biv::xz);
     auto bst = Gen::bst( cir.dual() * -t * amt1);

     switch((int)switcher){
        case 0:
          DrawB( Biv::xz, 0,1,0);
          tcir = c.spin(rot); break;
        case 1:
          Draw(Vec::y,0,1,0);
          tcir = c.spin(trs); break;
        case 2:
          Draw(point,0,1,0);
          tcir = c.spin(dil); break;
        case 3:
          Draw(dll,0,1,0);
          tcir = c.spin(mot); break;
        case 4:
         // Draw(cir,0,1,0);
          if (Round::size(cir) < 0) {
            Draw( Round::surround(cir),0,1,1,.2);
            Draw( cir.dual(), 0, 1,0);
          }
         // else if (Round::size(cir) == 0) Draw( cir.dual(),0,1,0 );
          
          tcir = c.spin(bst); break;
        case 5:
          glColor3f(.4+.4*t,0,.4-.4*t);
          glBegin(GL_LINE_LOOP);
          for (int i=0;i<100;i++){
            auto p = Construct::point( c, (float)i/100*TWOPI);
            auto np = Conic::Transform( p, Vec::y, amt1 * t );
            GL::vertex(np.begin());
          }
          glEnd();
          break;
        case 6:
          auto pa = Construct::point( c, 0);
          auto pb = Construct::point( c, PIOVERTWO);
          auto pc = Construct::point( c, PI);

          if (bToggle) Draw( (pa^pb^pc).spin( mot ), .4+.4*t,0,.4-.4*t);
          else {
            auto pa2 = pa.spin(mot);
            auto pb2 = pb.spin(mot);
            auto pc2 = pc.spin(mot);
            Draw(pa2 , 1,0,0);
            Draw(pb2 , 1,0,0);
            Draw(pc2 , 1,0,0);
            Draw( pa2^pb2^pc2 ,.4+.4*t,0,.4-.4*t);
          }
          break;
     }

      if (switcher != 5) Draw(tcir,.4+.4*t,0,.4-.4*t);

    }

  }




  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
