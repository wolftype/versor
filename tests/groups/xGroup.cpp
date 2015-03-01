//Window and Gui
#include "vsr_GLVimpl.h"

//Math
#include "vsr_group.h"
#include "vsr_graph.h"
#include "vsr_stat.h"
#include "vsr_hull.h"

//Draw Routines
#include "vsr_generic_draw.h"
#include "vsr_graph_draw.h"


using namespace vsr;

//Draw group ops
template<class V>
void Drw(const PointGroup2D<V>& p){
  for (auto i : p.ops ) Draw(i);
}

//Draw Group Operators
template<class V>
void Drw(const Group<V>& g){
  for (auto i : g.ops ) {
    Draw(i);
  }
}

// Draw Edge Arrow
template<class T>
void Drw( typename HEGraph<T>::HalfEdge& e, float r=1.0, float g=1.0, float b=1.0){
  DrawAt( e.a() - e.b(), e.b(), r, g, b );
}

struct MyApp : App {  
  
  float time;  

  float p,q,r;
  float ra,rb,rc,dist,amt;
  bool bOrtho,bSwap;

  float faceidx;

  typedef NEVec<3> V;
  typedef NEBiv<3> Biv;

  vector< V > group;

  ConvexHull<3> hull;

  V tmp;

  MyApp(Window * win ) : App(win)
    //group(3,2)
    {
        scene.camera.pos( 0,0,5 ); 
        time = 0;

        /*-----------------------------------------------------------------------------
         *  INIT ROOT SYSTEM
         *-----------------------------------------------------------------------------*/
        typedef NEVec<3> Type;

        //ROOT SYSTEM
        auto a = V::x.rot( Biv::xy * PIOVERTWO/4 );
        auto b = V::y.rot( Biv::xy * PIOVERTWO/4 );
        auto c = V::y.rot( Biv::yz * PIOVERTWO/4 );

        group = Root::System( a, b, c );
        cout << "GROUP SIZE: " << group.size() << endl;
        
        for (auto& i: group) i.print();
        
        //hull.calc(group); 
        hull.initialFace(group);
        hull.convexPass(group);
        hull.closeHoles(group);

  }

    
  /*-----------------------------------------------------------------------------
   *  INITIALIZE GUI
   *-----------------------------------------------------------------------------*/
    void initGui(){
      gui(p,"p",0,10)(q,"q",0,10)(r,"r",0,10); p = 2; q = 3, r=3;
      gui(ra)(rb)(rc)(dist,"dist",0,100);
      gui(bOrtho,"ortho")(bSwap,"bSwap");
      gui(faceidx,"faceidx",0,20);
      gui(amt,"amt",0,10);
    }
  

    /*-----------------------------------------------------------------------------
     *  ON DRAW
     *-----------------------------------------------------------------------------*/
    virtual void onDraw(){ 

        gfx::GL::twoSidedLighting();

        scene.camera.lens.ortho( bOrtho );
        time +=.01;
       
        for (auto& i : group ) Draw(i);
        
        Draw(hull.graph);

        //ith Face
        /* auto tf = graph.face( (int)faceidx % graph.face().size() ); */                 
        /* ColorTriangles(tf.a(), tf.b(), tf.c() ); */

        /* for (auto& i : hull.graph.face() ){ */
        /*   ColorTriangles(i->a(), i->b(), i->c() ); */
        /* } */

    
   }
   
  
};




/*-----------------------------------------------------------------------------
 *  MAIN
 *-----------------------------------------------------------------------------*/
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



