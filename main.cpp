#include "./lib/graph.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    cout << "Usage: ./main <filename>" << endl;
    return 1;
  }

  string filename = argv[1];

  Graph g(filename);
  return 0;
}
