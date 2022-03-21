/*
  Contains the HK class implementating the Hoshen-Kopelman algorithm for a class
assignment. Code based on algorithm found here
http://kelifos.physics.auth.gr/COURSES/Percolation.pdf .

Copyright (C) 2021  iakovts

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

Written and compiled with C++17.

 `g++ -g -std=c++17 -fsanitize=address -fopenmp  -Wall -O2 -o prob percol.cpp`
*/
#ifndef PERCOL_H
#define PERCOL_H

#include <algorithm>  // max_element
#include <array>
// #include <cmath>
#include <fstream>
#include <iomanip>  // setw
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <vector>

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a > b ? b : a)

template <typename T>
using Vec = std::vector<T>;

template <typename T>
using Set = std::set<T>;

template <class T, size_t N>
using Arr = std::array<T, N>;

class Percolator {
 private:
  std::mt19937 rng;
  double p;
  std::discrete_distribution<int> dis_distrib;
  Vec<int> L;
  Vec<int> S;
  int k = 1;
  double pN2;
  int grid_sqr = pow(grid_s, 2);

 public:
  static const int grid_s = 100;
  double I = 0, I_no_max = 0, pmax = 0;
  Arr<Arr<int, grid_s>, grid_s> grid{0};
  void init_percolator(std::mt19937 set_rng, double set_p);
  void set_dis_distrib();
  void print_grid();
  void fill_grid();
  void sweep();
  void modify_S();
  void set_pN2() { pN2 = p * grid_sqr; };
  void calculate_all();
  void run_calcs();
  void run();
  void reset();
  void test_run();
  Percolator() : L{Vec<int>()}, S{Vec<int>()} {};
};

void Percolator::init_percolator(std::mt19937 set_rng, double set_p) {
  S = Vec<int>(grid_sqr, 0);
  p = set_p;
  rng = set_rng;
  set_dis_distrib();
  set_pN2();
}

void Percolator::reset() {
  L = Vec<int>();
  S = Vec<int>(grid_sqr, 0);
  for (auto row : grid) {
    row.fill(0);
  }
  k = 1;
  I = 0;
  I_no_max = 0;
  pmax = 0;
}

void Percolator::set_dis_distrib() {
  dis_distrib = std::discrete_distribution<int>{1 - p, p};
}

void Percolator::print_grid() {
  for (int x = 0; x < grid_s; x++) {
    for (int y = 0; y < grid_s; y++) {
      std ::cout << std::setw(6) << grid[x][y];
    }
    std::cout << "\n";
  }
}

void Percolator::fill_grid() {
  /* Fills the grid randomly with 0 and 1 */
  int rc;
  for (int x = 0; x < grid_s; x++) {
    for (int y = 0; y < grid_s; y++) {
      rc = dis_distrib(rng);  // Randomly pick 0 or 1;
      grid[x][y] = rc;
    }
  }
}

void Percolator::sweep() {
  L.push_back(0);
  for (int y = 0; y < grid_s; y++) {
    for (int x = 0; x < grid_s; x++) {
      if (grid[x][y] != 0) {
        int left = (y == 0 ? 0 : grid[x][y - 1]);  // ternary for boundaries
        int up = (x == 0 ? 0 : grid[x - 1][y]);

        switch (!!up + !!left) {  // Check if none,
                                  // 1 or 2 sites left and above are occupied
          case 0:                 // none occupied
            L.push_back(k);
            grid[x][y] = L[k];
            S[L[k]] += 1;
            k++;
            break;
          case 1:  // Left or above occupied
            grid[x][y] = max(up, left);
            S[L[grid[x][y]]] += 1;
            break;
          case 2:  // Left and above occupied
            grid[x][y] = L[left];
            S[L[left]] += 1;
            if (L[left] != L[up]) {
              // Will check against `L_above` in next for loop,
              // as L[] will change when cond is met.
              int L_above = L[up];
              int size_l = 0;
              for (int i = 1; i < (int)L.size(); i++) {
                if (L[i] == L_above) {
                  L[i] = L[left];
                  size_l += S[i];
                  S[i] = 0;
                }
              }
              S[left] += size_l;
            }

            break;
        }
      }
    }
  }
  // second sweep
  for (int y = 0; y < grid_s; y++) {
    for (int x = 0; x < grid_s; x++) {
      if (grid[x][y]) {
        grid[x][y] = L[grid[x][y]];
      }
    }
  }
}

void Percolator::modify_S() {
  while (!S.empty() && S[S.size() - 1] == 0) {
    S.pop_back();
  }  // Remove trailing 0
}

void Percolator::calculate_all() {
  double max_S = *std::max_element(S.begin(), S.end());  // Largest cluster size
  for (auto x : S) {
    I += std::pow(x, 2) / pN2;
  }
  I_no_max = I - (std::pow(max_S, 2) / pN2);
  pmax = max_S / pN2;
}

void Percolator::run_calcs() {
  modify_S();
  calculate_all();
  // std::cout << "I= " << I << " I_no_max = " << I_no_max << " pmax= " << pmax
  //           << "\n";
}

void Percolator::run() {
  reset();
  fill_grid();
  sweep();
  run_calcs();
}

void Percolator::test_run() {
  reset();
  fill_grid();
  print_grid();
  sweep();
  run_calcs();
  std::cout << "\n";
  print_grid();
  std::cout << "\nL: ";
  for (auto x : L) {
    std::cout << x << " ";
  }
  std::cout << "\nS: ";
  for (auto x : S) {
    std::cout << x << " ";
  }
}

#endif
