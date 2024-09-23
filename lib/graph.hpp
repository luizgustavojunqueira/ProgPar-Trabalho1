#include <string>
#include <vector>

using namespace std;

class Graph {
public:
  Graph(string filename);
  int countCliquesSerial(int k);
  int countCliquesParalelo(int k, int t);
  int countCliquesParaleloBalanceado(int k);
  void print();
  void print_edges();
  int num_vertices;

private:
  vector<vector<int>> adj_list;
  vector<int> vertices;
  void add_edge(int v, int w);
  int isNeighbor(int v, int w);
  int connectToAll(vector<int> clique, int v);
  int isInClique(vector<int> clique, int v);
  int formsNewClique(vector<int> clique, int v);
};
