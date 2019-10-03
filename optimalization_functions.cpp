#include "optimalization_functions.h"

/// https://www.sfu.ca/~ssurjano/ackley.html
f64 ackley(const std::vector<f64> &x, const f64 a, const f64 b, const f64 c)
{
    const size_t dims = x.size();
    f64 sum1 = 0.0;
    f64 sum2 = 0.0;
    for (size_t i = 0; i < dims; ++i)
    {
        sum1 += pow(x[i], 2);
        sum2 += cos(c * x[i]);
    }

    f64 result = -a * exp(-b * sqrt((1.0 / static_cast<f64>(dims)) * sum1)) - exp((1.0 / static_cast<f64>(dims)) * sum2) + a + exp(1.0);
    return result;
}

/// https://www.sfu.ca/~ssurjano/griewank.html
f64 griewank(const std::vector<f64> &x)
{
    const size_t dims = x.size();
    f64 sum = 0.0;
    f64 product = 1.0;
    for (size_t i = 0; i < dims; ++i)
    {
        sum += (pow(x[i], 2) / (f64) 4000.0);
        product *= cos(x[i] / sqrt(i + 1));
    }
    f64 result = sum - product + 1;
    return result;
}

/// https://www.sfu.ca/~ssurjano/spheref.html
f64 sphere(const std::vector<f64> &x)
{
    const size_t dims = x.size();
    f64 sum = 0.0;
    for (size_t i = 0; i < dims; ++i)
    {
        sum += pow(x[i], 2);
    }
    return sum;
}

/// https://www.sfu.ca/~ssurjano/schwef.html
f64 schwefel(const std::vector<f64> &x)
{
    const f64 schwefelConstant = 418.9829;
    const size_t dims = x.size();
    f64 sum = 0.0;
    for (size_t i = 0; i < dims; ++i)
    {
        sum += x[i] * sin(sqrt(std::abs(x[i])));
    }
    f64 result = (schwefelConstant * static_cast<f64>(dims)) - sum;
    return result;
}

/// https://www.sfu.ca/~ssurjano/rastr.html
f64 rastrigin(const std::vector<f64> &x)
{
    const size_t dims = x.size();
    f64 sum = 0.0;
    for (size_t i = 0; i < dims; ++i)
    {
        sum += (pow(x[i], 2) - (10 * cos(2 * M_PI * x[i])));
    }
    f64 result = (10.0 * static_cast<f64>(dims)) + sum;
    return result;
}

/// https://www.sfu.ca/~ssurjano/rosen.html
f64 rosenbrock(const std::vector<f64> &x)
{
    const size_t dims = x.size();
    f64 sum = 0.0;
    for (size_t i = 0; i < (dims - 1); ++i)
    {
        sum += ((100.0 * (x[i + 1] - pow(x[i], 2))) + pow((x[i] - 1), 2));
    }
    return sum;
}

/// https://www.sfu.ca/~ssurjano/michal.html
f64 michalewicz(const std::vector<f64> &x, const f64 m)
{
    const size_t dims = x.size();
    f64 sum = 0.0;
    for (size_t i = 0; i < dims; ++i)
    {
        sum += sin(x[i]) * pow(sin((static_cast<f64>(i + 1) * pow(x[i], 2)) / M_PI), 2.0 * m);
    }
    f64 result = -1.0 * sum;
    return result;
}

/// https://www.sfu.ca/~ssurjano/zakharov.html
f64 zakharov(const std::vector<f64> &x)
{
    const size_t dims = x.size();
    f64 sum1 = 0.0;
    f64 sum2 = 0.0;
    for (size_t i = 0; i < dims; ++i)
    {
        sum1 += pow(x[i], 2);
        sum2 += 0.5 * static_cast<f64>(i+1) * x[i];
    }
    f64 result = sum1 + pow(sum2, 2) + pow(sum2, 4);
    return result;
}

/// https://www.sfu.ca/~ssurjano/powersum.html
f64 power_sum(const std::vector<f64> &x, const std::vector<f64> &b)
{
    const size_t dims = x.size();
    f64 xSum = 0.0;
    for (size_t i = 0; i < dims; ++i)
    {
        xSum += x[i];
    }
    f64 sum = 0.0;
    for (size_t i = 0; i < dims; ++i)
    {
        sum += pow((xSum - b[i]), 2);
    }
    return sum;
}

/// https://www.sfu.ca/~ssurjano/stybtang.html
f64 styblinski_tang(const std::vector<f64> &x)
{
    const size_t dims = x.size();
    f64 sum = 0.0;
    for (size_t i = 0; i < dims; ++i)
    {
        sum += (pow(x[i], 4) - (16.0 * pow(x[i], 2)) + (5.0 * x[i]));
    }
    f64 result = 0.5 * sum;
    return result;
}

/// https://www.sfu.ca/~ssurjano/dixonpr.html
f64 dixon_price(const std::vector<f64> &x)
{
    const size_t dims = x.size();
    f64 sum = 0.0;
    for (size_t i = 1; i < dims; ++i)
    {
        sum += (static_cast<f64>(i+1) * pow(((2.0 * pow(x[i], 2)) - x[i]), 2));
    }
    f64 result = pow((x[0] - 1.0), 2) + sum;
    return result;
}

//for (size_t i = 0; i < dims; ++i)
//f64 sphere(const size_t dims, const std::vector<f64> &x)
