#ifndef __MPG_INTPAIRVAR_HH__
#define __MPG_INTPAIRVAR_HH__

#include "intpairvarimp.hh"

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

    IntPair::Pair first(void) const {
      return x->first();
    }
    
    IntPair::Pair last(void) const{
      return x->last();
    }

    int size(void) const {
      return x->size();
    }
    
    bool assigned() const {
      return x->assigned();
    }
  };
  
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const IntPairVar& x) {
    std::basic_ostringstream<Char,Traits> s;
    s.copyfmt(os); s.width(0);
    if (x.assigned())
      s << x.first();
    else
      s << '[' << x.first() <<  ".." << x.last() << ']';
    return os << s.str();
  }
}    

#endif
