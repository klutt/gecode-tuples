#ifndef DISTINCT_H
#define DISTINCT_H

#include "../exact/array.hh"

class Distinct : public Propagator {
protected:
  ViewArray<IntPair::IntPairView> va;

public:
  Distinct(Space& home, ViewArray<IntPair::IntPairView> a)
    : Propagator(home), va(a)
  {
    va.subscribe(home, *this, IntPair::PC_INTPAIR_VAL);
  }

  Distinct(Space& home, bool share, Distinct& prop)
    : Propagator(home, share, prop) {
    va.update(home, share, prop.va);
  }

  static ExecStatus post(Space& home, ViewArray<IntPair::IntPairView> a) {
    (void) new (home) Distinct(home, a);
    return ES_OK;
  }

  virtual ExecStatus propagate(Space& home, const ModEventDelta&) {

    for(int i=0; i<va.size(); i++) {
        if(va[i].assigned())
            for(int j=0; j<va.size(); j++) {
                if(j==i)
                    continue;
                else if(va[j].nq(home, va[i].val()) == IntPair::ME_INTPAIR_FAILED)
                    return ES_FAILED;
                }
    }
    return ES_FIX;
  }

  virtual size_t dispose(Space& home) {
    va.cancel(home, *this, IntPair::PC_INTPAIR_VAL);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  virtual Propagator* copy(Space& home, bool share) {
    return new (home) Distinct(home, share, *this);
  }

  virtual PropCost cost(const Space&, const ModEventDelta&) const {
    return PropCost::linear(PropCost::HI, va.size());
  }

  };

void distinct(Space& home, const IntPairVarArgs& a) {
    std::cout << "Init distinct prop" << std::endl;
  ViewArray<IntPair::IntPairView> va(home, a);
  if (Distinct::post(home, va) != ES_OK)
    home.fail();
}


#endif // DISTINCT_H
