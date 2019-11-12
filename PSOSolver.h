#include "SOMASolver.h"

struct PSOIndividual : Individual
{
    f64 pBestFitness = std::numeric_limits<f64>::max();
    std::vector<f64> pBest;
    std::vector<f64> velocity;

    PSOIndividual() = default;

    explicit PSOIndividual(std::vector<f64> &attribs)
    {
        attributes = std::move(attribs);
        pBest = attributes;
    }
};

class PSOSolver
{
    size_t m_particleCount = 10;
    size_t m_dimension;
    size_t m_iterationCount = 10;
    std::vector<Limits> m_dimensionLimits;

    std::vector<f64> m_dimensionSize;
    std::vector<f64> m_vMax;
    std::function<f64(const std::vector<f64> &)> m_fitnessFunction;

    std::random_device m_rd;
    std::mt19937 m_generator;
    std::vector<PSOIndividual> m_particles;
    std::uniform_real_distribution<f64> m_01rand;

    PSOIndividual m_gBest;

    PSOIndividual generate_random_individual();

    void initialize_population();

    void set_gBest(const std::vector<PSOIndividual> &particles);

    f64 average_fitness() const;

    void update_particle(PSOIndividual &particle);

    f64 m_c1 = 2.0;
    f64 m_c2 = 2.0;

public:
    PSOSolver(const optimalization::OptimalizationProblem &problem,
              const size_t particleCount,
              const size_t iterationCount);

    OptimizationResult solve();

};
