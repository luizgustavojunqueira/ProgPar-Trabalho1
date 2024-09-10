#include <string>
#include <vector>

using namespace std;

class Graph {
public:
  Graph(string filename);
  void addEdge(int v, int w);
  void printGraph();

private:
  vector<vector<int>> adj_list;
};
