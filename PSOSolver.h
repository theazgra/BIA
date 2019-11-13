#include "SOMASolver.h"

struct PSOParticle : Individual
{
    f64 pBestFitness = std::numeric_limits<f64>::max();
    std::vector<f64> pBest;
    std::vector<f64> velocity;

    PSOParticle() = default;

    explicit PSOParticle(std::vector<f64> &attribs)
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

    std::vector<f64> m_vMax;
    std::vector<Limits> m_dimensionLimits;
    std::vector<std::uniform_real_distribution<f64>> m_inDimensionRandom;
    std::function<f64(const std::vector<f64> &)> m_fitnessFunction;

    std::random_device m_rd;
    std::mt19937 m_generator;
    std::vector<PSOParticle> m_particles;
    std::uniform_real_distribution<f64> m_01rand;

    PSOParticle m_gBest;

    PSOParticle generate_random_individual();

    void initialize_population();

    void init_gBest(const std::vector<PSOParticle> &particles);

    [[nodiscard]] f64 average_fitness() const;
    [[nodiscard]] f64 average_p_best_fitness() const;

    void update_particle(PSOParticle &particle);

    f64 m_c1 = 2.0;
    f64 m_c2 = 2.0;

public:
    PSOSolver(const optimalization::OptimalizationProblem &problem,
              const size_t particleCount,
              const size_t iterationCount);

    OptimizationResult solve();

};
