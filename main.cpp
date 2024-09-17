#include "./lib/graph.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

  if (argc < 3) {
    cout << "Usage: ./main <filename> <k>" << endl;
    return 1;
  }

  int debug = 0;

  if (argc == 4 && (string(argv[3]) == "-d" || string(argv[3]) == "--debug")) {
    cout << "Debug mode enabled." << endl;
    debug = 1;
  }

  string filename = argv[1];
  int k = stoi(argv[2]);

  Graph g(filename, debug);

  cout << g.num_vertices << endl;

  cout << g.countCliquesSerial(k) << endl;

  return 0;
}
