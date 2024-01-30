
#ifndef VSR_GUI_UTIL_INCLUDED
#define VSR_GUI_UTIL_INCLUDED

#include <math.h>

namespace vsr {
  namespace util {
/// Oscillating Parameter as used in xTVolumes.cpp
struct OParam {

  float val = 0;
  float frq = .02;
  bool osc = false;
  bool bAbs = false;
  string name;

  OParam (string _name) : name(_name) {}

  float eval(float timer)
  {
    float ts = sin(timer * frq);
    float ts2 = bAbs ? (ts+1)/2.0 : ts;
    return osc ? val * ts2 : val;
  }

};

}} //::vsr::util

#endif


