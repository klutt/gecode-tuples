#ifndef __INTPAIRAPPROXVIEW_HH__
#define __INTPAIRAPPROXVIEW_HH__

#include "var.hh"
#include <iostream>

// using Gecode::VarImpView;
// using Gecode::ConstView;

namespace MPG { namespace IntPair {

    class IntPairApproxView : public Gecode::VarImpView<IntPairApproxVar> {
    protected:
      using VarImpView<IntPairApproxVar>::x;
    public:


      IntPairApproxView(void) {}
      IntPairApproxView(const IntPairApproxVar& y)
	: VarImpView<IntPairApproxVar>(y.varimp()) {}
      IntPairApproxView(IntPairApproxVarImp* y)
	: VarImpView<IntPairApproxVar>(y) {}
      // access operations
      int xmax(void) const { return x->xmax(); }
      int xmin(void) const { return x->xmin(); }
      int size(void) const { return x->size(); }
      bool contains(const Pair &p) const { return x->contains(p); }
      Pair val() { return x->val(); }
      Pair getElement(int n) { return x->getElement(n); }
      int domsize() const { return x->domsize(); }
      int getx(int i) const { return x->getx(i); }
      int getl(int i) const { return x->getl(i); }
      int geth(int i) const { return x->geth(i); }
      // For branching
      Pair first() const { return x->first(); }

      PairApprox getpa(int i) const { return x->getpa(i); }
      int getxindex(int i) const { return x->getxindex(i); }
      Gecode::ModEvent xeq(Gecode::Space&home, const PairApprox& p) { x->xeq(home, p); }
      Gecode::ModEvent xlq(Gecode::Space& home, int n) { return x->xlq(home,n);  }
      Gecode::ModEvent eq(Gecode::Space& home, const Pair& p) { return x->eq(home,p);  }
      Gecode::ModEvent eq(Gecode::Space& home, const IntPairApproxView& p) { return x->eq(home,*p.varimp());  }
      Gecode::ModEvent nq(Gecode::Space& home, const Pair& p) { return x->nq(home,p);  }

      void subscribe(Gecode::Space& home, Gecode::Propagator & prop, Gecode::PropCond pc, bool schedule = true) {
          std::cout << "Subscribing" << std::endl;
        x->subscribe(home, prop, pc, schedule);
      }

      void cancel(Gecode::Space& home, Gecode::Propagator& prop, Gecode::PropCond pc) {
        x->cancel(home, prop, pc);
      }
      /*
    Gecode::ModEvent gq(Gecode::Space& home, int n) {
      return x->gq(home,n);
    }
    // delta information
    int min(const Delta& d) const {
      return IntVarImp::min(d);
    }
    int max(const Delta& d) const {
      return IntVarImp::max(d);
      } */
    }; 
    
    template<class Char, class Traits>
    std::basic_ostream<Char,Traits>&
    operator<<(std::basic_ostream<Char,Traits>& os, const IntPairApproxView& x) { os << x.varimp(); }

  }
}

