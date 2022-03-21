#ifndef POWERLAW_H
#define POWERLAW_H

#define N 10000

#include <array>
#include <cmath>
#include <iostream>
#include <random>

template <class T, size_t m>
using Arr = std::array<T, m>;

class PowerLaw {
 private:
  std::mt19937 rng;
  double gamma;

 public:
  Arr<int, N> Nodes{0};
  void set_rng(std::mt19937 set_rng) { rng = set_rng; }
  void set_gamma(double set_gamma) { gamma = set_gamma; }
  void power_law();
};

void PowerLaw::power_law() {
  std::fill(Nodes.begin(), Nodes.end(), 0);
  Arr<double, N> prob_array;
  // Array of all possible P(k)s
  for (int x = 1; x < N - 1; x++) {
    prob_array[x] = std::pow(x, gamma);
  }
  // Distribution of all possible P(k)s
  std::discrete_distribution<> dis(prob_array.begin(), prob_array.end());
  // Fill nodes with possible values
  for (auto &node : Nodes) {
    node = dis(rng);
  }
}

#endif
