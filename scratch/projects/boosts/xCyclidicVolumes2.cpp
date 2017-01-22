/*
 * =====================================================================================
 *
 *       Filename:  xCyclidicVolumes.   spawned from xCircleNet5.cpp
 *
 *    Description:  Growing these things by attaching one to another
 *
 *        Version:  1.0
 *        Created:  11/16/2015 15:46:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"
#include "form/vsr_shapes.h"

#include "form/vsr_cyclide.h"
#include "draw/vsr_cyclide_draw.h"

#include "form/vsr_graph.h"
#include "draw/vsr_graph_draw.h"

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {

  CyclideDraw cydraw;

  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;

  /// Curvature velocities of constant coord surfaces
  /// First 
  float YX = 0;
  float ZX = 0;
  float XY = 0;
  float ZY = 0;
  float XZ = 0;
  float YZ = 0;
  float Z1X = 0;
  float Z1Y = 0;
  float X1Y = 0;

  float da, db, dc, dd, de;

  float distX = 0;
  float distZ = 0;
  float distY = 0;

  float spanX = 0;
  float spanY = 0;

  float spin = 0;

  float tensor= 0;
  float tensor2= 0;

  Point mouse;

  /// Draw booleans
  bool bDrawX, bDrawY, bDrawZ;
  bool bDrawBox,bCyDraw;

  bool bFlipZLog;

  float radius;
  float height;

  vector<CyclidicVolume> lattices;

  /// Mesh to Deform
  Mesh mesh;
  /// Coordinates of Mesh inside Lattice
  vector<Vec> coordinates;

  float lightPosX, lightPosY, lightPosZ, alpha;

  Frame frame;

  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(radius,"radius",-100,100);
    gui(height,"height",-100,100);

    gui(spin,"spin",-PI,PI);

    gui(YX,"YX",-100,100)(ZX, "ZX",-100,100);
    gui(XY,"XY",-100,100)(ZY, "ZY",-100,100);
    gui(XZ,"XZ",-100,100)(YZ, "YZ",-100,100);
    gui(Z1X,"Z1X",-100,100)(Z1Y, "Z1Y",-100,100);
    gui(X1Y,"X1Y",-100,100);

    gui(da,"da",-100,100);
    gui(db,"db",-100,100);
    gui(dc,"dc",-100,100);
    gui(dd,"dd",-100,100);
    gui(de,"de",-100,100);

    gui(distX,"distX",-100,100);
    gui(distY,"distY",-100,100);
    gui(distZ,"distZ",-100,100);

    gui(bDrawX)(bDrawY)(bDrawZ);

    gui(bToggle,"bToggle")(bSet,"bSet");

    gui(bCyDraw,"bCyDraw");
    gui(bDrawBox, "bDrawBox");
    gui(alpha,"alpha",0,1);
    gui(lightPosX,"lightPosX",-100,100);
    gui(lightPosY,"lightPosY",-100,100);
    gui(lightPosZ,"lightPosZ",-100,100);

    //spanX = spanY = 1;
    distX=3;
    distY=3;
    distZ=3;
 
    objectController.attach(&frame);

    bDrawX = bDrawY = bDrawZ = false;

  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines
   *-----------------------------------------------------------------------------*/
  void onDraw(){


    mouse = calcMouse3D();

    //OpenGL settings
    GL::lightPos(lightPosX, lightPosY, lightPosZ);
    glColor3f(1,1,1);

    // Center of box at 0,0,0
    frame.pos( -distX/2.0, -distY/2.0, -distZ/2.0 );
    TangentFrame tf0(frame);

    // Create Conformal Volume from initial corner frame, curvature amounts
    CyclidicVolume cy;
    
    cy.setAbsolute(  frame,
             YX, ZX, XY, ZY, XZ, YZ,
             Z1X, Z1Y,   // constant Z=1 in x and y directions
             X1Y,           // constant X=1 in y direction
             distX, distY, distZ);
    
    lattices.clear();

    lattices.push_back(cy);
    for (int i=0;i<3;++i){
      float t = (float)i/10;
      auto& tmp = lattices.back();
      lattices.push_back( tmp.attach( i & 1 , da * t, db * t, dc * t, dd *t) );
    }



    // DRAW ROUTINES
    cydraw.bDrawSurface = false;
    // Draw all lattices
    for (auto& i : lattices){

      // Draw both frames
      cydraw.draw(i.cyclide[0], 0);
      cydraw.draw(i.cyclide[1], 0);
  
      // Draw the Deformation Lattice
      int res = 5;
      auto faces = i.faces(res,res,res);
      for (auto& f : faces) {
        if (bDrawBox) MeshRender::Draw(f,res+1,res+1, alpha);
        MeshRender::DrawEdges(f,res,res);
      }
   }

  }
};


int main(){

  MyApp app;
  app.start();

  return 0;

}
