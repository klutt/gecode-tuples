#ifndef __MATRIX_H
#define __MATRIX_H

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

typedef std::vector<std::vector<int> > matrix;

matrix readfile(std::string filename) {
  std::ifstream matfile(filename);
  int x, y;
  matrix ret;
  matfile >> x >> y;
  std::cout << x << " " << y << std::endl;
  for(int i=0; i<y; i++) {
    std::vector<int> row;
    for(int j=0; j<x; j++) {
      int cell;
      matfile >> cell;
      row.push_back(cell);
    }
    ret.push_back(row);
  }
  return ret;
}

#endif
