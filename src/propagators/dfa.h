#ifndef DFA_H
#define DFA_H

#include <vector>

#include "dfainterface.h"

using namespace MPG::IntPair;

class MyDFA : public Propagator {
protected:
    MPG::IntPair::IntPairView P;
    MPG::IntPair::IntPairView Q;
    Int::IntView Z;
    //    StateFunction S;
    //    CostFunction C;
    DFA_I *D;
    
public:
    MyDFA(Space& home, MPG::IntPair::IntPairView a, MPG::IntPair::IntPairView b, Int::IntView z, DFA_I *d)
        : Propagator(home), P(a), Q(b), Z(z), D(d)
    {
        P.subscribe(home, *this, MPG::IntPair::PC_INTPAIR_DOM);
        Q.subscribe(home, *this, MPG::IntPair::PC_INTPAIR_DOM);
        Z.subscribe(home, *this, Int::PC_INT_DOM);
    }

    MyDFA(Space& home, bool share, MyDFA& prop)
        : Propagator(home, share, prop), D(prop.D) {
        P.update(home, share, prop.P);
        Q.update(home, share, prop.Q);
        Z.update(home, share, prop.Z);
    }

    static ExecStatus post(Space& home, MPG::IntPair::IntPairView a, MPG::IntPair::IntPairView b, Int::IntView z, DFA_I *d) {
        (void) new (home) MyDFA(home, a, b, z, d);
        return ES_OK;
    }

    virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
        // std::vector<Pair> keep;
        //        std::cout << "Propagating DFA" << std::endl;
        Gecode::Int::ViewValues<Int::IntView> iter(Z);
        std::vector<MPG::IntPair::Pair> newP;
        std::vector<MPG::IntPair::Pair> newQ;
        std::vector<int> newZ;
        while(iter()) {
            for(int i=0; i<Q.size(); i++) {
                MPG::IntPair::Pair q = Q.getElement(i);
                MPG::IntPair::Pair p = MPG::IntPair::Pair(D->S(q.x, iter.val()), q.y + D->C(q.x, iter.val()));
                //  std::cout << "z: " << iter.val() << "  q.x: " << q.x << "  p.x: " << p.x << std::endl;
                if(p.x==0) {
                    //                    std::cout << "garbage state" << std::endl;
                    continue;
                }
                if(P.contains(p)) {
                    newP.push_back(p);
                    newQ.push_back(q);
                    newZ.push_back(iter.val());
                }
            }
            //            std::cout << "newz: " << newZ.size() << "  newp: " << newP.size() << "  newq: " << newQ.size() << std::endl;
            if(iter.val() == Z.max()) // TODO Ugly hack
                break;
            ++iter;
        }

        if(newZ.size()==0 || newP.size()==0 || newQ.size()==0) {
            //  std::cout << "newz: " << newZ.size() << "  newp: " << newP.size() << "  newq: " << newQ.size() << std::endl;
            return ES_FAILED;
        }

        //        std::cout << "Removing values" << std::endl;

        Gecode::Int::ViewValues<Int::IntView> iter2(Z);

        std::vector<int> removeZ;
        iter2.init(Z);
        while(iter2()) {
            int z = iter2.val();
            if(std::find(newZ.begin(), newZ.end(), z) == newZ.end())
                removeZ.push_back(z);
            if(z == Z.max() || Z.assigned()) // TODO Ugly hack
                break;
            ++iter2;
        }
        for(int i=0; i<removeZ.size(); i++)
            if(Z.nq(home, removeZ[i]) == Int::ME_INT_FAILED)
                return ES_FAILED;
        //        std::cout << "Z done ";

        std::vector<Pair> removeQ;
        for(int i=0; i<Q.size(); i++) {
            MPG::IntPair::Pair q = Q.getElement(i);
            if(std::find(newQ.begin(), newQ.end(), q) == newQ.end())
                removeQ.push_back(q);
        }
        for(int i=0; i<removeQ.size(); i++)
            if(Q.nq(home, removeQ[i]) == MPG::IntPair::ME_INTPAIR_FAILED)
                return ES_FAILED;
        //        st   d::cout << "Q done ";

        std::vector<Pair> removeP;
        for(int i=0; i<P.size(); i++) {
            MPG::IntPair::Pair p = P.getElement(i);
            if(std::find(newP.begin(), newP.end(), p) == newP.end())
                removeP.push_back(p);
        }
        for(int i=0; i<removeP.size(); i++)
            if(P.nq(home, removeP[i]) == MPG::IntPair::ME_INTPAIR_FAILED)
                return ES_FAILED;

        //        std::cout << "P done " << std::endl;

        //        std::cout << "Finish DFA" << std::endl;
        return ES_NOFIX;

    }

    virtual size_t dispose(Space& home) {
        P.cancel(home, *this, MPG::IntPair::PC_INTPAIR_DOM);
        Q.cancel(home, *this, MPG::IntPair::PC_INTPAIR_DOM);
        Z.cancel(home, *this, Int::PC_INT_DOM);
        (void) Propagator::dispose(home);
        return sizeof(*this);
    }

    virtual Propagator* copy(Space& home, bool share) {
        return new (home) MyDFA(home, share, *this);
    }

    virtual PropCost cost(const Space&, const ModEventDelta&) const {
        // Probably way wrong, but not very important. TODO
        return PropCost::linear(PropCost::HI, P.size());
    }

};

//
void mydfa(Space& home, MPG::IntPairVar P, MPG::IntPairVar Q, IntVar Z, DFA_I *D) {
    //    std::cout << "Init DFA prop" << std::endl;
    MPG::IntPair::IntPairView vp(P);
    MPG::IntPair::IntPairView vq(Q);
    Int::IntView vz(Z);
    if (MyDFA::post(home, vp, vq, vz, D) != ES_OK)
        home.fail();
}


#endif // DFA_H
