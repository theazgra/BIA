//
// Created by theaz on 26.11.2019.
//

#include "Solver.h"

Solver::Solver(const OptimizationProblem &problem, const size_t populationSize)
{
    m_populationSize = populationSize;
    m_dimensionCount = problem.dimensionCount;
    m_iterationCount = problem.iterationCount;
    m_fitnessFunction = problem.testFunction;
    m_dimensionLimits = problem.dimensionLimits;

    m_mt = std::mt19937(m_rd());
    m_rand01 = std::uniform_real_distribution<f64>(0.0, 1.0);
}

std::vector<Individual> Solver::generate_random_individuals()
{
    std::vector<Individual> individuals(m_populationSize);

    for (size_t i = 0; i < m_populationSize; ++i)
    {
        Individual individual = {};
        individual.attributes.resize(m_dimensionCount);

        for (size_t dim = 0; dim < m_dimensionCount; ++dim)
        {
            individual.attributes[dim] = m_dimensionLimits[dim].min +
                                         (m_rand01(m_mt) * (m_dimensionLimits[dim].max - m_dimensionLimits[dim].min));

            assert(individual.attributes[dim] >= m_dimensionLimits[dim].min && individual.attributes[dim] <= m_dimensionLimits[dim].max);
        }
        individual.fitness = m_fitnessFunction(individual.attributes);
        individuals[i] = individual;
    }
    return individuals;
}