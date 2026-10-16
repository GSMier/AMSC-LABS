#include "datasets.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <datasets file>" << std::endl;
    return 1;
  }
  std::ifstream file{argv[1]};
  if (!file) {
    std::cerr << "Cannot open " << argv[1] << std::endl;
    return 1;
  }

  // 1. Read
  // TODO: which element type? Who deletes the datasets?
  std::vector<std::unique_ptr<Dataset>> all;
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#')
      continue;
    std::istringstream iss{line};
    auto d = read_dataset(iss);
    if (d)
      all.push_back(std::move(d));
    else
      std::cerr << "Skipping invalid line: " << line << std::endl;
  }
  // TODO: write a function print_all(title, datasets) that prints name, number
  // of
  //       values and mean of each dataset. How should the vector be passed?
  std::cout << "all:" << std::endl;
  for (auto const &d : all)
    std::cout << "  " << d->name << ", " << d->values.size() << " values, mean "
              << mean(*d) << std::endl;

  // 2. The datasets with mean > 10 go to another vector
  // TODO: now "all" and "large" point to the same objects. Who owns them?
  //       Transfer the ownership instead, and remove the empty pointers from
  //       "all"
  std::vector<std::unique_ptr<Dataset>> large;
  for (auto &d : all) {
    if (mean(*d) > 10.) {
      large.push_back(std::move(d));
    }
  }
  std::erase_if(all, [](auto const &d) { return !d; });
  // 3. TODO: find the dataset with the largest mean in "large"
  // (std::max_element
  //          and a lambda) and print its name, keeping a non-owning pointer to
  //          it
  std::cout << "large (mean > 10):" << std::endl;
  for (auto const &d : large)
    std::cout << "  " << d->name << ", " << d->values.size() << " values, mean "
              << mean(*d) << std::endl;

  std::cout << "remaining:" << std::endl;
  for (auto const &d : all)
    std::cout << "  " << d->name << ", " << d->values.size() << " values, mean "
              << mean(*d) << std::endl;

  auto largest = std::max_element(
      large.begin(), large.end(),
      [](auto const &lhs, auto const &rhs) { return mean(*lhs) < mean(*rhs); });
  if (largest != large.end()) {
    Dataset const *largest_dataset = largest->get();
    std::cout << "Largest mean: " << largest_dataset->name << std::endl;
  }

  // Cleanup: is this correct? What if we also deleted the elements of "large"?
  return 0;
}
