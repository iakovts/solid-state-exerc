#ifndef NETWORK_ER_H
#define NETWORK_ER_H
#define N 50000
#define PROB 0.166666666667

#include <array>
#include <iomanip>
#include <iostream>
#include <random>

template <class T, size_t m>
using Arr = std::array<T, m>;

template <typename T>
using Vec = std::vector<T>;

// struct Node {
//   int k = 0;  // Connections of the node
// };

class NetworkER {
 private:
  std::mt19937 rng;
  std::discrete_distribution<int> dis_distrib{1 - PROB, PROB};

 public:
  Arr<int, N> Nodes;
  // Node Nodes[N];
  void init_network(std::mt19937 set_rng) { rng = set_rng; }
  void print_arr(Arr<int, N>& arr);
  void prob_arr(Arr<int, N>& arr);
  void prob_conn();
  double ret_aver();
};

void NetworkER::prob_conn() {
  std::fill(Nodes.begin(), Nodes.end(), 0);
  int node_idx = 0;
  for (auto node : Nodes) {
    Arr<int, N> poss_conn{0};  // Array storing connection truth values for each node
    prob_arr(poss_conn);
    for (int i = 0; i < N; i++) {
      if (poss_conn[i] != 0) {
        if (node_idx != i) { // Check if the node is connecting to itself.
          ++Nodes[node_idx]; 
        }
      }
    }
    node_idx++;
  }
}

void NetworkER::prob_arr(Arr<int, N>& arr) {
  for (auto& x : arr) {
    x = dis_distrib(rng);
  }
}

void NetworkER::print_arr(Arr<int, N>& arr) {
  for (auto& x : arr) {
    std::cout << std::setw(4) << x;
  }
}

#endif
