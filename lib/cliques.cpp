#include "cliques.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <pthread.h>

Graph::Graph(string filename) {
  // Abre o arquivo de edges
  ifstream file(filename);
  map<int, int> vertex_map;
  int num_vertices = 0;

  int v, w;
  // Le uma aresta do arquivo
  while (file >> v >> w) {

    // Caso o vertice v nao esteja no map, adiciona ele
    if (vertex_map.find(v) == vertex_map.end()) {
      // Adiciona o vertice no vetor de vertices
      this->vertices.push_back(num_vertices);

      // Adiciona o vertice no map
      vertex_map[v] = num_vertices++;
    }

    // Caso o vertice w nao esteja no map, adiciona ele
    if (vertex_map.find(w) == vertex_map.end()) {
      // Adiciona o vertice no vetor de vertices
      this->vertices.push_back(num_vertices);
      // Adiciona o vertice no map
      vertex_map[w] = num_vertices++;
    }

    // Adiciona a aresta no grafo (lista de adjacencia)
    this->add_edge(vertex_map[v], vertex_map[w]);
  }

  this->num_vertices = num_vertices;

  // Ordena as listas de adjacencia
  for (int i = 0; i < this->num_vertices; i++) {
    sort(this->adj_list[i].begin(), this->adj_list[i].end());
  }

  file.close();
}

// Adiciona uma aresta no grafo
void Graph::add_edge(long unsigned int v, long unsigned int u) {

  // Como os vértices são indexados de 0 a n-1, o tamanho do vetor de adjacencia
  // é o maior vértice + 1

  long unsigned int size = this->adj_list.size();

  // Caso o vertice v nao exista, adiciona a lista de adjacencia dele
  if (size <= v) {
    this->adj_list.push_back(vector<int>());
  }

  // Caso o vertice u nao exista, adiciona a lista de adjacencia dele
  if (size <= u) {
    this->adj_list.push_back(vector<int>());
  }

  // Adiciona o vértice u na lista de adjacencia de v
  if (find(this->adj_list[v].begin(), this->adj_list[v].end(), u) ==
      this->adj_list[v].end()) {
    this->adj_list[v].push_back(u);
  }

  // Adiciona o vértice v na lista de adjacencia de u
  if (find(this->adj_list[u].begin(), this->adj_list[u].end(), v) ==
      this->adj_list[u].end()) {
    this->adj_list[u].push_back(v);
  }
}

// Verifica se um vértice é vizinho de outro
int Graph::isNeighbor(int v, int vizinho) {
  // Como as listas de adjacencia estão ordenadas, é possível fazer uma busca
  // binária
  if (binary_search(this->adj_list[v].begin(), this->adj_list[v].end(),
                    vizinho)) {
    return 1;
  }
  return 0;
}

// Verifica se um vértice é vizinho de todos os vértices de uma clique
int Graph::connectToAll(vector<int> clique, int v) {
  // Para cada vértice da clique
  // Verifica se o vértice v é vizinho
  for (int vertex : clique) {
    if (this->isNeighbor(vertex, v) == 0) {
      return 0;
    }
  }
  return 1;
}

// Verifica se um vértice está em uma clique
int Graph::isInClique(vector<int> clique, int v) {
  for (int vertex : clique) {
    if (vertex == v) {
      return 1;
    }
  }
  return 0;
}

// Verifica se um vértice pode formar uma nova clique
int Graph::formsNewClique(vector<int> clique, int v) {
  // Para formar uma nova clique, o vértice v deve ser vizinho de todos os
  // vértices da clique e não deve estar na clique
  if (this->isInClique(clique, v) == 0 && this->connectToAll(clique, v) == 1) {
    return 1;
  }
  return 0;
}

