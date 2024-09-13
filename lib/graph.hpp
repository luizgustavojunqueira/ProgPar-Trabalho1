#include <string>
#include <vector>

using namespace std;

class Graph {
public:
  Graph(string filename, int debug);
  int countCliquesSerial(int k);
  void print();
  int num_vertices;

private:
  vector<vector<int>> adj_list;
  void add_edge(int v, int w, int num_vertices);
  int isNeighbor(int v, int w);
  int connectToAll(vector<int> clique, int v);
  int isInClique(vector<int> clique, int v);
  int debug;
};
