#include "../intpairapprox.h"

#include "_testbase.cpp"

int noSolutions;

using namespace MPG::IntPair;

class Test : public Script {
public:
  /// The actual problem
  IntPairApproxVarArray a;
  Test(const SizeOptions& opt) : a(*this, 2, 3,10, 4,4)
  {
    eq(*this, a[0], a[1]);
    nonenone(*this, a);
  }

  
  /// Constructor for cloning \a s
#ifndef GC_UPDATE
#define GC_UPDATE(var) var.update(*this, share, s.var)
#endif
  
  Test(bool share, Test& s) : Script(share,s) {
    // To update a variable var use:
    // GC_UPDATE(var)

  }
    
  /// Perform copying during cloning
  virtual Space*
  copy(bool share) {
        return new Test(share,*this);
  }

  /// Print solution (originally, now it's just for updating number of solutions)
  virtual void print(std::ostream& os) const {
    // Strange place to put this, but since this functions is called once for every solution ...
    noSolutions++;
  }
};

int main(int argc, char* argv[]) {
    SizeOptions opt("Queens");
    opt.solutions(0); // Calculate all solutions
    noSolutions=0;
    
    const int expected_no_solutions = 8;
    
    opt.parse(argc,argv);
    ScriptOutput::run<Test,DFS,SizeOptions>(opt);

    // cout << "No solutions: " << noSolutions << endl;
    assert (expected_no_solutions == noSolutions);

    cout << "  Ok" << endl;
    return 0;
}

// STATISTICS: example-any

