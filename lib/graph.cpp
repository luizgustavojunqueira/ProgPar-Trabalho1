#include "graph.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

Graph::Graph(string filename) {
  ifstream file(filename);
  map<int, int> vertex_map;
  vector<int> unique_vertices;
  int num_vertices = 0;

  int v, w;
  while (file >> v >> w) {
    if (vertex_map.find(v) == vertex_map.end()) {
      unique_vertices.push_back(v);
      vertex_map[v] = num_vertices++;
    }
    if (vertex_map.find(w) == vertex_map.end()) {
      unique_vertices.push_back(w);
      vertex_map[w] = num_vertices++;
    }
  }

  num_vertices = vertex_map.size();

  sort(unique_vertices.begin(), unique_vertices.end());

  file.close();

  this->adj_list.resize(num_vertices);

  file.open(filename);

  while (file >> v >> w) {

    this->adj_list[vertex_map[v]].push_back(vertex_map[w]);
    this->adj_list[vertex_map[w]].push_back(vertex_map[v]);
  }

  file.close();
}
