#pragma once

#include "search_algorithms.h"
#include <azgra/collection/enumerable.h>


using namespace azgra;
using namespace optimalization;

struct Individual
{
    std::vector<f64> attributes;
    f64 fitness = std::numeric_limits<f64>::max();
};

struct SOMAResult
{
    std::vector<std::vector<std::vector<f64>>> invidualsInTime;
    f64 result;
};

class SOMASolver
{
private:
    f64 m_pathLen = 1.1;
    f64 m_step = 0.11;
    f64 m_prt = 0.5;
    size_t m_dimension;
    size_t m_populationSize = 10;
    size_t m_migrations = 10;
    f64 m_minDiv = 0.5;
    std::vector<Limits> m_dimensionLimits;

    std::function<f64(const std::vector<f64> &)> m_fitnessFunction;

    std::random_device m_rd;
    std::mt19937 m_generator;
    std::vector<Individual> m_currentPopulation;
    std::uniform_real_distribution<f64> m_01rand;

    std::vector<f64> generate_perturbation_vector();

    Individual generate_random_individual();
    Individual find_leader(const std::vector<Individual> &population);

    Individual migrate(const Individual &current, const Individual &leader, const std::vector<f64> &prt, const f64 t);
    void calculate_fitness_for_population(std::vector<Individual> &population);

    void initialize_population();

    f64 average_fitness() const;

public:
    SOMASolver(const optimalization::OptimalizationProblem &problem,
               const size_t populationSize,
               const size_t migrationCount);

    void edit_parameters(const f64 pathLen, const f64 step, const f64 prt, const f64 minDiv);

    SOMAResult solve();
};
