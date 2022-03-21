#ifndef SMALLWORLD_H
#define SMALLWORLD_H

#define N 1000
#define PROB 0.2
// #define PROB 0
#define K 16

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <random>

template <class T, size_t m>
using Arr = std::array<T, m>;

template <typename T>
using Vec = std::vector<T>;

int search_vec(Vec<int> &vec, int val);

class SmallWorld {
 private:
  std::mt19937 rng;
  std::discrete_distribution<int> dis_distrib{1 - PROB, PROB};
  std::uniform_int_distribution<int> uni_distrib{0, N - 1};

 public:
  Arr<Vec<int>, N> adj_mat;  // Holds node's connections
  void set_rng(std::mt19937 s_rng) { rng = s_rng; }
  void make_ring_lattice();
  bool change_maybe() { return dis_distrib(rng); }
  void rewire_node(Vec<int> &node, int node_idx);
  int search_vec(Vec<int> &vec, int val);
  void rewire();
  void run();
  void print_graph();
};

void SmallWorld::make_ring_lattice() {
  for (int i = 0; i < N; i++) {
    for (int j = 1 + i; j < 1 + i + K / 2; j++) {
      adj_mat[i].push_back(j % N);
    }
  }
  // second pass to add connections made further
  // down in the lattice
  for (int i = 0; i < N; i++) {
    for (auto conn : adj_mat[i]) {
      if (!search_vec(adj_mat[conn], i)) {
        adj_mat[conn].push_back(i);
      }
    }
  }
}

void SmallWorld::rewire() {
  for (int i = 0; i < N; i++) {
    for (auto &conn : adj_mat[i]) {
      if (change_maybe()) {
        int tmp_new = 0;
        int tmp_old = conn;
        // do look for new connections while avoiding
        // self loops and already existing connections
        do {
          tmp_new = uni_distrib(rng);
        } while (search_vec(adj_mat[i], tmp_new) || i == tmp_new);

        // Change connection of node and remove
        // current node from list of connections
        // from the old node.
        conn = tmp_new;
        adj_mat[tmp_new].push_back(i);
        Vec<int> &n_old = adj_mat[tmp_old];
        n_old.erase(std::remove(n_old.begin(), n_old.end(), i), n_old.end());
      }
    }
  }
}

void SmallWorld::run() {
  for (auto &node : adj_mat) {
    node.clear();
  }  // reset nodes
  make_ring_lattice();
  rewire();
}

void SmallWorld::print_graph() {
  int node_idx = 0;
  for (auto node : adj_mat) {
    std::cout << "(" << node_idx;
    for (auto conn : node) {
      std::cout << std::setw(4) << conn;
    }
    std::cout << ")\n";
    ++node_idx;
  }
}

int SmallWorld::search_vec(Vec<int> &vec, int val) {
  // Searches node's connections
  return (std::find(vec.begin(), vec.end(), val) != vec.end());
}

#endif
