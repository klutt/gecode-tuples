#ifndef INTPAIRBRANCH_HH
#define INTPAIRBRANCH_HH

#include "intpair.h"

using namespace Gecode;
using namespace MPG;
using namespace MPG::IntPair;

namespace MPG { namespace IntPair {

Archive&
operator <<(Archive& os, const Pair& p) {
    return os << p.x << p.y;
};


Archive&
operator >> (Archive& os, Pair& p) {
    return os >> p.y >> p.x;
};

}}

class NoneMin : public Brancher {
protected:
  ViewArray<IntPair::IntPairView> x;
  // choice definition
  class PosVal : public Choice {
  public:
    int pos; Pair val;
    PosVal(const NoneMin& b, int p, Pair v)
      : Choice(b,2), pos(p), val(v) {}
    virtual size_t size(void) const {
      return sizeof(*this);
    }
    virtual void archive(Archive& e) const {
      Choice::archive(e);
      e << pos << val;
    }
  };
public:
  NoneMin(Home home, ViewArray<IntPair::IntPairView>& x0)
    : Brancher(home), x(x0) {}
  static void post(Home home, ViewArray<IntPair::IntPairView>& x) {
    (void) new (home) NoneMin(home,x);
  }
  virtual size_t dispose(Space& home) {
    (void) Brancher::dispose(home);
    return sizeof(*this);
  }
  NoneMin(Space& home, bool share, NoneMin& b)
    : Brancher(home,share,b) {
    x.update(home,share,b.x);
  }
  virtual Brancher* copy(Space& home, bool share) {
    return new (home) NoneMin(home,share,*this);
  }
  // status
  virtual bool status(const Space& home) const {
    for (int i=0; i<x.size(); i++)
      if (!x[i].assigned())
        return true;
    return false;
  }
  // choice
  virtual Choice* choice(Space& home) {
    for (int i=0; true; i++)
      if (!x[i].assigned())
        return new PosVal(*this,i,x[i].first());
    GECODE_NEVER;
    return NULL;
  }
  virtual Choice* choice(const Space&, Archive& e) {
    int pos;
    Pair val;
    e >> pos >> val;
    return new PosVal(*this, pos, val);
  }
  // commit
  virtual ExecStatus commit(Space& home,
                            const Choice& c,
                            unsigned int a) {
    const PosVal& pv = static_cast<const PosVal&>(c);
    int pos=pv.pos;
    Pair val(pv.val);
    if (a == 0)
      return me_failed(x[pos].eq(home,val)) ? ES_FAILED : ES_OK;
    else
      return me_failed(x[pos].neq(home,val)) ? ES_FAILED : ES_OK;
  }
  // print
  virtual void print(const Space& home, const Choice& c,
                     unsigned int a,
                     std::ostream& o) const {
    const PosVal& pv = static_cast<const PosVal&>(c);
    int pos=pv.pos;
    Pair val(pv.val);
    if (a == 0)
      o << "x[" << pos << "] = " << val;
    else
      o << "x[" << pos << "] != " << val;
  }
};
void nonemin(Home home, const IntPairArgs& x) {
  if (home.failed()) return;
  ViewArray<IntPair::IntPairView> y(home,x);
  NoneMin::post(home,y);
}


#endif
