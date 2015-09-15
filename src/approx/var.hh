#ifndef __MPG_INTPAIRAPPROXVAR_HH__
#define __MPG_INTPAIRAPPROXVAR_HH__

#include "varimp.hh"

using Gecode::VarImpVar;

namespace MPG {
  class IntPairApproxVar : public VarImpVar<MPG::IntPair::IntPairApproxVarImp> {
  protected:
    using VarImpVar<IntPair::IntPairApproxVarImp>::x;
  public:
    IntPairApproxVar(void) {}
    IntPairApproxVar(const IntPairApproxVar& y)
      : VarImpVar<IntPair::IntPairApproxVarImp>(y.varimp()) {}
    IntPairApproxVar(IntPair::IntPairApproxVarImp* y)
      : VarImpVar<IntPair::IntPairApproxVarImp>(y) {}
    // variable creation
    IntPairApproxVar(Space& home, int xmin,int ymin, int xmax, int ymax)
      : VarImpVar<IntPair::IntPairApproxVarImp>
      (new (home) IntPair::IntPairApproxVarImp(home,xmin, ymin, xmax, ymax)) {
    }

    int size(void) const { return x->size(); }
    bool assigned() const { return x->assigned(); }
    IntPair::Pair val() const { return x->val(); }
  };
  
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const IntPairApproxVar& x) { MPG::IntPair::IntPairApproxVarImp *ptr = x.varimp(); os << *ptr; }
}    

#endif
