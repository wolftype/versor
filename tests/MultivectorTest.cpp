#include <gtest/gtest.h>
#include <tests/Common.h>
#include <vsr/detail/vsr_multivector.h>
#include <vsr/detail/vsr_generic_op.h>

using namespace vsr;
using namespace vsr::nga;

TEST(MultivectorTest, DualTest)
{

  using ega_vec = GAVec <algebra <ega4, float>>;
  using cga_point = conformal_Pnt<5>;
  using cga_inf = conformal_Inf<5>;
  using cga_dlp = conformal_Dlp<5>;

  ::testing::StaticAssertTypeEq<ega_vec::basis, Basis<1,2,4,8>>();
  ::testing::StaticAssertTypeEq<decltype(ega_vec().dual())::basis, Basis<7,11,13,14>>();


  using pga_vec = GAVec <algebra <pga3, float>>;
  using pga_dlp = GADlp <algebra <pga3, float>>;

  ::testing::StaticAssertTypeEq<pga_vec::basis, Basis<1,2,4>>();
  ::testing::StaticAssertTypeEq<pga_dlp::basis, Basis<1,2,4,8>>();
  ::testing::StaticAssertTypeEq<cga_dlp::basis, Basis<1,2,4,16>>();
//  ::testing::StaticAssertTypeEq<decltype(pga_vec().dual())::basis, Basis<7,11,13,14>>();


}
