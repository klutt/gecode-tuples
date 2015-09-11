#ifndef DFAINT_H
#define DFAINT_H


#include <vector>

#include "dfainterface.h"

// This is the IntVar version of dfa propagator. It is used for performance comparision.


class MyDFAint : public Propagator {
protected:
    Int::IntView Px;
    Int::IntView Qx;
    Int::IntView Py;
    Int::IntView Qy;
    Int::IntView Z;
    MPG::IntPair::DFA_I *D;
    
 public:
    MyDFAint(Space& home, Int::IntView px, Int::IntView py, Int::IntView qx, Int::IntView qy, Int::IntView z, DFA_I *d)
      : Propagator(home), Px(px), Py(py), Qx(qx), Qy(qy), Z(z), D(d)
    {
        Px.subscribe(home, *this, Int::PC_INT_DOM);
        Qx.subscribe(home, *this, Int::PC_INT_DOM);
        Py.subscribe(home, *this, Int::PC_INT_DOM);
        Qy.subscribe(home, *this, Int::PC_INT_DOM);
        Z.subscribe(home, *this, Int::PC_INT_DOM);
    }

    MyDFAint(Space& home, bool share, MyDFAint& prop)
      : Propagator(home, share, prop), D(prop.D) {
        Px.update(home, share, prop.Px);
        Qx.update(home, share, prop.Qx);
        Py.update(home, share, prop.Py);
        Qy.update(home, share, prop.Qy);
        Z.update(home, share, prop.Z);
    }

    static ExecStatus post(Space& home, Int::IntView px, Int::IntView py, Int::IntView qx, Int::IntView qy, Int::IntView z, DFA_I *d) {
        (void) new (home) MyDFAint(home, px, py, qx, qy, z, d);
        return ES_OK;
    }

    virtual ExecStatus propagate(Space& home, const ModEventDelta&) {
            std::vector<int> newpx, newpy, newqx, newqy, newz;
            Gecode::Int::ViewValues<Int::IntView> iz(Z);
      
      while(iz()) {
            Gecode::Int::ViewValues<Int::IntView> iqx(Qx);
	    //	std::cout << " z " << iz.val() << " zmax: " << Z.max() << std::endl;
	while(iqx()) {
	  //	  std::cout << " qx " << iqx.val() << " Qxmax " << Qx.max() << std::endl;
	  	  int state = D->S(iqx.val(), iz.val());
	  //	  std::cout << " state: " << state << std::endl;
	  if(state>0 && Px.in(state)) {
	    newqx.push_back(iqx.val());
	    newpx.push_back(state);
	    newz.push_back(iz.val());
	    Gecode::Int::ViewValues<Int::IntView> iqy(Qy);
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
	return ES_FAILED;
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
	Gecode::Int::ViewValues<Int::IntView> iter(var);	\
	iter.init(var);						\
	while(iter()) {						\
	  int v = iter.val();					       \
	  if(std::find(newvar.begin(), newvar.end(), v) == newvar.end()) \
	    if(var.nq(home, v) == Int::ME_INT_FAILED)			\
	      return ES_FAILED;						\
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
      return ES_NOFIX;
      
      
    }

    virtual size_t dispose(Space& home) {
        Px.cancel(home, *this, Int::PC_INT_DOM);
        Qx.cancel(home, *this, Int::PC_INT_DOM);
        Py.cancel(home, *this, Int::PC_INT_DOM);
        Qy.cancel(home, *this, Int::PC_INT_DOM);
        Z.cancel(home, *this, Int::PC_INT_DOM);
        (void) Propagator::dispose(home);
        return sizeof(*this);
    }

    virtual Propagator* copy(Space& home, bool share) {
        return new (home) MyDFAint(home, share, *this);
    }

    virtual PropCost cost(const Space&, const ModEventDelta&) const {
        // Probably way wrong, but not very important. TODO
        return PropCost::linear(PropCost::HI, Px.size());
    }

};

//
void myintdfa(Space& home, IntVar Px, IntVar Py, IntVar Qx, IntVar Qy, IntVar Z, DFA_I *D) {
    std::cout << "Init DFAint prop" << std::endl;
    Int::IntView vpx(Px);
    Int::IntView vqx(Qx);
    Int::IntView vpy(Py);
    Int::IntView vqy(Qy);
    Int::IntView vz(Z);
    if (MyDFAint::post(home, vpx, vpy, vqx, vqy, vz, D) != ES_OK)
        home.fail();
}



#endif // DFAINT_H
