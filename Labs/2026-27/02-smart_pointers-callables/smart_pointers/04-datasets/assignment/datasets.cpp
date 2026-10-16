#include "datasets.hpp"

#include <numeric>

std::unique_ptr<Dataset> read_dataset(std::istream &in) {
  std::unique_ptr<Dataset> d =
      std::make_unique<Dataset>(); // TODO: no naked new
  if (!(in >> d->name))
    return std::nullptr_t();
  double v;
  while (in >> v)
    d->values.push_back(v);
  // stopped before the end of the line: something that is not a number
  if (!in.eof() || d->values.empty())
    return std::nullptr_t(); // BUG: what happens to d?
  return d;
}

double mean(Dataset const &d) {
  return std::accumulate(d.values.begin(), d.values.end(), 0.) /
         static_cast<double>(d.values.size());
}
