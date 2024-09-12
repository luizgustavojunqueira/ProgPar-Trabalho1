#include <string>
#include <vector>

using namespace std;

class Graph {
public:
  Graph(string filename);
  void print();

private:
  vector<vector<int>> adj_list;
  void add_edge(int v, int w, int num_vertices);
};
