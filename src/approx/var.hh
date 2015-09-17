#ifndef __MPG_INTPAIRAPPROXVAR_HH__
#define __MPG_INTPAIRAPPROXVAR_HH__

#include "varimp.hh"

// using Gecode::VarImpVar;

namespace MPG {
  class IntPairApproxVar : public Gecode::VarImpVar<MPG::IntPair::IntPairApproxVarImp> {
  protected:
    using Gecode::VarImpVar<MPG::IntPair::IntPairApproxVarImp>::x;
  public:
    IntPairApproxVar(void) {}
    IntPairApproxVar(const MPG::IntPairApproxVar& y)
      : Gecode::VarImpVar<MPG::IntPair::IntPairApproxVarImp>(y.varimp()) {}
    IntPairApproxVar(MPG::IntPair::IntPairApproxVarImp* y)
      : Gecode::VarImpVar<MPG::IntPair::IntPairApproxVarImp>(y) {}
    // variable creation
    IntPairApproxVar(Gecode::Space& home, int xmin,int ymin, int xmax, int ymax)
      : Gecode::VarImpVar<MPG::IntPair::IntPairApproxVarImp>
      (new (home) MPG::IntPair::IntPairApproxVarImp(home,xmin, ymin, xmax, ymax)) {
    }

    int size(void) const { return x->size(); }
    bool assigned() const { return x->assigned(); }
    IntPair::Pair val() const { return x->val(); }
  };
  
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const MPG::IntPairApproxVar& x) { MPG::IntPair::IntPairApproxVarImp *ptr = x.varimp(); os << *ptr; }
}    

#endif
