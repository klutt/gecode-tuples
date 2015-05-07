#ifndef DFAINT_H
#define DFAINT_H


#include <vector>

// This is the IntVar version of dfa propagator. It is used for performance comparision.


// Pre: S is current state, Z is next token
// Post: Returns next state. 0 is garbage state.
typedef int (*StateFunctionI)(int S, int Z);

// Pre: S is current state, Z is next token
// Post: returns the cost for next transition

typedef int (*CostFunctionI)(int S, int Z);



class MyDFAint : public Propagator {
protected:
    Int::IntView Px;
    Int::IntView Qx;
    Int::IntView Py;
    Int::IntView Qy;
    Int::IntView Z;
    StateFunctionI S;
    CostFunctionI C;
public:
    MyDFAint(Space& home, Int::IntView px, Int::IntView py, Int::IntView qx, Int::IntView qy, Int::IntView z, StateFunctionI s, CostFunctionI c)
        : Propagator(home), Px(px), Py(py), Qx(qx), Qy(qy), Z(z), S(s), C(c)
    {
        Px.subscribe(home, *this, Int::PC_INT_DOM);
        Qx.subscribe(home, *this, Int::PC_INT_DOM);
        Py.subscribe(home, *this, Int::PC_INT_DOM);
        Qy.subscribe(home, *this, Int::PC_INT_DOM);
        Z.subscribe(home, *this, Int::PC_INT_DOM);
    }

    MyDFAint(Space& home, bool share, MyDFAint& prop)
        : Propagator(home, share, prop), S(prop.S), C(prop.C) {
        Px.update(home, share, prop.Px);
        Qx.update(home, share, prop.Qx);
        Py.update(home, share, prop.Py);
        Qy.update(home, share, prop.Qy);
        Z.update(home, share, prop.Z);
    }

    static ExecStatus post(Space& home, Int::IntView px, Int::IntView py, Int::IntView qx, Int::IntView qy, Int::IntView z, StateFunctionI s, CostFunctionI c) {
        (void) new (home) MyDFAint(home, px, py, qx, qy, z, s, c);
        return ES_OK;
    }

    virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
        // std::vector<Pair> keep;


    }

    virtual size_t dispose(Space& home) {
        Px.cancel(home, *this, Int::PC_INT_DOM);
        Qx.cancel(home, *this, Int::PC_INT_DOM);
        Py.cancel(home, *this, Int::PC_INT_DOM);
        Qy.cancel(home, *this, Int::PC_INT_DOM);
        Z.cancel(home, *this, Int::PC_INT_DOM);
        (void) Propagator::dispose(home);
        return sizeof(*this);
    }

    virtual Propagator* copy(Space& home, bool share) {
        return new (home) MyDFAint(home, share, *this);
    }

    virtual PropCost cost(const Space&, const ModEventDelta&) const {
        // Probably way wrong, but not very important. TODO
        return PropCost::linear(PropCost::HI, Px.size());
    }

};

//
void myintdfa(Space& home, IntVar Px, IntVar Py, IntVar Qx, IntVar Qy, IntVar Z, StateFunctionI S, CostFunctionI C) {
    std::cout << "Init DFAint prop" << std::endl;
    Int::IntView vpx(Px);
    Int::IntView vqx(Qx);
    Int::IntView vpy(Py);
    Int::IntView vqy(Qy);
    Int::IntView vz(Z);
    if (MyDFAint::post(home, vpx, vpy, vqx, vqy, vz, S, C) != ES_OK)
        home.fail();
}



#endif // DFAINT_H
