#ifndef DFAINT_H
#define DFAINT_H


#include <vector>
#include <gecode/int.hh>
#include "dfainterface.h"

// This is the IntVar version of dfa propagator. It is used for performance comparision.


class MyDFAint : public Gecode::Propagator::Propagator {
protected:
  Gecode::Int::IntView Px;
    Gecode::Int::IntView Qx;
    Gecode::Int::IntView Py;
    Gecode::Int::IntView Qy;
    Gecode::Int::IntView Z;
    MPG::IntPair::DFA_I *D;
    
 public:
 MyDFAint(Gecode::Space& home, Gecode::Int::IntView px, Gecode::Int::IntView py, Gecode::Int::IntView qx, Gecode::Int::IntView qy, Gecode::Int::IntView z, MPG::IntPair::DFA_I *d)
      : Propagator(home), Px(px), Py(py), Qx(qx), Qy(qy), Z(z), D(d)
    {
        Px.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
        Qx.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
        Py.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
        Qy.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
        Z.subscribe(home, *this, Gecode::Int::PC_INT_DOM);
    }

    MyDFAint(Gecode::Space& home, bool share, MyDFAint& prop)
      : Propagator(home, share, prop), D(prop.D) {
        Px.update(home, share, prop.Px);
        Qx.update(home, share, prop.Qx);
        Py.update(home, share, prop.Py);
        Qy.update(home, share, prop.Qy);
        Z.update(home, share, prop.Z);
    }

    static Gecode::ExecStatus post(Gecode::Space& home, Gecode::Int::IntView px, Gecode::Int::IntView py, Gecode::Int::IntView qx, Gecode::Int::IntView qy, Gecode::Int::IntView z, MPG::IntPair::DFA_I *d) {
        (void) new (home) MyDFAint(home, px, py, qx, qy, z, d);
        return Gecode::ES_OK;
    }

    virtual Gecode::ExecStatus propagate(Gecode::Space& home, const Gecode::ModEventDelta&) {
            std::vector<int> newpx, newpy, newqx, newqy, newz;
            Gecode::Int::ViewValues<Gecode::Int::IntView> iz(Z);
	    std::cout << "Init DFAint" << std::endl;
      while(iz()) {
            Gecode::Int::ViewValues<Gecode::Int::IntView> iqx(Qx);
	    //	std::cout << " z " << iz.val() << " zmax: " << Z.max() << std::endl;
	while(iqx()) {
	  //	  std::cout << " qx " << iqx.val() << " Qxmax " << Qx.max() << std::endl;
	  	  int state = D->S(iqx.val(), iz.val());
	  //	  std::cout << " state: " << state << std::endl;
	  if(state>0 && Px.in(state)) {
	    newqx.push_back(iqx.val());
	    newpx.push_back(state);
	    newz.push_back(iz.val());
	    Gecode::Int::ViewValues<Gecode::Int::IntView> iqy(Qy);
	    while(iqy()) {
	      int cost = iqy.val() + D->C(iqx.val(), iz.val());
	      if(Py.in(cost)) {
		newqy.push_back(iqy.val());
		newpy.push_back(cost);
	      }
	      if(iqy.val() == Qy.max()) // TODO Ugly hack
		break;
	      ++iqy;
	    }
	  }
	  if(iqx.val() == Qx.max()) // TODO Ugly hack
	     break;
	  ++iqx;
	  //	  std::cout << " balle " << std::endl;
	}
	if(iz.val() == Z.max())
	  break;
	++iz;
      }
      //      std::cout << "mydfaint done searching" << std::endl;

      if(newz.size()==0 || newpx.size()==0 || newqx.size()==0 || newpy.size()==0 || newqy.size()==0) {
	return Gecode::ES_FAILED;
      }
      
      //      std::cout << " mydfaint not failed " << std::endl;
#define _printvec(vec) { \
      std::cout << #vec << ": "; \
      for (std::vector<int>::const_iterator i = vec.begin(); i != vec.end(); ++i) \
	std::cout << *i << ' '; \
      std::cout << std::endl; \
      }
      //      _printvec(newpx)
      //	_printvec(newpy)
      //	_printvec(newqx)
      //	_printvec(newqy)
      //	_printvec(newz)
#define _prune(var, newvar) {					\
	Gecode::Int::ViewValues<Gecode::Int::IntView> iter(var);	\
	iter.init(var);						\
	while(iter()) {						\
	  int v = iter.val();					       \
	  if(std::find(newvar.begin(), newvar.end(), v) == newvar.end()) \
	    if(var.nq(home, v) == Gecode::Int::ME_INT_FAILED)			\
	      return Gecode::ES_FAILED;						\
	  if(v == var.max() || var.assigned()) \
	    break;							\
	  ++iter;							\
	}								\
	} 
      
            _prune(Z, newz)
	
		_prune(Px, newpx)
		_prune(Py, newpy)
		_prune(Qx, newqx)
		_prune(Qy, newqy)
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
        return new (home) MyDFAint(home, share, *this);
    }

    virtual Gecode::PropCost cost(const Gecode::Space&, const Gecode::ModEventDelta&) const {
        // Probably way wrong, but not very important. TODO
        return Gecode::PropCost::linear(Gecode::PropCost::HI, Px.size());
    }

};

//
void myintdfa(Gecode::Space& home, Gecode::IntVar Px, Gecode::IntVar Py, Gecode::IntVar Qx, Gecode::IntVar Qy, Gecode::IntVar Z, MPG::IntPair::DFA_I *D) {
    std::cout << "Init DFAint prop" << std::endl;
    Gecode::Int::IntView vpx(Px);
    Gecode::Int::IntView vqx(Qx);
    Gecode::Int::IntView vpy(Py);
    Gecode::Int::IntView vqy(Qy);
    Gecode::Int::IntView vz(Z);
    if (MyDFAint::post(home, vpx, vpy, vqx, vqy, vz, D) != Gecode::ES_OK)
        home.fail();
}



#endif // DFAINT_H
