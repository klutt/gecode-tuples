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
  int S(int s, int t) { if(s>3 || t>3) return 0; return state[s][t]; }

  int C(int s, int t) { return cost[s][t]; }
};


class Test : public Script {
public:
  /// The actual problem
  IntVar px, py, qx, qy, z;
  Dfa_t *df;

  
  Test(const SizeOptions& opt) : px(*this, 1,3),
				 qx(*this, 1,3),
				 py(*this, 1,4),
				 qy(*this, 1,4),
				 z(*this, 1,3)
  {
    df = new Dfa_t();
    myintdfa(*this, px, py, qx, qy, z,df);

    branch(*this, px, INT_VAL_MIN());
    branch(*this, qx, INT_VAL_MIN());
    branch(*this, py, INT_VAL_MIN());
    branch(*this, qy, INT_VAL_MIN());
    branch(*this, z, INT_VAL_MIN());

  }

  
  /// Constructor for cloning \a s
#ifndef GC_UPDATE
#define GC_UPDATE(var) var.update(*this, share, s.var)
#endif
  
  Test(bool share, Test& s) : Script(share,s) {
    // To update a variable var use:
    // GC_UPDATE(var)
    GC_UPDATE(z);
    GC_UPDATE(px);
    GC_UPDATE(qx);
    GC_UPDATE(py);
    GC_UPDATE(qy);
  }
    
  /// Perform copying during cloning
  virtual Space*
  copy(bool share) {
        return new Test(share,*this);
  }

  /// Print solution (originally, now it's just for updating number of solutions)
  virtual void print(std::ostream& os) const {
    // Strange place to put this, but since this functions is called once for every solution ...
    
    //    cout << " Solution  "<< " px: " << px << " py: " << py
    //	 << " qx: " << qx << " qy: " << qy << " z: " << z << endl;
    noSolutions++;
  }
};

int main(int argc, char* argv[]) {
    SizeOptions opt("Queens");
    opt.solutions(0); // Calculate all solutions
    noSolutions=0;

    
    const int expected_no_solutions = 18;
    
    opt.parse(argc,argv);
    ScriptOutput::run<Test,DFS,SizeOptions>(opt);

    cout << "No solutions: " << noSolutions << endl;
    assert (expected_no_solutions == noSolutions);

    cout << "  Ok" << endl;
    return 0;
}

// STATISTICS: example-any

