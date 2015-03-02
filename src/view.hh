#ifndef __INTPAIRVIEW_HH__
#define __INTPAIRVIEW_HH__

#include "var.hh"
#include <iostream>

using Gecode::VarImpView;
using Gecode::ConstView;

namespace MPG { namespace IntPair {

    class IntPairView : public VarImpView<IntPairVar> {
    protected:
      using VarImpView<IntPairVar>::x;
    public:

      IntPairView(void) {}
      IntPairView(const IntPairVar& y)
	: VarImpView<IntPairVar>(y.varimp()) {}
      IntPairView(IntPairVarImp* y)
	: VarImpView<IntPairVar>(y) {}
      // access operations
      int xmax(void) const { return x->xmax(); }
      int xmin(void) const { return x->xmin(); }
      int size(void) const { return x->size(); }
      bool contains(const Pair &p) const { return x->contains(p); }

      // For branching
      Pair first() const { return x->first(); }

      ModEvent rel(Space&home, IP_INT_REL r, int dim, int n) { return x->rel(home, r, dim, n); }
      ModEvent xlq(Space& home, int n) { return x->xlq(home,n);  }
      ModEvent eq(Space& home, const Pair& p) { return x->eq(home,p);  }
      ModEvent eq(Space& home, const IntPairView& p) { return x->eq(home,*p.varimp());  }
      ModEvent neq(Space& home, const Pair& p) { return x->neq(home,p);  }

      void subscribe(Space& home, Propagator & prop, PropCond pc, bool schedule = true) {
          std::cout << "Subscribing" << std::endl;
        x->subscribe(home, prop, pc, schedule);
      }

      void cancel(Space& home, Propagator& prop, PropCond pc) {
        x->cancel(home, prop, pc);
      }
      /*
    ModEvent gq(Space& home, int n) {
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
    operator<<(std::basic_ostream<Char,Traits>& os, const IntPairView& x) { os << x.varimp(); }
  }
}
/*
// constant integer view
namespace MPG { namespace IntPair {

    class ConstIntPairView : public ConstView<IntPairView> {
    protected:
      Pair p;
    public:
      ConstIntPairView(void) : p(0,0) {}
      ConstIntPairView(int x, int y) : p(x,y) {}
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
    ModEvent lq(Space& home, int n) {
      return (x <= n) ? ME_INT_NONE : ME_INT_FAILED;
    }
    ModEvent gq(Space& home, int n) {
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
      void update(Space& home, bool share, ConstIntPairView& y) {
	ConstView<IntPairView>::update(home,share,y);
	p.x = y.p.x; p.y=y.p.y;
      }
    };
    // view tests
    
  inline bool same(const ConstIntPairView& x, const ConstIntPairView& y) {
    return x.min() == y.min();
  }
  inline bool before(const ConstIntPairView& x, const ConstIntPairView& y) {
    return x.min() < y.min();
  }
    
    template<class Char, class Traits>
    std::basic_ostream<Char,Traits>&
    operator <<(std::basic_ostream<Char,Traits>& os, const ConstIntPairView& x) { return os << x.varimp; }

  }}
*/
/*
// minus view
namespace MPG { namespace Int {

  class MinusView : public DerivedView<IntView> {
  protected:
    using DerivedView<IntView>::x;
    // modification events and propagation conditions
    static ModEvent minusme(ModEvent me) {
      switch (me) {
      case ME_INT_MIN: return ME_INT_MAX;
      case ME_INT_MAX: return ME_INT_MIN;
      default: return me;
      }
    }
    static PropCond minuspc(PropCond pc) {
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
    ModEvent lq(Space& home, int n) {
      return minusme(x.gq(home,-n));
    }
    ModEvent gq(Space& home, int n) {
      return minusme(x.lq(home,-n));
    }
    // support operations
    static void schedule(Space& home, Propagator& p, ModEvent me) {
      return IntView::schedule(home,p,minusme(me));
    }
    static ModEvent me(const ModEventDelta& med) {
      return minusme(IntView::me(med));
    }
    static ModEventDelta med(ModEvent me) {
      return IntView::med(minusme(me));
    }
    // subscriptions
    void subscribe(Space& home, Propagator& p, PropCond pc, 
                   bool schedule=true) {
      x.subscribe(home,p,minuspc(pc),schedule);
    }
    void subscribe(Space& home, Advisor& a) {
      x.subscribe(home,a);
    }
    void cancel(Space& home, Propagator& p, PropCond pc) {
      x.cancel(home,p,minuspc(pc));
    }
    void cancel(Space& home, Advisor& a) {
      x.cancel(home,a);
    }
    // delta information
    static ModEvent modevent(const Delta& d) {
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

    ModEvent lq(Space& home, int n) {
      return x.lq(home,n-c);
    }
    ModEvent gq(Space& home, int n) {
      return x.gq(home,n-c);
    }

    int min(const Delta& d) const {
      return x.min(d)+c;
    }
    int max(const Delta& d) const {
      return x.max(d)+c;
    }

    // update during cloning
    void update(Space& home, bool share, OffsetView& y) {
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
