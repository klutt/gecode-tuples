#ifndef INTPAIRARRAYA_HH
#define INTPAIRARRAYA_HH

#include "var.hh"

using Gecode::VarArray;
using Gecode::VarArgArray;

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
class IntPairApproxVarArgs : public VarArgArray<IntPairApproxVar> {
public:
    IntPairApproxVarArgs(void) {}
    IntPairApproxVarArgs(const IntPairApproxVarArgs& a) : VarArgArray<IntPairApproxVar>(a) {}
    IntPairApproxVarArgs(const VarArray<IntPairApproxVar>& a) : VarArgArray<IntPairApproxVar>(a) {}
    IntPairApproxVarArgs(Space& home, int n, int xmin, int xmax, int ymin, int ymax) : VarArgArray<IntPairApproxVar>(n) {
        for(int i=0; i<n; i++) {
            (*this)[i] = IntPairApproxVar(home, xmin, xmax, ymin, ymax);
        }
    }
};
class IntPairApproxVarArray : public VarArray<IntPairApproxVar> {
public:
    IntPairApproxVarArray(void) {}
    IntPairApproxVarArray(const IntPairApproxVarArray& a) : VarArray<IntPairApproxVar>(a) {}
    IntPairApproxVarArray(Space& home, int n) : VarArray<IntPairApproxVar>(home, n) {}
    IntPairApproxVarArray(Space& home, int n, int xmin, int xmax, int ymin, int ymax) : VarArray<IntPairApproxVar>(home, n) {
        for(int i=0; i<n; i++) {
            (*this)[i] = IntPairApproxVar(home, xmin, xmax, ymin, ymax);
        }
    }
};
}

#endif // INTPAIRARRAY_HH
