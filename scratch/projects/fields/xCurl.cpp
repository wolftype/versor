/**
 * Given a vector field, find and show divergence and curl
 * using Simplicial relations
 *
 * Also, find Conformal Energy (difference between 90 angles)
 */

#include <vsr/vsr_app.h>
#include <vsr/form/vsr_differential.h>
#include <vsr/form/vsr_graph.h>
#include <vsr/form/vsr_group.h>
#include <vsr/form/vsr_twist.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

//We'll make a Half-Edge structure out of this
struct VertexData {

  //position of vertex
  Point pnt;
  //normal at vertex (area gradient)
  Vec normal;
  //mean curvature (laplacian of position, normal * mean = SPUR)
  float mean;
  //gaussian curvature (angle deficit OR )
  float gauss;

  //constructor
  VertexData (const Point &p = Construct::point (0, 0, 0)) : pnt (p) {}

  //divergence at pos
  float div = 0.0;
  //curl (Shape Bivector?) at pos: n^dn(a)
  Biv curl = Biv (0, 0, 0);

  //simplicial domain around the vertex used to calculate derivatives
  vector<Simplicial2> simplex;
};

struct MyApp : App
{

  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;


  bool bDrawRecip = false;
  bool bDrawEdges = true;
  bool bPrintOut = false;
  //Use Mean or Gaussian values to color mesh
  bool bMean = false;
  //Use Deficit or Differential to Calculate Curvature
  bool bUseDeficit = true;

  float amt1 = 0;
  float amtGauss = 10;
  float amtMean = 1;

  //data
  MeshData<VertexData> mesh;
  //pointers to data
  HEGraph<VertexData> graph;

  //frame for warping
  Frame frame;

  /*-----------------------------------------------------------------------------
   *  Setup Gui
   *-----------------------------------------------------------------------------*/
  void setup ()
  {
    ///Add Variables to GUI
    gui (amt1, "amt1", -100, 100);
    gui (amtGauss, "amtGauss", -100, 100);
    gui (amtMean, "amtMean", -100, 100);
    gui (bToggle, "bToggle") (bSet, "bSet");
    gui (bMean, "bMean");
    gui (bUseDeficit, "bUseDeficit");
    gui (bDrawRecip, "bDrawRecip");
    gui (bDrawEdges, "bDrawEdges");
    gui (bPrintOut, "bPrint");

    //Generate SpaceGroup Tessellatation
    SpaceGroup2D<Vec> sg (3, 1.3628, true);
    //Hang on lattice, Graph
    int w = 10;
    int h = w;
    auto tmp = sg.hang (Construct::point (0, 0, 0), w, h);
    for (auto &i : tmp)
      mesh.add (VertexData (i));
    mesh.store ();

    graph.UV (w, h, mesh.vertex().data());

    // Add new simplices for each node
    for (auto &i : graph.node ())
      {
        int num = i->valence ().size ();
        i->data ().simplex = vector<Simplicial2> (num);
      }

    frame.pos (0, 0, 2);
    objectController.attach (&frame);
    amt1 = .05;
  }

