#pragma once

#include <azgra/collection/enumerable.h>
#include "Individual.h"
#include "Problem.h"
#include <random>

class Solver
{
protected:
    std::random_device m_rd;
    std::mt19937 m_mt;
    std::uniform_real_distribution<f64> m_rand01;

    size_t m_dimensionCount;
    size_t m_populationSize;
    size_t m_iterationCount;


    std::vector<Limits> m_dimensionLimits;
    std::function<f64(const std::vector<f64>)> m_fitnessFunction;

    std::vector<Individual> generate_random_individuals();
public:
    Solver(const OptimalizationProblem &problem, const size_t populationSize);
};