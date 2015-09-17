#ifndef INTPAIRARRAYA_HH
#define INTPAIRARRAYA_HH

#include "var.hh"

// using Gecode::VarArray;
// using Gecode::VarArgArray;

// array traits
namespace MPG {
class IntPairApproxVarArgs;
class IntPairApproxVarArray;
}
namespace Gecode {
template<> class ArrayTraits<Gecode::VarArray<MPG::IntPairApproxVar> > {
public:
    typedef MPG::IntPairApproxVarArray StorageType;
    typedef MPG::IntPairApproxVar ValueType;
    typedef MPG::IntPairApproxVarArgs ArgsType;
};
template<> class ArrayTraits<MPG::IntPairApproxVarArray> {
public:
    typedef MPG::IntPairApproxVarArray StorageType;
    typedef MPG::IntPairApproxVar ValueType;
    typedef MPG::IntPairApproxVarArgs ArgsType;
};
template<> class ArrayTraits<Gecode::VarArgArray<MPG::IntPairApproxVar> > {
public:
    typedef MPG::IntPairApproxVarArgs StorageType;
    typedef MPG::IntPairApproxVar ValueType;
    typedef MPG::IntPairApproxVarArgs ArgsType;
};
template<> class ArrayTraits<MPG::IntPairApproxVarArgs> {
public:
    typedef MPG::IntPairApproxVarArgs StorageType;
    typedef MPG::IntPairApproxVar ValueType;
    typedef MPG::IntPairApproxVarArgs ArgsType;
};
}
// variable arrays
namespace MPG {
  class IntPairApproxVarArgs : public Gecode::VarArgArray<MPG::IntPairApproxVar> {
public:
  IntPairApproxVarArgs(void) {}
  IntPairApproxVarArgs(const MPG::IntPairApproxVarArgs& a) : Gecode::VarArgArray<MPG::IntPairApproxVar>(a) {}
  IntPairApproxVarArgs(const Gecode::VarArray<MPG::IntPairApproxVar>& a) : Gecode::VarArgArray<MPG::IntPairApproxVar>(a) {}
  IntPairApproxVarArgs(Gecode::Space& home, int n, int xmin, int xmax, int ymin, int ymax) :
    Gecode::VarArgArray<MPG::IntPairApproxVar>(n) {
        for(int i=0; i<n; i++) {
            (*this)[i] = IntPairApproxVar(home, xmin, xmax, ymin, ymax);
        }
    }
};
  class IntPairApproxVarArray : public Gecode::VarArray<MPG::IntPairApproxVar> {
public:
    IntPairApproxVarArray(void) {}
  IntPairApproxVarArray(const IntPairApproxVarArray& a) : Gecode::VarArray<MPG::IntPairApproxVar>(a) {}
  IntPairApproxVarArray(Gecode::Space& home, int n) : Gecode::VarArray<MPG::IntPairApproxVar>(home, n) {}
  IntPairApproxVarArray(Gecode::Space& home, int n, int xmin, int xmax, int ymin, int ymax) :
    Gecode::VarArray<MPG::IntPairApproxVar>(home, n) {
        for(int i=0; i<n; i++) {
            (*this)[i] = IntPairApproxVar(home, xmin, xmax, ymin, ymax);
        }
    }
};
}

#endif // INTPAIRARRAY_HH
