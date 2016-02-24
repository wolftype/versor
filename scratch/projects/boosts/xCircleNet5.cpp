/*
 * =====================================================================================
 *
 *       Filename:  xCircleNet5.cpp
 *
 *    Description:  Triply Orthogonal six-sphere coordinate system and warping
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


struct Curve3D {

   CyclideQuad cyclide[2];

   struct Data{ Point pos; Vec normal; };

   void set( Frame f, float amtXY, float amtXZ,
             float amtYX, float amtYZ, 
             float amtZX, float amtZY,
             float amtYZ2, float amtXZ2,
             float amtY1, //float amtY2,
             float distX, float distY, float distZ ){
     
     TangentFrame tf0(f);

     auto tfx = tf0.xbend(amtXY, amtXZ, distX);
     auto tfy = tf0.ybend(amtYX, amtYZ, distY);
     auto tfz = tf0.zbend(amtZX, amtZY, distZ);

     TangentFrame tfzy = tfz.zclose(amtYZ2, tf0.point(), tfy.point());
     TangentFrame tfzx = tfz.zclose(amtXZ2, tf0.point(), tfx.point());
     TangentFrame tfxy = tfy.yclose(amtY1, tf0.point(), tfx.point());
     TangentFrame tfzxy = tfxy.close(tfx, tfzx, tfy, tfzy); 
    
     CyclideQuad c0( tf0, tfy, tfxy, tfx);
    // CyclideQuad c1( tf0, tfz, tfzy, tfy);
    // CyclideQuad c2( tf0, tfz, tfzx, tfx);
     CyclideQuad c3( tfz, tfzy, tfzxy, tfzx);
     cyclide[0] = c0;
     cyclide[1] = c3;

   }

   Pair zlog(float u) {
     auto sph1 = cyclide[0].apply(cyclide[0][0].sphere[2], u, 0).dual();
     auto sph2 = cyclide[1].apply(cyclide[1][0].sphere[2], u, 0).dual();
     return Gen::log( Gen::ratio(sph1,sph2, cyclide[0].altWU() || cyclide[0].altWV()) );
   }

   template<class T>
   T apply(const T& input, float u, float v, float w){
     return input.spin(  Gen::bst(zlog(u)*-w) * cyclide[0].xf(u,v) ); //cyclide[0].apply(input,u,v).spin( Gen::bst(zlog(u)*-w) );
   }

   Point at(float u, float v, float w){
      return Round::location(apply( cyclide[0][0].point(), u,v,w));
   }

   vector< vector<Data> > faces(int w, int h, int d){
      vector< vector<Data> > res(6);

      auto pt = cyclide[0][0].point();
      auto normx = cyclide[0][0].tan[0];
      auto normy = cyclide[0][0].tan[1];
      auto normz = cyclide[0][0].tan[2];

      for (int i=0;i<=w;++i){
        float tu = (float)i/w;
        auto log = zlog(tu);
        for (int j=0;j<=h;++j){
          float tv = (float)j/h;
          auto uvbst = cyclide[0].xf(tu,tv);

          int numd;
          if(i==0 || i==w || j==0 || j==h) numd = d;
          else numd = 1;
           
          for (int k=0;k<=numd;++k){
            float tw = (float)k/numd;
            auto bst = Gen::bst(log*-tw) * uvbst;
            auto tres = Round::location( pt.spin(bst) );
            
            if (k==0) res[0].push_back( {tres, Round::direction(normz.spin(bst)).copy<Vec>().unit() } );
            if (i==0) res[1].push_back( {tres, Round::direction(normx.spin(bst)).copy<Vec>().unit() } );
            if (j==0) res[2].push_back( {tres, Round::direction(normy.spin(bst)).copy<Vec>().unit() } );
            if (k==numd) res[3].push_back( {tres, -Round::direction(normz.spin(bst)).copy<Vec>().unit() } );
            if (i==w) res[4].push_back( {tres, -Round::direction(normx.spin(bst)).copy<Vec>().unit() } );
            if (j==h) res[5].push_back( {tres, -Round::direction(normy.spin(bst)).copy<Vec>().unit() } );

          }


        }
      }
      return res;
   }

   

};


struct MyApp : App {
  
  CyclideDraw cydraw;
   
  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  
  float amtXY = 0;
  float amtXZ = 0;
  float amtYX = 0;
  float amtYZ = 0;
  float amtZX = 0;
  float amtZY = 0;
  
  float amtXZ2 = 0;
  float amtYZ2 = 0;

  float amtY1,amtY2;

  float distX =0;
  float distZ = 0;
  float distY = 0;

  float spanX = 0;
  float spanY = 0;

  float spin = 0;

  float tensor= 0;
  float tensor2= 0;

  Point mouse;

  bool bDrawX, bDrawY, bDrawZ;
  bool bFlipZLog;

  float radius;
  float height;

  Curve3D curved;

  Mesh mesh;
  vector<Vec> coordinates;

  float lightPosX, lightPosY, lightPosZ, alpha;
  bool bDrawBox,bCyDraw;


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
    
    gui(amtXY,"amtXY",-100,100)(amtXZ, "amtXZ",-100,100);
    gui(amtYX,"amtYX",-100,100)(amtYZ, "amtYZ",-100,100);
    gui(amtZX,"amtZX",-100,100)(amtZY, "amtZY",-100,100);
    gui(amtXZ2,"amtXZ2",-100,100)(amtYZ2, "amtYZ2",-100,100);
    gui(amtY1,"amtY1",-100,100)(amtY2,"amtY2",-100,100);

    gui(distX,"distX",-100,100);        
    gui(distY,"distY",-100,100);
    gui(distZ,"distZ",-100,100);

    gui(amtYX,"amtYX",-100,100);
    gui(amtXY,"amtXY",-100,100);

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
    auto pt = curved.cyclide[0][0].point();

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

    GL::lightPos(lightPosX, lightPosY, lightPosZ);

    frame.pos( -distX/2.0, -distY/2.0, -distZ/2.0 );
    TangentFrame tf0(frame);
     cydraw.bDrawSurface =false;

     if (bCyDraw){
     cydraw.draw(curved.cyclide[0], 0); 
     cydraw.draw(curved.cyclide[1], 0); 
     }
   //  cydraw.draw(c1, int(tensor) ); 
   //  cydraw.draw(c2, int(tensor2) ); 
   //  cydraw.draw(c3,0);

   //  cout << c0.altWU() << " " << c0.altWV() << " " << c0.altU() << " " << c0.altV() << endl;

    curved.set(frame, amtXY, amtXZ, amtYX, amtYZ, amtZX, amtZY, amtYZ2, amtXZ2, amtY1, distX, distY, distZ);

    int w = 20; int h = 20;
    auto obj = Shape::Sphere(radius, w, h);
    setCoords( obj );

    glColor3f(1,1,1);

   // for(auto & i : coordinates){
    for (int i=0;i<coordinates.size();++i){
     auto p = curved.at( coordinates[i][0], coordinates[i][1], coordinates[i][2]);
     obj[i].Pos = p;
    }

   HEGraph<Vertex> graph;
   graph.UV(w,h, obj, true);

   for (auto& i : graph.node()){
//   
     auto va = i->data().Pos;
     auto vb = i->edge -> a().Pos;
     auto vc = i->edge -> next -> a().Pos;
     Vec3f normal =(vb-va).cross(vc-va).unit() ;//(mesh[i+2].Pos - mesh[i].Pos).cross( mesh[i+1].Pos - mesh[i].Pos).unit();
//   
     i->data().Norm = normal;
   }
    

    Draw(graph, .2, .6,.6);    

    //mesh.drawElements();
    
    //Box

    int res = 10;
    auto faces = curved.faces(res,res,res);
    for (auto& face : faces) {
       if (bDrawBox) MeshRender::Draw(face,res+1,res+1, alpha);
       MeshRender::DrawEdges(face,res,res);
    }
 
    if (bDrawX) {
      Draw( tf0.ysurface(amtXY), 1,.5,0,.2);   
      Draw( tf0.zsurface(amtXZ),1,0,.5,.2);
    }
    if (bDrawY) {
      Draw( tf0.xsurface(amtYX), .5,1,0,.2);   
      Draw( tf0.zsurface(amtYZ),0,1,.5,.2);
    }
    if (bDrawZ) {
      Draw( tf0.xsurface(amtZX), .5,0,1,.2);   
      Draw( tf0.ysurface(amtZY), 0,.5,1,.2);
    }

  }

  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
