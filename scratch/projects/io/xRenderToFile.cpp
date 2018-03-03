/*
 * =====================================================================================
 *
 *       Filename:  xRenderToFile.cpp
 *
 *    Description:  render pixelbuffer to a .ppm file
 *
 *        Version:  1.0
 *        Created:  03/25/2015 10:26:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "vsr_cga3D_app.h"  
#include "gfx/gfx_file.h" 
 

using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;

  gfx::File file;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    

  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
 
    Draw(CXY(1), 0,1,0);
  
    if (bReset){
      bReset = false;
      cout << windowData().width() << " " << windowData().height() << endl;
      file.write( windowData().width(), windowData().height(), "/Volumes/Medico/vsr_tmp_output/render/");
    }

  }

  void onResize(int w, int h){
    App::onResize(w,h);  
    printf("resize\n");
    if (file.pixels) delete[] file.pixels;

    file.pixels = new unsigned char[ int(w*h*4) ];   

  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
