#ifndef DFAAPPROX_H
#define DFAAPPROX_H

#include <vector>
#include "../approx/var.hh"
#include <gecode/int.hh>
#include <gecode/kernel.hh>
#include "dfainterface.h"

// using namespace MPG::IntPair;
// using namespace std;

int findPairX (std::vector<MPG::IntPair::PairApprox> v, int x) {
    for(int i=0; i<v.size(); i++)
        if(v[i].x==x)
            return i;
    return -1;
}

int findPairX (MPG::IntPair::IntPairApproxView v, int x) {
  for(int i=0; i<v.domsize(); i++) {
    if(v.getx(i)==x) return i;
  }
  return -1;
}

void mergePair(std::vector<MPG::IntPair::PairApprox> &v, const MPG::IntPair::PairApprox &p) {
  //  if(p.l>p.h)
  //    return;
    int i=findPairX(v, p.x);
    if(i==-1)
        v.push_back(p);
    else {
        if(v[i].l > p.l)
            v[i].l = p.l;
        if(v[i].h < p.h)
            v[i].h = p.h;
    }
}


class MyDFA : public Gecode::Propagator::Propagator {
protected:
    MPG::IntPair::IntPairApproxView P;
    MPG::IntPair::IntPairApproxView Q;
    Gecode::Int::IntView Z;
    MPG::IntPair::DFA_I *D;
    
public:
    MyDFA(Gecode::Space& home,
          MPG::IntPair::IntPairApproxView a,
          MPG::IntPair::IntPairApproxView b,
          Gecode::Int::IntView z,
          MPG::IntPair::DFA_I *d)
        : Gecode::Propagator::Propagator(home), P(a), Q(b), Z(z), D(d)
    {
        P.subscribe(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_DOM);
        Q.subscribe(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_DOM);
        Z.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
    }

    MyDFA(Gecode::Space& home, bool share, MyDFA& prop)
        : Gecode::Propagator::Propagator(home, share, prop), D(prop.D) {
        P.update(home, share, prop.P);
        Q.update(home, share, prop.Q);
        Z.update(home, share, prop.Z);
    }

    static Gecode::ExecStatus post(Gecode::Space& home,
                                   MPG::IntPair::IntPairApproxView a,
                                   MPG::IntPair::IntPairApproxView b,
                                   Gecode::Int::IntView z,
                                   MPG::IntPair::DFA_I *d) {
        (void) new (home) MyDFA(home, a, b, z, d);
        return Gecode::ES_OK;
    }

    virtual Gecode::ExecStatus propagate(Gecode::Space& home, const Gecode::ModEventDelta&) {
        //        std::cout << "Propagating DFA" << std::endl;
        Gecode::Int::ViewValues<Gecode::Int::IntView> iter(Z);
        std::vector<MPG::IntPair::PairApprox> newP;
        std::vector<MPG::IntPair::PairApprox> newQ;
        std::vector<int> newZ;
        while(iter()) {
            for(int i=0; i<Q.domsize(); i++) {
                MPG::IntPair::PairApprox q(Q.getx(i), Q.getl(i), Q.geth(i));
		int transCost=D->C(q.x, iter.val());
                MPG::IntPair::PairApprox p(D->S(q.x, iter.val()),
                                           q.l + transCost,
                                           q.h + transCost);
		int j=findPairX(P,p.x);
		MPG::IntPair::PairApprox q2(Q.getx(i),
					    P.getl(j) - transCost,
					    P.geth(j) - transCost);
		MPG::IntPair::PairApprox q3(q2.x,
					    q.l>q2.l ? q.l : q2.l,
					    q.h<q2.h ? q.h : q2.h);
		//		std::cout << "q   " << q <<  "    q2   " << q2 << "   q3  " << q3 << std::endl;
                if(p.x>0 && j!=-1 && q3.h >= q3.l) {
                    mergePair(newP, p);
		    mergePair(newQ, q2);
                    newZ.push_back(iter.val());
                }
            }
            if(iter.val() == Z.max()) // TODO Ugly hack
                break;
            ++iter;
        }

        if(newZ.size()==0 || newP.size()==0 || newQ.size()==0)
            return Gecode::ES_FAILED;

        Gecode::Int::ViewValues<Gecode::Int::IntView> iter2(Z);

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
            if(Z.nq(home, removeZ[i]) == Gecode::Int::ME_INT_FAILED)
                return Gecode::ES_FAILED;

        for(int i=0; i<newQ.size(); i++)
            if(Q.xeq(home, newQ[i]) == MPG::IntPair::ME_INTPAIRAPPROX_FAILED)
                return Gecode::ES_FAILED;

        std::vector<int> newQx;
        for(int i=0; i<newQ.size(); i++)
            newQx.push_back(newQ[i].x);

        if(Q.xeq(home, newQx) == MPG::IntPair::ME_INTPAIRAPPROX_FAILED)
            return Gecode::ES_FAILED;


        for(int i=0; i<newP.size(); i++)
            if(P.xeq(home, newP[i]) == MPG::IntPair::ME_INTPAIRAPPROX_FAILED)
                return Gecode::ES_FAILED;


        std::vector<int> newPx;
        for(int i=0; i<newP.size(); i++)
            newPx.push_back(newP[i].x);

        if(P.xeq(home, newPx) == MPG::IntPair::ME_INTPAIRAPPROX_FAILED)
            return Gecode::ES_FAILED;

        return Gecode::ES_NOFIX;
    }

    virtual size_t dispose(Gecode::Space& home) {
        P.cancel(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_DOM);
        Q.cancel(home, *this, MPG::IntPair::PC_INTPAIRAPPROX_DOM);
        Z.cancel(home, *this, Gecode::Int::PC_INT_DOM);
        (void) Gecode::Propagator::Propagator::dispose(home);
        return sizeof(*this);
    }

    virtual Propagator* copy(Gecode::Space& home, bool share) {
        return new (home) MyDFA(home, share, *this);
    }

    virtual Gecode::PropCost cost(const Gecode::Space&, const Gecode::ModEventDelta&) const {
        // Probably way wrong, but not very important. TODO
        return Gecode::PropCost::linear(Gecode::PropCost::HI, P.size());
    }

};

//
void mydfa(Gecode::Space& home,
           MPG::IntPairApproxVar P,
           MPG::IntPairApproxVar Q,
           Gecode::IntVar Z,
           MPG::IntPair::DFA_I *D) {
    //    std::cout << "Init DFA prop" << std::endl;
    MPG::IntPair::IntPairApproxView vp(P);
    MPG::IntPair::IntPairApproxView vq(Q);
    Gecode::Int::IntView vz(Z);
    if (MyDFA::post(home, vp, vq, vz, D) != Gecode::ES_OK)
        home.fail();
}


#endif // DFA_H
