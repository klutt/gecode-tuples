#ifndef EQ_H
#define EQ_H

class Eq : public Propagator {
protected:
  IntPair::IntPairView p1;
  IntPair::IntPairView p2;
public:
  Eq(Space& home, IntPair::IntPairView pv1, IntPair::IntPairView pv2)
    : Propagator(home), p1(pv1), p2(pv2)
  {
    p1.subscribe(home, *this, IntPair::PC_INTPAIR_DOM);
    p2.subscribe(home, *this, IntPair::PC_INTPAIR_DOM);
  }

  Eq(Space& home, bool share, Eq& prop)
    : Propagator(home, share, prop) {
    p1.update(home, share, prop.p1);
    p2.update(home, share, prop.p2);
  }

  static ExecStatus post(Space& home, IntPair::IntPairView p1, IntPair::IntPairView p2) {
    (void) new (home) Eq(home, p1, p2);
    return ES_OK;
  }

  virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
 //     std::cout << "Propagating Eq " << std::endl;
    if (p1.eq(home, p2) == IntPair::ME_INTPAIR_FAILED)
      return ES_FAILED;
    if (p2.eq(home, p1) == IntPair::ME_INTPAIR_FAILED)
      return ES_FAILED;
    return ES_NOFIX;
  }

  virtual size_t dispose(Space& home) {
//      std::cout << "Eq dispose" << std::endl;
    p1.cancel(home, *this, IntPair::PC_INTPAIR_DOM);
    p2.cancel(home, *this, IntPair::PC_INTPAIR_DOM);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  virtual Propagator* copy(Space& home, bool share) {
    return new (home) Eq(home, share, *this);
  }

  virtual PropCost cost(const Space&, const ModEventDelta&) const {
    return PropCost::linear(PropCost::HI, p1.size());
  }

  };

void eq(Space& home, IntPairVar p, IntPairVar q) {
//    std::cout << "Init Eq prop" << std::endl;
  IntPair::IntPairView pv(p);
  IntPair::IntPairView qv(q);
  if (Eq::post(home, pv, qv) != ES_OK)
    home.fail();
}


#endif // EQ_H
