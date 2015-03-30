#ifndef DFA_H
#define DFA_H

#include <vector>

// Pre: S is current state, Z is next token
// Post: Returns next state. 0 is garbage state.
typedef int (*StateFunction)(int S, int Z);

// Pre: S is current state, Z is next token
// Post: returns the cost for next transition

typedef int (*CostFunction)(int S, int Z);

// P.x = S(Q.x, Z)
// P.y = Q.y + S(Q.y, Z)

class MyDFA : public Propagator {
protected:
    IntPair::IntPairView P;
    IntPair::IntPairView Q;
    Int::IntView Z;
    StateFunction S;
    CostFunction C;
public:
  MyDFA(Space& home, IntPair::IntPairView a, IntPair::IntPairView b, Int::IntView z, StateFunction s, CostFunction c)
    : Propagator(home), P(a), Q(b), Z(z), S(s), C(c)
  {
      P.subscribe(home, *this, IntPair::PC_INTPAIR_DOM);
      Q.subscribe(home, *this, IntPair::PC_INTPAIR_DOM);
      Z.subscribe(home, *this, Int::PC_INT_DOM);
  }

  MyDFA(Space& home, bool share, MyDFA& prop)
    : Propagator(home, share, prop), S(prop.S), C(prop.C) {
    P.update(home, share, prop.P);
    Q.update(home, share, prop.Q);
    Z.update(home, share, prop.Z);
  }

  static ExecStatus post(Space& home, IntPair::IntPairView a, IntPair::IntPairView b, Int::IntView z, StateFunction s, CostFunction c) {
    (void) new (home) MyDFA(home, a, b, z, s, c);
    return ES_OK;
  }

  virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
   // std::vector<Pair> keep;
      std::cout << "Propagating DFA" << std::endl;
      Gecode::Int::ViewValues<Int::IntView> iter(Z);
      std::vector<MPG::IntPair::Pair> newP;
      std::vector<MPG::IntPair::Pair> newQ;
      std::vector<int> newZ;
      while(iter()) {
          for(int i=0; i<Q.size(); i++) {
            MPG::IntPair::Pair q = Q.getElement(i);
            MPG::IntPair::Pair p = MPG::IntPair::Pair(S(q.x, iter.val()), q.y + C(q.y, iter.val()));
            if(P.contains(p)) {
                newP.push_back(p);
                newQ.push_back(q);
                newZ.push_back(iter.val());

            }
            std::cout << "i: " << i << std::endl;
            if(iter.val() == Z.max()) // TODO Ugly hack
                goto finish;
            ++iter;
          }
          finish:
          std::cout << "Removing values" << std::endl;

          for(int i=0; i<newZ.size(); i++)
              if(Z.nq(home, newZ[i])==Int::ME_INT_FAILED)
                  return ES_FAILED;
          for(int i=0; i<newQ.size(); i++)
              if(Q.nq(home, newQ[i])==IntPair::ME_INTPAIR_FAILED)
                  return ES_FAILED;
          for(int i=0; i<newP.size(); i++)
              if(P.nq(home, newP[i])==IntPair::ME_INTPAIR_FAILED)
                  return ES_FAILED;

          std::cout << "Finish DFA" << std::endl;
          return ES_NOFIX;
      }
  }

  virtual size_t dispose(Space& home) {
      P.cancel(home, *this, IntPair::PC_INTPAIR_DOM);
      Q.cancel(home, *this, IntPair::PC_INTPAIR_DOM);
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
void mydfa(Space& home, IntPairVar P, IntPairVar Q, IntVar Z, StateFunction S, CostFunction C) {
    std::cout << "Init DFA prop" << std::endl;
    IntPair::IntPairView vp(P);
    IntPair::IntPairView vq(Q);
    Int::IntView vz(Z);
    if (MyDFA::post(home, vp, vq, vz, S, C) != ES_OK)
    home.fail();
}


#endif // DFA_H
