#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-use-emplace"

#include "search_algorithms.h"

namespace search_algorithms
{
    using namespace azgra;

    static std::vector<std::uniform_real_distribution<azgra::f64>>
    initialize_random_distributions(const size_t dimensionCount,
                                    const std::vector<Limits> &dimensionLimits)
    {
        always_assert(dimensionCount == dimensionLimits.size());
        std::vector<std::uniform_real_distribution<azgra::f64>> generators;
        for (size_t dim = 0; dim < dimensionCount; ++dim)
        {
            generators.push_back(std::uniform_real_distribution<azgra::f64>(dimensionLimits[dim].min,
                                                                            dimensionLimits[dim].max));
        }

        return generators;
    }

    SearchAlgorithmResult blind_search(const std::function<azgra::f64(const std::vector<azgra::f64> &)> &testFunction,
                                       const size_t iterationCount,
                                       const size_t dimensionCount,
                                       const std::vector<Limits> &dimensionLimits)
    {
        SearchAlgorithmResult result = {};
        result.bestSolutionValue = std::numeric_limits<f64>::max();

        std::random_device rd;
        std::mt19937 generator(rd());
        auto randomDistributions = initialize_random_distributions(dimensionCount, dimensionLimits);

        std::vector<f64> solution(dimensionCount);
        for (size_t iteration = 0; iteration < iterationCount; ++iteration)
        {
            for (size_t dim = 0; dim < dimensionCount; ++dim)
            {
                solution[dim] = randomDistributions[dim](generator);
            }

            f64 currentSolutionCost = testFunction(solution);

            if (currentSolutionCost < result.bestSolutionValue)
            {
                result.bestSolutionValue = currentSolutionCost;
                result.bestSolution = std::vector<f64>(solution);
            }
        }
        return result;
    }

    SearchAlgorithmResult
    blind_search_2d_with_history(const std::function<azgra::f64(const std::vector<azgra::f64> &)> &testFunction,
                                 const size_t iterationCount,
                                 const std::vector<Limits> &dimensionLimits)
    {
        const size_t dimensionCount = 2;
        always_assert(dimensionLimits.size() == dimensionCount);
        SearchAlgorithmResult result = {};
        result.bestSolutionValue = std::numeric_limits<f64>::max();

        std::random_device rd;
        std::mt19937 generator(rd());
        auto randomDistributions = initialize_random_distributions(dimensionCount, dimensionLimits);

        std::vector<f64> solution(dimensionCount);
        for (size_t iteration = 0; iteration < iterationCount; ++iteration)
        {
            for (size_t dim = 0; dim < dimensionCount; ++dim)
            {
                solution[dim] = randomDistributions[dim](generator);
            }

            f64 currentSolutionCost = testFunction(solution);

            result.solutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iteration, currentSolutionCost));

            if (currentSolutionCost < result.bestSolutionValue)
            {
                result.bestSolutionValue = currentSolutionCost;
                result.bestSolution = std::vector<f64>(solution);
                result.bestSolutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iteration, currentSolutionCost));
            }
        }
        result.bestSolutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iterationCount - 1, result.bestSolutionValue));
        return result;
    }

    SearchAlgorithmResult hill_climbing_2d_with_history(const std::function<azgra::f64(const std::vector<azgra::f64> &)> &testFunction,
                                                        const size_t iterationCount,
                                                        const std::vector<Limits> &dimensionLimits,
                                                        const size_t neighborhoodSize,
                                                        const azgra::f64 stdDev)
    {
        const size_t dimensionCount = 2;
        always_assert(dimensionLimits.size() == dimensionCount);
        SearchAlgorithmResult result = {};
        result.bestSolutionValue = std::numeric_limits<f64>::max();

        std::vector<f64> solution(dimensionCount);

        std::random_device rd;
        std::mt19937 generator(rd());
        // Generate initial solution.
        {
            auto randomDistributions = initialize_random_distributions(dimensionCount, dimensionLimits);
            for (size_t dim = 0; dim < dimensionCount; ++dim)
            {
                solution[dim] = randomDistributions[dim](generator);
            }
        }


        for (size_t iteration = 0; iteration < iterationCount; ++iteration)
        {
            f64 currentSolutionCost = testFunction(solution);

            // Generate neighborhood of solution
            std::vector<f64> bestNeighbor(dimensionCount);
            f64 lowestNeighborCost = std::numeric_limits<f64>::max();

            // Generate normal distributions for neighborhood of solution.
            std::vector<std::normal_distribution<f64>> normalDistributions(dimensionCount);
            for (size_t dim = 0; dim < dimensionCount; ++dim)
            {
                normalDistributions[dim] = std::normal_distribution<f64>(solution[dim], stdDev);
            }

            // Generate `neighborhoodSize` neighbors
            for (size_t neighborId = 0; neighborId < neighborhoodSize; ++neighborId)
            {
                // Generate neighbor parameters
                std::vector<f64> neighbor(dimensionCount);
                for (size_t dim = 0; dim < dimensionCount; ++dim)
                {
                    neighbor[dim] = normalDistributions[dim](generator);
                }
                // Test neighbor.
                f64 neighborCost = testFunction(neighbor);
                if (neighborCost <= lowestNeighborCost)
                {
                    lowestNeighborCost = neighborCost;
                    bestNeighbor = std::vector<f64>(neighbor);
                }
            }

            if (lowestNeighborCost <= currentSolutionCost)
            {
                solution = bestNeighbor;
                result.solutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iteration, currentSolutionCost));

                if (lowestNeighborCost < result.bestSolutionValue)
                {
                    result.bestSolutionValue = lowestNeighborCost;
                    result.bestSolution = std::vector<f64>(solution);
                    result.bestSolutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iteration, lowestNeighborCost));
                }
            }
        }

        result.bestSolutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iterationCount - 1, result.bestSolutionValue));
        return result;
    }
}
#pragma clang diagnostic pop