/*
 * =====================================================================================
 *
 *       Filename:  xPlunge.cpp
 *
 *    Description:  plunge of ortho circles
 *
 *        Version:  1.0
 *        Created:  06/09/2015 19:14:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {

  DualSphere sa, sb;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){

    DualSphere sph = Construct::sphere (0,0,0);
    sa = sph.trs(-1,0,0);
    sb = sph.trs(2,0,0);

    objectController.attach(&sa);
    objectController.attach(&sb);
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    auto plunge = sa ^ sb;

    plunge.print();

    Draw(plunge,0,1,1,.5);
    Draw(sa, 1,0,0,.5);
    Draw(sb, 1,0,0,.5);

  }

};


int main(){

  MyApp app;
  app.start();

  return 0;

}
