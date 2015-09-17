#ifndef __MPG_INTPAIRAPPROXVARIMP_HH__
#define __MPG_INTPAIRAPPROXVARIMP_HH__

#include <gecode/kernel.hh>

#include <iostream>
#include <vector>
#include <assert.h>
#include <gecode/int.hh>

// using Gecode::Space;
// using Gecode::ModEvent;
// using Gecode::Propagator;
// using Gecode::PropCond;
// using Gecode::Archive;
// using Gecode::Delta;
// using namespace Gecode;
// using namespace MPG::IntPair;

#include "../common/pair.hh"
#include "../common/dummydelta.hh"


namespace MPG { namespace IntPair {
struct PairApprox {
  int x, l, h;
  PairApprox() {};
  PairApprox(const PairApprox& p) : x(p.x), l(p.l), h(p.h) {};
  PairApprox(int x, int l, int h) : x(x), l(l), h(h) {};
};

    template<class Char, class Traits>
    std::basic_ostream<Char,Traits>&
    operator <<(std::basic_ostream<Char,Traits>& os, const PairApprox& p) {
      std::basic_ostringstream<Char,Traits> s;
      s.copyfmt(os); s.width(0);
      s << '<' << p.x << ",(" << p.l << ',' << p.h << ")>";
      return os << s.str();
    };
    

class IntPairApproxVarImp : public IntPairApproxVarImpBase {
protected:
    std::vector<PairApprox> domain;

  
public:
  IntPairApproxVarImp(Gecode::Space& home, int x_min, int x_max, int y_min, int y_max)
        : IntPairApproxVarImpBase(home) {
        assert(x_min<=x_max && y_min<=y_max);
        for(int x=x_min; x<=x_max; x++)
	  domain.push_back(PairApprox(x,y_min, y_max));
    }
    // For printouts and brancher
    Pair first() const {
        assert(domain.size()>0);
        return Pair(domain[0].x, domain[0].l);
    }

    Pair last() const {
        assert(domain.size()>0);
        return Pair(domain.back().x, domain.back().h);
    }

    // access operations
    int xmax(void) const {
        assert(domain.size()>0);
        return domain.back().x;
    }

    int xmin(void) const {
        assert(domain.size()>0);
        return domain[0].x;
    }

    bool contains(const Pair& p) const {
        assert(domain.size()>0);
        for(int i=0; i<domain.size(); i++)
            if(p.x == domain[i].x && p.y>=domain[i].l && p.y <=domain[i].h)
                return true;
        return false;
    }

  Pair val() const {
        assert(assigned());
        return Pair(domain[0].x, domain[0].l);
    }

  int getxindex(int x) const {
    int ret = -1;
    for(int i=0; i<domain.size(); i++)
      if(domain[i].x==x)
	ret = i;
    std::cout << "getxindex returning " << ret << " while searching for " << x << std::endl;
    return ret;
  }
 
  Pair getElement(int n) const;

    // For cost computation and printout
    int size(void) const {
      int sum = 0;
      for(int i=0; i<domain.size(); i++)
	sum+=domain[i].h-domain[i].l+1;
      return sum;
    }

  int domsize(void) const { return domain.size(); }

  int getx(int i) const { return domain[i].x; }
  
  int getl(int i) const { return domain[i].l;}
  int geth(int i) const { return domain[i].h;}
  PairApprox getpa(int i) const { return domain[i]; }
    // assignment test
    bool assigned(void) const {
        return domain.size()==1 && domain[0].l == domain[0].h;
    }

    // Mod events
    enum IP_INT_REL { IP_LQ, IP_LT, IP_GQ, IP_GT, IP_EQ, IP_NQ };


