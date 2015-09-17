#ifndef DFAAPPROX_H
#define DFAAPPROX_H

#include <vector>

#include "dfainterface.h"

using namespace MPG::IntPair;
using namespace std;

int findPairX (std::vector<PairApprox> v, int x) {
  for(int i=0; i<v.size(); i++)
    if(v[i].x==x)
      return i;
  return -1;
}

void mergePair(std::vector<PairApprox> &v, PairApprox p) {
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
  

class MyDFA : public Propagator {
protected:
    IntPair::IntPairApproxView P;
    IntPair::IntPairApproxView Q;
    Int::IntView Z;
    DFA_I *D;
    
public:
    MyDFA(Space& home, IntPair::IntPairApproxView a, IntPair::IntPairApproxView b, Int::IntView z, DFA_I *d)
      : Propagator(home), P(a), Q(b), Z(z), D(d)
    {
        P.subscribe(home, *this, IntPair::PC_INTPAIRAPPROX_DOM);
        Q.subscribe(home, *this, IntPair::PC_INTPAIRAPPROX_DOM);
        Z.subscribe(home, *this, Int::PC_INT_DOM);
    }

    MyDFA(Space& home, bool share, MyDFA& prop)
        : Propagator(home, share, prop), D(prop.D) {
        P.update(home, share, prop.P);
        Q.update(home, share, prop.Q);
        Z.update(home, share, prop.Z);
    }

    static ExecStatus post(Space& home, IntPair::IntPairApproxView a, IntPair::IntPairApproxView b, Int::IntView z, DFA_I *d) {
        (void) new (home) MyDFA(home, a, b, z, d);
        return ES_OK;
    }

    virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
        std::cout << "Propagating DFA" << std::endl;
        Gecode::Int::ViewValues<Int::IntView> iter(Z);
	std::vector<MPG::IntPair::PairApprox> newP;
	std::vector<MPG::IntPair::PairApprox> newQ;
	std::vector<int> newZ;
        while(iter()) {
            for(int i=0; i<Q.domsize(); i++) {
	      MPG::IntPair::PairApprox q(Q.getx(i), Q.getl(i), Q.geth(i));
	      MPG::IntPair::PairApprox p(D->S(q.x, iter.val()),
					 q.l + D->C(q.x, iter.val()),
					 q.h + D->C(q.x, iter.val()));
	      std::cout << "z: " << iter.val() << "  q: " << q << "  p: " << p << std::endl;
	      //		std::cout << "Z: " << Z << "  Q: " << Q << "  P: " << P << std::endl;
	      cout <<  "  p: " << p << endl;
	      if(p.x>0) {
		mergePair(newP, p);
		mergePair(newQ, q);
		newZ.push_back(iter.val());
	      }
            }
	    //            std::cout << "newz: " << newZ.size() << "  newp: " << newP.size() << "  newq: " << newQ.size() << std::endl;
            if(iter.val() == Z.max()) // TODO Ugly hack
                break;
            ++iter;
        }

	cout << "newP: ";
	for(int i=0; i<newP.size(); i++)
	  cout << newP[i] << " ";
	cout << endl;
	
        if(newZ.size()==0 || newP.size()==0 || newQ.size()==0) 
            return ES_FAILED;
        

	//        std::cout << "Removing values" << std::endl;

        Gecode::Int::ViewValues<Int::IntView> iter2(Z);

        iter2.init(Z);
        while(iter2()) {
//            std::cout << "bajs" << std::endl;
            int z = iter2.val();
//            std::cout << "bajs2" << std::endl;
            if(std::find(newZ.begin(), newZ.end(), z) == newZ.end())
                if(Z.nq(home, z) == Int::ME_INT_FAILED)
                    return ES_FAILED;
 //           std::cout << "bajs3" << std::endl;
    //        std::cout << "size " << Z.size() << std::endl;

            if(z == Z.max() || Z.assigned()) // TODO Ugly hack
                break;
            ++iter2;
    //        std::cout << "bajs4" << std::endl;
        }

	
	//        std::cout << "Z done ";


	for(int i=0; i<newQ.size(); i++)
	      if(Q.xeq(home, newQ[i]) == ME_INTPAIRAPPROX_FAILED)
		return ES_FAILED;

	for(int i=0; i<newP.size(); i++)
	      if(P.xeq(home, newP[i]) == ME_INTPAIRAPPROX_FAILED)
		return ES_FAILED;

	/*
        for(int i=0; i<Q.size(); i++) {
            IntPair::Pair q = Q.getElement(i);
            if(std::find(newQ.begin(), newQ.end(), q) == newQ.end())
                if(Q.nq(home, q) == IntPair::ME_INTPAIRAPPROX_FAILED)
                    return ES_FAILED;
        }
	//        std::cout << "Q done ";

        for(int i=0; i<P.size(); i++) {
            IntPair::Pair p = P.getElement(i);
            if(std::find(newP.begin(), newP.end(), p) == newP.end())
                if(P.nq(home, p) == IntPair::ME_INTPAIRAPPROX_FAILED)
                    return ES_FAILED;
        }
	//        std::cout << "P done " << std::endl;

	//        std::cout << "Finish DFA" << std::endl; */
        return ES_NOFIX;
    }

    virtual size_t dispose(Space& home) {
        P.cancel(home, *this, IntPair::PC_INTPAIRAPPROX_DOM);
        Q.cancel(home, *this, IntPair::PC_INTPAIRAPPROX_DOM);
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
void mydfa(Space& home, IntPairApproxVar P, IntPairApproxVar Q, IntVar Z, DFA_I *D) {
  //    std::cout << "Init DFA prop" << std::endl;
    IntPair::IntPairApproxView vp(P);
    IntPair::IntPairApproxView vq(Q);
    Int::IntView vz(Z);
    if (MyDFA::post(home, vp, vq, vz, D) != ES_OK)
        home.fail();
}


#endif // DFA_H
