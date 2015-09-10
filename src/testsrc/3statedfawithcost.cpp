#include "../intpair.h"
#include <vector>

#include "_testbase.cpp"

int noSolutions;

using namespace MPG::IntPair;

typedef vector<vector<int> > matrix;

class Dfa_t : public DFA_I{
  // Remember: 0 is garbage state
private:
  matrix state;
  matrix cost;
  
public:
  Dfa_t() {
    state = matrix{{0,0,0,0},
		   {0,2,3,3},
		   {0,1,0,3},
		   {0,0,2,0}
    };
    cost  = matrix{{0,0,0,0},
		   {0,1,1,1},
		   {0,1,0,1},
		   {0,0,1,0}
    };
  }
  int S(int s, int t) { return state[s][t]; }
  int C(int s, int t) { return cost[s][t]; }
};


class Test : public Script {
public:
  /// The actual problem
  IntPairVarArray a;
  IntVar z;
  Dfa_t *df;

  
  Test(const SizeOptions& opt) : a(*this, 2,1,3,1,2), z(*this, 1,3)
  {
    df = new Dfa_t();
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
    os << "a[0]: " << a[0] << "   a[1]: " << a[1] <<  "   z: " << z << endl;
    noSolutions++;
  }
};

int main(int argc, char* argv[]) {
    SizeOptions opt("Queens");
    opt.solutions(0); // Calculate all solutions
    noSolutions=0;

    
    const int expected_no_solutions = 6;
    
    opt.parse(argc,argv);
    ScriptOutput::run<Test,DFS,SizeOptions>(opt);

    cout << "No solutions: " << noSolutions << endl;
    assert (expected_no_solutions == noSolutions);

    cout << "  Ok" << endl;
    return 0;
}

// STATISTICS: example-any

