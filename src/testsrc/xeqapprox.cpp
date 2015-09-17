#include "../intpairapprox.h"

#include "_testbase.cpp"

#include "../propagators/xeqapprox.h"
#include <iostream>


std::ostringstream res;

int noSolutions;

using namespace MPG::IntPair;
using namespace MPG;

class Test : public Script {
public:
  /// The actual problem
  IntPairApproxVar p;
  Test(const SizeOptions& opt) : p(*this, 1,2,4,6)
  {
    xeq(*this, p, PairApprox(2,3,4));
    xeq(*this, p, PairApprox(1,8,10));
  }

  
  /// Constructor for cloning \a s
#ifndef GC_UPDATE
#define GC_UPDATE(var) var.update(*this, share, s.var)
#endif
  
  Test(bool share, Test& s) : Script(share,s) {
    // To update a variable var use:
    // GC_UPDATE(var)
    GC_UPDATE(p);

  }
    
  /// Perform copying during cloning
  virtual Space*
  copy(bool share) {
        return new Test(share,*this);
  }

  /// Print solution (originally, now it's just for updating number of solutions)
  virtual void print(std::ostream& os) const {
    // Strange place to put this, but since this functions is called once for every solution ...
    res << p;
    noSolutions++;
  }
};

int main(int argc, char* argv[]) {
    SizeOptions opt("Queens");
    opt.solutions(0); // Calculate all solutions
    noSolutions=0;
    
    const int expected_no_solutions = 1;
    std::string expected_answer = "<2,4>";

    opt.parse(argc,argv);
    ScriptOutput::run<Test,DFS,SizeOptions>(opt);

    // cout << "No solutions: " << noSolutions << endl;
    std::string str = res.str();
    //    std::cout << str.size() << "   " << expected_answer.size() << std::endl;
    assert(str.compare(expected_answer) == 0);
    assert (expected_no_solutions == noSolutions);

    cout << "  Ok" << endl;
    return 0;
}

// STATISTICS: example-any

