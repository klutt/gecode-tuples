#include <gecode/kernel.hh>
#include "../intpairapprox.h"
#include <vector>
#include <stdlib.h>
#include "../common/solutionok.h"

#include "../testsrc/_testbase.cpp"

int noSolutions;

using namespace MPG::IntPair;
using namespace MPG;

#include "dfaimp.h"

Dfa *df;

int seed, nostates, notokens, maxcost, maxcosttotal;

class Test : public Script {
public:
  /// The actual problem
  IntPairApproxVarArray a;
  IntVar z;

  
  Test(const SizeOptions& opt) : a(*this, 2,1,nostates,0,maxcosttotal), z(*this, 1,notokens)
  {
    mydfa(*this, a[0],a[1],z,df);
    nonenone(*this, a);
    branch(*this, z, INT_VAL_MIN());
  }

  
  /// Constructor for cloning \a s
#ifndef GC_UPDATE
#define GC_UPDATE(var) var.update(*this, share, s.var)
#endif
  
  Test(bool share, Test& s) : Script(share,s) {
    // To update a variable var use:
    // GC_UPDATE(var)
    GC_UPDATE(a);
    GC_UPDATE(z);
  }
    
  /// Perform copying during cloning
  virtual Space*
  copy(bool share) {
        return new Test(share,*this);
  }

  /// Print solution (originally, now it's just for updating number of solutions)
  virtual void print(std::ostream& os) const {
    // Strange place to put this, but since this functions is called once for every solution ...
    assert(solutionOk(df, a[0].val().x, a[0].val().y, a[1].val().x, a[1].val().y, z.val()));
    cout << a[1] << " "  << a[0] << " " << z << endl;
    noSolutions++;
  }
};

int main(int argc, char* argv[]) {
    SizeOptions opt("Queens");
    opt.solutions(0); // Calculate all solutions
    noSolutions=0;

    if(argc != 6) {
      std::cout << "Usage: " << argv[0] << " <seed> <no states> <no tokens> <max cost per path> <max total cost>" << std::endl;
      return -1;
    }

    seed = atoi(argv[1]);
    nostates = atoi(argv[2]);
    notokens = atoi(argv[3]);
    maxcost = atoi(argv[4]);
    maxcosttotal = atoi(argv[5]);
    
    if(seed == 0 || nostates == 0 || notokens == 0 || maxcost == 0 || maxcosttotal==0) {
      std::cout << "Wrong parameters" << std::endl;
      return -1;
    }
      
    df = new Dfa(seed, nostates, notokens, maxcost);
    df->print();
    
    //    opt.parse(argc,argv);
    ScriptOutput::run<Test,DFS,SizeOptions>(opt);

    cout << "No solutions: " << noSolutions << endl;
    return 0;
}

// STATISTICS: example-any

