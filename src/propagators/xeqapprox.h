#ifndef XEQ_H
#define XEQ_H

class Xeq : public Propagator {
protected:
  IntPair::IntPairApproxView p;
  IntPair::PairApprox q;
public:
  Xeq(Space& home, IntPair::IntPairApproxView pv1, IntPair::PairApprox pv2)
    : Propagator(home), p(pv1), q(pv2)
  {
    p.subscribe(home, *this, IntPair::PC_INTPAIRAPPROX_DOM);
  }

  Xeq(Space& home, bool share, Xeq& prop)
    : Propagator(home, share, prop) {
    p.update(home, share, prop.p);
  }

  static ExecStatus post(Space& home, IntPair::IntPairApproxView p, IntPair::PairApprox q) {
    (void) new (home) Xeq(home, p, q);
    return ES_OK;
  }

  virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
 //     std::cout << "Propagating Xeq " << std::endl;
    if (p.xeq(home, q) == IntPair::ME_INTPAIRAPPROX_FAILED)
      return ES_FAILED;
    return ES_NOFIX;
  }

  virtual size_t dispose(Space& home) {
//      std::cout << "Xeq dispose" << std::endl;
    p.cancel(home, *this, IntPair::PC_INTPAIRAPPROX_DOM);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  virtual Propagator* copy(Space& home, bool share) {
    return new (home) Xeq(home, share, *this);
  }

  virtual PropCost cost(const Space&, const ModEventDelta&) const {
    return PropCost::linear(PropCost::HI, p.size());
  }

  };

void xeq(Space& home, IntPairApproxVar p, MPG::IntPair::PairApprox q) {
//    std::cout << "Init Xeq prop" << std::endl;
  IntPair::IntPairApproxView pv(p);
  if (Xeq::post(home, pv, q) != ES_OK)
    home.fail();
}


#endif // XEQ_H
