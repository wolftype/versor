#include <vsr/vsr_app.h>

using namespace vsr::cga;


decltype (auto) calc (const DualLine& arg)
{
  return (arg + arg * ((arg * 0.5) + arg * ((arg / 6.0) + arg * (arg / 24.0)))
          + 1.0);
}

auto calc3 (const DualLine& arg) RETURNS (
(
  ((arg *(arg / 24.0)))
))

auto calc2 (DualLine arg) RETURNS (
(
  (arg + arg * ((arg * 0.5) + arg * ((arg / 6.0) + arg * (arg / 24.0))) + 1.0 )
))

int main ()
{

  float radians = 0.244346;

  auto Pz = Construct::point (0.0, 0.0, 1.0);
  auto Lz = (Origin (1) ^ Pz ^ Inf (1)).dual ();

  Lz.print ();

  auto arg = Lz * radians * 0.5;

  auto alz = arg * (arg / 24.0);
  auto blz = arg / 6.0 + alz;
  auto clz = arg * blz;
  auto dlz = arg * 0.5 + clz;
  auto elz = arg * dlz;
  auto flz = arg + dlz;
  auto glz = flz + 1.0;

  auto tlz =
    arg + arg * ((arg * 0.5) + arg * ((arg / 6.0) + arg * (arg / 24.0))) + 1.0;

  auto wlz = calc (arg);
  auto xlz = calc2 (arg);

  alz.print ();
  blz.print ();
  clz.print ();
  dlz.print ();
  elz.print ();
  flz.print ();
  glz.print ();

  tlz.print ();
  wlz.print ();
  xlz.print ();

  return 0;
}
