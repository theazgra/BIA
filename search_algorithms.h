#pragma once

#include <azgra/azgra.h>
#include <functional>
#include <random>
#include <azgra/geometry/point.h>
#include "optimalization_functions.h"

namespace search_algorithms
{
    inline void print_solution(const std::vector<azgra::f64> &solution)
    {
        const size_t dimCount = solution.size();
        fprintf(stdout, "[");
        for (size_t dim = 0; dim < dimCount; ++dim)
        {

            if (dim != dimCount - 1)
            { fprintf(stdout, "%.3f,", solution[dim]); }
            else
            { fprintf(stdout, "%.3f", solution[dim]); }
        }
        fprintf(stdout, "]\n");
    }

    struct Limits
    {
        azgra::f64 min;
        azgra::f64 max;

        Limits() = default;

        explicit Limits(const azgra::f64 _min, const azgra::f64 _max) : min(_min), max(_max)
        {}
    };

    struct SearchAlgorithmResult
    {
        std::vector<azgra::f64> bestSolution;
        azgra::f64 bestSolutionValue = 0;
        std::vector<azgra::geometry::Point2D<double>> solutionValueHistoryFor2D;
        std::vector<azgra::geometry::Point2D<double>> bestSolutionValueHistoryFor2D;
    };

    SearchAlgorithmResult blind_search(const std::function<azgra::f64(const std::vector<azgra::f64> &)> &testFunction,
                                       const size_t iterationCount,
                                       const size_t dimensionCount,
                                       const std::vector<Limits> &dimensionLimits);

    SearchAlgorithmResult blind_search_2d_with_history(const std::function<azgra::f64(const std::vector<azgra::f64> &)> &testFunction,
                                                       const size_t iterationCount,
                                                       const std::vector<Limits> &dimensionLimits);

    SearchAlgorithmResult hill_climbing_2d_with_history(const std::function<azgra::f64(const std::vector<azgra::f64> &)> &testFunction,
                                                        const size_t iterationCount,
                                                        const std::vector<Limits> &dimensionLimits,
                                                        const size_t neighborhoodSize,
                                                        const azgra::f64 stdDev);
}