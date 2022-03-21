#include <stdio.h>

#include "../lib/Walker.hpp"


Set<Vec<int>> midpoint_circle(int radius, int x0, int y0);
void set_grid_points(Set<Vec<int>> &set_pt, int x0, int y0);

int main(int argc, char **argv) {
  int radius = 200;
  Set<Vec<int>> circle = midpoint_circle(radius, 225, 225);
  Vec<int> temp_aggr(2, 225);
  Set<Vec<int>> aggr;
  aggr.insert(temp_aggr);
  std::mt19937 rng(4385);
  Walker walk(rng, 450, circle, radius);
  walk.walk();
  walk.write_to_file();
}

void set_grid_points(Set<Vec<int>> &set_pt, int x0, int y0) {
  Vec<int> pt(2);
  pt[0] = x0;
  pt[1] = y0;
  set_pt.insert(pt);
}

Set<Vec<int>> midpoint_circle(int radius, int x0, int y0) {
  int f = 1 - radius;
  int ddf_x = 1;
  int ddf_y = -2 * radius;
  int x = 0;
  int y = radius;
  Set<Vec<int>> grid_circle;
  set_grid_points(grid_circle, x0, y0 + radius);
  set_grid_points(grid_circle, x0, y0 - radius);
  set_grid_points(grid_circle, x0 + radius, y0);
  set_grid_points(grid_circle, x0 - radius, y0);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddf_y += 2;
      f += ddf_y;
    }
    x++;
    ddf_x += 2;
    f += ddf_x + 1;
    set_grid_points(grid_circle, x0 + x, y0 + y);
    set_grid_points(grid_circle, x0 - x, y0 + y);
    set_grid_points(grid_circle, x0 + x, y0 - y);
    set_grid_points(grid_circle, x0 - x, y0 - y);
    set_grid_points(grid_circle, x0 + y, y0 + x);
    set_grid_points(grid_circle, x0 - y, y0 + x);
    set_grid_points(grid_circle, x0 + y, y0 - x);
    set_grid_points(grid_circle, x0 - y, y0 - x);
  }
  return grid_circle;
}
