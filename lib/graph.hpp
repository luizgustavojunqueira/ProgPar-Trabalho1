#include <pthread.h>
#include <set>
#include <string>
#include <vector>

using namespace std;

class Graph {
public:
  Graph(string filename);
  int countCliquesSerial(long unsigned int k);
  void print();
  void print_edges();
  int num_vertices;
  vector<vector<int>> adj_list;
  int isNeighbor(int v, int w);
  int connectToAll(vector<int> clique, int v);
  int isInClique(vector<int> clique, int v);
  int formsNewClique(vector<int> clique, int v);

private:
  vector<int> vertices;
  void add_edge(long unsigned int v, long unsigned int w);
};

class ThreadDataAlg2 {
public:
  long unsigned int k;
  set<vector<int>> cliques;
  Graph *graph;
};

void *countCliquesThread(void *threadarg);
int countCliquesParalelo(int k, int t, Graph *g);

class ThreadDataAlg3 {
public:
  long unsigned int k;
  Graph *graph;
  int id;
  set<vector<int>> *threads_cliques;
  int num_threads;
  vector<pthread_mutex_t> *mutexes;
};

void *countCliquesThreadBalanceada(void *threadarg);
int countCliquesParaleloBalanceado(int k, int t, Graph *g);
