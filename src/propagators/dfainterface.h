#ifndef DFAINTERFACE_H
#define DFAINTERFACE_H

namespace MPG { namespace IntPair {
    class DFA_I {
    public:
      virtual int S(int, int) = 0; // State function
      virtual int C(int, int) = 0; // Cost function
    };
  }}

#endif
