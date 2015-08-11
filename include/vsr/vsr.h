#ifndef  vsr_INC
#define  vsr_INC

#include "detail/vsr_generic_op.h"

#endif   /* ----- #ifndef vsr_INC  ----- */



/*!@page operations Operations

  Many functions have the same name but different arguments.
  For instance we can generate a twisting transformation (a vsr::cga::Motor)
  by either relative Lines
  
      Gen::ratio(const DualLine& dllA, const DualLine& dllB, amt t)
  
  or relative Motors
      
      Gen::ratio(const Motor& motA, const Motor& motB, amt t)  
  

  
*/


