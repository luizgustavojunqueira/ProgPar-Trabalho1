#include "graph.hpp"
#include <fstream>
#include <iostream>
#include <map>

Graph::Graph(string filename, int debug) {
  ifstream file(filename);
  map<int, int> vertex_map;
  int num_vertices = 0;

  int v, w;
  while (file >> v >> w) {
    if (vertex_map.find(v) == vertex_map.end()) {
      vertex_map[v] = num_vertices++;
    }

    if (vertex_map.find(w) == vertex_map.end()) {
      vertex_map[w] = num_vertices++;
    }

    if (debug) {
      cout << "Adding edge between " << v << " and " << w << endl;
      cout << "Which are remapped to " << vertex_map[v] << " and "
           << vertex_map[w] << endl;
    }

    this->add_edge(vertex_map[v], vertex_map[w], num_vertices);
  }

  file.close();
}

void Graph::add_edge(int v, int w, int num_vertices) {
  this->adj_list.resize(num_vertices);
  this->adj_list[v].push_back(w);
  this->adj_list[w].push_back(v);
}

void Graph::print() {
  for (long unsigned int i = 0; i < this->adj_list.size(); i++) {
    cout << i << ": ";
    for (long unsigned int j = 0; j < this->adj_list[i].size(); j++) {
      cout << this->adj_list[i][j] << " ";
    }
    cout << endl;
  }

  cout << "This graph has " << this->adj_list.size() << " vertices." << endl;
}
