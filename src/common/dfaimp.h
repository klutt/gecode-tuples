#ifndef DFAIMP_H
#define DFAIMP_H

#include "../propagators/dfainterface.h"
#include <fstream>
#include <iostream>

typedef std::vector<std::vector<int> > matrix;

class Dfa : public MPG::IntPair::DFA_I {
 private:
  matrix state, cost;
  unsigned int seed;
  
 public:
  unsigned int rnd(unsigned int r) { seed = (1103515245 * seed + 12345) % 4294967296; return seed % r; };
  int S(int s, int t) { return state[s-1][t-1]; };
  int C(int s, int t) { return cost[s-1][t-1]; };

 Dfa(int seed, int nostates, int notokens, int maxcost) : seed(seed)
  {
    for(int i=0; i<nostates; i++) {
      std::vector<int> row;
      for(int j=0; j<notokens; j++) 
	row.push_back(rnd(nostates));
      state.push_back(row);
    }
    for(int i=0; i<nostates; i++) {
      std::vector<int> row;
      for(int j=0; j<notokens; j++) 
	row.push_back(rnd(maxcost));
      cost.push_back(row);
    }
  };

  void print() {
    for(int i=0; i<state.size(); i++) {
      for(int j=0; j<state[i].size(); j++)
	std::cout << state[i][j] << " ";
      std::cout << std::endl;
    }
    std::cout << std::endl;
    for(int i=0; i<cost.size(); i++) {
      for(int j=0; j<cost[i].size(); j++)
	std::cout << cost[i][j] << " ";
      std::cout << std::endl;
    }
  }    
};

#endif
