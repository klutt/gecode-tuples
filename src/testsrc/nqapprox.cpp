#include "../intpairapprox.h"

#include "_testbase.cpp"

int noSolutions;


#include <iostream>
std::ostringstream res;

using namespace MPG::IntPair;
using namespace MPG;

class Test : public Script {
public:
  /// The actual problem
  IntPairApproxVarArray a;
  Test(const SizeOptions& opt) : a(*this, 1, 1,2,5,5)
  {
    neq(*this, a[0], Pair(1,5));
  }

  
  /// Constructor for cloning \a s
#ifndef GC_UPDATE
#define GC_UPDATE(var) var.update(*this, share, s.var)
#endif
  
  Test(bool share, Test& s) : Script(share,s) {
    // To update a variable var use:
    // GC_UPDATE(var)
      GC_UPDATE(a);
  }
    
  /// Perform copying during cloning
  virtual Space*
  copy(bool share) {
        return new Test(share,*this);
  }

  /// Print solution (originally, now it's just for updating number of solutions)
  virtual void print(std::ostream& os) const {
    // Strange place to put this, but since this functions is called once for every solution ...
    res << a[0];
    noSolutions++;
  }
};

int main(int argc, char* argv[]) {
    SizeOptions opt("Queens");
    opt.solutions(0); // Calculate all solutions
    noSolutions=0;
    
    const int expected_no_solutions = 1;
    std::string expected_answer="<2,5>";
    opt.parse(argc,argv);
    ScriptOutput::run<Test,DFS,SizeOptions>(opt);
    std::string str=res.str();
    // cout << "No solutions: " << noSolutions << endl;
    assert(str.compare(expected_answer) == 0);
    
    assert (expected_no_solutions == noSolutions);

    cout << "  Ok" << endl;
    return 0;
}

// STATISTICS: example-any

