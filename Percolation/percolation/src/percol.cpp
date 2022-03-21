/*
  iakovts' implementation of the Hoshen-Kopelman algorithm for a class
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

#include "../lib/Percol.hpp"

#include <omp.h>

void run();

int main() { run(); }

void run() {
  Arr<Arr<double, 3>, 22> results;
  int nThreads;
  Arr<double, 22> p_vals{0.1,  0.2,  0.3, 0.4,  0.5,  0.55, 0.56, 0.57,
                         0.58, 0.59, 0.6, 0.61, 0.62, 0.63, 0.64, 0.65,
                         0.7,  0.75, 0.8, 0.85, 0.9,  0.95};
#pragma omp parallel
  { nThreads = omp_get_num_threads(); }
  std::mt19937 generator[nThreads];
  for (int i = 0; i < nThreads; i++) {
    generator[i] = std::mt19937(4385 * i);
  }
  for (int p_val = 0; p_val < (int)p_vals.size(); p_val++) {
    std::cout << p_vals[p_val] << "\n";
    double p_value = p_vals[p_val];
    double sumI = 0, sumI_no_max = 0, sum_pmax = 0;
#pragma omp parallel shared(results) firstprivate(p_value, p_val)
    {
      Percolator percol;
      percol.init_percolator(generator[omp_get_thread_num()], p_value);
#pragma omp for reduction(+ : sumI, sumI_no_max, sum_pmax)
      for (int x = 0; x < 1000; x++) {
        percol.run();
        sumI += percol.I;
        sumI_no_max += percol.I_no_max;
        sum_pmax += percol.pmax;
      }
      results[p_val][0] = sumI / 1000;
      results[p_val][1] = sumI_no_max / 1000;
      results[p_val][2] = sum_pmax / 1000;
    }
  }

  std::ostringstream filename;
  filename << "percolation_res.txt";
  std::ofstream fil;
  fil.open(filename.str());
  std::cout << std::setw(20) << "I" << std::setw(20) << "I'" << std::setw(20)
            << "pmax\n";
  for (int x = 0; x < 22; x++) {
    for (int y = 0; y < 3; y++) {
      std::cout << std::setw(20) << results[x][y];
    }
    fil << results[x][0] << "," << results[x][1] << "," << results[x][2]
        << "\n";
    std::cout << "\n";
  }
  fil.close();
}
