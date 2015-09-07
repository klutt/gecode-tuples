#include "_testbase.cpp"

int noSolutions;

class Test : public Script {
public:
  /// The actual problem
  IntVar a,b;
  Test(const SizeOptions& opt) : a(*this, 0,1), b(*this, 2,3)
  {
    rel(*this, a, IRT_EQ, b);
  }
    
  /// Constructor for cloning \a s
#ifndef GC_UPDATE
#define GC_UPDATE(var) var.update(*this, share, s.var)
#endif
  Test(bool share, Test& s) : Script(share,s) {
    // To update a variable var use:
    // var.update(*this, share, s.var);
    GC_UPDATE(a);
    GC_UPDATE(b);
  }
    
  /// Perform copying during cloning
  virtual Space*
  copy(bool share) {
        return new Test(share,*this);
  }

  /// Print solution (originally, now it's just for updating number of solutions)
  virtual void print(std::ostream& os) const {
    
    // Strange place to put this, but since this functions is called once for every solution ...
    //    cout << "kuken " << solution();
    noSolutions++;
  }
};

int main(int argc, char* argv[]) {
    SizeOptions opt("Queens");
    opt.solutions(0); // Calculate all solutions
    noSolutions=0;
    
    const int expected_no_solutions = 0;
    
    opt.parse(argc,argv);
    ScriptOutput::run<Test,DFS,SizeOptions>(opt);
    assert (expected_no_solutions == noSolutions);

    cout << "  Ok" << endl;
    return 0;
}

// STATISTICS: example-any

