/*
 * =====================================================================================
 *
 *       Filename:  xCircleNet5.cpp
 *
 *    Description:  Triply Orthogonal six-sphere coordinate system warping a mesh
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

  /// Curvatures of constant coord surfaces
  float YX = 0;
  float ZX = 0;
  float XY = 0;
  float ZY = 0;
  float XZ = 0;
  float YZ = 0;
  float Z1X = 0;
  float Z1Y = 0;
  float X1Y;

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

  CyclidicVolume lattice;

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

    gui(distX,"distX",-100,100);
    gui(distY,"distY",-100,100);
    gui(distZ,"distZ",-100,100);

    gui(bDrawX)(bDrawY)(bDrawZ);
    gui(tensor,"tensor",0,2);
    gui(tensor2,"tensor2",0,2);
    gui(bFlipZLog,"bFlipZLog");

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
    spin=-2.4;
    //amtX = amtY = .01;
    tensor = 2; tensor2 = 1;

//    mesh.mode(GL::T);
//    mesh.load("resources/obj/bunny.obj");
//
//     for (int i =0;i< mesh.num();i+=3){
//        int a = i; int b = a+1; int c = a+2;
//        auto norm = (mesh[b].Pos-mesh[a].Pos).cross(mesh[c].Pos - mesh[a].Pos).unit();
//        mesh[a].Norm = mesh[b].Norm = mesh[c].Norm = norm;
//    }
//
//    mesh.scale(50);
//    mesh.store();



    objectController.attach(&frame);

    bDrawX = bDrawY = bDrawZ = false;

  }

  template<class T>
  void setCoords(const vector<T>& m){
    coordinates.clear();
    auto pt = lattice.cyclide[0][0].point();

    for (auto& i : m){
      auto x=(i.Pos[0] - pt[0] ) / distX;
      auto y=(i.Pos[1] - pt[1] ) / distY;
      auto z=(i.Pos[2] - pt[2] ) / distZ;

      coordinates.push_back( Vec(x,y,z) );
    }
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

    SixSphere sixsphere(frame);
    sixsphere.set(YX, ZX, XY, ZY, XZ, YZ, distX, distY, distZ); 
    lattice.origin( sixsphere, Z1X, Z1Y, X1Y );

    TangentFrame& tf0 = sixsphere.frame;
    
    // Mesh to be deformed by Lattice
    int w = 20; int h = 20;
    auto obj = Shape::Sphere(radius, w, h);
    setCoords( obj );

    // Conformal map of coords in lattice
    for (int i=0;i<coordinates.size();++i){
     auto p = lattice.at( coordinates[i][0], coordinates[i][1], coordinates[i][2]);
     obj[i].Pos = p;
    }

     // Half-edge data structure over the deformed mesh
     HEGraph<Vertex> graph;
     graph.UV(w,h, obj, true);
     // calc normals @todo put this somewhere more useful
     for (auto& i : graph.node()){
       auto va = i->data().Pos;
       auto vb = i->edge -> a().Pos;
       auto vc = i->edge -> next -> a().Pos;
       Vec3f normal =(vb-va).cross(vc-va).unit() ;
       i->data().Norm = normal;
     }


    // DRAW ROUTINES
    // Draw warped mesh
    Draw(graph, .2, .6,.6);

    // Draw Bounding Lattice
    // 1. Draw Tangent Frames only
    cydraw.bDrawSurface = false;
    if (bCyDraw){
       cydraw.draw(lattice.cyclide[0], 0);
       cydraw.draw(lattice.cyclide[1], 0);
    }

    // 2. Draw the Deformation Lattice
    int res = 10;
    auto faces = lattice.faces(res,res,res);
    for (auto& face : faces) {
       if (bDrawBox) MeshRender::Draw(face,res+1,res+1, alpha);
       MeshRender::DrawEdges(face,res,res);
    }
    
    // 3. consider drawing differently
    auto top = lattice.top();
    cydraw.bDrawSurface = true;
    cydraw.draw(top, tensor);

    if (bDrawX) {
      Draw( tf0.ysurface(YX), 1,.5,0,.2);
      Draw( tf0.zsurface(ZX),1,0,.5,.2);
    }
    if (bDrawY) {
      Draw( tf0.xsurface(XY), .5,1,0,.2);
      Draw( tf0.zsurface(ZY),0,1,.5,.2);
    }
    if (bDrawZ) {
      Draw( tf0.xsurface(XZ), .5,0,1,.2);
      Draw( tf0.ysurface(YZ), 0,.5,1,.2);
    }
  }
};


int main(){

  MyApp app;
  app.start();

  return 0;

}
