#include "graph.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

Graph::Graph(string filename, int debug) {
  this->debug = debug;
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

    if (this->debug) {
      cout << "Adding edge between " << v << " and " << w << endl;
      cout << "Which are remapped to " << vertex_map[v] << " and "
           << vertex_map[w] << endl;
    }

    this->add_edge(vertex_map[v], vertex_map[w], num_vertices);
  }

  this->num_vertices = num_vertices;

  file.close();
}

void Graph::add_edge(int v, int vizinho, int num_vertices) {
  this->adj_list.resize(num_vertices);
  this->adj_list[v].push_back(vizinho);
  this->adj_list[vizinho].push_back(v);
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

int Graph::isNeighbor(int v, int vizinho) {
  return find(this->adj_list[v].begin(), this->adj_list[v].end(), vizinho) !=
         this->adj_list[v].end();
}

int Graph::connectToAll(vector<int> clique, int v) {
  for (int i = 0; i < (int)clique.size(); i++) {
    if (!this->isNeighbor(clique[i], v)) {

      if (this->debug) {
        cout << "    Vertex " << v << " is not connected to " << clique[i]
             << endl;
      }
      return 0;
    }
  }

  if (this->debug) {
    cout << "    Vertex " << v << " is connected to all vertices in clique"
         << endl;
  }

  return 1;
}

int Graph::isInClique(vector<int> clique, int v) {
  if (find(clique.begin(), clique.end(), v) == clique.end()) {
    if (this->debug) {
      cout << "    Vertex " << v << " is not in clique" << endl;
    }
    return 0;
  }
  if (this->debug) {
    cout << "    Vertex " << v << " is in clique" << endl;
  }

  return 1;
}

int Graph::countCliquesSerial(int k) {
  int count = 0;

  vector<vector<int>> cliques;

  if (this->debug) {
    cout << "Starting to check cliques" << endl;
  }

  for (int i = 0; i < (int)this->adj_list.size(); i++) {
    cliques.push_back(vector<int>{i});
    if (this->debug) {
      cout << "Adding " << i << " to cliques" << endl;
    }
  }

  while (!cliques.empty()) {
    if (this->debug) {
      cout << "Current cliques: " << endl;
      for (int i = 0; i < (int)cliques.size(); i++) {
        cout << " [ ";
        for (int j = 0; j < (int)cliques[i].size(); j++) {
          cout << cliques[i][j] << " ";
        }
        cout << "]" << endl;
      }
    }

    vector<int> clique = cliques.back();
    cliques.pop_back();

    if (this->debug) {
      cout << "Checking clique: [ ";
      for (int i = 0; i < (int)clique.size(); i++) {
        cout << clique[i] << " ";
      }
      cout << "]" << endl;
    }

    if ((int)clique.size() == k) {
      if (this->debug) {
        cout << "Clique found!" << endl;
      }
      count++;
      continue;
    }

    int last_v = clique.back();

    if (this->debug) {
      cout << " with last vertex " << last_v << endl;
    }

    // Para cada vertice v no clique
    for (int i = 0; i < (int)clique.size(); i++) {
      int v = clique[i];

      if (this->debug) {
        cout << "  Checking vertex " << v << endl;
      }

      // Para cada vizinho de v
      for (int j = 0; j < (int)this->adj_list[v].size(); j++) {
        int vizinho = this->adj_list[v][j];

        if (this->debug) {
          cout << "   Checking neighbor " << vizinho << endl;
        }
        // Se vizinho nao esta no clique e conecta a todos os vertices do clique
        // e vizinho > last_v
        if (vizinho > last_v && this->isInClique(clique, vizinho) == 0 &&
            this->connectToAll(clique, vizinho) == 1) {
          if (this->debug) {
            cout << "     Adding " << vizinho << " to clique" << endl;
          }
          // Adiciona vizinho ao clique
          vector<int> new_clique = clique;
          new_clique.push_back(vizinho);
          if (find(cliques.begin(), cliques.end(), new_clique) ==
              cliques.end()) {
            // Adiciona novo clique a lista de cliques
            cliques.push_back(new_clique);
          }
        }

        if (this->debug) {
          cout << "   Done checking neighbor " << vizinho << endl;
        }
      }

      if (this->debug) {
        cout << "  Done checking vertex " << v << endl;
      }
    }

    if (this->debug) {
      cout << "Done checking clique" << endl;
    }
  }

  if (this->debug) {
    cout << "Done checking all cliques" << endl;
  }

  return count;
}