    Gecode::ModEvent rel(Gecode::Space &home, int dim, int n, IP_INT_REL r);

  
    Gecode::ModEvent lq(Gecode::Space& home, int dim, int n) { return rel(home, dim, n, IP_LQ); }
    Gecode::ModEvent lt(Gecode::Space& home, int dim, int n) { return rel(home, dim, n, IP_LT); }
    Gecode::ModEvent gq(Gecode::Space& home, int dim, int n) { return rel(home, dim, n, IP_GQ); }
    Gecode::ModEvent gt(Gecode::Space& home, int dim, int n) { return rel(home, dim, n, IP_GT); }
    Gecode::ModEvent eq(Gecode::Space& home, int dim, int n) { return rel(home, dim, n, IP_EQ); }
    Gecode::ModEvent nq(Gecode::Space& home, int dim, int n) { return rel(home, dim, n, IP_NQ); }


    Gecode::ModEvent xlq(Gecode::Space& home, int n);
    Gecode::ModEvent eq(Gecode::Space& home, const Pair& p);
    Gecode::ModEvent eq(Gecode::Space& home, const IntPairApproxVarImp& p);
  Gecode::ModEvent xeq(Gecode::Space& home, const PairApprox& p);
  //    Gecode::ModEvent eq(Gecode::Space& home, const std::vector<Pair> & v);
  Gecode::ModEvent xeq(Gecode::Space& home, const std::vector<int>& v);

    Gecode::ModEvent nq(Gecode::Space& home, const Pair& p);

    // Subscriptions and cancel
    void subscribe(Gecode::Space& home, Gecode::Propagator & prop, Gecode::PropCond pc, bool schedule = true) {
      //        std::cout << "Subscribing from varimp" << std::endl;
        IntPairApproxVarImpBase::subscribe(home, prop, pc, assigned(), schedule);
    }

    void cancel(Gecode::Space& home, Gecode::Propagator& prop, Gecode::PropCond pc) {
        IntPairApproxVarImpBase::cancel(home, prop, pc, assigned());
    }

    IntPairApproxVarImp(Gecode::Space& home, bool share, IntPairApproxVarImp& y)
        : IntPairApproxVarImpBase(home,share,y), domain(y.domain) {}
    IntPairApproxVarImp* copy(Gecode::Space& home, bool share) {
        if (copied())
            return static_cast<IntPairApproxVarImp*>(forward());
        else
            return new (home) IntPairApproxVarImp(home,share,*this);
    }

};

#ifndef MOD_CLEANUP
#define MOD_CLEANUP { \
  if (domain.size()==0)\
    return ME_INTPAIRAPPROX_FAILED;\
  else if (!modified)       \
    return ME_INTPAIRAPPROX_NONE;\
  DummyDelta d;\
  return notify(home, assigned() ? ME_INTPAIRAPPROX_VAL : ME_INTPAIRAPPROX_DOM, d);\
  }
#endif

    // Removes elements whose x values does not exist in v
    Gecode::ModEvent IntPairApproxVarImp::xeq(Gecode::Space& home, const std::vector<int>& v) {
      bool modified = false;
      for(int i=0; i<domain.size(); i++){
	auto it = find(v.begin(), v.end(), domain[i].x);
	if (it == v.end()) {
	  modified = true;
	  domain.erase(domain.begin()+i); i--;
	}
      }

      MOD_CLEANUP
    }

