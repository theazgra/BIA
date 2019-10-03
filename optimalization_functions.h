#pragma once

#include <azgra/azgra.h>
#include <cmath>
#include <vector>

using namespace azgra;

/// https://www.sfu.ca/~ssurjano/ackley.html
f64 ackley(const std::vector<f64> &x, const f64 a = 20.0, const f64 b = 0.2, const f64 c = (2 * M_PI_2));

inline f64 ackley_simple(const std::vector<f64> &x)
{
    return ackley(x);
}

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
f64 michalewicz(const std::vector<f64> &x, const f64 m = 10.0);

inline f64 michalewicz_simple(const std::vector<f64> &x)
{
    return michalewicz(x);
}

/// https://www.sfu.ca/~ssurjano/zakharov.html
f64 zakharov(const std::vector<f64> &x);

/// https://www.sfu.ca/~ssurjano/powersum.html
f64 power_sum(const std::vector<f64> &x, const std::vector<f64> &b);

inline f64 power_sum_simple(const std::vector<f64> &x)
{
    return power_sum(x, {8,18,44,144});
}



/// https://www.sfu.ca/~ssurjano/stybtang.html
f64 styblinski_tang(const std::vector<f64> &x);

/// https://www.sfu.ca/~ssurjano/dixonpr.html
f64 dixon_price(const std::vector<f64> &x);
