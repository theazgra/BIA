#pragma once

#include "Solver.h"
using namespace azgra;

struct NSGA2Individual : Individual
{
    size_t compareOnIndex{};
    std::vector<f64> evaluations{};
    std::vector<size_t> dominatedSolutions{};
    float distance{};
    size_t dominatedByCount = 0;
    size_t rank{};

    NSGA2Individual() = default;
    explicit NSGA2Individual(const Individual &individual, std::vector<f64> &functionEvaluations)
    {
        rank = 999;
        fitness = 0.0;
        attributes = individual.attributes;
        evaluations = std::move(functionEvaluations);
    }

    explicit NSGA2Individual(std::vector<f64> &attribs, std::vector<f64> &functionEvaluations)
    {
        rank = 999;
        fitness = 0.0;
        attributes = std::move(attribs);
        evaluations = std::move(functionEvaluations);
    }

    bool operator<(const NSGA2Individual &other)
    {
        return (evaluations[compareOnIndex] < other.evaluations[compareOnIndex]);
    }

    bool operator>(const NSGA2Individual &other)
    {
        return (distance > other.distance);
    }
};

class NSGA2Solver : public Solver
{
private:
    size_t m_objectiveCount;
    std::vector<std::function<azgra::f64(const std::vector<azgra::f64> &)>> m_objectiveFunctions;
    std::vector<NSGA2Individual> m_population;
    std::uniform_int_distribution<size_t> m_randomIndividual;
    std::uniform_int_distribution<size_t> m_randomDim;

    void initialize_population();
    size_t tournament_selection(size_t parentA, bool useCrowdedComparsionOp, const size_t k = 2);
    std::vector<std::vector<size_t>> fast_non_dominated_sort(std::vector<NSGA2Individual> &population);
    NSGA2Individual crossover_with_mutation(const NSGA2Individual &parentA, const NSGA2Individual &parentB);
    std::vector<NSGA2Individual> create_offsprings(bool useCrowdedComparsionOp);
    bool dominates(const NSGA2Individual &p, const NSGA2Individual &q) const;
    void crowded_sort(std::vector<NSGA2Individual> &individuals);

public:
    explicit NSGA2Solver(const std::vector<OptimizationProblem> &problems, size_t populationSize);
    std::vector<std::pair<float,float>> solve();
};