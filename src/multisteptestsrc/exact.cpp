#include <gecode/kernel.hh>
#include "../intpair.h"
#include <vector>
#include <gecode/gist.hh>

#include <stdlib.h>
#include "../common/solutionok.h"

#include "../testsrc/_testbase.cpp"

int noSolutions;

using namespace MPG::IntPair;
using namespace MPG;

#include "../common/dfaimp.h"

Dfa *df;

int seed, nostates, notokens, maxcost, maxcosttotal, nosteps;

class Test : public Script {
public:
  /// The actual problem
  IntPairVarArray p;
  IntVarArray z;
  IntPairVar init;
  
  Test(const SizeOptions& opt) : p(*this, nosteps+1,1,nostates,0,maxcosttotal),
				 z(*this, nosteps,1,notokens),
				 init(*this, 1,1,0,0)
  {
    eq(*this, p[0], init);
    for(int i=0; i<nosteps; i++)
      mydfa(*this, p[i+1],p[i],z[i],df);
    nonenone(*this, p);
    branch(*this, z, INT_VAR_NONE(), INT_VAL_MIN());
  }

  
  /// Constructor for cloning \a s
#ifndef GC_UPDATE
#define GC_UPDATE(var) var.update(*this, share, s.var)
#endif
  
  Test(bool share, Test& s) : Script(share,s) {
    // To update a variable var use:
    // GC_UPDATE(var)
    GC_UPDATE(p);
    GC_UPDATE(z);
    GC_UPDATE(init);
  }
    
  /// Perform copying during cloning
  virtual Space*
  copy(bool share) {
        return new Test(share,*this);
  }

  /// Print solution (originally, now it's just for updating number of solutions)
  virtual void print(std::ostream& os) const {
    // Strange place to put this, but since this functions is called once for every solution ...
    //    assert(solutionOk(df, a[0].val().x, a[0].val().y, a[1].val().x, a[1].val().y, z.val()));
    //  cout << a[1] << " "  << a[0] << " " << z << endl;
    for(int i=0; i<nosteps; i++)
      assert(solutionOk(df, p[i+1].val().x, p[i+1].val().y, p[i].val().x, p[i].val().y, z[i].val()));
    for(int i=0; i<nosteps; i++)
      cout << z[i].val() << " " << p[i].val().x << " " << p[i].val().y << " ";
    cout << p[nosteps].val().x << " " << p[nosteps].val().y << endl;
    
    noSolutions++;
  }
};


#include "_main.cpp"