// Conta o número de cliques de tamanho k no grafo de forma serial
unsigned int Graph::countCliquesSerial(long unsigned int k) {
  unsigned int count = 0;

  // Utiliza uma set para armazenar as cliques pois impede a inserção de
  // cliques repetidas
  set<vector<int>> cliques;

  // Inicializa o set com cliques de tamanho 1, uma clique para cada vértice
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

// Thread para contar o número de cliques de tamanho k no grafo
// Recebe como argumento um set de cliques, o grafo e o tamanho K
// retorna o número de cliques de tamanho k naquele set
void *countCliquesThread(void *args) {

  ThreadDataAlg2 *data = (ThreadDataAlg2 *)args;

  unsigned int count = 0;

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

// Conta o número de cliques de tamanho k no grafo de forma paralela
// Separa o set de cliques em t sets e cria t threads para contar o número de
// cliques em cada set
// Retorna o número total de cliques de tamanho k no grafo
unsigned int countCliquesParalelo(int k, int t, Graph *graph) {

  vector<set<vector<int>>> threads_cliques(t);

  for (int v = 0; v < graph->num_vertices; v++) {
    threads_cliques[v % t].insert(vector<int>{v});
  }

  vector<pthread_t> threads(t);
  vector<unsigned int> counts(t);

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

  unsigned int count = 0;
  for (int i = 0; i < t; i++) {
    count += counts[i];
  }

  return count;
}

// Conta o número de cliques de tamanho k no grafo de forma paralela fazendo
// balanceamento de carga Separa o set de cliques em t sets e cria t threads
// para contar o número de cliques em cada set Retorna o número total de cliques
// de tamanho k no grafo
unsigned int countCliquesParaleloBalanceado(int k, int t, int r, int tentativas,
                                            Graph *graph) {

  // t sets de cliques
  set<vector<int>> threads_cliques[t];

  for (int i = 0; i < graph->num_vertices; i++) {
    threads_cliques[i % t].insert(vector<int>{i});
  }

  pthread_t threads[t];
  unsigned int counts[t];
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

  unsigned int count = 0;
  for (int i = 0; i < t; i++) {
    count += counts[i];
  }

  return count;
}

// Thread para contar o número de cliques de tamanho k no grafo com
// balanceamento de carga Recebe como argumento o Grafo, o id da thread, o
// número de threads, o fator de roubo, o número de tentativas de roubo, um
// vetor de sets de cliques e um vetor de mutexes
// retorna o número de cliques de tamanho k que essa thread contou
void *countCliquesThreadBalanceada(void *args) {

  ThreadDataAlg3 *data = (ThreadDataAlg3 *)args;

  unsigned int count = 0;

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

          // Travando o mutex para inserir a nova clique
          pthread_mutex_lock(&data->mutexes[data->id]);
          // Inserindo a nova clique
          data->threads_cliques[data->id].insert(nova_clique);
          // Liberando o mutex
          pthread_mutex_unlock(&data->mutexes[data->id]);
        }
      }
    }

    // Caso a thread não tenha mais cliques para contar, tenta roubar de outra
    // thread
    if (data->threads_cliques[data->id].empty()) {
      while (tentativasRoubar < data->tentativas) {
        bool allEmpty = true;

        for (int i = 0; i < data->num_threads; i++) {

          // Não rouba de si mesmo
          if (i != data->id) {

            // Calcula a quantidade de cliques que vai roubar da thread i
            // Calcula 10% do tamanho do set de cliques da thread i
            int qntRoubar = data->threads_cliques[i].size() * (data->r / 100);

            if (qntRoubar < 10) {
              continue;
            }

            allEmpty = false;

            // Rouba qntRoubar cliques da thread i uma por uma
            for (int j = 0; j < qntRoubar; j++) {
              // Trava o mutex da thread i
              pthread_mutex_lock(&data->mutexes[i]);
              // Remove a última clique do set de cliques da thread i
              vector<int> clique_roubada = *data->threads_cliques[i].rbegin();
              data->threads_cliques[i].erase(clique_roubada);
              // Libera o mutex da thread i
              pthread_mutex_unlock(&data->mutexes[i]);
              // Insere a clique roubada no set de cliques da thread atual
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
