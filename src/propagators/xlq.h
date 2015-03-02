#ifndef XLQ_H
#define XLQ_H
// p.x <= x

class Xlq : public Propagator {
protected:
  Int::IntView x;
  IntPair::IntPairView p;
public:
  Xlq(Space& home, IntPair::IntPairView pv, Int::IntView xv)
    : Propagator(home), p(pv), x(xv)
  {
    p.subscribe(home, *this, IntPair::PC_INTPAIR_DOM);
    x.subscribe(home, *this, Int::PC_INT_DOM);
  }

  Xlq(Space& home, bool share, Xlq& prop)
    : Propagator(home, share, prop) {
    x.update(home, share, prop.x);
    p.update(home, share, prop.p);
  }

  static ExecStatus post(Space& home, IntPair::IntPairView p, Int::IntView x) {
    (void) new (home) Xlq(home, p, x);
    return ES_OK;
  }

  virtual ExecStatus propagate(Space& home, const ModEventDelta&) {

    if (x.gr(home, p.xmin()) == Int::ME_INT_FAILED)
      return ES_FAILED;
    if (p.xlq(home, x.max()) == IntPair::ME_INTPAIR_FAILED)
      return ES_FAILED;
    return ES_NOFIX;
  }

  virtual size_t dispose(Space& home) {
    p.cancel(home, *this, IntPair::PC_INTPAIR_DOM);
    x.cancel(home, *this, Int::PC_INT_DOM);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  virtual Propagator* copy(Space& home, bool share) {
    return new (home) Xlq(home, share, *this);
  }

  virtual PropCost cost(const Space&, const ModEventDelta&) const {
    return PropCost::linear(PropCost::HI, p.size());
  }

};

void xlq(Space& home, IntPairVar p, IntVar x) {
  Int::IntView xv(x);
  IntPair::IntPairView pv(p);
  if (Xlq::post(home, pv, xv) != ES_OK)
    home.fail();
}

#endif // XLQ_H
