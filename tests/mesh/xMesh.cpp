// testing half-edge data structure

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_field.h"
#include "vsr_graph.h"

using namespace vsr;
using namespace vsr::cga3D;


//DRAWING THE MESH GRAPH
template<class T>
void Draw( HEGraph<T>& graph){
  
  for (int i = 0; i < graph.edge().size(); ++i){
  
      if ( graph.edge(i).opp == NULL ) glColor3f(1,0,0);
      else glColor3f(0,1,0);
    
     glBegin(GL_LINES);
        GL::vertex(  graph.edge(i).a().begin() );
        glColor3f(0,0,1);
        GL::vertex(  graph.edge(i).b().begin() );
     glEnd();

      Draw( graph.edge(i).a() );

      /* mesh.mHalfEdge[i] -> next -> next ->  node -> ptr -> vprint(); */

      /* mesh.mHalfEdge[i] -> node -> ptr -> vprint(); */
  }
}

struct MyApp : App {    
  
   gfx::HEGraph<Pnt> graph; //a graph -- does NOT store any data itself, except its own topology
   
   Field<Pnt> f; 
   
    MyApp(Window * win ) : App(win){

    f.resize(10,10,1);

    scene.camera.pos( 0,0,10 ); 

    static Point pa = Ro::null(-2,0,0);

    int n = 10;
          
    for (int j = 0; j < 10; ++j){
        if (j<2) graph.add( f.at(0,j) );
        else graph.addAt( f.at(0,j), graph.edge(-3) );
        if (j==0) graph.add( f.at(1,j) );
        else if (j<2) graph.addAt( f.at(1, j), graph.edge(-2) );
        else  graph.addAt( f.at(1,j), graph.edge(-1) );
    }

    for (int i = 2; i < 10; ++i){
      int idx = ((i-2) * 9 + 1)*6 -1;
      graph.addAt( f.at(i,0), graph.edge( idx ) );
      graph.addAt( f.at(i,1), graph.edge(-3) );
      for (int j = 2; j < 10; ++j){
          graph.close( graph.edge(-3), graph.edge( idx + (j-1) * 6 ) );
          graph.add( f.at(i,j) );  
      }

    }
    
  }
  
  virtual void onDraw(){ 
       
    
       Draw( graph );
             
  }
   

  
};


MyApp * app;


int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(500,500,"Versor",&glv);    
  app = new MyApp( win ); 
  
  
  glv << *app;

  Application::run();

  return 0;

}
