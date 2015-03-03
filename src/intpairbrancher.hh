#ifndef __INTPAIRBRANCHER_HH__
#define __INTPAIRBRANCHER_HH__

using Gecode::Brancher
using Gecode::View;

void intPairNone(Home, const IntPairArg& x) {
  if (home.failed()) return;
  View<IntPair::IntPairView> y(home, x);
  IntPairNone::post(home, y);
}

class IntPairNone : public Brancher {
protected:
  View<IntPair::IntPairView> x;
public:
  IntPairNone(Home home, View<IntPair::IntPairView>& x0)
    : Brancher(home), x(x0) {}
  static void post(Home home, View<IntPair::IntPairView>& x) {
    (void) new (home) IntPairNone(home, x);
  }

  
#endif
