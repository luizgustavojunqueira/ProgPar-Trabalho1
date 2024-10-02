#include "cliques.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <pthread.h>

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

  for (int i = 0; i < this->num_vertices; i++) {
    sort(this->adj_list[i].begin(), this->adj_list[i].end());
  }

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
  if (binary_search(this->adj_list[v].begin(), this->adj_list[v].end(),
                    vizinho)) {
    return 1;
  }
  return 0;
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

    vector<int> clique_atual = *cliques.begin();
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

void *countCliquesThread(void *args) {

  ThreadDataAlg2 *data = (ThreadDataAlg2 *)args;

  int count = 0;

  while (!data->cliques.empty()) {

    vector<int> clique_atual = *data->cliques.begin();
    data->cliques.erase(clique_atual);

    if (clique_atual.size() == data->k) {
      count += 1;
      continue;
    }

    int last_vertex = clique_atual.back();

    for (int v : clique_atual) {

      for (int vizinho : data->graph->adj_list[v]) {
        if (vizinho > last_vertex &&
            data->graph->formsNewClique(clique_atual, vizinho)) {
          vector<int> nova_clique = clique_atual;
          nova_clique.push_back(vizinho);

          data->cliques.insert(nova_clique);
        }
      }
    }
  }

  pthread_exit((void *)count);
}

int countCliquesParalelo(int k, int t, Graph *graph) {

  vector<set<vector<int>>> threads_cliques(t);

  for (int v = 0; v < graph->num_vertices; v++) {
    threads_cliques[v % t].insert(vector<int>{v});
  }

  vector<pthread_t> threads(t);
  vector<int> counts(t);

  ThreadDataAlg2 data[t];

  for (int i = 0; i < t; i++) {
    data[i].k = k;
    data[i].cliques = threads_cliques[i];
    data[i].graph = graph;
    pthread_create(&threads[i], NULL, &countCliquesThread, (void *)&data[i]);
  }

  for (int i = 0; i < t; i++) {
    pthread_join(threads[i], (void **)&counts[i]);
  }

  int count = 0;
  for (int i = 0; i < t; i++) {
    count += counts[i];
  }

  return count;
}

int countCliquesParaleloBalanceado(int k, int t, int r, int tentativas,
                                   Graph *graph) {

  // t sets de cliques
  set<vector<int>> threads_cliques[t];

  for (int i = 0; i < graph->num_vertices; i++) {
    threads_cliques[i % t].insert(vector<int>{i});
  }

  pthread_t threads[t];
  int counts[t];
  pthread_mutex_t mutexes[t];

  ThreadDataAlg3 data[t];

  for (int i = 0; i < t; i++) {

    pthread_mutex_init(&mutexes[i], NULL);
    data[i].k = k;
    data[i].id = i;
    data[i].num_threads = t;
    data[i].r = r;
    data[i].tentativas = tentativas;
    data[i].graph = graph;
    data[i].threads_cliques = threads_cliques;
    data[i].mutexes = mutexes;
    pthread_create(&threads[i], NULL, &countCliquesThreadBalanceada,
                   (void *)&data[i]);
  }

  for (int i = 0; i < t; i++) {
    pthread_join(threads[i], (void **)&counts[i]);
  }

  int count = 0;
  for (int i = 0; i < t; i++) {
    count += counts[i];
  }

  return count;
}

void *countCliquesThreadBalanceada(void *args) {

  ThreadDataAlg3 *data = (ThreadDataAlg3 *)args;

  int count = 0;

  int tentativasRoubar = 0;

  while (!data->threads_cliques[data->id].empty()) {

    vector<int> clique_atual = *data->threads_cliques[data->id].begin();
    data->threads_cliques[data->id].erase(clique_atual);

    if (clique_atual.size() == data->k) {
      count += 1;

      continue;
    }

    int last_vertex = clique_atual.back();

    for (int v : clique_atual) {

      for (int vizinho : data->graph->adj_list[v]) {
        if (vizinho > last_vertex &&
            data->graph->formsNewClique(clique_atual, vizinho)) {
          vector<int> nova_clique = clique_atual;
          nova_clique.push_back(vizinho);

          pthread_mutex_lock(&data->mutexes[data->id]);
          data->threads_cliques[data->id].insert(nova_clique);
          pthread_mutex_unlock(&data->mutexes[data->id]);
        }
      }
    }

    if (data->threads_cliques[data->id].empty()) {
      while (tentativasRoubar < data->tentativas) {
        bool allEmpty = true;

        for (int i = 0; i < data->num_threads; i++) {

          if (i != data->id) {

            int qntRoubar = data->threads_cliques[i].size() * (data->r / 100);

            if (qntRoubar < 10) {
              continue;
            }

            allEmpty = false;

            for (int j = 0; j < qntRoubar; j++) {
              pthread_mutex_lock(&data->mutexes[i]);
              vector<int> clique_roubada = *data->threads_cliques[i].rbegin();
              data->threads_cliques[i].erase(clique_roubada);
              pthread_mutex_unlock(&data->mutexes[i]);
              data->threads_cliques[data->id].insert(clique_roubada);
            }
          }
        }

        if (data->threads_cliques[data->id].empty()) {
          tentativasRoubar++;
        } else {
          tentativasRoubar = 0;

          break;
        }

        if (allEmpty) {
          break;
        }
      }
    }
  }

  pthread_exit((void *)count);
}
