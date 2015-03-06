#ifndef __MPG_INTPAIRVARIMP_HH__
#define __MPG_INTPAIRVARIMP_HH__

#include <gecode/kernel.hh>

#include <iostream>
#include <vector>
#include <assert.h>
#include <gecode/int.hh>

using Gecode::Space;
using Gecode::ModEvent;
using Gecode::Propagator;
using Gecode::PropCond;
using Gecode::Archive;
using Gecode::Delta;
using namespace Gecode;

namespace MPG { namespace IntPair {

    class DummyDelta : public Delta {
    };
    
struct Pair {
    int x, y;
    Pair() {};
    Pair(const Pair& p) : x(p.x), y(p.y) {};
    Pair(int x, int y) : x(x), y(y) {};
};

bool operator==(const Pair& p1, const Pair& p2) {
    return (p1.x==p2.x && p1.y==p2.y);
}

Archive&
operator <<(Archive& os, const Pair& p) {
    return os << p.x << p.y;
};


Archive&
operator >> (Archive& os, Pair& p) {
    return os >> p.y >> p.x;
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
protected:
    std::vector<Pair> domain;

public:
    IntPairVarImp(Space& home, int x_min, int x_max, int y_min, int y_max)
        : IntPairVarImpBase(home) {
        assert(x_min<=x_max && y_min<=y_max);
        for(int x=x_min; x<=x_max; x++)
            for(int y=y_min; y<=y_max; y++)
                domain.push_back(Pair(x,y));
    }
    // For printouts and brancher
    Pair first() const {
        assert(domain.size()>0);
        return domain[0];
    }

    Pair last() const {
        assert(domain.size()>0);
        return domain.back();
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
            if(p.x == domain[i].x && p.y==domain[i].y)
                return true;
        return false;
    }

    Pair val() const {
        assert(assigned());
        return Pair(domain[0]);
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
    enum IP_INT_REL { IP_LQ, IP_LT, IP_GQ, IP_GT, IP_EQ, IP_NQ };


    ModEvent rel(Space &home, int dim, int n, IP_INT_REL r);


    ModEvent lq(Space& home, int dim, int n) { return rel(home, dim, n, IP_LQ); }
    ModEvent lt(Space& home, int dim, int n) { return rel(home, dim, n, IP_LT); }
    ModEvent gq(Space& home, int dim, int n) { return rel(home, dim, n, IP_GQ); }
    ModEvent gt(Space& home, int dim, int n) { return rel(home, dim, n, IP_GT); }
    ModEvent eq(Space& home, int dim, int n) { return rel(home, dim, n, IP_EQ); }
    ModEvent nq(Space& home, int dim, int n) { return rel(home, dim, n, IP_NQ); }


    ModEvent xlq(Space& home, int n);
    ModEvent eq(Space& home, const Pair& p);
    ModEvent eq(Space& home, const IntPairVarImp& p);

    ModEvent neq(Space& home, const Pair& p);

    // Subscriptions and cancel
    void subscribe(Space& home, Propagator & prop, PropCond pc, bool schedule = true) {
        std::cout << "Subscribing from varimp" << std::endl;
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

ModEvent IntPairVarImp::rel(Space&home, int dim, int n, IP_INT_REL r)
{
    // This is very inefficient with the current representation.
    // It would be much better with vector<vector<int>> istead
    // of vector<Pair>
    bool modified=false;

    for(int i=0; i<domain.size(); i++) {
        //	  std::cout << "Test " << domain[i] << std::endl;
        int *ptr;
        if(dim==0)
            ptr=&domain[i].x;
        else if(dim==1)
            ptr=&domain[i].y;

        bool erase = false;
        switch (r) {
            case IP_LQ : if(*ptr > n) erase = true; break;
            case IP_LT : if(*ptr >= n) erase = true; break;
            case IP_GQ : if(*ptr < n) erase = true; break;
            case IP_GT : if(*ptr <= n) erase = true; break;
            case IP_EQ : if(*ptr != n) erase = true; break;
            case IP_NQ : if(*ptr == n) erase = true; break;
        default : break;
        }

        if (erase) {
            //	    std::cout << "Erase " << domain[i] << std::endl;
            modified=true;
            domain.erase(domain.begin() + i);
            i--;
        }
    }
    if (!modified)
      return ME_INTPAIR_NONE;
    else if (domain.size()==0)
      return ME_INTPAIR_FAILED;
    DummyDelta d;
    return notify(home, assigned() ? ME_INTPAIR_VAL : ME_INTPAIR_DOM, d);
}



ModEvent IntPairVarImp::eq(Space& home, const IntPairVarImp& p) {
    // Probably the most inefficient in the universe. TODO
    std::cout << "Eq pair pair" << std::endl;
    bool modified=false;
    std::cout << "Erase " << domain.size() << " " << p.domain.size() << std::endl;
    for(int i=0; i<domain.size(); i++)
        if(!p.contains(domain[i])) {
            domain.erase(domain.begin()+i); i--;
            modified=true;
        }


    if (!modified)
      return ME_INTPAIR_NONE;
    else if (domain.size()==0)
      return ME_INTPAIR_FAILED;
    DummyDelta d;
    return notify(home, assigned() ? ME_INTPAIR_VAL : ME_INTPAIR_DOM, d);
}

ModEvent IntPairVarImp::eq(Space& home, const Pair& p)
{
    // Ugly and slow as fuck! Rewrite! TODO
    bool modified = false;
    std::cout << "Eq" << std::endl;
    for(int i=0; i<domain.size(); i++) {
        std::cout << "Eq trying " << p << " and " << domain[i] << std::endl;

        if (domain[i].x!=p.x || domain[i].y!=p.y) {
            std::cout << "Eq erase " << domain[i] << std::endl;
            domain.erase(domain.begin() + i); i--;
            modified=true;

        }
    }


    if (!modified)
      return ME_INTPAIR_NONE;
    else if (domain.size()==0)
      return ME_INTPAIR_FAILED;
    DummyDelta d;
    return notify(home, assigned() ? ME_INTPAIR_VAL : ME_INTPAIR_DOM, d);
}

ModEvent IntPairVarImp::neq(Space& home, const Pair& p)
{
    // Ugly and slow as fuck! Rewrite! TODO
    bool modified = false;
    std::cout << "Neq before " << *this << std::endl;
    for(int i=0; i<domain.size(); i++) {
        std::cout << "Neq trying " << p << " and " << domain[i] << std::endl;
        if (domain[i].x==p.x && domain[i].y==p.y) {
            std::cout << "Neq erase " << domain[i] << std::endl;
            domain.erase(domain.begin() + i); i--;
            modified=true;
        }
    }
    std::cout << "Neq after " << *this << std::endl;

    if (!modified)
      return ME_INTPAIR_NONE;
    else if (domain.size()==0)
      return ME_INTPAIR_FAILED;
    DummyDelta d;
    return notify(home, assigned() ? ME_INTPAIR_VAL : ME_INTPAIR_DOM, d);
}



ModEvent IntPairVarImp::xlq(Space& home, int n) {
    return lq(home, 0, n);
}

}}
#endif

