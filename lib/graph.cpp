#include "graph.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

Graph::Graph(string filename) {
  ifstream file(filename);
  map<int, int> vertex_map;
  int num_vertices = 0;

  int v, w;
  while (file >> v >> w) {
    if (vertex_map.find(v) == vertex_map.end()) {
      this->vertices.push_back(num_vertices);
      vertex_map[v] = num_vertices++;
    }

    if (vertex_map.find(w) == vertex_map.end()) {
      this->vertices.push_back(num_vertices);
      vertex_map[w] = num_vertices++;
    }

    this->add_edge(vertex_map[v], vertex_map[w]);
  }

  this->num_vertices = num_vertices;

  file.close();
}

void Graph::add_edge(long unsigned int v, long unsigned int u) {
  long unsigned int size = this->adj_list.size();

  if (size <= v) {
    this->adj_list.push_back(vector<int>());
  }

  if (size <= u) {
    this->adj_list.push_back(vector<int>());
  }

  if (find(this->adj_list[v].begin(), this->adj_list[v].end(), u) ==
      this->adj_list[v].end()) {
    this->adj_list[v].push_back(u);
  }

  if (find(this->adj_list[u].begin(), this->adj_list[u].end(), v) ==
      this->adj_list[u].end()) {
    this->adj_list[u].push_back(v);
  }
}

int Graph::isNeighbor(int v, int vizinho) {
  if (find(this->adj_list[v].begin(), this->adj_list[v].end(), vizinho) ==
      this->adj_list[v].end()) {
    return 0;
  }
  return 1;
}

int Graph::connectToAll(vector<int> clique, int v) {
  for (int vertex : clique) {
    if (this->isNeighbor(vertex, v) == 0) {
      return 0;
    }
  }
  return 1;
}

int Graph::isInClique(vector<int> clique, int v) {
  for (int vertex : clique) {
    if (vertex == v) {
      return 1;
    }
  }
  return 0;
}

int Graph::formsNewClique(vector<int> clique, int v) {
  if (this->isInClique(clique, v) == 0 && this->connectToAll(clique, v) == 1) {
    return 1;
  }
  return 0;
}

int Graph::countCliquesSerial(long unsigned int k) {
  int count = 0;

  set<vector<int>> cliques;

  for (int v = 0; v < this->num_vertices; v++) {
    cliques.insert(vector<int>{v});
  }

  while (!cliques.empty()) {

    vector<int> clique_atual = *cliques.rbegin();
    cliques.erase(clique_atual);

    if (clique_atual.size() == k) {
      count += 1;
      continue;
    }

    int last_vertex = clique_atual.back();

    for (int v : clique_atual) {

      for (int vizinho : this->adj_list[v]) {
        if (vizinho > last_vertex &&
            this->formsNewClique(clique_atual, vizinho)) {
          vector<int> nova_clique = clique_atual;
          nova_clique.push_back(vizinho);

          cliques.insert(nova_clique);
        }
      }
    }
  }

  return count;
}

int Graph::countCliquesParalelo(int k, int t) {
  int count = 0;

  return count;
}
