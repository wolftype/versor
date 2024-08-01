
#include <vsr/vsr_app.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

struct MyApp : App {

  //Some Variables
  bool bToggle = false;
  bool bSet = false;
  bool bMouseSet = false;
  float amt1 = 0;
  float amt2 = 0;

  void setup(){
  }

  void onDraw(){
   }

};


int main(){

//  MyApp app;
//  app.start();

  //using pga = algebra< metric<3,0,1,false>, double >;
  using pga = algebra< metric<4,0,0,false>, double >;
  using t = pga::types;

  auto va = t::vector(-1,1,1,1);
  auto vb = t::vector(1,-1,1,1);
  auto vc = t::vector(1,1,-1,1);
  auto vd = t::vector(1,1,1,-1);

  auto s = t::scalar(1);

  auto e1 = t::e<1>(1);
  auto e2 = t::e<2>(1);
  auto e3 = t::e<3>(1);
  auto e4 = t::e<4>(1);

  auto e12 = t::e<1,2>(1);
  auto e13 = t::e<1,3>(1);
  auto e23 = t::e<2,3>(1);
  auto e14 = t::e<1,4>(1);
  auto e24 = t::e<2,4>(1);
  auto e34 = t::e<3,4>(1);

  auto e123 = t::e<1,2,3>(1);
  auto e124 = t::e<1,2,4>(1);
  auto e134 = t::e<1,3,4>(1);
  auto e234 = t::e<2,3,4>(1);

  auto e1234 = t::e<1,2,3,4>(1);

  s.print();

  e1.print();
  e2.print();
  e3.print();
  e4.print();

  e12.print();
  e13.print();
  e23.print();
  e14.print();
  e24.print();
  e34.print();

  e123.print();
  e124.print();
  e134.print();
  e234.print();

  e1234.print();

  s.dual().print();

    e1.dual().print();
    e2.dual().print();
     e3.dual().print();
     e4.dual().print();

    e12.dual().print();
    e13.dual().print();
    e23.dual().print();
    e14.dual().print();
    e24.dual().print();
    e34.dual().print();

   e123.dual().print();
   e124.dual().print();
   e134.dual().print();
   e234.dual().print();

  e1234.dual().print();


  auto biv = va ^ vb;
  auto tri = va ^ vb ^ vc;
  auto quad = va ^ vb ^ vc ^ vd;

  quad.print();


  return 0;
}
