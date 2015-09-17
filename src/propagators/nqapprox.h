#ifndef NEQAPPROX_H
#define NEQAPPROX_H

class Neq : public Gecode::Propagator::Propagator {
protected:
  IntPair::IntPairApproxView p1;
  IntPair::IntPairApproxView p2;
public:
  Neq(Space& home, IntPair::IntPairApproxView pv1, IntPair::IntPairApproxView pv2)
    : Propagator(home), p1(pv1), p2(pv2)
  {
    p1.subscribe(home, *this, IntPair::PC_INTPAIRAPPROX_VAL);
    p2.subscribe(home, *this, IntPair::PC_INTPAIRAPPROX_VAL);
  }

  Neq(Space& home, bool share, Neq& prop) {
    : Propagator(home, share, prop) {
      p1.update(home, share, prop.p1);
      p2.update(home, share, prop.p2);
    }
  }
  static ExecStatus post(Space& home, IntPair::IntPairApproxView p1, IntPair::IntPairApproxView p2) {
    (void) new (home) Neq(home, p1, p2);
    return ES_OK;
  }

  virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
//      std::cout << "Propagating Neq " << std::endl;
    if (p1.nq(home, p2.val()) == IntPair::ME_INTPAIRAPPROX_FAILED)
      return ES_FAILED;
    if (p2.nq(home, p1.val()) == IntPair::ME_INTPAIRAPPROX_FAILED)
      return ES_FAILED;
    return ES_NOFIX;
  }
  
  virtual size_t dispose(Space& home) {
    p1.cancel(home, *this, IntPair::PC_INTPAIRAPPROX_VAL);
    p2.cancel(home, *this, IntPair::PC_INTPAIRAPPROX_VAL);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  virtual Propagator* copy(Space& home, bool share) {
    return new (home) NeqApprox(home, share, *this);
  }

  virtual PropCost cost(const Space&, const ModEventDelta&) const {
    return PropCost::linear(PropCost::HI, p1.size());
  }
  
  };

void neq(Space& home, IntPairApproxVar p, IntPairApproxVar q) {
  //  std::cout << "Init Neq prop" << std::endl;
  IntPair::IntPairApproxView pv(p);
  IntPair::IntPairApproxView qv(q);
  if (Neq::post(home, pv, qv) != ES_OK)
    home.fail();
}

#endif // NEQ_H
