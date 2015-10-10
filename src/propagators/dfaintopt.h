#ifndef DFAINTOPT_H
#define DFAINTOPT_H


#include <vector>
#include <gecode/int.hh>
#include "dfainterface.h"

// This is the IntVar version of dfa propagator. It is used for performance comparision.


class MyDFAintOpt : public Gecode::Propagator::Propagator {
protected:
    Gecode::Int::IntView Px;
    Gecode::Int::IntView Qx;
    Gecode::Int::IntView Py;
    Gecode::Int::IntView Qy;
    Gecode::Int::IntView Z;
    MPG::IntPair::DFA_I *D;
    
public:
    MyDFAintOpt(Gecode::Space& home, Gecode::Int::IntView px, Gecode::Int::IntView py, Gecode::Int::IntView qx, Gecode::Int::IntView qy, Gecode::Int::IntView z, MPG::IntPair::DFA_I *d)
        : Propagator(home), Px(px), Py(py), Qx(qx), Qy(qy), Z(z), D(d)
    {
        Px.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
        Qx.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
        Py.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
        Qy.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
        Z.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
    }

    MyDFAintOpt(Gecode::Space& home, bool share, MyDFAintOpt& prop)
        : Propagator(home, share, prop), D(prop.D) {
        Px.update(home, share, prop.Px);
        Qx.update(home, share, prop.Qx);
        Py.update(home, share, prop.Py);
        Qy.update(home, share, prop.Qy);
        Z.update(home, share, prop.Z);
    }

    static Gecode::ExecStatus post(Gecode::Space& home, Gecode::Int::IntView px, Gecode::Int::IntView py, Gecode::Int::IntView qx, Gecode::Int::IntView qy, Gecode::Int::IntView z, MPG::IntPair::DFA_I *d) {
        (void) new (home) MyDFAintOpt(home, px, py, qx, qy, z, d);
        return Gecode::ES_OK;
    }

    int _prunevar(Gecode::Space &home, Gecode::Int::IntView &var, std::vector<int> &newvar) {
        Gecode::Int::ViewValues<Gecode::Int::IntView> iter(var);
        iter.init(var);
        std::vector<int> remove;
	remove.reserve(newvar.size());
        std::vector<int>::iterator newvar_i = newvar.begin();
	std::sort(newvar.begin(), newvar.end());
        while(iter()) {
            int v = iter.val();
            if(v == var.max() || var.assigned())
                break;
            if(! std::binary_search (newvar.begin(), newvar.end(), v, [](int a, int b){return a<b;}))
                remove.push_back(v);
            ++iter;
        }

        for(int i=0; i<remove.size(); i++)
            if(var.nq(home, remove[i]) == Gecode::Int::ME_INT_FAILED)
                return 0;
        return 1;

    }


    virtual Gecode::ExecStatus propagate(Gecode::Space& home, const Gecode::ModEventDelta&) {
        std::vector<int> newpx, newqx, newz;
	newpx.reserve(Px.size());
	newqx.reserve(Qx.size());
	newz.reserve(Z.size());
	
	Gecode::Iter::Ranges::NaryUnion u;
	
        Gecode::Int::ViewValues<Gecode::Int::IntView> iz(Z);
        while(iz()) {
            Gecode::Int::ViewValues<Gecode::Int::IntView> iqx(Qx);
            while(iqx()) {
                int state = D->S(iqx.val(), iz.val());
                if(state>0 && Px.in(state)) {
                    newqx.push_back(iqx.val());
                    newpx.push_back(state);
                    newz.push_back(iz.val());
		    
		    int c = D->C(iqx.val(), iz.val());
		    Gecode::Int::ViewRanges<Gecode::Int::IntView> r(Qy);
		    Gecode::Iter::Ranges::Offset<Gecode::Int::ViewRanges<Gecode::Int::IntView> > r2(r,c);
		    u|=r2; 
                }
                if(iqx.val() == Qx.max()) // TODO Ugly hack
                    break;
                ++iqx;
            }
            if(iz.val() == Z.max())
                break;
            ++iz;
        }

	if(Py.inter_r(home, u) == Gecode::Int::ME_INT_FAILED)
	  return Gecode::ES_FAILED;

        if(newz.size()==0 || newpx.size()==0 || newqx.size()==0) {
            return Gecode::ES_FAILED;
        }

        if(_prunevar(home, Z, newz) == 0)
            return Gecode::ES_FAILED;
        if(_prunevar(home, Px, newpx) == 0)
            return Gecode::ES_FAILED;
	        if(_prunevar(home, Qx, newqx) == 0)
	            return Gecode::ES_FAILED;
	//        if(_prunevar(home, Py, newpy) == 0)
	//            return Gecode::ES_FAILED;
	if(Py.size() == 0 )
	  return Gecode::ES_FAILED;
        //if(_prunevar(home, Qy, newqy) == 0)
	//  return Gecode::ES_FAILED;

        return Gecode::ES_NOFIX;


    }

    virtual size_t dispose(Gecode::Space& home) {
        Px.cancel(home, *this, Gecode::Int::PC_INT_DOM);
        Qx.cancel(home, *this, Gecode::Int::PC_INT_DOM);
        Py.cancel(home, *this, Gecode::Int::PC_INT_DOM);
        Qy.cancel(home, *this, Gecode::Int::PC_INT_DOM);
        Z.cancel(home, *this, Gecode::Int::PC_INT_DOM);
        (void) Propagator::dispose(home);
        return sizeof(*this);
    }

    virtual Propagator* copy(Gecode::Space& home, bool share) {
        return new (home) MyDFAintOpt(home, share, *this);
    }

    virtual Gecode::PropCost cost(const Gecode::Space&, const Gecode::ModEventDelta&) const {
        // Probably way wrong, but not very important. TODO
        return Gecode::PropCost::linear(Gecode::PropCost::HI, Px.size());
    }

};

//
void optintdfa(Gecode::Space& home, Gecode::IntVar Px, Gecode::IntVar Py, Gecode::IntVar Qx, Gecode::IntVar Qy, Gecode::IntVar Z, MPG::IntPair::DFA_I *D) {
    Gecode::Int::IntView vpx(Px);
    Gecode::Int::IntView vqx(Qx);
    Gecode::Int::IntView vpy(Py);
    Gecode::Int::IntView vqy(Qy);
    Gecode::Int::IntView vz(Z);
    if (MyDFAintOpt::post(home, vpx, vpy, vqx, vqy, vz, D) != Gecode::ES_OK)
        home.fail();
}



#endif // DFAINT_H
