#include "../lib/SmallWorld.hpp"

#include <fstream>
#define RUNS 1000

int main() {
  SmallWorld SW;
  SW.set_rng(std::mt19937(4385));
  Arr<int, N / 10> output{
      0};  // output array, accounting for N/10 possible k vals;
  // SW.run();
  for (int x = 0; x < RUNS; x++) {
    SW.run();
    for (auto node : SW.adj_mat) {
      output[node.size()]++;
    }
  }

  std::ostringstream filename;
  filename << "SmallWorld.txt";
  std::ofstream fil;
  fil.open(filename.str());
  for (auto y : output) {
    // std::cout << y << "   ";
    fil << y << "\n";
  }
  fil.close();
}
