#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

#include <stdio.h>
#include <math.h>
#include <cstdio>
#include "intpairvar.hh"
#include "propagators.hh"

using namespace std;
using namespace Gecode;
using namespace Gecode::Driver;

const int TeamNr = 7;

int N = 9;
int n;

class Squarepack;

template<class BaseSpace>
class ScriptBaseCustom : public BaseSpace {
public:
    /// Default constructor
    ScriptBaseCustom(void) {}
    /// Constructor used for cloning
    ScriptBaseCustom(bool share, ScriptBaseCustom& e) : BaseSpace(share,e) {}
    /// Print a solution to \a os
    virtual void print(std::ostream& os) const { (void) os; }
    /// Compare with \a s
    virtual void compare(const Space&, std::ostream& os) const {
        (void) os;
    }
    /// Choose output stream according to \a name
    static std::ostream& select_ostream(const char* name, std::ofstream& ofs);
    
    template<class Script, template<class> class Engine, class Options>
    static void run(const Options& opt, Script* s=NULL);
private:
    template<class Script, template<class> class Engine, class Options,
    template<template<class> class,class> class Meta>
    static void runMeta(const Options& opt, Script* s);
    /// Catch wrong definitions of copy constructor
    explicit ScriptBaseCustom(ScriptBaseCustom& e);
};

typedef ScriptBaseCustom<Space> ScriptOutput;

template<class Space>
std::ostream&
ScriptBaseCustom<Space>::select_ostream(const char* name, std::ofstream& ofs) {
    if (strcmp(name, "stdout") == 0) {
        return std::cout;
    } else if (strcmp(name, "stdlog") == 0) {
        return std::clog;
    } else if (strcmp(name, "stderr") == 0) {
        return std::cerr;
    } else {
        ofs.open(name);
        return ofs;
    }
}

template<class Space>
template<class Script, template<class> class Engine, class Options>
void
ScriptBaseCustom<Space>::run(const Options& o, Script* s) {
    if (o.restart()==RM_NONE) {
        runMeta<Script,Engine,Options,EngineToMeta>(o,s);
    } else {
        runMeta<Script,Engine,Options,RBS>(o,s);
    }
}

template<class Space>
template<class Script, template<class> class Engine, class Options,
template<template<class> class,class> class Meta>
void
ScriptBaseCustom<Space>::runMeta(const Options& o, Script* s) {
    using namespace std;
    
    ofstream sol_file, log_file;
    
    ostream& s_out = select_ostream(o.out_file(), sol_file);
    ostream& l_out = select_ostream(o.log_file(), log_file);
    
    try {
        switch (o.mode()) {
            case SM_SOLUTION:
            {
//                l_out << o.name() << endl;
                Support::Timer t;
                int i = o.solutions();
                t.start();
                if (s == NULL)
                    s = new Script(o);
//                unsigned int n_p = s->propagators();
//                unsigned int n_b = s->branchers();
                Search::Options so;
                so.threads = o.threads();
                so.c_d     = o.c_d();
                so.a_d     = o.a_d();
                so.stop    = CombinedStop::create(o.node(),o.fail(), o.time(),
                                                  o.interrupt());
                so.cutoff  = createCutoff(o);
                so.clone   = false;
                
				if(o.interrupt()) CombinedStop::installCtrlHandler(true);
                
				{
                    Meta<Engine,Script> e(s,so);
                    if (o.print_last()) {
                        Script* px = NULL;
                        do {
                            Script* ex = e.next();
                            if (ex == NULL) {
                                if (px != NULL) {
                                    px->print(s_out);
                                    delete px;
                                }
                                break;
                            } else {
                                delete px;
                                px = ex;
                            }
                        } while (--i != 0);
                    } else {
                        do {
                            Script* ex = e.next();
                            if (ex == NULL)
                                break;
                            ex->print(s_out);
                            delete ex;
                        } while (--i != 0);
                    }
                    if (o.interrupt())
                        CombinedStop::installCtrlHandler(false);
                    Search::Statistics stat = e.statistics();

	                   //                    s_out << endl;
                    if (e.stopped()) {
                        //                        l_out << "Search engine stopped..." << endl
                        //                        << "\treason: ";
                        int r = static_cast<CombinedStop*>(so.stop)->reason(stat,so);
                        if (r & CombinedStop::SR_INT)
                            l_out << "user interrupt " << endl;
                        else {
                            if (r & CombinedStop::SR_NODE)
                                l_out << "node ";
                            if (r & CombinedStop::SR_FAIL)
                                l_out << "fail ";
                            if (r & CombinedStop::SR_TIME)
                                l_out << "time ";
                            //                            l_out << "limit reached" << endl << endl;
                            l_out << "$>\\Timeout$";
                        }
                    }
                    else {
                        double runtime_msec = t.stop();
                        double runtime_sec = runtime_msec/1000;
                        //l_out.width(8);
//                    cout.fill(' ');
                        s_out << "runtime: " << showpoint << fixed  << setprecision(3) << runtime_sec << endl;
			//                        s_out << showpoint << fixed  << setprecision(3) << runtime_sec << " & ";
                        //                    cout << " (";
                        //                    cout.width(9);
                        //                    cout.fill('0');
                        //                    cout << runtime_msec << " ms)";
                    }
		                        s_out << "failure: " << stat.fail << endl;
		    //                    s_out << stat.fail << " \\\\ " << endl;
                }
                delete so.stop;
            }
                break;
            default:
                Script::template run<Squarepack,DFS,Options>(o);
        }
    } catch (Exception& e) {
        cerr << "Exception: " << e.what() << "." << endl
        << "Stopping..." << endl;
        if (sol_file.is_open())
            sol_file.close();
        if (log_file.is_open())
            log_file.close();
        exit(EXIT_FAILURE);
    }
    if (sol_file.is_open())
        sol_file.close();
    if (log_file.is_open())
        log_file.close();
}

// using namespace MPG::IntPair;
using namespace MPG;

class Squarepack : public Script {
public:

  IntPairVar p;

  Squarepack(const SizeOptions& opt) :
    p(*this, 0, 0, N, N)
  {

    // branch(*this, p, INTPAIR_VAL_MIN());
    // branch(*this, p, INT_VAR_NONE(), INT_VAL_MIN());
  }

  Squarepack(bool share, Squarepack& sh) : Script(share, sh) {
    p.update(*this, share, sh.p);
  }

	virtual Space* copy(bool share) {
		return new Squarepack(share, *this);
	}

	virtual void print(std::ostream& os) const {
	  os << "n: " << N << endl;

		os << "p: " << p << endl; 
		  //os << N << " & ";

	}

	static int size(int index) {
		return N - index;
	}

	static int index(int size) {
		return N - size;
	}
};

int main(int argc, char* argv[]) {

	SizeOptions opt("Squarepack");
	
	// comment out the following line to get a graphical view of the search tree
	//opt.mode(Gecode::SM_GIST);

	if(argc < 2) return 1;
	N = atoi(argv[1]); 
	/*	if(N < 2) {
		cout << "The solver does not accept n < 2." << endl;
		return 0;
		}*/
	n = N-1;
	
	//	std::stringstream filename;
	//	filename << "project1-t" << TeamNr << "-n" << N << ".txt";
	//	freopen(filename.str().c_str(),"w",stdout);
	
	//cout << "enter n: " << endl;
	//cin >> N;

	opt.parse(argc,argv);
	ScriptOutput::run<Squarepack,DFS,SizeOptions>(opt);
	return 0;
}
