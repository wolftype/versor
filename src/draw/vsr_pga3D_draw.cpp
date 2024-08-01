/*
  (c) Pablo Colapinto
*/

#include <vsr/draw/vsr_pga3D_draw.h>

namespace gfx{


  using namespace vsr;
  using vsr::nga::Op;

  template<> void Renderable< pga::Vec,0 >::DrawImmediate (const pga::Vec& s){
    gfx::Glyph::Line(s);
    glPushMatrix();
      gfx::GL::translate( s.begin() );
      gfx::GL::rotate( Op::AA(s).begin() );
      Glyph::SolidCone();
    glPopMatrix();
  }

  template<> void Renderable< pga::Biv,0 >::DrawImmediate(const  pga::Biv& s){
		double ta = fabs(s.norm());
	  bool sn = Op::sn( s ,  pga::Biv::xy * (-1));

		glPushMatrix();
			gfx::GL::rotate( Op::AA(s).begin() );
			gfx::Glyph::DirCircle( ta, sn );
		glPopMatrix();
  }

  template<> void Renderable< pga::Biv,1 >::DrawImmediate(const  pga::Biv& s){
		double ta = s.norm();
	  bool sn = Op::sn( s ,  pga::Biv::xy * (-1));

		glPushMatrix();
			gfx::GL::rotate( Op::AA(s).begin() );
			gfx::Glyph::FillCircle( ta );
		glPopMatrix();
  }

 template<> void Renderable< pga::Drv,0 >::DrawImmediate (const  pga::Drv& s){
    Renderable< pga::Vec >::DrawImmediate( s.copy< pga::Vec>() );
  }

  template<> void Renderable< pga::Plane,0 >::DrawImmediate (const  pga::Plane& s){
    gfx::GL::translate( Op::Pos(s).begin() );
    gfx::GL::rotate( Op::AA(s).begin() );
    //Glyph::SolidGrid(6,6,5);
     Glyph::Rect(10,10);
  }

  template<> void Renderable< pga::Line,0 >::DrawImmediate (const  pga::Line& s){
      pga::Drv d = nga::Flat::dir( s.undual() );
      pga::Dls v = nga::Flat::loc( s , PAO, true);

      gfx::GL::translate (v.begin());
      gfx::Glyph::DashedLine(d * 10, d * -10);
  }

  template<> void Renderable< pga::Point,0 >::DrawImmediate (const  pga::Point& s){
       gfx::Glyph::Point(s);
  }


} //vsr::

