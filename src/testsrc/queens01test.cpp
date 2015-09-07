#include "_testbase.cpp"

int noSolutions;

class Test : public Script {
public:
    /// Position of queens on boards
	BoolVarArray b;
	int size; //used for printing tables

    /// The actual problem
    Test(const SizeOptions& opt) : b(*this, opt.size()*opt.size(), 0, 1)
	{
        const int n = opt.size();
		size = n;
		
		for (int i = 0; i < n; i++) {
			linear(*this, b.slice(i*n, 1, n), IRT_EQ, 1); //Rows
			linear(*this, b.slice(i, n, n), IRT_EQ, 1); //Columns
		}
		
		linear(*this, b.slice(0, n+1, n), IRT_LQ, 1); //Middle descend. diag.
		linear(*this, b.slice(n-1, n-1, n), IRT_LQ, 1); //Middle asscend. diag.	

		for (int i = 1; i < n-1; i++) {
			linear(*this, b.slice(i, n+1, n-i), IRT_LQ, 1); //Upper descend. diag.
			linear(*this, b.slice(n*i, n+1, n-i), IRT_LQ, 1); //Lower descend. diag.
			linear(*this, b.slice(n-(i+1), n-1, n-i), IRT_LQ, 1); //Upper ascend. diag.
			linear(*this, b.slice(n*(i+1)-1, n-1, n-i), IRT_LQ, 1); //Lower ascend. diag.
		}

		branch(*this, b, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    }
    
  
  /// Constructor for cloning \a s
#ifndef GC_UPDATE
#define GC_UPDATE(var) var.update(*this, share, s.var)
#endif
    Test(bool share, Test& s) : Script(share,s), size(s.size) {
      GC_UPDATE(b);
    }
    
    /// Perform copying during cloning
    virtual Space*
    copy(bool share) {
        return new Test(share,*this);
    }
    
    /// Print solution
    virtual void print(std::ostream& os) const {
      // Strange place to put this, but since this functions is called once for every solution ...
      noSolutions++;
    }
};

int main(int argc, char* argv[]) {
    SizeOptions opt("Test");
    //    opt.iterations(500);
    opt.size(8);
    opt.solutions(0);
    // comment out the following line to get a graphical view of the search tree
	//opt.mode(Gecode::SM_GIST);
    noSolutions=0;
    const int expected_no_solutions=92;
    
    opt.parse(argc,argv);
    ScriptOutput::run<Test,DFS,SizeOptions>(opt);

    assert (expected_no_solutions == noSolutions);

    cout << "  Ok" << endl;
    return 0;
}

// STATISTICS: example-any

