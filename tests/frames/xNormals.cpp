/*
 * =====================================================================================
 *
 *       Filename:  xNormals.cpp
 *
 *    Description:  calculate normals at vertices of a mesh using reciprocal frames
 *
 *    assumptions: sum of reciprocal frames along loop around vertex returns normal at vertex
 *
 *        Version:  1.0
 *        Created:  04/07/2014 14:46:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  pretty awesome
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "gfx/gfx_mesh.h"
#include "vsr_graph.h"

using namespace vsr;
using namespace vsr::cga3D;

#define INF Inf(1)

struct VertexData {
  Pnt pnt;
  Dlp dlp;
  VT operator [] (int idx) { return pnt[idx]; }
};


//DRAWING THE MESH GRAPH
template<class T>
void Drw( HEGraph<T>& graph){
  
  for (int i = 0; i < graph.edge().size(); ++i){
  
      if ( graph.edge(i).opp == NULL ) glColor3f(1,0,0);
      else glColor3f(0,1,0);
    
      T& a = graph.edge(i).a();
      T& b = graph.edge(i).b();
     glBegin(GL_LINES);
        GL::vertex( a[0], a[1], a[2]   );
        glColor3f(0,0,1);
        GL::vertex( b[0], b[1], b[2] );
     glEnd();

     // Draw( graph.edge(i).a() );

  }
}



Dlp rframe(const Pnt& pa, const Pnt& pb){
  auto line = pa ^ pb ^ INF;
  VT norm = line.rnorm();
  VT sqnorm = norm*norm;

  auto rpa = (pb^INF)<=(line);
  return rpa / sqnorm;
}

//Reciprocal frame function
vector<Dlp> rframe(const Pnt& pa, const Pnt& pb, const Pnt& pc){

    //1. Make a "tangent space" (here the subspace pseudoscalar is a direct OPNS plane that contains the simplex)
    auto plane = pa ^ pb ^ pc ^ INF;

    //Get squared norm of plane
    VT norm = plane.rnorm();
    VT sqnorm = norm*norm;

    ///plane /= sqnorm;

     //2. A reciprocal frame is built from contraction of the the wedge of others with the plane pseudoscalar . . .
    auto rpa = ( ( pb ^ pc ^ INF ) <= plane );
    auto rpb = ( ( pc ^ pa ^ INF ) <= plane );
    auto rpc = ( ( pa ^ pb ^ INF ) <= plane );

    //AND Normalized?
    rpa /= (-sqnorm);
    rpb /= (-sqnorm);
    rpc /= (-sqnorm);
    
    vector<Dlp> recip;

    recip.push_back(rpa);
    recip.push_back(rpb);
    recip.push_back(rpc);

    return recip;
}

template<class T>
Pnt tnull(const T& t){
  return Ro::null(t[0],t[1],t[2]);
}




/*-----------------------------------------------------------------------------
 *  app
 *-----------------------------------------------------------------------------*/
struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt, amtA,amtB,amtC,vNum;
  bool bDrawRecip;

  Mesh mesh;

  vector<VertexData> vdata;
  HEGraph<VertexData> heg;

  bool bNorm;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;

    int w = 5; 
    int h = w;
    mesh = Mesh::Grid2D(w,h,1);
    for (auto& i: mesh.vertex()){
      vdata.push_back( { tnull(i.Pos), Dlp() } ); 
    }
    heg.UV(w,h,vdata);
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(amtA,"amtA",-1,1);
      gui(amtB,"amtB",-1,1);
      gui(amtC,"amtC",-1,10);
      gui(vNum,"num",1,10);
      gui(bDrawRecip, "drawRecip");
      gui(bNorm, "bNormalize");

  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ INF; 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      getMouse();
      
      static Frame f; Touch(interface,f);
      DrawAt(f.z(), f.pos(), 1,0,0);
      Bst bst = Gen::bst( f.tz() * amt );
      
      int it =0;
      for(auto& i: mesh.original() ){
        auto np = Ro::null( i.Pos[0], i.Pos[1], i.Pos[2] ).sp( bst );
        vdata[it].pnt = np;// = Vec3f(np[0], np[1], np[2]);
        it++;
      }
      //mesh.drawElements();  
      Drw(heg);

      //Given some half edge structure, find normals
      for (auto& i : heg.node() ){
        Dlp sum;
        for (auto& j : i->valence()){
          auto rcp = rframe( i->data().pnt, j->a().pnt );
          sum += rcp + Inf(amtC);
       }
        auto n = sum.rnorm();
        if (bNorm) sum /= n ;
        i->data().dlp = sum ;
      }

      for(auto& i : heg.node() ){ 
        DrawAt( Vec(i->data().dlp), i->data().pnt,0,1,0);
      }
      
      /* for(auto& i : heg.node() ){ */
      /*   if ( i->closed() ){ */
      /*     Dlp sum; */
      /*     for (auto& j : i->valence()){ */
      /*       auto tangent = i->data().dlp ^ j->a().dlp;// ^ j->next->a().dlp; */
      /*       /1* auto td = tangent.dual(); *1/ */
      /*       /1* td /= td[3]; *1/ */
      /*       auto recip = (j->a().dlp) <= tangent; */
      /*      // sum += recip; */
      /*       recip.print(); */
      /*      // tangent.print(); */
      /*       Draw( recip ); */
      /*     } */
      /*     /1* auto n = sum.rnorm(); *1/ */
      /*     /1* if (bNorm) sum /= n ; *1/ */
      /*     //Draw(sum); */
      /*  } */
      /* } */
      
      for (auto& i: heg.node() ){
        if ( i->closed() ) {
          auto valence = i->valence();
          auto a = i->data().dlp.dual(); //I psuedoscalar
          for (auto& j : valence ){
            auto dn = ( j->a().dlp - i->data().dlp);
            auto nn = dn <= a;
            auto td = nn <= !a;
            /* Pnt np = i->data().pnt; */
            /* for (int k=0;k<10;++k){ */
            /*   VT t = (float)k/10 * amtC; */
            /*   np += ( np % (td*amtB) ); */
            /*   Draw( np.null(),1,1,0 ); */
            /*   //Ro::null( i->data().pnt.mot (td*t) ),1,1,0 ); // Ro::cen( i->data().pnt % (td*t)),1,1,0 ); // */
            /*   Draw( i->data().pnt.mot( td*t),0,0,1 ); */
            /* } */
           } 
        }
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
