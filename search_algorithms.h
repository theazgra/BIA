#pragma once

#include <azgra/azgra.h>
#include <functional>
#include <random>
#include <azgra/geometry/point.h>
#include "optimalization_functions.h"
#include "Problem.h"

namespace optimalization
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

    struct SearchAlgorithmResult
    {
        std::vector<azgra::f64> bestSolution;
        azgra::f64 bestSolutionValue = 0;
        std::vector<azgra::geometry::Point2D<double>> solutionValueHistoryFor2D;
        std::vector<azgra::geometry::Point2D<double>> bestSolutionValueHistoryFor2D;
    };


    struct HillClimbingConfig : OptimalizationProblem
    {
        size_t neighborhoodSize;
        azgra::f64 stdDev;

        void set_probem(OptimalizationProblem problem);
    };

    struct SimulatedAnnealingConfig : HillClimbingConfig
    {
        azgra::f64 initialTemperature;
        azgra::f64 finalTemperature;
        azgra::f64 temperatureReductionFactor;
        size_t repetitionOfMetropolisAlg;
    };

    SearchAlgorithmResult blind_search(const OptimalizationProblem &problem);

    SearchAlgorithmResult blind_search_with_history(const OptimalizationProblem &problem);

    SearchAlgorithmResult hill_climbing_with_history(const HillClimbingConfig &problem);

    SearchAlgorithmResult simulated_annealing_with_history(const SimulatedAnnealingConfig &problem);
}