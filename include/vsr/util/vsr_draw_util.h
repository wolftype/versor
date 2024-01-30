#ifndef Versor_vsr_draw_h
#define Versor_vsr_draw_h

#include <array>
#include "vsr/form/vsr_graph.h"
#include "vsr/form/vsr_tangent.h"


namespace vsr {
namespace util {

using namespace vsr::cga;
using namespace gfx;

struct GraphUtil {

 static void DrawGrid (const HEGraph<Point>& graph, int resU, int resV, bool bStart,
     float red=1.0, float green=1.0, float blue=1.0, float alpha=1.0)
 {
    glBegin(GL_TRIANGLES);
      bool bC= bStart;
      bool bS= false;
      int iter =0;
      //black and white
      for (auto& i : graph.face()){
          auto& a = i->a();
          auto& b = i->b();
          auto& c = i->c();
          //glColor4f(.2,1,.2,.7);
          Vec normal = (Vec(b-a) ^ Vec(c-a)).duale().unit();
          GL::normal( normal.begin() );

          glColor4f(bC * red,bC * green,bC * blue, alpha);
          GL::vertex( a.begin() );
          GL::vertex( b.begin() );
          GL::vertex( c.begin() );
          if (bS) bC = !bC;
          bS = !bS;
      }

    glEnd();

 }
};

struct Cylinder {

  HEGraph<Point> graph;
  std::vector<Point> pnts;
  std::vector<Vec> coords;

  int resU, resV;
  float height, radius;

  void init(int _resU, int _resV) {
    resU = _resU;
    resV = _resV;
    pnts.resize(resU * resV);
    coords.resize(resU * resV);

    graph.UV (resU, resV, pnts.data (), true, false );

    makeCoordsX();
    //diagCoords();
  }

  void makeCoordsX(){

    for (int i = 0; i < resU; ++i)
    {
      for (int j = 0; j < resV; ++j)
      {
            int idx = i * resV + j;
            auto tu = (float) i / resU * PI;
            auto tv = (float) j / (resV-1);

            auto rot = Gen::rot (Biv::yz * tu);
            auto vecx = Vec::y.spin (rot) * 0.5 + Vec(0.0, 0.5, 0.5);
            coords[idx] = vecx + Vec(tv,0,0);
      }
    }
  }
  void makeCoordsZ(){

    for (int i = 0; i < resU; ++i)
    {
      for (int j = 0; j < resV; ++j)
      {
            int idx = i * resV + j;
            auto tu = (float) i / resU * PI;
            auto tv = (float) j / (resV-1);

            auto rot = Gen::rot (Biv::xy * tu);
            auto vecx = Vec::x.spin (rot) * 0.5 + Vec(0.5, 0.5, 0.0);
            //coords[idx] = vecx + Vec(tv,0,0);
            coords[idx] = vecx + Vec(0,0,tv);
      }
    }
  }

  void diagCoords(){

    Vec vec (1.0, 1.0, 1.0);
    Rot rotax = Gen::ratio (Vec::y, vec.unit());

    for (int i = 0; i < resU; ++i)
    {
      for (int j = 0; j < resV; ++j)
      {
            int idx = i * resV + j;

            auto tu = (float) i / resU;
            auto tv = (float) j / (resV-1);

            auto rot = Gen::rot (Biv::xz * tu * PI);
            auto vecx = Vec::x.spin (rot)  * tv * .2;
            Vec tvec = vecx + Vec(0,tv * ROOT2,0);
            coords[idx] = tvec.spin(rotax);
      }
    }
  }
  void coneCoords(){

    Vec vec (1.0, 1.0, 1.0);
    Rot rotax = Gen::ratio (Vec::y, vec.unit());

    for (int i = 0; i < resU; ++i)
    {
      for (int j = 0; j < resV; ++j)
      {
            int idx = i * resV + j;

            auto tu = (float) i / resU;
            auto tv = (float) j / (resV-1);

            auto rot = Gen::rot (Biv::xz * tu * PI);
            auto vecx = Vec::x.spin (rot) * tv * .5;
            Vec tvec = vecx + Vec(0,tv,0);
            coords[idx] = tvec.spin(rotax);
      }
    }
  }

  void draw(bool bStart){
    GraphUtil::DrawGrid (graph, resU, resV, bStart, 1.0, .3, .3, 1.0);
  }
};

struct Grid {

  HEGraph<Point> graph;
  std::vector<Point> pnts;

  int resU, resV;

  bool bInit = false;

  void init(int _resU, int _resV) {
    resU = _resU;
    resV = _resV;
    pnts.resize(resU * resV);
    graph.UV (resU, resV, pnts.data (), false, false);
    bInit = true;
  }

  void draw(bool bStart, float alpha){
    GraphUtil::DrawGrid (graph, resU, resV, bStart, 1.0, .3, .3, alpha);
  }

  void plot(const Point& p0, const Pair& genU, const Pair& genV)
  {
    if (!bInit) return;

    for (int i = 0; i<resU; ++i){
      float tu = 1.0 * i / (resU-1);
      auto bstU = Gen::bst(genU * tu);
      for (int j = 0; j<resV; ++j){
        float tv = 1.0 * j / (resV-1);
        auto bstV = Gen::bst(genV*tv);
        auto con = bstV * bstU;
        pnts[i*resV +j] = Tops::Xf(p0, con);
      }
    }
  }

};

}} //vsr::util::

#endif
