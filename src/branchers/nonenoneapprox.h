#ifndef NONEMINAPPROX_H
#define NONEMINAPPROX_H

class NoneNone : public Brancher {
protected:
  ViewArray<IntPair::IntPairApproxView> x;
  // choice definition
  class PosVal : public Choice {
  public:
    int pos; Pair val;
    PosVal(const NoneNone& b, int p, Pair v)
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
  NoneNone(Home home, ViewArray<IntPair::IntPairApproxView>& x0)
    : Brancher(home), x(x0) {}
  static void post(Home home, ViewArray<IntPair::IntPairApproxView>& x) {
    (void) new (home) NoneNone(home,x);
  }
  virtual size_t dispose(Space& home) {
    (void) Brancher::dispose(home);
    return sizeof(*this);
  }
  NoneNone(Space& home, bool share, NoneNone& b)
    : Brancher(home,share,b) {
    x.update(home,share,b.x);
  }
  virtual Brancher* copy(Space& home, bool share) {
    return new (home) NoneNone(home,share,*this);
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
    std::cout << "nonenoneapprox::choice" << std::endl;
    for (int i=0; true; i++)
      if (!x[i].assigned())
        return new PosVal(*this,i,x[i].first());
    GECODE_NEVER;
    return NULL;
  }
  virtual Choice* choice(const Space&, Archive& e) {
    int pos;
    Pair val;
    std::cout << "nonenoneapprox::choice with archive" << std::endl;
    e >> pos >> val;
    return new PosVal(*this, pos, val);
  }
  // commit
  virtual ExecStatus commit(Space& home,
                            const Choice& c,
                            unsigned int a) {
        std::cout << "nonenoneapprox::commit" << std::endl;

    const PosVal& pv = static_cast<const PosVal&>(c);
    int pos=pv.pos;
    Pair val(pv.val);
    if (a == 0)
      return me_failed(x[pos].eq(home,val)) ? ES_FAILED : ES_OK;
    else
      return me_failed(x[pos].nq(home,val)) ? ES_FAILED : ES_OK;
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
void nonenone(Home home, const IntPairApproxVarArgs& x) {
  if (home.failed()) return;
  ViewArray<IntPair::IntPairApproxView> y(home,x);
  NoneNone::post(home,y);
}


#endif // NONEMIN_H
