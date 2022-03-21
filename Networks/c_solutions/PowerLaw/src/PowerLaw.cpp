#include "../lib/PowerLaw.hpp"

#include <fstream>
#include <ostream>
#include <sstream>

#define RUNS 1000

int main(int argc, char **argv) {
  std::mt19937 rng(4385);
  double gamma = -3.0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
        case 'g':
          sscanf(argv[i + 1], "%lf", &gamma);
      }
    }
  }
  PowerLaw PL;
  PL.set_rng(rng);
  PL.set_gamma(gamma);
  Arr<int, N> results;
  for (int i = 0; i < RUNS; i++) {
    PL.power_law();
    for (auto node : PL.Nodes) {
      results[node]++;
    }
  }
  std::ostringstream filename;
  filename << "PowerLaw" << gamma << ".txt";
  std::ofstream fil;
  fil.open(filename.str());
  for (auto x : results) {
    fil << x << "\n";
  }
  fil.close();
}
