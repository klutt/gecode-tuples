#ifndef NONEMINAPPROX_H
#define NONEMINAPPROX_H

class NoneNone : public Gecode::Brancher::Brancher {
protected:
  Gecode::ViewArray<MPG::IntPair::IntPairApproxView> x;
  // choice definition
  class PosVal : public Gecode::Choice {
  public:
    int pos; MPG::IntPair::Pair val;
  PosVal(const NoneNone& b, int p, MPG::IntPair::Pair v)
      : Choice(b,2), pos(p), val(v) {}
    virtual size_t size(void) const {
      return sizeof(*this);
    }
    virtual void archive(Gecode::Archive& e) const {
      Choice::archive(e);
      e << pos << val;
    }
  };
public:
 NoneNone(Gecode::Home home, Gecode::ViewArray<MPG::IntPair::IntPairApproxView>& x0)
    : Brancher(home), x(x0) {}
  static void post(Gecode::Home home, Gecode::ViewArray<MPG::IntPair::IntPairApproxView>& x) {
    (void) new (home) NoneNone(home,x);
  }
  virtual size_t dispose(Gecode::Space& home) {
    (void) Brancher::dispose(home);
    return sizeof(*this);
  }
 NoneNone(Gecode::Space& home, bool share, NoneNone& b)
    : Brancher(home,share,b) {
    x.update(home,share,b.x);
  }
  virtual Brancher* copy(Gecode::Space& home, bool share) {
    return new (home) NoneNone(home,share,*this);
  }
  // status
  virtual bool status(const Gecode::Space& home) const {
    for (int i=0; i<x.size(); i++)
      if (!x[i].assigned())
        return true;
    return false;
  }
  // choice
  virtual Gecode::Choice* choice(Gecode::Space& home) {
    //    std::cout << "nonenoneapprox::choice" << std::endl;
    for (int i=0; true; i++)
      if (!x[i].assigned())
        return new PosVal(*this,i,x[i].first());
    GECODE_NEVER;
    return NULL;
  }
  virtual Gecode::Choice* choice(const Gecode::Space&, Gecode::Archive& e) {
    int pos;
    MPG::IntPair::Pair val;
    //    std::cout << "nonenoneapprox::choice with archive" << std::endl;
    e >> pos >> val;
    return new PosVal(*this, pos, val);
  }
  // commit
  virtual Gecode::ExecStatus commit(Gecode::Space& home,
				    const Gecode::Choice& c,
                            unsigned int a) {
    //        std::cout << "nonenoneapprox::commit" << std::endl;

    const PosVal& pv = static_cast<const PosVal&>(c);
    int pos=pv.pos;
    MPG::IntPair::Pair val(pv.val);
    if (a == 0) {
      //      std::cout << "nonenoneapprox       eq" << std::endl;
      return Gecode::me_failed(x[pos].eq(home,val)) ? Gecode::ES_FAILED : Gecode::ES_OK;
    }
    else {
      //      std::cout << "nonenoneapprox       nq" << std::endl;
      return Gecode::me_failed(x[pos].nq(home,val)) ? Gecode::ES_FAILED : Gecode::ES_OK;
    }
  }
  // print
  virtual void print(const Gecode::Space& home, const Gecode::Choice& c,
                     unsigned int a,
                     std::ostream& o) const {
    const PosVal& pv = static_cast<const PosVal&>(c);
    int pos=pv.pos;
    MPG::IntPair::Pair val(pv.val);
    if (a == 0)
      o << "x[" << pos << "] = " << val;
    else
      o << "x[" << pos << "] != " << val;
  }
};

void nonenone(Gecode::Home home, const MPG::IntPairApproxVarArgs& x) {
  if (home.failed()) return;
  Gecode::ViewArray<MPG::IntPair::IntPairApproxView> y(home,x);
  NoneNone::post(home,y);
}


#endif // NONEMIN_H
