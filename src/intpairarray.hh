#ifndef INTPAIRARRAY_HH
#define INTPAIRARRAY_HH

#include "intpairvar.hh"

using Gecode::VarArray;
using Gecode::VarArgArray;

// array traits
namespace MPG {
class IntPairArgs;
class IntPairArray;
}
namespace Gecode {
template<> class ArrayTraits<Gecode::VarArray<MPG::IntPairVar> > {
public:
    typedef MPG::IntPairArray StorageType;
    typedef MPG::IntPairVar ValueType;
    typedef MPG::IntPairArgs ArgsType;
};
template<> class ArrayTraits<MPG::IntPairArray> {
public:
    typedef MPG::IntPairArray StorageType;
    typedef MPG::IntPairVar ValueType;
    typedef MPG::IntPairArgs ArgsType;
};
template<> class ArrayTraits<Gecode::VarArgArray<MPG::IntPairVar> > {
public:
    typedef MPG::IntPairArgs StorageType;
    typedef MPG::IntPairVar ValueType;
    typedef MPG::IntPairArgs ArgsType;
};
template<> class ArrayTraits<MPG::IntPairArgs> {
public:
    typedef MPG::IntPairArgs StorageType;
    typedef MPG::IntPairVar ValueType;
    typedef MPG::IntPairArgs ArgsType;
};
}
// variable arrays
namespace MPG {
class IntPairArgs : public VarArgArray<IntPairVar> {
public:
    IntPairArgs(void) {}
    IntPairArgs(const IntPairArgs& a) : VarArgArray<IntPairVar>(a) {}
    IntPairArgs(const VarArray<IntPairVar>& a) : VarArgArray<IntPairVar>(a) {}
    IntPairArgs(Space& home, int n, int xmin, int xmax, int ymin, int ymax) : VarArgArray<IntPairVar>(n) {
        for(int i=0; i<n; i++) {
            (*this)[i] = IntPairVar(home, xmin, xmax, ymin, ymax);
        }
    }
};
class IntPairArray : public VarArray<IntPairVar> {
public:
    IntPairArray(void) {}
    IntPairArray(const IntPairArray& a) : VarArray<IntPairVar>(a) {}
    IntPairArray(Space& home, int n) : VarArray<IntPairVar>(home, n) {}
    IntPairArray(Space& home, int n, int xmin, int xmax, int ymin, int ymax) : VarArray<IntPairVar>(home, n) {
        for(int i=0; i<n; i++) {
            (*this)[i] = IntPairVar(home, xmin, xmax, ymin, ymax);
        }
    }
};
}

#endif // INTPAIRARRAY_HH
