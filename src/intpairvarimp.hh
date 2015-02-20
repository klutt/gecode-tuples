#ifndef __MPG_INTPAIRVARIMP_HH__
#define __MPG_INTPAIRVARIMP_HH__

#include <gecode/kernel.hh>

#include <iostream>
#include <vector>

using Gecode::Space;
using Gecode::ModEvent;
using Gecode::Propagator;
using Gecode::PropCond;

namespace MPG { namespace IntPair {

    struct Pair {
      int x, y;
      Pair(int x, int y) : x(x), y(y) {};
    };

    template<class Char, class Traits>
    std::basic_ostream<Char,Traits>&
    operator <<(std::basic_ostream<Char,Traits>& os, const Pair& p) {
      std::basic_ostringstream<Char,Traits> s;
      s.copyfmt(os); s.width(0);
      s << '<' << p.x << ',' << p.y << '>';
      return os << s.str();
    };
    
    class IntPairVarImp : public IntPairVarImpBase {
    private:
      Pair first(void) const {
	return Pair(domain[0]);
      }

      Pair last(void) const {
	return Pair(domain.back());
      }


    protected:
      std::vector<Pair> domain;
      
    public:
      IntPairVarImp(Space& home, int x_min, int y_min, int x_max, int y_max)
	: IntPairVarImpBase(home) {
	for(int x=x_min; x<=x_max; x++)
	  for(int y=y_min; y<=y_max; y++)
	    domain.push_back(Pair(x,y));
      }
      // access operations
      int xmax(void) const {
	return domain.back().x;
      }

      int xmin(void) const {
	return domain[0].x;
      }

      // For cost computation and printout
      int size(void) const {
	return domain.size();
      }
      
      // assignment test
      bool assigned(void) const {
	return domain.size()==1;
      }

      // Mod events
      ModEvent xlq(Space& home, int n);

      // Subscriptions and cancel
      void subscribe(Space& home, Propagator & prop, PropCond pc, bool schedule = true) {
	IntPairVarImpBase::subscribe(home, prop, pc, assigned(), schedule);
      }

      void cancel(Space& home, Propagator& prop, PropCond pc) {
	IntPairVarImpBase::cancel(home, prop, pc, assigned());
      }
      
      IntPairVarImp(Space& home, bool share, IntPairVarImp& y)
	: IntPairVarImpBase(home,share,y), domain(y.domain) {}
      IntPairVarImp* copy(Space& home, bool share) {
	if (copied())
	  return static_cast<IntPairVarImp*>(forward());
	else
	  return new (home) IntPairVarImp(home,share,*this);
      }
      
    };
  
    template<class Char, class Traits>
    std::basic_ostream<Char,Traits>&
    operator <<(std::basic_ostream<Char,Traits>& os, const IntPairVarImp& x) {
      std::basic_ostringstream<Char,Traits> s;
      s.copyfmt(os); s.width(0);
      if (x.assigned())
	s << x.first();
      else
	s << '[' << x.first() <<  ".." << x.last() << "][" << x.size() << "]";
      return os << s.str();
    }
    


    


  ModEvent IntPairVarImp::xlq(Space& home, int n) {
	bool modified=false;
	std::cout << "xlq" << std::endl;
	
	for(int i=0; i<domain.size(); i++) {
	  std::cout << "Test " << domain[i] << std::endl;
	  if (domain[i].x > n) {
	    std::cout << "Erase " << domain[i] << std::endl;
	    modified=true; 
	    domain.erase(domain.begin() + i);
	    i--;
	  }
	}
	if(domain.size()==0)
	  return ME_INTPAIR_FAILED;
	else if(modified)
	  return ME_INTPAIR_DOM; // TODO: Or NONE?
	else
	  return ME_INTPAIR_NONE;
      }

  }}
#endif

