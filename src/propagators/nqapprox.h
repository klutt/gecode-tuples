#ifndef NEQAPPROX_H
#define NEQAPPROX_H

#include "../approx/var.hh"
#include "../approx/view.hh"

class Neq : public Gecode::Propagator::Propagator {
protected:
  MPG::IntPair::IntPairApproxView p1;
  //  MPG::IntPair::IntPairApproxView p2;
  MPG::IntPair::Pair p2;
public:
 Neq(Gecode::Space& home, MPG::IntPair::IntPairApproxView pv1, MPG::IntPair::Pair pv2)
   : Propagator(home), p1(pv1), p2(pv2)
  {
    std::cout << "Neq constructor 1" << std::endl;
    p1.subscribe(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_VAL);
    //    p2.subscribe(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_VAL);
  }

  Neq(Gecode::Space& home, bool share, Neq& prop)
    : Propagator(home, share, prop) {
    std::cout << "Neq constructor 2" << std::endl;
    p1.update(home, share, prop.p1);
    //    p2.update(home, share, prop.p2);
  }

  
  static Gecode::ExecStatus post(Gecode::Space& home, MPG::IntPair::IntPairApproxView p1, MPG::IntPair::Pair p2) {
    (void) new (home) Neq(home, p1, p2);
    return Gecode::ES_OK;
  }

  virtual Gecode::ExecStatus propagate(Gecode::Space& home, const Gecode::ModEventDelta&) {
    std::cout << "Propagating Neq " << std::endl;
    if (p1.nq(home, p2) == MPG::IntPair::ME_INTPAIRAPPROX_FAILED)
      return Gecode::ES_FAILED;
    return Gecode::ES_NOFIX;
  }
  
  
  virtual size_t dispose(Gecode::Space& home) {
    p1.cancel(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_VAL);
    //    p2.cancel(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_VAL);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  virtual Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Neq(home, share, *this);
  }

  virtual Gecode::PropCost cost(const Gecode::Space&, const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::linear(Gecode::PropCost::HI, p1.size());
  }
  
  };

void neq(Gecode::Space& home, MPG::IntPairApproxVar p, MPG::IntPair::Pair q) {
  std::cout << "Init Neq prop" << std::endl;
  MPG::IntPair::IntPairApproxView pv(p);
  //  MPG::IntPair::IntPairApproxView qv(q);
  if (Neq::post(home, pv, q) != Gecode::ES_OK)
    home.fail();
}

#endif // NEQ_H
