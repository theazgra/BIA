#pragma once

#include <cmath>
#include <vector>

typedef double f64;

/// https://www.sfu.ca/~ssurjano/ackley.html
f64 ackley(const f64 a, const f64 b, const f64 c,
           const std::vector<f64> &x);
/// https://www.sfu.ca/~ssurjano/griewank.html
f64 griewank(const std::vector<f64> &x);
/// https://www.sfu.ca/~ssurjano/spheref.html
f64 sphere(const std::vector<f64> &x);
/// https://www.sfu.ca/~ssurjano/schwef.html
f64 schwefel(const std::vector<f64> &x);
/// https://www.sfu.ca/~ssurjano/rastr.html
f64 rastrigin(const std::vector<f64> &x);
/// https://www.sfu.ca/~ssurjano/rosen.html
f64 rosenbrock(const std::vector<f64> &x);
/// https://www.sfu.ca/~ssurjano/michal.html
f64 michalewicz(const std::vector<f64> &x, const f64 m);
/// https://www.sfu.ca/~ssurjano/zakharov.html
f64 zakharov(const std::vector<f64> &x);
f64 power_sum(const std::vector<f64> &x, const std::vector<f64> &b);
/// https://www.sfu.ca/~ssurjano/stybtang.html
f64 styblinski_tang(const std::vector<f64> &x);
/// https://www.sfu.ca/~ssurjano/dixonpr.html
f64 dixon_price(const std::vector<f64> &x);