    // limits the pairapprox a where a.x=p.x to p.l and p.h if that x value exists
    Gecode::ModEvent IntPairApproxVarImp::xeq(Gecode::Space& home, const PairApprox& p) {
      int index=getxindex(p.x);
      if(index==-1)
	return ME_INTPAIRAPPROX_NONE;
      bool modified=false;
      if(p.l>domain[index].l) {
	domain[index].l=p.l;
	modified=true;
      }
      if(p.h<domain[index].h){
	domain[index].h=p.h;
	modified=true;
      }
      if(domain[index].h<domain[index].l) {
	modified = true;
	domain.erase(domain.begin()+index);
      }

      if(domain.size()==0)
	return ME_INTPAIRAPPROX_FAILED;
      
      if(modified) {
	  DummyDelta d;
	  return notify(home, assigned() ? ME_INTPAIRAPPROX_VAL : ME_INTPAIRAPPROX_DOM, d);
      }

      return ME_INTPAIRAPPROX_NONE;
    }
	  

	
  

template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator <<(std::basic_ostream<Char,Traits>& os, const IntPairApproxVarImp& x) {
    std::basic_ostringstream<Char,Traits> s;
    s.copyfmt(os); s.width(0);
    if (x.assigned())
        s << x.first();
    else
        s << '[' << x.first() <<  ".." << x.last() << "][" << x.size() << "]";
    return os << s.str();
}

Pair IntPairApproxVarImp::getElement(int n) const {
      assert(n<size());
      int i=0, j=0;
      int m=0;
      std::cout << "getElement searching for "<< n << " in " << *this << std::endl;
      while(i<domain.size()) {
	//	  std::cout << "outer loop i: " << i << "   j:" << j << std::endl;

	j=0;
	while(j<domain[i].h-domain[i].l+1) {
	  std::cout << "inner loop i: " << i << "   j:" << j << std::endl;
	  if(m==n) {
	    Pair p (domain[i].x, domain[i].l+j);
	    std::cout << "getElement returning " << p << std::endl;
	  }
  	  m++;

	  j++;
	}
	i++;
      }
      
    }



Gecode::ModEvent IntPairApproxVarImp::eq(Gecode::Space& home, const IntPairApproxVarImp& p) {
    // Probably the most inefficient in the universe. TODO
//    std::cout << "Eq pair pair" << std::endl;
     std::cout << "IntPairApproxVarImp::eq IPAVI" << std::endl;
    bool modified=false;
//    std::cout << "Erase " << domain.size() << " " << p.domain.size() << std::endl;
    std::cout << "Modifying "  << *this << " with argument " << p << std::endl;
    for(int i=0; i<domain.size(); i++) {
      int index=p.getxindex(domain[i].x);
      if(index==-1) {
	std::cout << "Erasing " << p << std::endl;
	domain.erase(domain.begin()+i); i--; modified=true; break;
      }
      else {
	if(domain[i].l < p.domain[index].l) {
	  std::cout << "Changing lower bound to " << domain[i].l <<  std::endl;
	  domain[i].l=p.domain[index].l; modified = true;
	}
	if(domain[i].h > p.domain[index].h) {
	  std::cout << "Changing upper bound to "<< domain[i].h  << std::endl;
	  domain[i].h=p.domain[index].h; modified = true;
	}
      } 
    }


    MOD_CLEANUP
}

    
Gecode::ModEvent IntPairApproxVarImp::rel(Gecode::Space&home, int dim, int n, IP_INT_REL r)
{
    // This is very inefficient with the current representation.
    // It would be much better with vector<vector<int>> istead
    // of vector<Pair>
     std::cout << "IntPairApproxVarImp::rel " << std::endl;
    bool modified=false;

    for(int i=0; i<domain.size(); i++) {
        bool erase = false;

        if(dim==0) {
	    int *ptr;
            ptr=&domain[i].x;

	    switch (r) {
            case IP_LQ : if(*ptr > n) erase = true; break;
            case IP_LT : if(*ptr >= n) erase = true; break;
            case IP_GQ : if(*ptr < n) erase = true; break;
            case IP_GT : if(*ptr <= n) erase = true; break;
            case IP_EQ : if(*ptr != n) erase = true; break;
            case IP_NQ : if(*ptr == n) erase = true; break;
	    default : break;
	    }
	}
	else if(dim==1) {
	  int *l, *h;
	  l=&domain[i].l; h=&domain[i].h;
	  
	    switch (r) {
            case IP_LQ : if(*h > n) { *h = n; modified = true; }break;
            case IP_LT : if(*h >= n) { *h=n-1; modified = true; } break;
            case IP_GQ : if(*l < n) { *l = n; modified=true; } break;
            case IP_GT : if(*l <= n) { *l = n+1; modified = true; } break;
            case IP_EQ : if(*l > n || *h < n) erase = true; else { *l=*h=n; modified=true;} break;
            case IP_NQ : if(*l == n) {*l++; modified=true; } if(*h == n) { *h--; modified=true; } break;
	    default : break;
	    }
	    if(*l>*h)
	      erase=true;
	}
	if (erase) {
	  //	    std::cout << "Erase " << domain[i] << std::endl;
	  modified=true;
	  domain.erase(domain.begin() + i);
	  i--;
	}

    }

    MOD_CLEANUP
}



/*
Gecode::ModEvent IntPairApproxVarImp::eq(Gecode::Space& home, const std::vector<Pair> & v) {
    // Probably the most inefficient in the universe. TODO
//    std::cout << "Eq vector" << std::endl;
    bool modified=false;
//    std::cout << "Erase " << domain.size() << " " << v.size() << std::endl;
    for(int i=0; i<domain.size(); i++)
        for(int j=0; j<v.size(); j++)
            if(domain[i] == v[j]) {
                domain.erase(domain.begin()+i); i--;
                modified=true;
            }

    MOD_CLEANUP
    }*/



Gecode::ModEvent IntPairApproxVarImp::eq(Gecode::Space& home, const Pair& p)
{
  std::cout << "IntPairApproxVarImp::eq Pair" << *this << "     " << p << std::endl;

  bool modified = false;
    int index = getxindex(p.x);
    if(index == -1) {
      std::cout << "IntPairApproxVarImp::eq Pair     x not found" << std::endl;
      return ME_INTPAIRAPPROX_FAILED;
    }
    else if(p.y < domain[index].l || p.y > domain[index].h) {
      std::cout << "IntPairApproxVarImp::eq Pair     y out of range" << std::endl;
      return ME_INTPAIRAPPROX_FAILED;
    }
    else if(p.y > domain[index].l && p.y < domain[index].h) {
      std::cout << "IntPairApproxVarImp::eq Pair     y not on bound, nothing to do" << std::endl;
      return ME_INTPAIRAPPROX_NONE;
    }
    else {
      std::cout << "IntPairApproxVarImp::eq Pair     changing" << std::endl;
      domain[0] = MPG::IntPair::PairApprox(p.x, p.y, p.y);
      domain.erase(domain.begin()+1, domain.end());
      DummyDelta d;							
      return notify(home, assigned() ? ME_INTPAIRAPPROX_VAL : ME_INTPAIRAPPROX_DOM, d);
    }
}

Gecode::ModEvent IntPairApproxVarImp::nq(Gecode::Space& home, const Pair& p)
{
  bool modified = false;
     std::cout << "IntPairApproxVarImp::nq IPAVI" << std::endl;

     int index = getxindex(p.x);
     if(index == -1) {
       std::cout << "nq x not found" << std::endl;
       return ME_INTPAIRAPPROX_NONE;
     }
     else if (p.y < domain[index].l || p.y > domain[index].h) {
       std::cout << "nq y not on boundaries" << std::endl;
      return ME_INTPAIRAPPROX_NONE;
     }
    else if (p.y == domain[index].l) {
      std::cout << "nq increasing lower bound" << std::endl;
      domain[index].l++;
      modified = true;
    }
    else if (p.y == domain[index].h) {
      std::cout << "nq x not found" << std::endl;
      domain[index].h--;
      modified = true;
    }

     if(domain[index].l > domain[index].h) {
       domain.erase(domain.begin()+index);
       modified=true;
       if(domain.size()==0)
	 return ME_INTPAIRAPPROX_FAILED;
     }

    if(modified) {
      DummyDelta d;
      return notify(home, assigned() ? ME_INTPAIRAPPROX_VAL : ME_INTPAIRAPPROX_DOM, d);
    }
    else
      return ME_INTPAIRAPPROX_NONE;
}



Gecode::ModEvent IntPairApproxVarImp::xlq(Gecode::Space& home, int n) {
    return lq(home, 0, n);
}

}}
#endif

