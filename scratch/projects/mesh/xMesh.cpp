// 2D Plane Mesh Inside A Half-Edge data structure

#include "vsr/vsr_app.h"
#include "vsr/form/vsr_field.h"
#include "vsr/form/vsr_graph.h"

using namespace vsr::cga;
using namespace gfx;

//DRAWING THE MESH GRAPH
template <class T>
void Draw (HEGraph<T> &graph)
{
  for (int i = 0; i < graph.edge ().size (); ++i)
    {
      if (graph.edge (i).opp == NULL)
        glColor3f (1, 0, 0);
      else
        glColor3f (0, 1, 0);

      glBegin (GL_LINES);
      GL::vertex (graph.edge (i).a ().begin ());
      glColor3f (0, 0, 1);
      GL::vertex (graph.edge (i).b ().begin ());
      glEnd ();

      Draw (graph.edge (i).a ());
    }
}

struct MyApp : App
{
  //a graph -- does NOT store any data itself, except its own topology
  gfx::HEGraph<Pnt> graph;

  Field<Pnt> f;

  virtual void setup ()
  {
    scene.camera.pos (0, 0, 10);

    int num = 20;
    f.resize (num, num, 1);

    graph.UV (num, num, f.dataPtr (), false, false);
  }

  virtual void onDraw () { Draw (graph); }
};



int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
