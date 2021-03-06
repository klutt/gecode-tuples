#ifndef __MPG_INTPAIRVAR_HH__
#define __MPG_INTPAIRVAR_HH__

#include "varimp.hh"

using Gecode::VarImpVar;

namespace MPG {
  class IntPairVar : public VarImpVar<MPG::IntPair::IntPairVarImp> {
  protected:
    using VarImpVar<IntPair::IntPairVarImp>::x;
  public:
    IntPairVar(void) {}
    IntPairVar(const IntPairVar& y)
      : VarImpVar<IntPair::IntPairVarImp>(y.varimp()) {}
    IntPairVar(IntPair::IntPairVarImp* y)
      : VarImpVar<IntPair::IntPairVarImp>(y) {}
    // variable creation
    IntPairVar(Space& home, int xmin,int ymin, int xmax, int ymax)
      : VarImpVar<IntPair::IntPairVarImp>
      (new (home) IntPair::IntPairVarImp(home,xmin, ymin, xmax, ymax)) {
    }

    int size(void) const { return x->size(); }
    bool assigned() const { return x->assigned(); }
    IntPair::Pair val() const { return x->val(); }
  };
  
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const IntPairVar& x) { MPG::IntPair::IntPairVarImp *ptr = x.varimp(); os << *ptr; }
}    

#endif
