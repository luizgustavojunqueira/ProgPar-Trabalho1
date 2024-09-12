#include "./lib/graph.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    cout << "Usage: ./main <filename>" << endl;
    return 1;
  }

  int debug = 0;

  if (argc == 3 && (string(argv[2]) == "-d" || string(argv[2]) == "--debug")) {
    cout << "Debug mode enabled." << endl;
    debug = 1;
  }

  string filename = argv[1];

  Graph g(filename, debug);

  if (debug) {
    cout << "Graph created." << endl;
    g.print();
  }
  return 0;
}
