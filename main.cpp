#include "./lib/graph.hpp"
#include <iostream>
#include <sys/time.h>
#include <thread>
#include <time.h>

double read_timer() {
  static int initialized = 0;
  static struct timeval start;
  struct timeval end;
  if (!initialized) {
    gettimeofday(&start, NULL);
    initialized = 1;
  }
  gettimeofday(&end, NULL);
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

double start_time, end_time; /* start and end times */

int main(int argc, char *argv[]) {

  if (argc < 4) {
    cout << "Usage: ./main <filename> <k> <alg>" << endl;
    return 1;
  }

  string filename = argv[1];
  int k = stoi(argv[2]);
  int alg = stoi(argv[3]);

  Graph g(filename);

  if (alg == 1) {
    start_time = read_timer();
    int numCliques = g.countCliquesSerial(k);
    end_time = read_timer();
    cout << "Serial " << k << "-cliques: " << numCliques
         << " Time: " << end_time - start_time << "s" << endl;
  } else if (alg == 2) {
    const int processor_count = std::thread::hardware_concurrency();
    start_time = read_timer();
    int numCliques = countCliquesParalelo(k, processor_count, &g);
    end_time = read_timer();
    cout << "Paralelo " << k << "-cliques: " << numCliques
         << " Time: " << end_time - start_time << "s" << endl;
  } else if (alg == 3) {

    const int processor_count = std::thread::hardware_concurrency();
    start_time = read_timer();
    int numCliques = countCliquesParaleloBalanceado(k, processor_count, &g);
    end_time = read_timer();
    cout << "Paralelo Balanceado " << k << "-cliques: " << numCliques
         << " Time: " << end_time - start_time << "s" << endl;
  }

  return 0;
}
