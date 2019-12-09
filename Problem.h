#pragma once

#include <functional>

struct Limits
{
    azgra::f64 min;
    azgra::f64 max;

    Limits() = default;

    explicit Limits(const azgra::f64 _min, const azgra::f64 _max) : min(_min), max(_max)
    {}
};

inline std::vector<Limits> generate_limits(const size_t dimension, const f64 min, const f64 max)
{
    std::vector<Limits> limits(dimension);
    for (size_t dim = 0; dim < dimension; ++dim)
    {
        limits[dim] = Limits(min, max);
    }
    return limits;
}

struct OptimizationProblem
{
    std::function<azgra::f64(const std::vector<azgra::f64> &)> testFunction;
    size_t iterationCount{};
    size_t dimensionCount{};
    std::vector<Limits> dimensionLimits;

    OptimizationProblem() = default;

    OptimizationProblem(const std::function<azgra::f64(const std::vector<azgra::f64> &)> &fn,
                          const size_t itCount,
                          const size_t dimCount,
                          const std::vector<Limits> &limits)
            : testFunction(fn), iterationCount(itCount), dimensionCount(dimCount), dimensionLimits(limits)
    {}
};