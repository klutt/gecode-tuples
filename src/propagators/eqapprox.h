#ifndef EQAPPROX_H
#define EQAPPROX_H

#include "../approx/var.hh"
#include "../approx/view.hh"

class Eq : public Gecode::Propagator::Propagator {
protected:
  MPG::IntPair::IntPairApproxView p1;
  MPG::IntPair::IntPairApproxView p2;
public:
  Eq(Gecode::Space& home, MPG::IntPair::IntPairApproxView pv1, MPG::IntPair::IntPairApproxView pv2)
    : Propagator(home), p1(pv1), p2(pv2)
  {
    p1.subscribe(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_DOM);
    p2.subscribe(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_DOM);
  }

  Eq(Gecode::Space& home, bool share, Eq& prop)
    : Propagator(home, share, prop) {
    p1.update(home, share, prop.p1);
    p2.update(home, share, prop.p2);
  }

  static Gecode::ExecStatus post(Gecode::Space& home, MPG::IntPair::IntPairApproxView p1, MPG::IntPair::IntPairApproxView p2) {
    (void) new (home) Eq(home, p1, p2);
    return Gecode::ES_OK;
  }

  virtual Gecode::ExecStatus propagate(Gecode::Space& home, const Gecode::ModEventDelta&) {
      std::cout << "Propagating Eq " << std::endl;
    if (p1.eq(home, p2) == MPG::IntPair::ME_INTPAIRAPPROX_FAILED)
      return Gecode::ES_FAILED;
    if (p2.eq(home, p1) == MPG::IntPair::ME_INTPAIRAPPROX_FAILED)
      return Gecode::ES_FAILED;
    return Gecode::ES_NOFIX;
  }

  virtual size_t dispose(Gecode::Space& home) {
//      std::cout << "Eq dispose" << std::endl;
    p1.cancel(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_DOM);
    p2.cancel(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_DOM);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  virtual Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Eq(home, share, *this);
  }

  virtual Gecode::PropCost cost(const Gecode::Space&, const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::linear(Gecode::PropCost::HI, p1.size());
  }

  };

void eq(Gecode::Space& home, MPG::IntPairApproxVar p, MPG::IntPairApproxVar q) {
//    std::cout << "Init Eq prop" << std::endl;
  MPG::IntPair::IntPairApproxView pv(p);
  MPG::IntPair::IntPairApproxView qv(q);
  if (Eq::post(home, pv, qv) != Gecode::ES_OK)
    home.fail();
}


#endif // EQ_H
