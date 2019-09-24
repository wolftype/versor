#include <gtest/gtest.h>
#include <vsr/detail/vsr_xlists.h>
#include <vsr/detail/vsr_instructions.h>

using namespace vsr;

constexpr bits::type e1 = bit<0>();
constexpr bits::type e2 = bit<1>();
constexpr bits::type e3 = bit<2>();
constexpr bits::type e4 = bit<3>();
constexpr bits::type e5 = bit<4>();
constexpr bits::type e12 = e1 ^ e2;
constexpr bits::type e13 = e1 ^ e3;
constexpr bits::type e14 = e1 ^ e4;
constexpr bits::type e15 = e1 ^ e5;
constexpr bits::type e23 = e2 ^ e3;
constexpr bits::type e24 = e2 ^ e4;
constexpr bits::type e25 = e2 ^ e5;
constexpr bits::type e34 = e3 ^ e4;
constexpr bits::type e35 = e3 ^ e5;
constexpr bits::type e45 = e4 ^ e5;

using ega3= metric<3,0,0,false>;
using ega4= metric<4,0,0,false>;
using mga21= metric<2,1,0,false>;
using mga31 = metric<3,1,0,false>;
using cga3 = metric<4,1,0,true>;
using pga3 = metric<3,0,1,false>;
