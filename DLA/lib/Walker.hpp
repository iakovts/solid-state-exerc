#ifndef WALKER_H
#define WALKER_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>

#include "VectorOps.hpp"

template <typename T>
using Vec = std::vector<T>;

template <typename T>
using Set = std::set<T>;

class Walker {
 private:
  std::mt19937 rng;
  int grid_size = 450;
  Set<Vec<int>> circle_pts;
  int radius;

 public:
  std::array<std::array<int, 450>, 450> aggregate = {0}; 
  Walker(std::mt19937 set_rng, int set_grid_size, Set<Vec<int>> set_circle_pts,
         int set_radius) {
    rng = set_rng;
    grid_size = set_grid_size;
    circle_pts = set_circle_pts;
    // aggregate = set_aggregate;
    radius = set_radius;
  };
  std::discrete_distribution<int> dis_distrib{1, 0, 1};
  Vec<int> rand_circ_pt();
  Vec<int> random_step();
  void walk();
  int set_aggregate(int curr_x, int curr_y, int prev_x, int prev_y);
  void write_to_file();
  double euclidean_dist(int pos_x, int pos_y);
};

Vec<int> Walker::rand_circ_pt() {
  /* Returns a random vector containing the coordinates of a point on the
circle */
  std::uniform_int_distribution<> uni_distrib{0, (int)circle_pts.size() - 1};
  int size = circle_pts.size();
  int n;
  std::uniform_int_distribution<> distrib(0, size - 1);

  n = uni_distrib(rng);
  Set<Vec<int>>::iterator set_it = std::begin(circle_pts);
  std::advance(set_it, n);
  return *set_it;
}

Vec<int> Walker::random_step() {
  Vec<int> result(2);
  for (int x = 0; x < 2; x++) {
    // Distrib range is [0, 1, 2] so subtract one with
    // weights {1, 0, 1} to get -1 or 1;
    result[x] = dis_distrib(rng) - 1;
  }
  return result;
}

void Walker::walk() {
  aggregate[grid_size / 2][grid_size / 2] = 1;
  Vec<int> start = rand_circ_pt();
  Vec<int> step(2);
  int curr_x = step[0], curr_y = step[1], prev_x, prev_y;
  int aggregate_check = 0;
  while (true) {
    step = random_step();
    prev_x = curr_x;
    prev_y = curr_y;
    curr_x += step[0];
    curr_y += step[1];
    if (curr_x == 0 || curr_x == grid_size || curr_y == 0 || curr_y == grid_size){
      // Reached bounds - Reset
      start = rand_circ_pt();
      curr_x = start[0];
      curr_y = start[1];
      continue;
    }

    aggregate_check = set_aggregate(curr_x, curr_y, prev_x, prev_y);
    if (aggregate_check >= 1) {
      if (aggregate_check == 2) {
        break;
      }
      start = rand_circ_pt();
      curr_x = start[0];
      curr_y = start[1];
      aggregate_check = 0;
      continue;
    }
  }
}

int Walker::set_aggregate(int curr_x, int curr_y, int prev_x, int prev_y){
  int ret_val = 0;
  if(aggregate[curr_x][curr_y] == 1){
    // If the current position is occupied, then the last one is adjacent to the aggregate.
    aggregate[prev_x][prev_y] = 1; 
    ret_val += 1;
    std::cout << prev_x << " " << prev_y << "\n";
    if(euclidean_dist(prev_x, prev_y) > radius - 1){
      // The aggregate has reached the circle
      std::cout << "exiting";
      ret_val += 1;
    }
  }
  return ret_val;
}

double Walker::euclidean_dist(int pos_x, int pos_y){
  double dist;
  dist = sqrt(pow(pos_x - 225, 2) + pow(pos_y - 225, 2));
  return dist;
} 

void Walker::write_to_file(){
  std::ostringstream filename;
  filename << "walkthis.txt";
  std::ofstream fil;
  fil.open(filename.str());
  for (int x = 0; x < grid_size; x++){
    for (int y = 0; y < grid_size; y++){
      if (aggregate[x][y] == 1){
        fil << x << "," << y << "\n";
      }
    }
  }
  fil.close();
  }
  

#endif