  void onAnimate ()
  {
    //calculate mouse position in world space
    if (bMouseSet)
      calcMouse3D ();

    for (int i = 0; i < mesh.vertex ().size (); ++i)
      {
        auto &tp = mesh.store (i).pnt;
        float dist = 1.0 / (.01+ Round::dist (tp, frame.pos ()));
        auto dll = Gen::rot (frame.dly() * dist * amt1);
           // Twist::Along (frame.dly (), dist * amt1, 0);
        auto mot = Gen::mot (dll);
        auto bst = Gen::bst (frame.tz () * dist * -amt1);
        mesh[i].pnt = Round::loc (tp.spin (mot));
        //mesh[i].pnt = Round::loc (tp.spin (bst));
      }

    //calculate normal and reciprocal simplices in domain at v
    //to calc normal, we can use area or volume gradient, which
    //are different in the discrete case
    //
    // area gradient = 1/2 * sum of (area * grad)
    // volu gradient = 1/3 * sum of (area * norm)
    //
    //or sphere
    //aka cotan formula = discretize laplacian of position
    //where Df = 2HN (div of curl of position f)
    //
    //Q: how does ra ^ rb area (R) compare to ea ^ eb area (A) ?
    //A: 4RA = 1
    //
    //1. ra is gradient of hat function of x at a (eb_perp/2A)
    //2. rb is gradient of hat function of x at b (ea_perp/2A)
    //3. <ra,ra> (i.e. (ra <= ra * area) is 1/2 (cot a + cot b)
    //maybe L2 DOES multiply dot product by area . . .
    //3. Laplacian = sum: <rb, rb> * area * ea
    //
    //L2 inner prod (pointwise dot * area)
    //Green's identity: <NABLA^2mu,phi> = <NABLAmu, NABLAphi>
    //and mu = sum of phi
    for (auto &i : graph.node ())
      {
        float area = 0;
        float deficit = TWOPI;

        bool bBoundaryVertex = false;

        auto &v = i->data ();

        auto &a = v.pnt;
        auto e = i->valence ();

        Vec normal (0, 0, 1);

        Vec area_grad (0, 0, 0);
        Vec laplacian1 (0, 0, 0);
        Vec laplacian2 (0, 0, 0);
        Vec laplacian3 (0, 0, 0);
        for (int j = 0; j < e.size (); ++j)
          {

            if (e[j]->isBorder ())
              bBoundaryVertex = true;

            auto &b = e[j]->a ().pnt;
            auto &c = e[j]->next->a ().pnt;

            //Build simplex info from triangle positions
            Simplicial2 &sim = v.simplex[j];
            sim = Simplicial2 (Vec (a), Vec (b), Vec (c));

            //Classic cotangent formula for mean curvature
            area_grad += sim.eb * sim.cb + sim.ea * sim.ca;

            //Classic angle deficit formula for gauss curvature
            deficit -= sim.deficit ();  //theta;

            float area = sim.area;
            float area2 = (sim.ra ^ sim.rb).rnorm () / 2.0;

            //Lengths
            float na = sim.ea.norm ();
            float nb = sim.eb.norm ();
            float nra = sim.ra.norm ();
            float nrb = sim.rb.norm ();
            auto biv1 = (sim.ra ^ sim.rb);
            auto biv2 = (sim.ea ^ sim.eb) / (na * na * nb * nb);

            //l2 inner product of inverse mapping gradient
            float raa = (sim.ra <= sim.ra)[0];
            float rbb = (sim.rb <= sim.rb)[0];
            float rab = (sim.ra <= sim.rb)[0];

            // GRADIENT of POSITION (conjugates of each other)
            auto gradA = sim.ra * sim.ea;
            auto gradB = sim.rb * sim.eb;

            // ELEMENTS
            auto gaa = sim.ra * (gradA);
            auto gab = sim.ra * (gradB);
            auto gba = sim.rb * (gradA);
            auto gbb = sim.rb * (gradB);

            // COTAN (DIV of GRADIENT)
            auto lap1 = sim.ea * sim.ca + sim.eb * sim.cb;
            laplacian1 += lap1;

            // EVERY COMBINATION (GREEN'S IDENITY <Grad U, Grad X>)
            auto lap2 =
              (sim.ea * (raa + rab) + sim.eb * (rbb + rab)) * 2.0 * area;
            laplacian2 += lap2;

            //dsum += raa + rbb + rab;

            // SUM OF ELEMENTS
            auto lap3 = (gab + gba + gaa + gbb) * area;
            laplacian3 += lap3;

            // SUM OF DIFFERENT ELEMENTS?
            auto tmpA = sim.ra * (sim.ea.wt ());
            auto tmpB = sim.rb * (sim.eb.wt ());

            if (bPrintOut)
              {
                cout << "OTHER: " << tmpA + tmpB << endl;
                cout << "OTHER: " << tmpB << endl;

                cout << endl;
                cout << " ************** " << endl;
                cout << " ************** " << endl;
                cout << " **** GOAL **** " << endl;
                cout << "CNTRL1: " << lap1 << endl;
                cout << "MATCH2: " << lap2 << endl;
                cout << "MATCH3: " << lap3 << endl;

                cout << " **** COMPONENTS **** " << endl;
                cout << "raa: " << raa << endl;
                cout << "rbb: " << raa << endl;
                cout << "rab: " << rab << endl;
                cout << " **** GRADIENT **** " << endl;
                cout << "grad a: " << gradA << endl;
                cout << "grad b: " << gradB << endl;
                cout << " **** TEST GRADIENT **** " << endl;
                cout << "GAA: " << gaa << endl;
                cout << "GBB: " << gbb << endl;
                cout << "GBA: " << gba << endl;
                cout << "GAB: " << gab << endl;
                //            cout << "GABBA: " << gba + gab << endl;

                //            cout << " **** COMPONENT MATCH **** " << endl;
                //            cout << "CNTRL A: " << sim.ea * raa << endl;
                //            cout << "CNTRL B: " << sim.eb * rbb << endl;
                //            cout << "CNTRL A2: " << sim.ea * rab << endl;
                //            cout << "CNTRL B2: " << sim.eb * rab << endl;
                //            cout << "CNTRL C: " << (sim.ea + sim.eb) * rab << endl;

                //maybe L2 DOES multiply dot product by area . . .
                cout << " COTA: " << .5 * (sim.cc + sim.ca)
                     << " l2 * area: " << raa * area << endl;
                cout << " COTB: " << .5 * (sim.cc + sim.cb)
                     << " l2 * area: " << rbb * area << endl;
                cout << " COTC: " << -.5 * (sim.cc)
                     << " l2 * area: " << rab * area << endl;
              }
          }

        area_grad *= .5;

        v.normal = -laplacian2.unit ();
        v.mean = laplacian2.norm () / 2.0;
        v.gauss = deficit / TWOPI;

        if (!bBoundaryVertex)
          {
            //            cout << "SPUR 1: " << laplacian1 * .5 << endl;
            //            cout << "SPUR 2: " << laplacian2 * .5 << endl;
            //            cout << "SPUR 3: " << laplacian3 * .5 << endl;
            //            cout << "GAUSS: " << v.gauss << endl;
          }
      }

    // Now we have calculated normals
    // we can calculate shape
    for (auto &i : graph.node ())
      {
        bool bBoundaryVertex = !i->closed();

        //Vertex Data
        auto &v = i->data ();
        //Normal calculated above
        Vec n = v.normal;
        //emanating edges
        auto e = i->valence ();
        //eh,
        //
        Rot derivF;
        for (int j = 0; j < e.size (); ++j)
          {
            auto &na = e[j]->a ().normal;
            auto &nb = e[j]->next->a ().normal;

            // Get Simplex Data created previously
            Simplicial2 &sim = v.simplex[j];
            derivF += sim.derivative (n, na, nb);

          }

        auto differential = derivF <= n.duale();
        //differential.print();
        auto gauss = !n.duale() <= differential;
        cout << "GAU: " << gauss << endl;
        cout << "DEF: " << v.gauss << endl;
        cout << "DEF: " << v.gauss * TWOPI << endl;

        if (!bUseDeficit)
          v.gauss = gauss[0];

      }
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routine
   *-----------------------------------------------------------------------------*/
  void onDraw ()
  {

    Draw (frame);

    GL::lightsOff ();
    //EDGES
    if (bDrawEdges)
      {
        for (auto &i : graph.node ())
          {
            auto &v = i->data ();
            for (auto &j : v.simplex)
              {
                glBegin (GL_LINES);
                GL::color (0, 0, 1, 1);
                GL::vertex ((Vec (v.pnt)).begin ());
                GL::vertex ((Vec (v.pnt) + j.ea).begin ());
                GL::vertex ((Vec (v.pnt) + j.ea + j.eb).begin ());
                glEnd ();
              }
          }
      }

    for (auto &i : graph.node ())
      {
        auto &v = i->data ();
        DrawAt (v.normal, v.pnt, 0, 1, 0);
      }
    //RECIPROCAL EDGES
    if (bDrawRecip)
      {
        for (auto &i : graph.node ())
          {
            auto &v = i->data ();
            for (auto &j : v.simplex)
              {
                DrawAt (j.ra, v.pnt, 1, 1, 0);
                DrawAt (j.rb, v.pnt, 1, 1, 0);
              }
          }
      }

    //FACETS (red = more bMean ? mean : gauss curvature)
    glBegin (GL_TRIANGLES);
    for (auto &i : graph.face ())
      {
        auto &a = i->a ();
        auto &b = i->b ();
        auto &c = i->c ();

        float tr = 1.0;
        float tg = 1.0;
        float tb = 1.0;

        float ca = bMean ? (a.mean * amtMean) : (a.gauss * amtGauss);
        float cb = bMean ? (b.mean * amtMean) : (b.gauss * amtGauss);
        float cc = bMean ? (c.mean * amtMean) : (c.gauss * amtGauss);

        bool ba = bMean ? (a.mean < 0) : (a.gauss < 0);
        bool bb = bMean ? (b.mean < 0) : (b.gauss < 0);
        bool bc = bMean ? (c.mean < 0) : (c.gauss < 0);

        float alpha = 1;
        GL::color (tr * ca, ba, tb * (1.0 - ca), alpha);
        GL::normal (a.normal.begin ());
        GL::vertex (a.pnt.begin ());

        GL::color (tr * cb, bb, tb * (1.0 - cb), alpha);
        GL::normal (b.normal.begin ());
        GL::vertex (b.pnt.begin ());

        GL::color (tr * cc, bc, tb * (1.0 - cc), alpha);
        GL::normal (c.normal.begin ());
        GL::vertex (c.pnt.begin ());
      }
    glEnd ();
  }

  /*-----------------------------------------------------------------------------
   *  Keyboard
   *-----------------------------------------------------------------------------*/
  void onKeyDown (const gfx::Keyboard &k)
  {
    if (k.code == 's')
      bMouseSet = !bMouseSet;
  }
};


int main ()
{

  MyApp app;
  app.start ();

  return 0;
}
