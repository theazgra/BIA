#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-use-emplace"

#include "search_algorithms.h"

namespace optimalization
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

    static inline std::vector<std::normal_distribution<f64>>
    get_normal_distributions_around_solution(const std::vector<f64> &solution, const f64 stdDev)
    {

        std::vector<std::normal_distribution<f64>> distributions(solution.size());
        for (size_t dim = 0; dim < solution.size(); ++dim)
        {
            distributions[dim] = std::normal_distribution<f64>(solution[dim], stdDev);
        }
        return distributions;
    }

    template<typename Generator, typename Distribution>
    static inline std::vector<f64> generate_random_solution(Generator &generator, std::vector<Distribution> &distributions)
    {
        std::vector<f64> solution(distributions.size());
        // Generate `neighborhoodSize` neighbors
        for (size_t dim = 0; dim < distributions.size(); ++dim)
        {
            solution[dim] = distributions[dim](generator);
        }
        return solution;
    }


    SearchAlgorithmResult blind_search(const OptimalizationProblem &problem)
    {
        SearchAlgorithmResult result = {};
        result.bestSolutionValue = std::numeric_limits<f64>::max();

        std::random_device rd;
        std::mt19937 generator(rd());
        auto randomDistributions = initialize_random_distributions(problem.dimensionCount, problem.dimensionLimits);

        std::vector<f64> solution(problem.dimensionCount);
        for (size_t iteration = 0; iteration < problem.iterationCount; ++iteration)
        {
            for (size_t dim = 0; dim < problem.dimensionCount; ++dim)
            {
                solution[dim] = randomDistributions[dim](generator);
            }

            f64 currentSolutionCost = problem.testFunction(solution);

            if (currentSolutionCost < result.bestSolutionValue)
            {
                result.bestSolutionValue = currentSolutionCost;
                result.bestSolution = std::vector<f64>(solution);
            }
        }
        return result;
    }

    SearchAlgorithmResult
    blind_search_with_history(const OptimalizationProblem &problem)
    {
        SearchAlgorithmResult result = {};
        result.bestSolutionValue = std::numeric_limits<f64>::max();

        std::random_device rd;
        std::mt19937 generator(rd());
        auto randomDistributions = initialize_random_distributions(problem.dimensionCount, problem.dimensionLimits);

        std::vector<f64> solution(problem.dimensionCount);
        for (size_t iteration = 0; iteration < problem.iterationCount; ++iteration)
        {
            for (size_t dim = 0; dim < problem.dimensionCount; ++dim)
            {
                solution[dim] = randomDistributions[dim](generator);
            }

            f64 currentSolutionCost = problem.testFunction(solution);

            result.solutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iteration, currentSolutionCost));

            if (currentSolutionCost < result.bestSolutionValue)
            {
                result.bestSolutionValue = currentSolutionCost;
                result.bestSolution = std::vector<f64>(solution);
                result.bestSolutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iteration, currentSolutionCost));
            }
        }
        result.bestSolutionValueHistoryFor2D.push_back(geometry::Point2D<double>(problem.iterationCount - 1, result.bestSolutionValue));
        return result;
    }

    SearchAlgorithmResult hill_climbing_with_history(const HillClimbingConfig &problem)
    {
        SearchAlgorithmResult result = {};
        result.bestSolutionValue = std::numeric_limits<f64>::max();

        std::vector<f64> solution(problem.dimensionCount);

        std::random_device rd;
        std::mt19937 generator(rd());
        // Generate initial solution.
        {
            auto randomDistributions = initialize_random_distributions(problem.dimensionCount, problem.dimensionLimits);
            for (size_t dim = 0; dim < problem.dimensionCount; ++dim)
            {
                solution[dim] = randomDistributions[dim](generator);
            }
        }


        size_t better=0;
        for (size_t iteration = 0; iteration < problem.iterationCount; ++iteration)
        {
            f64 currentSolutionCost = problem.testFunction(solution);

            // Generate neighborhood of solution
            std::vector<f64> bestNeighbor(problem.dimensionCount);
            f64 lowestNeighborCost = std::numeric_limits<f64>::max();

            // Generate normal distributions for neighborhood of solution.
            std::vector<std::normal_distribution<f64>> normalDistributions(problem.dimensionCount);
            for (size_t dim = 0; dim < problem.dimensionCount; ++dim)
            {
                normalDistributions[dim] = std::normal_distribution<f64>(solution[dim], problem.stdDev);
            }

            // Generate `neighborhoodSize` neighbors
            for (size_t neighborId = 0; neighborId < problem.neighborhoodSize; ++neighborId)
            {
                // Generate neighbor parameters
                std::vector<f64> neighbor(problem.dimensionCount);
                for (size_t dim = 0; dim < problem.dimensionCount; ++dim)
                {
                    neighbor[dim] = normalDistributions[dim](generator);
                }
                // Test neighbor.
                f64 neighborCost = problem.testFunction(neighbor);
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
                    result.bestSolutionValueHistoryFor2D.push_back(geometry::Point2D<double>(better++, lowestNeighborCost));
                }
            }
        }

        //result.bestSolutionValueHistoryFor2D.push_back(geometry::Point2D<double>(problem.iterationCount - 1, result.bestSolutionValue));
        return result;
    }

    SearchAlgorithmResult simulated_annealing_with_history(const SimulatedAnnealingConfig &problem)
    {
        SearchAlgorithmResult result = {};
        result.bestSolutionValue = std::numeric_limits<f64>::max();

        std::vector<f64> solution(problem.dimensionCount);
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_real_distribution<f64> zeroOneDistribution(0, 1);

        // Generate initial solution.
        {
            auto randomDistributions = initialize_random_distributions(problem.dimensionCount, problem.dimensionLimits);
            for (size_t dim = 0; dim < problem.dimensionCount; ++dim)
            {
                solution[dim] = randomDistributions[dim](generator);
            }
        }

        f64 currentTemperature = problem.initialTemperature;
        size_t iteration = 0;
        while (currentTemperature > problem.finalTemperature)
        {
            f64 currentSolutionCost = problem.testFunction(solution);

            // Generate normal distributions for neighborhood of solution.
            std::vector<std::normal_distribution<f64>> neighborhoodDistribution = get_normal_distributions_around_solution(solution,
                                                                                                                           problem.stdDev);
            for (size_t metropolisIt = 0; metropolisIt < problem.repetitionOfMetropolisAlg; ++metropolisIt)
            {
                std::vector<f64> neighbor = generate_random_solution(generator, neighborhoodDistribution);
                f64 neighborCost = problem.testFunction(neighbor);
                f64 deltaF = neighborCost - currentSolutionCost;

                if (deltaF < 0)
                {
                    // Move to a better solution is always accepted
                    solution = neighbor;
                    currentSolutionCost = neighborCost;
                    result.bestSolutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iteration++, currentSolutionCost));
                }
                else
                {
                    f64 r = zeroOneDistribution(generator);
                    f64 rTest = std::exp(((-deltaF) / currentTemperature));
                    if (r < rTest)
                    {
                        // Move to a worse solution or current solution will (would?) remain unchanged.
                        solution = neighbor;
                        currentSolutionCost = neighborCost;
                        result.bestSolutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iteration++, currentSolutionCost));
                    }
                }
            }

            result.solutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iteration, currentSolutionCost));
            ++iteration;
            currentTemperature *= problem.temperatureReductionFactor;
        }
        result.bestSolution = solution;
        result.bestSolutionValue = problem.testFunction(solution);
        //result.bestSolutionValueHistoryFor2D.push_back(geometry::Point2D<double>(iteration, result.bestSolutionValue));
        return result;
    }

    void HillClimbingConfig::set_probem(OptimalizationProblem problem)
    {
        testFunction = problem.testFunction;
        iterationCount = problem.iterationCount;
        dimensionLimits = problem.dimensionLimits;
        dimensionCount = problem.dimensionCount;
    }
}
#pragma clang diagnostic pop
