#ifndef INTPAIRARRAY_HH
#define INTPAIRARRAY_HH

#include "var.hh"

using Gecode::VarArray;
using Gecode::VarArgArray;

// array traits
namespace MPG {
class IntPairVarArgs;
class IntPairVarArray;
}
namespace Gecode {
template<> class ArrayTraits<Gecode::VarArray<MPG::IntPairVar> > {
public:
    typedef MPG::IntPairVarArray StorageType;
    typedef MPG::IntPairVar ValueType;
    typedef MPG::IntPairVarArgs ArgsType;
};
template<> class ArrayTraits<MPG::IntPairVarArray> {
public:
    typedef MPG::IntPairVarArray StorageType;
    typedef MPG::IntPairVar ValueType;
    typedef MPG::IntPairVarArgs ArgsType;
};
template<> class ArrayTraits<Gecode::VarArgArray<MPG::IntPairVar> > {
public:
    typedef MPG::IntPairVarArgs StorageType;
    typedef MPG::IntPairVar ValueType;
    typedef MPG::IntPairVarArgs ArgsType;
};
template<> class ArrayTraits<MPG::IntPairVarArgs> {
public:
    typedef MPG::IntPairVarArgs StorageType;
    typedef MPG::IntPairVar ValueType;
    typedef MPG::IntPairVarArgs ArgsType;
};
}
// variable arrays
namespace MPG {
class IntPairVarArgs : public VarArgArray<IntPairVar> {
public:
    IntPairVarArgs(void) {}
    IntPairVarArgs(const IntPairVarArgs& a) : VarArgArray<IntPairVar>(a) {}
    IntPairVarArgs(const VarArray<IntPairVar>& a) : VarArgArray<IntPairVar>(a) {}
    IntPairVarArgs(Space& home, int n, int xmin, int xmax, int ymin, int ymax) : VarArgArray<IntPairVar>(n) {
        for(int i=0; i<n; i++) {
            (*this)[i] = IntPairVar(home, xmin, xmax, ymin, ymax);
        }
    }
};
class IntPairVarArray : public VarArray<IntPairVar> {
public:
    IntPairVarArray(void) {}
    IntPairVarArray(const IntPairVarArray& a) : VarArray<IntPairVar>(a) {}
    IntPairVarArray(Space& home, int n) : VarArray<IntPairVar>(home, n) {}
    IntPairVarArray(Space& home, int n, int xmin, int xmax, int ymin, int ymax) : VarArray<IntPairVar>(home, n) {
        for(int i=0; i<n; i++) {
            (*this)[i] = IntPairVar(home, xmin, xmax, ymin, ymax);
        }
    }
};
}

#endif // INTPAIRARRAY_HH
