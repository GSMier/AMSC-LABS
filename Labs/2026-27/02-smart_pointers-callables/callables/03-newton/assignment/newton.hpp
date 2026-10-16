#ifndef NEWTON_HPP
#define NEWTON_HPP

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

// All the parameters of the method in one place, with sensible defaults
struct NewtonOptions {
  unsigned max_iter = 100;
  double rtol = 1e-12; // tolerance on the residual |f(x_k)|
  double stol = 1e-12; // tolerance on the step |x_k - x_{k-1}|
};

struct NewtonResult {
  double root = 0.;
  unsigned iterations = 0;
  double residual = 0.;
  bool converged = false;
  std::vector<double> history; // x_0, x_1, ..., x_k
};

// Newton's method.
// TODO: f and df are function pointers. Make newton a function template, so
// that
//       it also accepts functors and lambdas with captures.
template <typename F, typename DF>
inline NewtonResult newton(F const &f, DF const &df, double x0,
                           const bool print_convergence = false,
                           NewtonOptions const &options = {}) {
  NewtonResult result;
  // TODO: implement x_{k+1} = x_k - f(x_k) / f'(x_k), storing the iterates in
  //       result.history, and stop when
  //       - |f(x_k)| < rtol, or
  //       - |x_k - x_{k-1}| < stol, or
  //       - max_iter iterations have been done.
  //       What if f'(x_k) == 0?
  // (void)f, (void)df, (void)options; // remove this line when you use them

  result.history.emplace_back(x0);
  if (print_convergence)
    std::cout << "Quadratic convergence " << std::abs(x0 - std::sqrt(2))
              << std::endl;

  double step;
  do {
    ++result.iterations;

    if (df(x0) == 0)
      break;

    x0 = x0 - (f(x0) / df(x0));

    result.history.emplace_back(x0);
    result.residual = std::abs(f(x0));
    step = std::abs(result.history.at(result.iterations) -
                    result.history.at(result.iterations - 1));
    if (print_convergence)
      std::cout << "Quadratic convergence " << std::abs(x0 - std::sqrt(2))
                << std::endl;

  } while (result.residual > options.rtol && step > options.stol &&
           result.iterations < options.max_iter);
  result.root = x0;
  result.converged = result.iterations < options.max_iter ? true : false;
  return result;
}

// TODO: write a function template centred_difference(f, h = 1e-6) that RETURNS
// a
//       callable approximating f' with  f'(x) ~ (f(x + h) - f(x - h)) / (2h).
//       How should f be captured?
template <typename F>
inline auto centred_difference(F const &f, double h = 1e-6) {
  return [f, h](double x) -> double {
    double df = (f(x + h) - f(x - h)) / (2 * h);
    return df;
  };
}

#endif /* NEWTON_HPP */
