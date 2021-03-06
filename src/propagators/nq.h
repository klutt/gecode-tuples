#ifndef NEQ_H
#define NEQ_H
class Neq : public Propagator {
protected:
  MPG::IntPair::IntPairView p1;
  MPG::IntPair::IntPairView p2;
public:
  Neq(Space& home, MPG::IntPair::IntPairView pv1, MPG::IntPair::IntPairView pv2)
    : Propagator(home), p1(pv1), p2(pv2)
  {
    p1.subscribe(home, *this, MPG::IntPair::PC_INTPAIR_VAL);
    p2.subscribe(home, *this, MPG::IntPair::PC_INTPAIR_VAL);
  }

  Neq(Space& home, bool share, Neq& prop)
    : Propagator(home, share, prop) {
    p1.update(home, share, prop.p1);
    p2.update(home, share, prop.p2);
  }

  static ExecStatus post(Space& home, MPG::IntPair::IntPairView p1, MPG::IntPair::IntPairView p2) {
    (void) new (home) Neq(home, p1, p2);
    return ES_OK;
  }

  virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
//      std::cout << "Propagating Neq " << std::endl;
    if (p1.nq(home, p2.val()) == MPG::IntPair::ME_INTPAIR_FAILED)
      return ES_FAILED;
    if (p2.nq(home, p1.val()) == MPG::IntPair::ME_INTPAIR_FAILED)
      return ES_FAILED;
    return ES_NOFIX;
  }

  virtual size_t dispose(Space& home) {
    p1.cancel(home, *this, MPG::IntPair::PC_INTPAIR_VAL);
    p2.cancel(home, *this, MPG::IntPair::PC_INTPAIR_VAL);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  virtual Propagator* copy(Space& home, bool share) {
    return new (home) Neq(home, share, *this);
  }

  virtual PropCost cost(const Space&, const ModEventDelta&) const {
    return PropCost::linear(PropCost::HI, p1.size());
  }

  };

void neq(Space& home, MPG::IntPairVar p, MPG::IntPairVar q) {
  //  std::cout << "Init Neq prop" << std::endl;
  MPG::IntPair::IntPairView pv(p);
  MPG::IntPair::IntPairView qv(q);
  if (Neq::post(home, pv, qv) != ES_OK)
    home.fail();
}

#endif // NEQ_H
