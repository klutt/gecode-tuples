#ifndef __SOLUTION_OK__
#define __SOLUTION_OK__

#include "../propagators/dfainterface.h"

bool solutionOk(MPG::IntPair::DFA_I *d, int px, int py, int qx, int qy, int z) {
  if(px == d->S(qx, z) && py == qy + d->C(qx, z))
    return true;
  return false;
}


#endif
