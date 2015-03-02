#ifndef __MPG_INTPAIRVARIMP_HH__
#define __MPG_INTPAIRVARIMP_HH__

#include <gecode/kernel.hh>

#include <iostream>
#include <vector>
#include <assert.h>

using Gecode::Space;
using Gecode::ModEvent;
using Gecode::Propagator;
using Gecode::PropCond;
using Gecode::Archive;

namespace MPG { namespace IntPair {

struct Pair {
    int x, y;
    Pair() {};
    Pair(const Pair& p) : x(p.x), y(p.y) {};
    Pair(int x, int y) : x(x), y(y) {};
};

Archive&
operator <<(Archive& os, const Pair& p) {
    return os << p.x << p.y;
};


Archive&
operator >> (Archive& os, Pair& p) {
    return os >> p.y >> p.x;
};


typedef bool(*IP_INT_REL) (int, int);

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
    // For printouts only
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

    // For cost computation and printout
    int size(void) const {
        return domain.size();
    }

    // assignment test
    bool assigned(void) const {
        return domain.size()==1;
    }

    // Mod events

    ModEvent rel(Space&home, IP_INT_REL, int dim, int n);
    ModEvent lq(Space& home, int dim, int n);
    ModEvent xlq(Space& home, int n);
    ModEvent eq(Space& home, const Pair& p);
    ModEvent eq(Space& home, const IntPairVarImp& p);
    ModEvent neq(Space& home, const Pair& p);

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


// Remove all Pairs that does not fullfill the relation p[dim] r n
ModEvent IntPairVarImp::rel(Space&home, IP_INT_REL r, int dim, int n)
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
        if (r(*ptr,n)) {
            //	    std::cout << "Erase " << domain[i] << std::endl;
            modified=true;
            domain.erase(domain.begin() + i);
            i--;
        }
    }
    if(domain.size()==0)
        return ME_INTPAIR_FAILED;
    else if(modified)
        return ME_INTPAIR_DOM;
    else
        return ME_INTPAIR_NONE;
}


ModEvent IntPairVarImp::lq(Space&home, int dim, int n)
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
        if (*ptr > n) {
            //	    std::cout << "Erase " << domain[i] << std::endl;
            modified=true;
            domain.erase(domain.begin() + i);
            i--;
        }
    }
    if(domain.size()==0)
        return ME_INTPAIR_FAILED;
    else if(modified)
        return ME_INTPAIR_DOM;
    else
        return ME_INTPAIR_NONE;
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

    if (domain.size()==0)
           return ME_INTPAIR_FAILED;
    if(assigned())
        return ME_INTPAIR_VAL;
    else if(modified)
        return ME_INTPAIR_DOM;
    else
        return ME_INTPAIR_NONE;
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
        if(domain.size()==0)
            return ME_INTPAIR_FAILED;
        else if(assigned())
            return ME_INTPAIR_VAL;
        else if(modified)
            return ME_INTPAIR_DOM;
        else
            return ME_INTPAIR_NONE;
}

ModEvent IntPairVarImp::neq(Space& home, const Pair& p)
{
    // Ugly and slow as fuck! Rewrite! TODO
    bool modified = false;
    std::cout << "Neq" << std::endl;
    for(int i=0; i<domain.size(); i++) {
        std::cout << "Neq trying " << p << " and " << domain[i] << std::endl;
        if (domain[i].x==p.x && domain[i].y==p.y) {
            std::cout << "Neq erase " << domain[i] << std::endl;
            domain.erase(domain.begin() + i); i--;
            modified=true;
        }
    }
        if(domain.size()==0)
            return ME_INTPAIR_FAILED;
        else if(assigned())
            return ME_INTPAIR_VAL;
        else if(modified)
            return ME_INTPAIR_DOM;
        else
            return ME_INTPAIR_NONE;
}



ModEvent IntPairVarImp::xlq(Space& home, int n) {
    return lq(home, 0, n);
}

}}
#endif

