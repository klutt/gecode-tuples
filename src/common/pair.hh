#ifndef PAIR_HH
#define PAIR_HH

namespace MPG { namespace IntPair {
    
struct Pair {
  int x, y;
  Pair() {};
  Pair(const Pair& p) : x(p.x), y(p.y) {};
  Pair(int x, int y) : x(x), y(y) {};
};

    bool operator==(const Pair& p1, const MPG::IntPair::Pair& p2) {
  return (p1.x==p2.x && p1.y==p2.y);
}

    Gecode::Archive&
    operator <<(Gecode::Archive& os, const MPG::IntPair::Pair& p) {
  return os << p.x << p.y;
};


    Gecode::Archive&
    operator >> (Gecode::Archive& os, MPG::IntPair::Pair& p) {
  return os >> p.y >> p.x;
};


template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator <<(std::basic_ostream<Char,Traits>& os, const MPG::IntPair::Pair& p) {
  std::basic_ostringstream<Char,Traits> s;
  s.copyfmt(os); s.width(0);
  s << '<' << p.x << ',' << p.y << '>';
  return os << s.str();
};


  }}
#endif
