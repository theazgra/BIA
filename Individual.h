#pragma once

#include <azgra/azgra.h>

using namespace azgra;

struct Individual
{
    std::vector<f64> attributes;
    f64 fitness = std::numeric_limits<f64>::max();

    bool operator<(const Individual &other)
    {
        return (fitness < other.fitness);
    }
};

struct OptimizationResult
{
    std::vector<std::vector<std::vector<f64>>> invidualsInTime;
    f64 result;
};