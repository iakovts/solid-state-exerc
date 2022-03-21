#include "../lib/NetworkER.hpp"

#include <omp.h>
#include <fstream>
#include <ostream>

#define RUNS 1000

// int * results = new int[N]{0}; // Allocate large array on the heap.
// static int results[N] = {0}; // Allocate large array on the heap.
int main(){
  int nThreads;
#pragma omp parallel
  {
  nThreads = omp_get_num_threads();
  }
  NetworkER netwrk[nThreads];
  for (int i=0; i < nThreads; i++){
    netwrk[i].init_network(std::mt19937(4385*i));
  }
  // Arr<int, N> results{0};
  Vec<int> results(N, 0);
#pragma omp parallel shared(results) firstprivate(netwrk)
  {
    #pragma omp for
    for (int x=0; x < RUNS; x++){
      std::cout << x << "\n";
      int t_num = omp_get_thread_num();
      netwrk[t_num].prob_conn();
      for (int i = 0; i < N; i++){
        int node_k = netwrk[t_num].Nodes[i];
        if (node_k !=0){
          results[node_k] ++;
        }
      }
    }
  }


  std::ostringstream filename;
  filename << "ErdosNetwork" << N << ".txt";
  std::ofstream fil;
  fil.open(filename.str());
  // for (auto x : results){
  for (int x = 0; x < N; x ++){
    fil << results[x] << "\n";
  }
  fil.close();
}
