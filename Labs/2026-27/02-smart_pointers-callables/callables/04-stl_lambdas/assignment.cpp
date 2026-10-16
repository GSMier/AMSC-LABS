/*
Standard algorithms + lambdas (homework). Fill in the TODOs, using one
algorithm of <algorithm> or <numeric> and one lambda for each of them:
no for loops, except for printing.

compile with
  g++ -Wall -std=c++20 assignment.cpp -o assignment
*/

#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Particle {
  int id;
  double mass;
  double x; // position
};

void print(std::string const &title, std::vector<double> const &v) {
  std::cout << title << ":";
  for (double x : v)
    std::cout << " " << x;
  std::cout << std::endl;
}

int main() {
  std::vector<double> v{0.5, -1.2, 2.3, -0.4, 1.8, -2.1, 0.9, 0.1, -0.7, 1.5};
  print("values", v);

  // 1. Count the values in [-1, 1] (std::count_if). Make the bounds two local
  //    variables, lo and hi, and capture them
  // TODO
  auto number_values =
      std::count_if(v.begin(), v.end(), [](int i) { return i > -1 && i < 1; });

  std::cout << std::format("The number of values is {0}", number_values)
            << std::endl;

  // 2. Sort v by decreasing absolute value (std::sort with a comparison)
  // TODO
  std::sort(v.begin(), v.end(), [](auto const &lhs, auto const &rhs) {
    return std::abs(lhs) > std::abs(rhs);
  });
  print("sorted by |x|", v);

  // 3. Compute the mean of v (std::accumulate)
  // TODO
  auto mean = std::accumulate(v.begin(), v.end(), 0.f) / v.size();
  std::cout << std::format("mean = {0}", mean) << std::endl;

  // 4. Subtract the mean from every value, in place (std::transform).
  //    How do you capture the mean?
  // TODO
  std::transform(v.begin(), v.end(), v.begin(),
                 [mean](auto v) { return v - mean; });
  print("minus the mean", v);

  // 5. Remove the negative values (std::erase_if, C++20)
  // TODO
  std::erase_if(v, [](auto v) { return v < 0.; });
  print("non negative", v);

  // ---- 6. Optional ----
  std::vector<Particle> particles{{0, 1., 0.3}, {1, 2., -1.2}, {2, 2., 0.8},
                                  {3, 7., 2.5}, {4, 3., -0.5}, {5, 6., 1.1}};
  // Sort the particles by position, then find the first one with mass > 4
  // (std::find_if) and print its id
  // TODO
  std::sort(particles.begin(), particles.end(),
            [](auto const &lhs, auto const &rhs) { return lhs.x - rhs.x; });
  auto p = std::find_if(particles.begin(), particles.end(),
                        [](auto p) { return p.mass > 4; });
  std::cout << std::format("first particle with mass > 4: id {0}, x = {1}",
                           p->id, p->x)
            << std::endl;

  return 0;
}
