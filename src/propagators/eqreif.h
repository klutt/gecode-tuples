#ifndef EQREIF_H
#define EQREIF_H

class EqReif : public Propagator {
protected:
    IntPair::IntPairView p1;
    IntPair::IntPairView p2;
    Int::BoolView b;
public:
    EqReif(Space& home, IntPair::IntPairView pv1, IntPair::IntPairView pv2, Int::BoolView bv)
        : Propagator(home), p1(pv1), p2(pv2), b(bv)
    {
        p1.subscribe(home, *this, IntPair::PC_INTPAIR_DOM);
        p2.subscribe(home, *this, IntPair::PC_INTPAIR_DOM);
        b.subscribe(home, *this, Int::PC_BOOL_VAL);
    }

    EqReif(Space& home, bool share, EqReif& prop)
        : Propagator(home, share, prop) {
        p1.update(home, share, prop.p1);
        p2.update(home, share, prop.p2);
        b.update(home, share, prop.b);
    }

    static ExecStatus post(Space& home, IntPair::IntPairView p1, IntPair::IntPairView p2, Int::BoolView b) {
        (void) new (home) EqReif(home, p1, p2, b);
        return ES_OK;
    }

    virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
        std::cout << "Propagating EqReif " << std::endl;
        if(p1.assigned() && p2.assigned())
            if(p1.val().x==p2.val().x && p1.val().y==p2.val().y)
                if(b.one(home) == Int::ME_INT_FAILED)
                    return ES_FAILED;
                else
                    return ES_NOFIX;
            else
                if(b.zero(home) == Int::ME_INT_FAILED)
                    return ES_FAILED;
                else
                    return ES_NOFIX;
        else if (b.assigned())
            if(b.one()) {
                if (p1.eq(home, p2) == IntPair::ME_INTPAIR_FAILED)
                    return ES_FAILED;
                if (p2.eq(home, p1) == IntPair::ME_INTPAIR_FAILED)
                    return ES_FAILED;
            }
            else if(b.zero()) {
                if (p2.assigned())
                    if (p1.nq(home, p2.val()) == IntPair::ME_INTPAIR_FAILED)
                        return ES_FAILED;
                if (p1.assigned())
                    if (p2.nq(home, p1.val()) == IntPair::ME_INTPAIR_FAILED)
                        return ES_FAILED;
            }
        return ES_NOFIX;
    }

    virtual size_t dispose(Space& home) {
        std::cout << "Eq dispose" << std::endl;
        p1.cancel(home, *this, IntPair::PC_INTPAIR_DOM);
        p2.cancel(home, *this, IntPair::PC_INTPAIR_DOM);
        (void) Propagator::dispose(home);
        return sizeof(*this);
    }

    virtual Propagator* copy(Space& home, bool share) {
        return new (home) EqReif(home, share, *this);
    }

    virtual PropCost cost(const Space&, const ModEventDelta&) const {
        return PropCost::linear(PropCost::HI, p1.size());
    }

};

void eq(Space& home, IntPairVar p, IntPairVar q, BoolVar b) {
    //  std::cout << "Init EqReif prop" << std::endl;
    IntPair::IntPairView pv(p);
    IntPair::IntPairView qv(q);
    Int::BoolView bv(b);
    if (EqReif::post(home, pv, qv, bv) != ES_OK)
        home.fail();
}


#endif // EQREIF_H
