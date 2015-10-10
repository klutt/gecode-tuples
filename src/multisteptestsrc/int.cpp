#include <gecode/kernel.hh>
#include <vector>
#include <gecode/gist.hh>
#include "../propagators/dfaint.h"
#include "../testsrc/_testbase.cpp"
#include "../common/solutionok.h"

int noSolutions;

using namespace MPG::IntPair;
using namespace MPG;

#include "../common/dfaimp.h"

Dfa *df;


int seed, nostates, notokens, maxcost, maxcosttotal, nosteps;


class Test : public Script {
public:
  /// The actual problem
  IntVarArray sv;
  IntVarArray cv;
  IntVarArray zv;

  
  Test(const SizeOptions& opt) : sv(*this, nosteps+1,1,nostates),
				 cv(*this ,nosteps+1,0, maxcosttotal),
				 zv(*this, nosteps,1,notokens)
  {
    rel(*this, sv[0], IRT_EQ, 1); // Starting state
    rel(*this, cv[0], IRT_EQ, 0); // Cost = 0 at start
    for(int i=0; i<nosteps; i++) 
      myintdfa(*this, sv[i+1] ,cv[i+1], sv[i], cv[i] ,zv[i],df);
    branch(*this, sv, INT_VAR_NONE(), INT_VAL_MIN());
    branch(*this, cv, INT_VAR_NONE(), INT_VAL_MIN());
    branch(*this, zv, INT_VAR_NONE(), INT_VAL_MIN());
  }

  
  /// Constructor for cloning \a s
#ifndef GC_UPDATE
#define GC_UPDATE(var) var.update(*this, share, s.var)
#endif
  
  Test(bool share, Test& s) : Script(share,s) {
    // To update a variable var use:
    // GC_UPDATE(var)
    GC_UPDATE(sv);
    GC_UPDATE(cv);
    GC_UPDATE(zv);
  }
    
  /// Perform copying during cloning
  virtual Space*
  copy(bool share) {
        return new Test(share,*this);
  }

  /// Print solution (originally, now it's just for updating number of solutions)
  virtual void print(std::ostream& os) const {
    // Strange place to put this, but since this functions is called once for every solution ...
    //    assert(solutionOk(df, a.val(), b.val(), c.val(), d.val(), z.val()));
    //    cout << "<" << c << "," << d << "> <"  << a << "," << b << "> " << z << endl;
    for(int i=0; i<nosteps; i++)
      assert(solutionOk(df, sv[i+1].val(), cv[i+1].val(), sv[i].val(), cv[i].val(), zv[i].val()));
    for(int i=0; i<nosteps; i++)
      cout << zv[i].val() << " " << sv[i].val() << " " << cv[i].val() << " ";
    cout << sv[nosteps].val() << " " << cv[nosteps].val() << endl;
    
    noSolutions++;
  }
};

#include "_main.cpp"

