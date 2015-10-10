

int main(int argc, char* argv[]) {
  SizeOptions opt("Queens");
  opt.solutions(0); // Calculate all solutions
  noSolutions=0;

  if(argc < 9) {
    std::cout << "Usage: " << argv[0] << " <seed> <no states> <no tokens> <max cost per path> <max total cost> <nosteps>" << std::endl;
    return -1;
  }

  const int offset = 2;
  seed = atoi(argv[1+offset]);
  nostates = atoi(argv[2+offset]);
  notokens = atoi(argv[3+offset]);
  maxcost = atoi(argv[4+offset]);
  maxcosttotal = atoi(argv[5+offset]);
  nosteps = atoi(argv[6+offset]);
  if(seed == 0 || nostates == 0 || notokens == 0 || maxcost == 0 || maxcosttotal==0 || nosteps == 0) {
    std::cout << "Wrong parameters" << std::endl;
    return -1;
  }

  df = new Dfa(seed, nostates, notokens, maxcost);
  //    df->print();
  opt.mode(Gecode::SM_STAT);
  opt.parse(argc,argv);
  ScriptOutput::run<Test,DFS,SizeOptions>(opt);
  /*
  Test *T = new Test(opt);
  Gist::Options o;
  Gist::Print<Test> p("print");
  o.inspect.click(&p);
  Gist::dfs(T,o); */

  cout << "No solutions: " << noSolutions << endl;
  return 0;
}