/*
// constant integer view
namespace MPG { namespace IntPair {

    class ConstIntPairApproxView : public ConstView<IntPairApproxView> {
    protected:
      Pair p;
    public:
      ConstIntPairApproxView(void) : p(0,0) {}
      ConstIntPairApproxView(int x, int y) : p(x,y) {}
      /*
      IntPair::Pair first(void) const {
	return p;
	}*/

      /*
      int min(void) const {
	return x;
      }
      int max(void) const {
	return x;
	} */

      /*
    Gecode::ModEvent lq(Gecode::Space& home, int n) {
      return (x <= n) ? ME_INT_NONE : ME_INT_FAILED;
    }
    Gecode::ModEvent gq(Gecode::Space& home, int n) {
      return (x >= n) ? ME_INT_NONE : ME_INT_FAILED;
    }
    // delta information
    int min(const Delta& d) const {
      GECODE_NEVER; return 0;
    }
    int max(const Delta& d) const {
      GECODE_NEVER; return 0;
      } 
      // update during cloning
      void update(Gecode::Space& home, bool share, ConstIntPairApproxView& y) {
	ConstView<IntPairApproxView>::update(home,share,y);
	p.x = y.p.x; p.y=y.p.y;
      }
    };
    // view tests
    
  inline bool same(const ConstIntPairApproxView& x, const ConstIntPairApproxView& y) {
    return x.min() == y.min();
  }
  inline bool before(const ConstIntPairApproxView& x, const ConstIntPairApproxView& y) {
    return x.min() < y.min();
  }
    
    template<class Char, class Traits>
    std::basic_ostream<Char,Traits>&
    operator <<(std::basic_ostream<Char,Traits>& os, const ConstIntPairApproxView& x) { return os << x.varimp; }

  }}
*/
/*
// minus view
namespace MPG { namespace Int {

  class MinusView : public DerivedView<IntView> {
  protected:
    using DerivedView<IntView>::x;
    // modification events and propagation conditions
    static Gecode::ModEvent minusme(Gecode::ModEvent me) {
      switch (me) {
      case ME_INT_MIN: return ME_INT_MAX;
      case ME_INT_MAX: return ME_INT_MIN;
      default: return me;
      }
    }
    static Gecode::PropCond minuspc(Gecode::PropCond pc) {
      switch (pc) {
      case PC_INT_MIN: return PC_INT_MAX;
      case PC_INT_MAX: return PC_INT_MIN;
      default: return pc;
      }
    }
  public:
    MinusView(void) {}
    explicit MinusView(const IntView& y) 
      : DerivedView<IntView>(y) {}
    // access operations
    int min(void) const {
      return -x.max();
    }
    int max(void) const {
      return -x.min();
    }
    // modification operations
    Gecode::ModEvent lq(Gecode::Space& home, int n) {
      return minusme(x.gq(home,-n));
    }
    Gecode::ModEvent gq(Gecode::Space& home, int n) {
      return minusme(x.lq(home,-n));
    }
    // support operations
    static void schedule(Gecode::Space& home, Propagator& p, Gecode::ModEvent me) {
      return IntView::schedule(home,p,minusme(me));
    }
    static Gecode::ModEvent me(const Gecode::ModEventDelta& med) {
      return minusme(IntView::me(med));
    }
    static Gecode::ModEventDelta med(Gecode::ModEvent me) {
      return IntView::med(minusme(me));
    }
    // subscriptions
    void subscribe(Gecode::Space& home, Propagator& p, Gecode::PropCond pc, 
                   bool schedule=true) {
      x.subscribe(home,p,minuspc(pc),schedule);
    }
    void subscribe(Gecode::Space& home, Advisor& a) {
      x.subscribe(home,a);
    }
    void cancel(Gecode::Space& home, Propagator& p, Gecode::PropCond pc) {
      x.cancel(home,p,minuspc(pc));
    }
    void cancel(Gecode::Space& home, Advisor& a) {
      x.cancel(home,a);
    }
    // delta information
    static Gecode::ModEvent modevent(const Delta& d) {
      return minusme(IntView::modevent(d));
    }
    int min(const Delta& d) const {
      return -x.max(d);
    }
    int max(const Delta& d) const {
      return -x.min(d);
    }
  };

  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const MinusView& x) {
    std::basic_ostringstream<Char,Traits> s;
    s.copyfmt(os); s.width(0);
    if (x.assigned())
    s << x.min();
    else
    s << '[' << x.min() << ".." << x.max() << ']';
    return os << s.str();
  }

}}
// offset view
namespace MPG { namespace IntPair {

  class OffsetView : public DerivedView<IntView> {
  protected:
    using DerivedView<IntView>::x;
     c;
  public:
    OffsetView(void) {}
    OffsetView(const IntView& y, int d)
      : DerivedView<IntView>(y), c(d) {}

    int offset(void) const {
      return c;
    }
    int min(void) const {
      return x.min()+c;
    }
    int max(void) const {
      return x.max()+c;
    }

    Gecode::ModEvent lq(Gecode::Space& home, int n) {
      return x.lq(home,n-c);
    }
    Gecode::ModEvent gq(Gecode::Space& home, int n) {
      return x.gq(home,n-c);
    }

    int min(const Delta& d) const {
      return x.min(d)+c;
    }
    int max(const Delta& d) const {
      return x.max(d)+c;
    }

    // update during cloning
    void update(Gecode::Space& home, bool share, OffsetView& y) {
      x.update(home,share,y.x);
      c=y.c;
    }
  };
  // view tests
  inline bool same(const OffsetView& x, const OffsetView& y) {
    return same(x.base(),y.base()) && (x.offset() == y.offset());
  }
  inline bool before(const OffsetView& x, const OffsetView& y) {
    return before(x.base(),y.base())
      || (same(x.base(),y.base()) && (x.offset() < y.offset()));
  }

  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const OffsetView& x) {
    std::basic_ostringstream<Char,Traits> s;
    s.copyfmt(os); s.width(0);
    if (x.assigned())
    s << x.min();
    else
    s << '[' << x.min() << ".." << x.max() << ']';
    return os << s.str();
  }
  
}}
*/




#endif
