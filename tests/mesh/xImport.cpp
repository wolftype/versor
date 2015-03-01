/*
 * =====================================================================================
 *
 *       Filename:  xImport.cpp
 *
 *    Description:  import an obj file
 *
 *        Version:  1.0
 *        Created:  08/11/2014 18:30:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_cga3D_frame.h"

using namespace vsr;
using namespace vsr::cga3D;

template<int N>
struct bin {
  float val[N];
};

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,scale;

  bool bReset,bDraw;

  Mesh mesh;

  bin<100> * xco;
  bin<100> * yco;
  bin<100> * zco;

  MyApp(Window * win ) : App(win){

    scene.camera.pos( 0,0,10 ); 
    time = 0;

    mesh.load( "resources/obj/bunny.obj" );

    static const int num = mesh.num();

    xco = new bin<100>[num];
    yco = new bin<100>[num];
    zco = new bin<100>[num];
  }

  void initGui(){
      gui(scale,"scale",-100,100);
      gui(amt,"amt",-100,100);
      gui(bReset,"reset")(bDraw,"draw");
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      getMouse();
      Frame fa;

      vector<Point> point;
      vector<Point> sphere;
      Vec avg(0,0,0);
      for (auto& i : mesh.vertex() ){
        avg += Vec(i[0],i[1],i[2]);
      }
      avg = avg * (1.0 / mesh.num());
      for (auto& i : mesh.vertex() ){
        auto tv = Vec(i[0] - avg[0], i[1] - avg[1], i[2] - avg[2]);
        sphere.push_back( tv.unit().null() ); 
        auto tp = ( tv * scale).null();
        point.push_back( tp );   
        if (bDraw) Draw(tp,1,0,0);
 
      }

      auto px = fa.ipx();
      auto py = fa.ipy();
      auto pz = fa.ipz();

      for (int i = 0; i<100;++i){
        float t = -1.0 + 2.0*(float)i/100;
        auto bx = Gen::bst( px*t );
        auto by = Gen::bst( py*t );
        auto bz = Gen::bst( pz*t );

        for (int j=0;j<mesh.num();++j ){
          auto nx = sphere[j].spin(bx);
          auto ny = sphere[j].spin(by);
          auto nz = sphere[j].spin(bz);
          
          xco[j].val[i] = (Vec(point[j]) <= Vec(nx))[0];//1.0 / Ro::sqd(point[j], nx);
          yco[j].val[i] = (Vec(point[j]) <= Vec(ny))[0];//1.0 / Ro::sqd(point[j], ny);
          zco[j].val[i] = (Vec(point[j]) <= Vec(nz))[0];//1.0 /  Ro::sqd(point[j], nz);

        }

      }

      for (int i=0;i<mesh.num();++i ){
        Par tpar;
        for (int j=0;j<100;++j){
          float t = -1.0 + 2.0*(float)i/100;
           tpar += px*t*xco[i].val[j];
           tpar += py*t*yco[i].val[j];
           tpar += pz*t*zco[i].val[j];
        }
        Draw( Ro::loc( sphere[i].boost( tpar * amt ) ), 0,1,1 );

      }      
  }
   

  
};


MyApp * app;


int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(500,500,"Versor",&glv);    
  app = new MyApp( win ); 
  app -> initGui();
  
  
  glv << *app;

  Application::run();

  return 0;

}
