#include "newton.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// 1. Free functions
double f(double x) { return x * x - 2.; }
double df(double x) { return 2. * x; }

// 2. A functor with state: a polynomial p(x) = c_0 + c_1 x + ... + c_n x^n
struct Polynomial {
  std::vector<double> coeffs;
  std::vector<double> deriv_coeffs;

  // Horner's rule
  double operator()(double x) const {
    double p = 0.;
    for (auto it = coeffs.rbegin(); it != coeffs.rend(); ++it)
      p = p * x + *it;
    return p;
  }

  auto derivative() const {
    Polynomial derivative_polynomial;
    if (coeffs.size() <= 1)
      return derivative_polynomial;

    derivative_polynomial.coeffs.resize(coeffs.size() - 1);
    std::size_t degree = 1;
    std::transform(coeffs.begin() + 1, coeffs.end(),
                   derivative_polynomial.coeffs.begin(),
                   [&degree](double coefficient) {
                     return static_cast<double>(degree++) * coefficient;
                   });
    return derivative_polynomial;
  }

  // TODO (at home): Polynomial derivative() const
};

void report(std::string const &title, NewtonResult const &r) {
  std::cout << std::setw(28) << std::left << title
            << " root = " << std::setprecision(15) << r.root
            << ", iterations = " << r.iterations
            << ", residual = " << std::setprecision(3) << r.residual
            << (r.converged ? "" : "  NOT CONVERGED") << std::endl;
}

int main() {
  double const x0 = 1.;

  // 1. functions
  report("functions", newton(f, df, x0));

  // 2. (at home) TODO: functor. Find the root of p(x) = x^3 - 2x - 5 starting
  // from x0 = 2
  struct P {
    double operator()(double x) const { return x * x * x - 2. * x - 5; }
  };
  P p{};
  struct Dp {
    double operator()(double x) const { return 3 * x * x - 2; }
  };
  Dp dp{};

  report("Functor", newton(p, dp, 2));

  // 3. TODO: lambdas. For a = 2, 3, 5 find sqrt(a) as the root of x^2 - a,
  //          with a lambda that captures a
  double a = 2;
  auto lambda = [&a](double x) -> double { return x * x - a; };
  auto dlambda = [](double x) -> double { return 2. * x; };
  report("Lambda a=2", newton(lambda, dlambda, x0));
  a = 3;
  report("Lambda a=3", newton(lambda, dlambda, x0));
  a = 5;
  report("Lambda a=5", newton(lambda, dlambda, x0));

  // 4. TODO: approximate the derivative with centred_difference(f)
  auto numerical_df = centred_difference(f);
  report("centred difference", newton(f, numerical_df, x0));

  // 5. (at home) TODO: print the error |x_k - sqrt(2)| for each iterate of
  // case 1.
  //          Is the convergence quadratic?
  report("Convergence", newton(f, df, x0, true));

  // 6. (if you have time) TODO: count how many times f is evaluated in case 4,
  //          with rtol = 1e-8. Hint: a lambda that wraps f and increments a
  //          counter. How must the counter be captured?
  double counter = 0;
  auto counter_lambda = [&counter](double x) -> double {
    ++counter;
    return f(x);
  };
  newton(counter_lambda, centred_difference(counter_lambda), x0);
  std::cout << "Count f calling " << counter << std::endl;
  return 0;
}
