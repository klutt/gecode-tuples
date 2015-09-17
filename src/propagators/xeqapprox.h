#ifndef XEQAPPROX_H
#define XEQAPPROX_H


class Xeq : public Gecode::Propagator::Propagator {
protected:
  MPG::IntPair::IntPairApproxView p;
  MPG::IntPair::PairApprox q;
public:
  Xeq(Gecode::Space& home, MPG::IntPair::IntPairApproxView pv1, MPG::IntPair::PairApprox pv2)
    : Propagator(home), p(pv1), q(pv2)
  {
    p.subscribe(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_DOM);
  }

  Xeq(Gecode::Space& home, bool share, Xeq& prop)
    : Propagator(home, share, prop) {
    p.update(home, share, prop.p);
  }

  static Gecode::ExecStatus post(Gecode::Space& home, MPG::IntPair::IntPairApproxView p, MPG::IntPair::PairApprox q) {
    (void) new (home) Xeq(home, p, q);
    return Gecode::ES_OK;
  }

  virtual Gecode::ExecStatus propagate(Gecode::Space& home, const Gecode::ModEventDelta&) {
 //     std::cout << "Propagating Xeq " << std::endl;
    if (p.xeq(home, q) == MPG::IntPair::ME_INTPAIRAPPROX_FAILED)
      return Gecode::ES_FAILED;
    return Gecode::ES_NOFIX;
  }

  virtual size_t dispose(Gecode::Space& home) {
//      std::cout << "Xeq dispose" << std::endl;
    p.cancel(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_DOM);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  virtual Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Xeq(home, share, *this);
  }

  virtual Gecode::PropCost cost(const Gecode::Space&, const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::linear(Gecode::PropCost::HI, p.size());
  }

  };

void xeq(Gecode::Space& home, MPG::IntPairApproxVar p, MPG::IntPair::PairApprox q) {
//    std::cout << "Init Xeq prop" << std::endl;
  MPG::IntPair::IntPairApproxView pv(p);
  if (Xeq::post(home, pv, q) != Gecode::ES_OK)
    home.fail();
}


#endif // XEQ_H
